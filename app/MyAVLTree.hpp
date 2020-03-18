#ifndef __PROJ_THREE_AVL_HPP
#define __PROJ_THREE_AVL_HPP

#include "runtimeexcept.hpp"
#include <string>
#include <vector>

class ElementNotFoundException : public RuntimeException {
	public:
		ElementNotFoundException(const std::string & err) : RuntimeException(err) {}
};

template<typename Key, typename Value>
class MyAVLTree {

	private:
		// fill in private member data here
		// If you need to declare private functions, do so here too.

		struct Node {
		Node (Key k, Value v) : key(k), value(v), 
									lchild(nullptr), rchild(nullptr), 
									parent(nullptr) {}
			Key key;
			Value value;
			Node* lchild;
			Node* rchild;
			Node* parent;
		};

		Node* root;
		unsigned numDistinctKeys;

	public:
		MyAVLTree();

		// In general, a copy constructor and assignment operator
		// are good things to have.
		// For this quarter, I am not requiring these. 
		//	MyAVLTree(const MyAVLTree & st);
		//	MyAVLTree & operator=(const MyAVLTree & st);

		// The destructor is, however, required. 
		~MyAVLTree();
			void destructorHelper(Node* x);

		// size() returns the number of distinct keys in the tree.
		size_t size() const noexcept;

		// isEmpty() returns true if and only if the tree has no values in it. 
		bool isEmpty() const noexcept;

		//  contains() returns true if and only if there
		//  is a (key, value) pair in the tree
		//	that has the given key as its key.
		bool contains(const Key& k) const; 
			bool containsHelper(Node* x, const Key& k) const;

		// find returns the value associated with the given key
		// If !contains(k), this will throw an ElementNotFoundException
		// There needs to be a version for const and non-const MyAVLTrees.
		Value& find(const Key& k);
		const Value& find(const Key& k) const;
			Node* findHelper(Node* x, const Key& k) const;

		// Inserts the given key-value pair into 
		// the tree and performs the AVL re-balance
		// operation, as described in lecture. 
		// If the key already exists in the tree, 
		// you may do as you please (no test cases in
		// the grading script will deal with this situation)
		void insert(const Key& k, const Value& v);
			int absVal(int x) const;
			void insertHelper(Node* traverse, Node* x) const;
			Node* findUnbalanced(Node* x, int& balance) const;
				int height(Node* x) const;
					int max(int x, int y) const;
			void leftRotate(Node* x);
			void rightRotate(Node* x);
		
		// in general, a "remove" function would be here
		// It's a little trickier with an AVL tree
		// and I am not requiring it for this quarter's ICS 46.
		// You should still read about the remove operation
		// in your textbook. 

		// The following three functions all return
		// the set of keys in the tree as a standard vector.
		// Each returns them in a different order.
		std::vector<Key> inOrder() const;
			void inOrderHelper(std::vector<Key>& foo, Node* x) const;
		std::vector<Key> preOrder() const;
			void preOrderHelper(std::vector<Key>& foo, Node* x) const;
		std::vector<Key> postOrder() const;
			void postOrderHelper(std::vector<Key>& foo, Node* x) const;

};

// ○ For comparing keys, use the “natural” comparison offered by <.
// Any test cases provided will have something for the key that has this defined.

//--------------------------------------------------------

// size() returns the number of distinct keys in the tree.
template<typename Key, typename Value>
size_t MyAVLTree<Key, Value>::size() const noexcept {
	return numDistinctKeys;
}

// isEmpty() returns true if and only if the tree has no values in it. 
template<typename Key, typename Value>
bool MyAVLTree<Key, Value>::isEmpty() const noexcept {
	if (numDistinctKeys == 0) {
		return true;
	}
	return false;
}

//--------------------------------------------------------

//  constructor
template<typename Key, typename Value>
MyAVLTree<Key,Value>::MyAVLTree() {
	numDistinctKeys = 0;
	root = nullptr;
}

//  destructor
template<typename Key, typename Value>
MyAVLTree<Key,Value>::~MyAVLTree() {

	destructorHelper(root);
	
}
	//  destructor helper
	template<typename Key, typename Value>
	void MyAVLTree<Key,Value>::destructorHelper(Node* x) {
		if (x != nullptr) {
			destructorHelper(x->lchild);
			destructorHelper(x->rchild);
			delete x;
		}
	}
//--------------------------------------------------------

//  contains() returns true if and only if there
//  is a (key, value) pair in the tree
//	that has the given key as its key.
template<typename Key, typename Value>
bool MyAVLTree<Key, Value>::contains(const Key& k) const {
	return containsHelper(root, k);
}

	template<typename Key, typename Value>
	bool MyAVLTree<Key, Value>::containsHelper(Node* traverse, const Key& k) const {
		if (traverse == nullptr) {
			return false;
		}
		if (traverse->key == k) {
			return true;
		}
		if (k < traverse->key) {
			return containsHelper(traverse->lchild, k);
		}
		else {
			return containsHelper(traverse->rchild, k);
		}
	}

// find returns the value associated with the given key
// If !contains(k), this will throw an ElementNotFoundException
// There needs to be a version for const and non-const MyAVLTrees.
template<typename Key, typename Value>
Value& MyAVLTree<Key, Value>::find(const Key& k) {
	if (!contains(k)) {
		throw ElementNotFoundException("Element not found");
	}
	else {
		Node* x = findHelper(root, k);
		return x->value;
	}
}

template<typename Key, typename Value>
const Value& MyAVLTree<Key, Value>::find(const Key& k) const {
	if (!contains(k)) {
		throw ElementNotFoundException("Element not found");
	}
	else {
		Node* x = findHelper(root, k);
		return x->value;
	}
}

	template<typename Key, typename Value>
	typename MyAVLTree<Key, Value>::Node* MyAVLTree<Key, Value>::findHelper(Node* x, const Key& k) const {
		if (x->key == k) {
			return x;
		}
		if (k < x->key) {
			return findHelper(x->lchild, k);
		}
		else {
			return findHelper(x->rchild, k);
		}
	}

//--------------------------------------------------------

// Inserts the given key-value pair into 
// the tree and performs the AVL re-balance
// operation, as described in lecture. 
// If the key already exists in the tree, 
// you may do as you please (no test cases in
// the grading script will deal with this situation)
template<typename Key, typename Value>
void MyAVLTree<Key, Value>::insert(const Key& k, const Value& v) {
	// if repeated value return
	if (contains(k)) {
		return;
	}

	Node* x = new Node(k, v);

	if (root == nullptr && numDistinctKeys == 0) {
		root = x;
	}
	else {
		insertHelper(root, x);
	}
	++numDistinctKeys;

	//  find the first unbalanced node up the tree if it exists
	int balance;
	Node* firstUnbalanced = findUnbalanced(x, balance);

	if (firstUnbalanced != nullptr) {
		int left = 0;
		int right = 0;
		// didn't check if lchild exists for balance > 0 nor rchild exists for balance < 0 
		// because if unbalanced left will have lchild, if unbalanced right will have rchild

		// left heavy cases
		if (balance > 0) {
			
			// get the height of firstUnbalanced's lchild's 
			// left and right trees + 1 for edge to lchild
			if (firstUnbalanced->lchild->lchild != nullptr) {
				left = height(firstUnbalanced->lchild->lchild) + 1;
			}
			if (firstUnbalanced->lchild->rchild != nullptr) {
				right = height(firstUnbalanced->lchild->rchild) + 1;
			}	

			// left left case
			if (left > right) {
				rightRotate(firstUnbalanced);
			}
			// left right case
			else if (left < right) {
				leftRotate(firstUnbalanced->lchild);
				rightRotate(firstUnbalanced);
				
			}
		}
		// right heavy cases
		else if (balance < 0) {

			// get the height of firstUnbalanced's rchild's 
			// left and right trees + 1 for edge to rchild
			if (firstUnbalanced->rchild->lchild != nullptr) {
				left = height(firstUnbalanced->rchild->lchild) + 1;
			}
			if (firstUnbalanced->rchild->rchild != nullptr) {
				right = height(firstUnbalanced->rchild->rchild) + 1;
			}	

			// right right case
			if (left < right) {
				leftRotate(firstUnbalanced);
			}
			// right left case
			else if (left > right) {
				rightRotate(firstUnbalanced->rchild);
				leftRotate(firstUnbalanced);
			}
		}
	}

}



	template<typename Key, typename Value>
	void MyAVLTree<Key, Value>::leftRotate(Node* x) {

		Node* temp = x->rchild;

		// aka parent of x is nullptr
		if (x == root) {
			root = temp;
		}
		else if (x->parent != nullptr) {
			if (x->parent->lchild == x) {
				x->parent->lchild = temp;
			}
			else if (x->parent->rchild == x) {
				x->parent->rchild = temp;
			}
		}
		
		x->rchild = temp->lchild;
		temp->lchild = x;
		temp->parent = x->parent;
		x->parent = temp;

		if (x->rchild != nullptr) {
			x->rchild->parent = x;
		}


	}

	template<typename Key, typename Value>
	void MyAVLTree<Key, Value>::rightRotate(Node* x) {
		Node* temp = x->lchild;

		// aka parent of x is nullptr
		if (x == root) {
			root = temp;
		}
		else if (x->parent != nullptr) {
			if (x->parent->lchild == x) {
				x->parent->lchild = temp;
			}
			else if (x->parent->rchild == x) {
				x->parent->rchild = temp;
			}
		}
		
		x->lchild = temp->rchild;
		temp->rchild = x;
		temp->parent = x->parent;
		x->parent = temp;

		if (x->lchild != nullptr) {
			x->lchild->parent = x;
		}

	}

	//  insert helper
	template<typename Key, typename Value>
	typename MyAVLTree<Key, Value>::Node* MyAVLTree<Key, Value>::findUnbalanced(Node* x, int& balance) const {
		
		Node* traverse = x->parent;
		int left;
		int right;
		//  root parent is nullptr
		while (traverse != nullptr) {

			left = 0;
			right = 0;
			// if lchild or rchild is nullptr then height will return -1 so we set default to 0
			// + 1 height for the unaccounted edge to parent
			if (traverse->lchild != nullptr) {
				left = height(traverse->lchild) + 1;
			}
			if (traverse->rchild != nullptr) {
				right = height(traverse->rchild) + 1;
			}

			if (absVal(left - right) > 1) {
				// update balance which is balance at the first unbalanced node
				balance = left - right;
				return traverse;
			}
			traverse = traverse->parent;
		}

		return nullptr;

	}
		//  find unbalanced helper - height
		//  assuming Node is not nullptr initially
		template<typename Key, typename Value>
		int MyAVLTree<Key, Value>::height(Node* x) const {
			if (x == nullptr) {
				return -1;
			}
			return max(height(x->lchild), height(x->rchild)) + 1;
		}
		
			//  height helper - max
			template<typename Key, typename Value>
			int MyAVLTree<Key, Value>::max(int x, int y) const {
				if (x > y) {
					return x;
				}
				else {
					return y;
				}
			}

	template<typename Key, typename Value>
	int MyAVLTree<Key, Value>::absVal(int x) const {
		if (x < 0) {
			return x * -1;
		}
		return x;
	}
	//  insert helper
	template<typename Key, typename Value>
	void MyAVLTree<Key, Value>::insertHelper(Node* traverse, Node* x) const {
		if (traverse->key < x->key) {
			if (traverse->rchild == nullptr) {
				traverse->rchild = x;
				x->parent = traverse;
				return;
			}
			else {
				return insertHelper(traverse->rchild, x);
			}
		}
		else {
			if (traverse->lchild == nullptr) {
				traverse->lchild = x;
				x->parent = traverse;
				return;
			}
			else {
				return insertHelper(traverse->lchild, x);
			}
		}
		
	}



// The following three functions all return
// the set of keys in the tree as a standard vector.
// Each returns them in a different order.
template<typename Key, typename Value>
std::vector<Key> MyAVLTree<Key, Value>::inOrder() const {
	std::vector<Key> foo;
	inOrderHelper(foo, root);
	return foo; 
}
	template<typename Key, typename Value>
	void MyAVLTree<Key, Value>::inOrderHelper(std::vector<Key>& foo, Node* x) const {
		if (x == nullptr) {
			return;
		}
		inOrderHelper(foo, x->lchild);
		foo.push_back(x->key);
		inOrderHelper(foo, x->rchild);
	}

template<typename Key, typename Value>
std::vector<Key> MyAVLTree<Key, Value>::preOrder() const {
	std::vector<Key> foo;
	preOrderHelper(foo, root);
	return foo; 
}

	template<typename Key, typename Value>
	void MyAVLTree<Key, Value>::preOrderHelper(std::vector<Key>& foo, Node* x) const {
		if (x == nullptr) {
			return;
		}
		foo.push_back(x->key);
		preOrderHelper(foo, x->lchild);
		preOrderHelper(foo, x->rchild);
	}


template<typename Key, typename Value>
std::vector<Key> MyAVLTree<Key, Value>::postOrder() const {
	std::vector<Key> foo;
	postOrderHelper(foo, root);
	return foo; 
}

	template<typename Key, typename Value>
	void MyAVLTree<Key, Value>::postOrderHelper(std::vector<Key>& foo, Node* x) const {
		if (x == nullptr) {
			return;
		}
		postOrderHelper(foo, x->lchild);
		postOrderHelper(foo, x->rchild);
		foo.push_back(x->key);
	}

#endif 