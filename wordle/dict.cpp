#include "dict.h"
#include <fstream>
vector<string> dictionary = {};
vector<string> words = {};
void makeDictionary() {
	ifstream file("wordle/wordle-La.txt");
	string word;
	while (getline(file, word)) {
		dictionary.push_back(word);
		words.push_back(word);
	}
	file.close();
}
