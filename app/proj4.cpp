
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>

#include "proj4.hpp"

std::string keepOnlyLetters(std::string s);

// ○ This function will read words from the input stream. It will break them into given
// words. All words with the same letters, in the same order, are considered the
// same word: disregard capitalization. For example, “Bill” and “bill” are the same
// word, even if the context might imply a person named William in one case and an
// invoice in the other.
// ○ You can treat istream just like std::cin -- there is some sample code given.
// Please let me know if it is insufficient, I can explain more.
// ○ You may assume that the given tree is empty when the function is first called.
// This will certainly hold true for any test cases.
// ○ When this function returns, the tree parameter should now have a map of strings
// to non-negative integers, where the associated value with a given string key is
// the number of times that word appeared in the stream.

// This is a demo of how to read from the stream, isolate each 
// word, and convert them all to lowercase.
// While this isn't a perfect way to count words, it's what we will use 
// for this project.
// It does mean that "Bill" (in the context of a person's name) and "bill" 
// (in the context of an invoice) are counted as the same word.
// You may use or modify any of this code as you wish for this project. 
void countWords(std::istream& in, MyAVLTree<std::string, unsigned>& counter) {
	std::string line, word;
	std::stringstream ss;

	while (getline(in, line)) {

		ss.clear();
		ss << line;

		while (ss >> word) {
			
			word = keepOnlyLetters(word);
			// "" means no letters got retained. 
			if (word != "") { 

				// for each word in the stream
					// if the word has already been seen
						// retrieve the count of how many times it has been seen.
						// increment that count.
					// otherwise
						// add this to the tree with a count of 1.
				if (counter.contains(word)) {
					++counter.find(word);
				}
				else {
					counter.insert(word, 1);
					
				}
			}
			else {
				//std::cout << "Skipped because found something like a number or special characters." << std::endl; 
			}
		}
	}

}



std::string keepOnlyLetters(std::string s) {
	std::string ret = "";
	for (size_t i = 0; i < s.length(); i++) {
		// isalpha() checks if this is a letter (is in alphabet).
		if (isalpha(s[i])) { 
			ret += std::tolower(s[i]);
		}

	}
	return ret;
}



