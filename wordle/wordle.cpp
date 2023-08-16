#include "dict.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;
enum class MatchValue : int { Exact = 0, Partial = 1, None = 2 };
class Match {
	MatchValue matches[5];
	int16_t matchVal = 0;

public:
	Match() {}
	static Match fromAnswer(string answer) {
		Match m = Match();
		for (int i = 0; i < 5; i++) {
			switch (answer[i]) {
			case 'G':
				m.matches[i] = MatchValue::Exact;
				break;
			case 'Y':
				m.matches[i] = MatchValue::Partial;
				break;
			case 'N':
				m.matches[i] = MatchValue::None;
				break;
			default:
				m.matches[i] = MatchValue::None;
				break;
			}
			m.matchVal += ((int)m.matches[i]) * pow(3, i);
		}
		return m;
	}
	static Match fromValue(int16_t val) {
		Match m;
		m.matchVal = val;
		for (int i = 0; i < 5; i++) {
			m.matches[i] = (MatchValue)((val / (int)pow(3, i)) % 3);
		}
		return m;
	}
	Match(string guess, string truth) {
		for (int i = 0; i < 5; i++) {
			matches[i] = MatchValue::None;
		}
		if (guess.length() != 5 || truth.length() != 5) {
			cout << "Words must be 5 characters long" << endl;
			return;
		}
		for (int i = 0; i < 5; i++) {
			if (guess[i] == truth[i]) {
				matches[i] = MatchValue::Exact;
				truth[i] = ' ';
				guess[i] = ' ';
			}
		}
		for (int i = 0; i < 5; i++) {
			if (guess[i] == ' ') {
				continue;
			}
			size_t matchIndex = truth.find(guess[i]);
			if (matchIndex != string::npos) {
				matches[i] = MatchValue::Partial;
				truth[matchIndex] = ' ';
				guess[i] = ' ';
			}
		}
		for (int i = 0; i < 5; i++) {
			matchVal += ((int)matches[i]) * pow(3, i);
		}
	}
	friend ostream& operator<<(ostream& os, const Match& match) {
		for (int i = 0; i < 5; i++) {
			switch (match.matches[i]) {
			case MatchValue::Exact:
				os << "G";
				break;
			case MatchValue::Partial:
				os << "Y";
				break;
			case MatchValue::None:
				os << "N";
				break;
			default:
				os << "?";
				break;
			}
		}
		return os;
	}
	bool operator==(const Match& other) const {
		return matchVal == other.matchVal;
	}
	int16_t getMatchVal() const {
		return matchVal;
	}
};
int findBestIndex() {
	int highestIndex = 0;
	double highestAvg = 0;
	if (dictionary.size() == 1) {
		return (std::find(words.begin(), words.end(), dictionary[0]) -
				words.begin());
	}
	for (size_t i = 0; i < words.size(); i++) {
		// 3^5 = 243
		int matchesFor[243] = {0};
		for (size_t x = 0; x < dictionary.size(); x++) {
			Match m = Match(words[i], dictionary[x]);
			matchesFor[m.getMatchVal()]++;
		}
		// calculate average reduction in dictionary size
		double avg = 0;
		for (int x = 0; x < 243; x++) {
			avg += (matchesFor[x] / 243.0) *
			       (dictionary.size() - matchesFor[x]) / 243.0;
		}
		// the second condition gives a little weight to words that might be the
		// solution this is more useful in the endgame bc we might get lucky
		if (avg > highestAvg ||
			(avg >= highestAvg &&
				std::find(dictionary.begin(), dictionary.end(), words[i]) !=
					dictionary.end())) {
			highestAvg = avg;
			highestIndex = i;
		}
	}
	return highestIndex;
}
int solveFor(string word) {

	string guess = "";
	int i = 0;
	while (true) {
		int guessIndex = findBestIndex();
		guess = words[guessIndex];
		// if (guess == actualWord) {
		// 	cout << "Found word: " << guess << endl;
		// 	break;
		// }
		cout << "Guess: " << guess << endl;
		Match m;
		if (word == "") {
			cout << "Enter answer: ";
			string answer;
			cin >> answer;
			m = Match::fromAnswer(answer);
			if (answer == "GGGGG") {
				cout << "Found word: " << guess << endl;
				return i + 1;
			}
		} else {
			m = Match(guess, word);
			if (m == Match::fromAnswer("GGGGG")) {
				// cout << "Found word: " << guess << endl;
				return i + 1;
			}
		}
		cout << "Match: " << m << endl;
		vector<string> newDictionary;
		int dictSize = dictionary.size();
		for (size_t i = 0; i < dictionary.size(); i++) {
			if (Match(guess, dictionary[i]) == m) {
				newDictionary.push_back(dictionary[i]);
			}
		}
		dictionary = newDictionary;
		cout << "New dictionary size: " << dictionary.size()
			 << ", old dictionary size: " << dictSize << endl;
		i++;
		if (i > 6) {
			cout << "I give up" << endl;
			return 7;
		}
	}
	return 7;
}
int main() {
	srand(time(NULL));
	makeDictionary();
	solveFor("");
	// string actualWord = dictionary[rand() % dictionary.size()];
	// cout << "Actual word: " << actualWord << endl;
	cout << "Press enter to exit..." << endl;
	string s = "";
	cin >> s;
}
