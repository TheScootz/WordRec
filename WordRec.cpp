// File: WordRec.cpp 
// Member function implementations for WordRec class

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include "WordRec.h"

using namespace std;

// Constructor
WordRec::WordRec(string input, int times) {
    setWord(input);
    setCount(times);
}

// Mutator: set word
void WordRec::setWord(string input) {
    word = input;
}

// Inspector: get word
string WordRec::getWord() const {
    return word;
}

// Inspector: get count
int WordRec::getCount() const {
    return count;
}

// Mutator: preincrement operator
WordRec &WordRec::operator++() {
    count++;
    return *this;
}

// Mutator: postincrement operator
WordRec WordRec::operator++(int) {
    WordRec temp = *this;
    count++;
    return temp;
}

// Inspector: less than operator compares WORDS, not counts
bool WordRec::operator<(const WordRec &right) const {
    return word < right.getWord();
}

// Inspector: equals operator compares WORDS, not counts
bool WordRec::operator==(const WordRec &right) const {
    return word == right.getWord();
}
// Inspector: comparison with a string
bool WordRec::operator==(string &right) {
    return word == right;
}

// Mutator: set count
void WordRec::setCount(int input) {
    count = input;
}

// Inspector: insertion operator outputs both the word and count
ostream &operator<<(ostream &out, const WordRec &right) {
    cout << setw(10) << left << right.getWord() << " " << right.getCount();
    return out;
}

// Mutator: extraction operator takes the word
ifstream &operator>>(ifstream &inf, WordRec &right) {
    string input;
    inf >> input;
    right.setWord(input);
    return inf;
}