#include "gtest/gtest.h"
#include "MyAVLTree.hpp"
#include "proj4.hpp"
#include <string>
#include <sstream>
#include <vector>


#include <iostream>
#include <fstream>


// NOTE:  these are not intended as exhaustive tests.
// This should get you started testing.

// The four tests marked "CheckPoint" are going to be run
// on your submission for the checkpoint code.
// This is worth one-sixth of your grade on the assignment
// and is due at the time marked "checkpoint"


// None of the "checkpoint" tests require you to have
// AVL functionality OR the counting of words.
// Implementing your tree as a plain binary search
// tree is more than enough to pass these tests.

// Of course, you are expected to implement AVL functionality
// for the full project.

// BE SURE TO FULLY TEST YOUR CODE.
// This means making sure your templating is not hard-coding for 
// a specific type, that every function is called somewhere in 
// your test cases, etc. 
// There will NOT be a project 2 style redo for this;  if your 
//  code does not compile, your score will be a zero. 

namespace {

	TEST(CheckPoint, CheckPoint_FindTheRoot) {
		MyAVLTree<int, std::string> tree;
		tree.insert(5, "foo");

		EXPECT_TRUE( tree.contains(5) );
	}

	TEST(CheckPoint, CheckPoint_FindOneHop) {
		MyAVLTree<int, std::string> tree;
		tree.insert(5, "foo");
		tree.insert(10, "bar");

		EXPECT_TRUE( tree.contains(5) );
		EXPECT_TRUE( tree.contains(10) );
	}

	TEST(CheckPoint, CheckPoint_FindTwoHops) {
		MyAVLTree<int, std::string> tree;
		tree.insert(5, "foo");
		tree.insert(3, "sna");
		tree.insert(10, "bar");
		tree.insert(12, "twelve");

		EXPECT_TRUE( tree.contains(5) );
		EXPECT_TRUE( tree.contains(3) );
		EXPECT_TRUE( tree.contains(10) );
		EXPECT_TRUE( tree.contains(12) );
	}


	TEST(CheckPoint, CheckPoint_Add5) {
		MyAVLTree<int, std::string> tree;
		tree.insert(5, "foo");
		tree.insert(3, "sna");
		tree.insert(10, "bar");
		tree.insert(12, "twelve");
		tree.insert(15, "fifteen");

		EXPECT_TRUE( tree.size() == 5 );

	}

	//----------------------------------------

	TEST(PostCheckPoint, InOrderTraversal) {
		MyAVLTree<int, std::string> tree;
		tree.insert(5, "foo");
		tree.insert(3, "sna");
		tree.insert(10, "bar");
		tree.insert(12, "twelve");
		tree.insert(15, "fifteen");
		tree.insert(13, "seen");	
		//	tests RR 
		std::vector<int> trav = tree.inOrder();
		std::vector<int> expected = {3, 5, 10, 12, 13, 15};
		EXPECT_TRUE( trav == expected );
	}

	TEST(PostCheckPoint, AVLRotation1) {
		MyAVLTree<int, std::string> tree;
		tree.insert(3, "foo");
		tree.insert(6, "sna");
		tree.insert(9, "bar");
		tree.insert(12, "twelve");
		tree.insert(4, "fifteen");
		tree.insert(5, "bsar");
		tree.insert(10, "bzar");
		tree.insert(25, "bkar");
		tree.insert(1, "bacr");
		tree.insert(2, "bazxr");

		std::vector<int> trav1 = tree.inOrder();
		std::vector<int> expected = {1, 2, 3, 4, 5, 6, 9, 10, 12, 25};
		EXPECT_TRUE( trav1 == expected );

		std::vector<int> trav2 = tree.postOrder();
		std::vector<int> expected2 = {1, 3, 2, 5, 4, 9, 25, 12, 10, 6};
		EXPECT_TRUE( trav2 == expected2 );

		std::vector<int> trav3 = tree.preOrder();
		std::vector<int> expected3 = {6, 4, 2, 1, 3, 5, 10, 9, 12, 25};
		EXPECT_TRUE( trav3 == expected3 );

		EXPECT_TRUE( tree.size() == 10 );
	}

	TEST(PostCheckPoint, JackSparrow) {
		std::string quote = "I'm dishonest, and a dishonest man you can ";
		quote += "always trust to be dishonest. Honestly. It's the honest ";
		quote += "ones you want to watch out for, because you can never ";
		quote += "predict when they're going to do something incredibly... stupid.";

		std::istringstream stream( quote );

		MyAVLTree<std::string, unsigned> tree;

		countWords(stream, tree);
		EXPECT_TRUE(tree.find("dishonest") == 3);
	}

	TEST(PostCheckPoint, Hamlet) {
		std::ifstream infile;
		infile.open("/home/ics46/projects/proj4/gtest/hamletact1.txt");

		if (!infile.is_open()) {
			std::cout << "can't open" << std::endl;
		}

		MyAVLTree<std::string, unsigned> tree;

		countWords(infile, tree);

		EXPECT_TRUE(tree.find("hamlet") == 103);
		//Horatio,--or so 85
		EXPECT_TRUE(tree.find("horatio") == 85);
		EXPECT_TRUE(tree.find("lord") == 60);

		infile.close();
	}

}