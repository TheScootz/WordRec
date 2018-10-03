// File: WordRec.h
// Author: Dr. Spiegel
// Last Revision; Sept. 15, 2018
// A WordRec is a container used to track a word and its multiplicity in data 

#ifndef WORDREC_H
#define WORDREC_H
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

class WordRec {
  public:

  // Constructor
  WordRec(string word="",int times=1);

  //Sets the WordRec's data member word
  void setWord(string words);

  //Returns WordRec's word data member
  string getWord() const;
  //Returns WordRec's count data member
  int getCount() const;

  //Operator ++ overload(Pre and Post): Increments count
  WordRec &operator++();
  WordRec operator++(int);

  //Operator < overload: 
  // Returns whether a WordRec's word is alphanumerically less than another 
  //	WordRec's word
  bool operator<(const WordRec &right) const;

  //Operator == overload: 
  // Returns whether a WordRec's word is equal to another WordRec's word
  bool operator==(const WordRec &right) const;
  // Returns whether a WordRec's word is equal to a string
  bool operator==(string &right);

  private:

  //Contains a word from a file
  string word;
  //Contains the multiplicity of a word from a file
  int count;

  //Sets the WordRec data member count (why private?)
  void setCount(int counts);
};

//Operator << overload: Prints a WordRec object
ostream &operator<<(ostream &out, const WordRec &right);

//Operator >> overload: Inputs token from file into a WordRec object
ifstream &operator>>(ifstream &inf, WordRec &right);

#endif
