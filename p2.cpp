////////////////////////////////////////////////////////////////////////////////
//
//  Author:           Emre Shively
//  Creation Date:    September 27, 2018
//  Due Date:         October 6, 2018
//  Course:           CSC136 010
//  Professor Name:   Dr. Spiegel
//  Assignment:       #2
//  Filename:         p2.cpp
//
//  Purpose:
//    This program will read a file of words separated by whitespace and
//    store them in a WordRec object array, where each object contains the word
//    and the number of times it appears in the file. The array will be
//    sorted by the lexographical order of the word.
//
////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include "WordRec.h"

// Maximum number of tokens to store
#define MAX_WORDS 5

using namespace std;


/********** PROTOTYPES **********/

void findDuplicates(ifstream &wordFile, WordRec words[], int numElements);
bool openFile(ifstream &filestream);
void outputResults(WordRec words[], int numElements);
void readSort(ifstream &wordFile, WordRec words[], int &numElements);
void removeDuplicate(WordRec words[], int &spot, int &indexMin, int &numElements);
int searchWords(WordRec words[], int numElements, string key);
void sortWords(WordRec words[], int &numElements);
void swap(WordRec&, WordRec&);
//char menuPrompt();



/********** MAIN PROGRAM **********/

int main() {
    WordRec words[MAX_WORDS];
    int numElements = 0;
    ifstream wordFile;
    
    // prompt user for filename and open the file
    // exit if the file is not found
    if (!openFile(wordFile)) {
        cout << "File failed to open." << endl;
        return 1;
    }
    
    // read until array is full or EOF. elements will be unique and sorted
    readSort(wordFile, words, numElements);
    // if there is more to read, only process duplicates of existing members
    if (numElements == MAX_WORDS && !wordFile.fail())
        findDuplicates(wordFile, words, numElements);
    outputResults(words, numElements);
    
    return 0;
}



/********** FUNCTION DEFINITIONS **********/

void findDuplicates(ifstream &wordFile, WordRec words[], int numElements) {
    int location;
    string next;
    while (wordFile >> next) {
        //cout << "next = " << next;
        location = searchWords(words, numElements, next);
        //cout << "; location = " << location << endl;
        if (location != -1) words[location]++;
    }
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  openFile
//  Description:    Ask the user for the name of the file containing words
//                  to count and open it for reading
//  Parameters:     ifstream &filestream: the filestream to use - output
//  Return Value:   bool - true if the file was successfully opened
//
////////////////////////////////////////////////////////////////////////////////
bool openFile(ifstream &filestream) {
    // gather input
    string filename;
    cout << "Enter the name of the file to read: ";
    cin >> filename;
    
    // attempt to open and return success or failure
    filestream.open(filename.c_str());
    return filestream.is_open();
}

void readSort(ifstream &wordFile, WordRec words[], int &numElements) {
    string next; // next word read from the file
    
    // keep reading and sorting until no space is available or read fail
    while (numElements < MAX_WORDS && !wordFile.fail()) {
        int i;
        for (i = numElements; i < MAX_WORDS; i++) {
            if (wordFile >> next) {
                words[i].setWord(next);
                //cout << "Inserted " << words[i].getWord() << endl;
                numElements++;
            }
        }
        
        sortWords(words, numElements);
        cout << "Sort returned numElements = " << numElements << endl;
    }
    // need another sort
    sortWords(words, numElements);
    cout << "FINAL Sort returned numElements = " << numElements << endl;
}

void removeDuplicate(WordRec words[], int &spot, int &indexMin, int &numElements) {
    // if positive, remove from the array by swapping to the end and decreasing
    // numElements. increment count for existing word
    //cout << "Duplicate: " << words[indexMin].getWord() << " " << words[spot-1].getWord() << endl;
    swap(words[indexMin], words[numElements-1]);
    words[spot-1]++;
    numElements--;
    spot--; // don't move unsorted index when finding dupe
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  searchWords
//  Description:    uses binary search on a WordRec array to find the object
//                  with the matching word
//  Parameters:     WordRec words[]: UNIQUE SORTED array containing WordRec 
//                  objects to sort - input
//                  int numElements: number of elements in the array - input
//                  string key: token to be searched for - input
//  Return Value:   int - index of matching WordRec object or -1 if not found
//
////////////////////////////////////////////////////////////////////////////////
int searchWords(WordRec words[], int numElements, string key) {
    int min = 0, max = numElements-1; // search boundaries
    int midpoint;
    
    while (min < max) {
        midpoint = floor((min + max)/2);
        //cout << "min = " << min << "; max = " << max << "; midpoint = " << midpoint << endl;
        if (words[midpoint] == key) return midpoint;
        if (words[midpoint] < key) min = midpoint + 1;
        else max = midpoint - 1;
    }
    if (words[min] == key) return min;
    else return -1;
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  sortWords
//  Description:    uses selection sort to sort an array of WordRec objects in
//                  lexographical order. when encountering duplicates, removes
//                  them from the array and increases the WordRec's count
//                  accordingly. numElements will be decremented for each dupe
//  Parameters:     WordRec words[]: array containing WordRec objects to sort
//                      - input
//                  int numElements: number of elements in the array - input
//  Return Value:   none
//
////////////////////////////////////////////////////////////////////////////////
void sortWords(WordRec words[], int &numElements) {
    int indexMin; // index of minimum word
    int spot;     // index of first unsorted element
    
    for (spot = 0; spot < numElements; spot++) {
        cout << "----- spot = " << spot << " -----" << endl;
        indexMin = spot;
        // nb: spot+1 because we don't need to compare the word with itself
        // we do however need to check the last element for duplicate
        for (int i = spot+1; i < numElements; i++)
            if (words[i] < words[indexMin]) indexMin = i;
        cout << "indexMin = " << indexMin << endl;
        
        // duplicate check
        // nb: first condition ensures the first word is not checked
        //     and also prevents a segfault
        if (spot > 0 && words[indexMin] == words[spot-1]) {
            removeDuplicate(words, spot, indexMin, numElements);
        }
        else if (indexMin != spot)
            swap(words[spot], words[indexMin]);
        cout << "numElements = " << numElements << endl;
    }
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  menuPrompt
//  Description:    prompts the user for one of five options (functions called):
//                  A)ll words with number of appearances     listAllWords()
//                  P)rint words with specific count          listWordsByCount()
//                  S)how first n chars of all words          listFirstChars()
//                  F)ind a word                              searchWords()
//                  Q)uit                                     N/A
//  Parameters:     WordRec words[]: array containing WordRec objects to sort
//                      - input
//                  int numElements: number of elements in the array - input
//  Return Value:   none
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  swap
//  Description:    swap two WordRec objects
//  Parameters:     WordRec &a: first WordRec - input/output
//                  WordRec &b: second WordRec - input/output
//  Return Value:   none
//
////////////////////////////////////////////////////////////////////////////////
void swap(WordRec &a, WordRec &b) {
    //cout << "Swapping words " << a.getWord() << " and " << b.getWord() << endl;
    WordRec temp = a;
    a = b;
    b = temp;
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  outputResults
//  Description:    display all the recorded words and how many times they
//                  appeared
//  Parameters:     WordRec words[]: array containing WordRec objects
//                      storing the data - input
//                  int numElements: number of elements in the array - input
//  Return Value:   none
//
////////////////////////////////////////////////////////////////////////////////
void outputResults(WordRec words[], int numElements) {
    // are there words?
    if (numElements == 0) {
        cout << "No words were found in the file." << endl;
        return;
    }
    
    // print header
    cout << setw(11) << left << "Word" << right << "Count" << endl;
    cout << "---------- -----" << endl;
    
    for (int i = 0; i < numElements; i++)
        cout << words[i] << endl;
}