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
#define MAX_WORDS 100

using namespace std;


/********** PROTOTYPES **********/

void findDuplicates(ifstream &wordFile, WordRec words[], int numElements);
int findLongestWord(WordRec words[], int numElements);
char menuPrompt();
bool openFile(ifstream &filestream);
void printAllWords(WordRec words[], int numElements);
bool processChoice(char choice, WordRec words[], int numElements);
void readSort(ifstream &wordFile, WordRec words[], int &numElements);
void removeDuplicate(WordRec words[], int &spot, int &indexMin, int &numElements);
int searchWords(WordRec words[], int numElements, string key);
void sortWords(WordRec words[], int &numElements);
void swap(WordRec&, WordRec&);



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
    
    // prompt for and print desired output until user chooses to quit
    while (processChoice(menuPrompt(), words, numElements));
    
    return 0;
}



/********** FUNCTION DEFINITIONS **********/

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  findDuplicates
//  Description:    reads remainder of the word file and increases WordRec
//                  counts for any duplicates found
//  Parameters:     ifstream &wordFile: the filestream to read - input/output
//                  WordRec words[]: array of WordRec objects - input/output
//                  int numElements: number of array elements - input
//  Return Value:   none
//
////////////////////////////////////////////////////////////////////////////////
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
//  Function name:  findLongestWord
//  Description:    find the length of the longest word in WordRec array
//  Parameters:     WordRec words[]: array of WordRec objects - input
//                  int numElements: number of array elements - input
//  Return Value:   int - length of the longest token in the array
//
////////////////////////////////////////////////////////////////////////////////
int findLongestWord(WordRec words[], int numElements) {
    int length = 0;
    for (int i = 0; i < numElements; i++)
        if (length < words[i].getWord().length())
            length = words[i].getWord().length();
    return length;
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  menuPrompt
//  Description:    prompts user for one of five options for use with
//                  processChoice()
//  Parameters:     none
//  Return Value:   char - option chosen (uppercase)
//
////////////////////////////////////////////////////////////////////////////////
char menuPrompt() {
    char choice;
    
    cout << "Please choose an option:" << endl
         << "A)ll words with number of appearances" << endl
         << "P)rint words with specific count" << endl
         << "S)how first n chars of all words" << endl
         << "F)ind a word" << endl
         << "Q)uit" << endl << "> ";
    
    cin >> choice;
    return toupper(choice);
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  openFile
//  Description:    prompts user for desired file and opens it for reading
//  Parameters:     ifstream &filestream: the input stream to use - output
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

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  printAllWords
//  Description:    display all the recorded words and how many times they
//                  appeared
//  Parameters:     WordRec words[]: array of WordRec objects - input
//                  int numElements: number of array elements - input
//  Return Value:   none
//
////////////////////////////////////////////////////////////////////////////////
void printAllWords(WordRec words[], int numElements) {
    // are there words?
    if (numElements == 0) {
        cout << "No words were found in the file." << endl;
        return;
    }
    
    // dynamically determine width of the word header
    int width = max(findLongestWord(words, numElements)+1, 5);
    
    // print header
    cout << endl << setw(width+1) << left << "Word" << "Count" << endl;
    for (int i = 0; i < width; i++)
        cout << '-';
    cout << " -----" << endl;
    
    for (int i = 0; i < numElements; i++)
        cout << setw(width) << left << words[i] << endl;
    cout << endl;
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  processChoice
//  Description:    processes user's menu choice from menuPrompt()
//                  relevant functions:
//                  A)ll:    printAllWords()
//                  P)rint:  printWordsWithCount()
//                  S)how:   printFirstChars()
//                  F)ind:   findWord()
//  Parameters:     char choice - user's choice of output
//                  WordRec words[]: array of WordRec objects - input
//                  int numElements: number of array elements - input
//  Return Value:   bool - false when the user chooses to quit
//
////////////////////////////////////////////////////////////////////////////////
bool processChoice(char choice, WordRec words[], int numElements) {
    switch (choice) {
        case 'A':
            printAllWords(words, numElements);
            break;
        case 'P':
            cout << "P)rint words with specific count" << endl;
            break;
        case 'S':
            cout << "S)how first n chars of all words" << endl;
            break;
        case 'F':
            cout << "F)ind a word" << endl;
            break;
        case 'Q':
            return false;
        default: // prompt again
            cout << "Invalid option chosen." << endl;
            return processChoice(menuPrompt(), words, numElements);
    }
    
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  readSort
//  Description:    reads the file and fills a WordRec in the array for each
//                  word encountered, then calls sortWords(). repeats this
//                  process until EOF or the array is full of unique elements
//  Parameters:     ifstream &wordFile: the filestream to read - input/output
//                  WordRec words[]: array of empty WordRec objects - output
//                  int &numElements: number of array elements - output
//  Return Value:   none
//
////////////////////////////////////////////////////////////////////////////////
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
        //cout << "Sort returned numElements = " << numElements << endl;
    }
    // need another sort
    sortWords(words, numElements);
    //cout << "FINAL Sort returned numElements = " << numElements << endl;
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  removeDuplicate
//  Description:    for use with sortWords(): removes encountered duplicate
//                  from the array by swapping with the last element and
//                  decreasing numElements; increments count for existing word
//  Parameters:     WordRec words[]: array of WordRec objects - input/output
//                  int &spot: index of first unsorted element
//                  int &indexMin: index of minimum unsorted word
//                  int &numElements: number of array elements - output
//  Return Value:   none
//
////////////////////////////////////////////////////////////////////////////////
void removeDuplicate(WordRec words[],int &spot,int &indexMin,int &numElements) {
    // if positive, remove 
    //cout << "Duplicate: " << words[indexMin].getWord() << " " << words[spot-1].getWord() << endl;
    swap(words[indexMin], words[numElements-1]);
    words[spot-1]++;
    numElements--;
    spot--; // offset increment done by for loop
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  searchWords
//  Description:    uses binary search on a WordRec array to find the object
//                  with the matching word
//  Parameters:     WordRec words[]: UNIQUE SORTED array containing WordRec 
//                  objects to search - input
//                  int numElements: number of array elements - input
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
//                  lexographical order, and calls removeDuplicates() for any
//                  duplicate words encountered
//  Parameters:     WordRec words[]: array of WordRec objects to sort
//                      - input/output
//                  int &numElements: number of array elements - input/output
//  Return Value:   none
//
////////////////////////////////////////////////////////////////////////////////
void sortWords(WordRec words[], int &numElements) {
    int indexMin; // index of minimum unsorted word
    int spot;     // index of first unsorted element
    
    for (spot = 0; spot < numElements; spot++) {
        //cout << "----- spot = " << spot << " -----" << endl;
        indexMin = spot;
        // nb: spot+1 because we don't need to compare the word with itself
        // we do however need to check the last element for duplicate
        for (int i = spot+1; i < numElements; i++)
            if (words[i] < words[indexMin]) indexMin = i;
        //cout << "indexMin = " << indexMin << endl;
        
        // duplicate check
        // nb: first condition ensures the first word is not checked
        //     and also prevents a segfault
        if (spot > 0 && words[indexMin] == words[spot-1]) {
            removeDuplicate(words, spot, indexMin, numElements);
        }
        else if (indexMin != spot)
            swap(words[spot], words[indexMin]);
        //cout << "numElements = " << numElements << endl;
    }
}

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