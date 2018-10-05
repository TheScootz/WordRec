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
//    sorted by the lexographical order of the word. The user will be able to
//    make various queries about the stored words, such as showing their counts,
//    what words appeared N times, searching for a word, etc.
//
////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <fstream>
#include <limits>
#include <iomanip>
#include <iostream>
#include <string>
#include "WordRec.h"

// Maximum number of tokens to store
#define MAX_WORDS 100
// Chars specifying menu options
#define OPT_ALLWORDS 'A' // print all words
#define OPT_COUNT 'P' // print words with specific count
#define OPT_FIRST 'S' // print first chars of all words
#define OPT_FIND 'F' // search for a word
#define OPT_QUIT 'Q' // exit the program
// Minimum width when printing words
#define MIN_WIDTH 5

using namespace std;


/**************** PROTOTYPES ****************/

/******** FILE PROCESSING ********/

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  openFile
//  Description:    prompts user for desired file and opens it for reading
//  Parameters:     ifstream &filestream: the input stream to use - output
//  Return Value:   bool - true if the file was successfully opened
//
////////////////////////////////////////////////////////////////////////////////
bool openFile(ifstream &filestream);

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  readSort
//  Description:    reads each word into a WordRec in the array until the array
//                  is full or EOF. counts how many times each word appeared.
//                  the resulting array will have unique elements and be
//                  sorted, lexicographically
//  Parameters:     ifstream &wordFile: the filestream to read - input/output
//                  WordRec words[]: array of empty WordRec objects - output
//  Return Value:   int - size of the array
//
////////////////////////////////////////////////////////////////////////////////
int readSort(ifstream &wordFile, WordRec words[]);

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  sortWords
//  Description:    sorts an array of WordRec objects in lexographical order,
//                  removes duplicates, and counts number of word appearances
//  Parameters:     WordRec words[]: array of WordRec objects to sort
//                      - input/output
//                  int &numElements: number of array elements - input/output
//  Return Value:   none
//
////////////////////////////////////////////////////////////////////////////////
void sortWords(WordRec words[], int &numElements);

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  swap
//  Description:    swap two WordRec objects
//  Parameters:     WordRec &a: first WordRec - input/output
//                  WordRec &b: second WordRec - input/output
//  Return Value:   none
//
////////////////////////////////////////////////////////////////////////////////
void swap(WordRec&, WordRec&);

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  removeDuplicate
//  Description:    removes a duplicate word from the array, and decrements
//                  numElements
//  Parameters:     WordRec words[]: array of WordRec objects - input/output
//                  int &spot: index of first unsorted element - input
//                  int &indexMin: index of minimum unsorted word - input
//                  int &numElements: number of array elements - output
//  Return Value:   none
//
////////////////////////////////////////////////////////////////////////////////
void removeDuplicate(WordRec words[], int &spot, int &indexMin, int &numElements);

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  findDuplicates
//  Description:    searches the rest of the file for any words matching a
//                  WordRec in the array and increments its counter
//  Parameters:     ifstream &wordFile: the filestream to read - input/output
//                  WordRec words[]: array of WordRec objects - input/output
//                  int numElements: number of array elements - input
//  Return Value:   none
//
////////////////////////////////////////////////////////////////////////////////
void findDuplicates(ifstream &wordFile, WordRec words[], int numElements);

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  searchWords
//  Description:    searches a sorted WordRec array for a matching object
//  Parameters:     WordRec words[]: UNIQUE SORTED array containing WordRec 
//                  objects to search - input
//                  int numElements: number of array elements - input
//                  string key: word to be searched for - input
//  Return Value:   int - index of matching WordRec object or -1 if not found
//
////////////////////////////////////////////////////////////////////////////////
int searchWords(WordRec words[], int numElements, string key);


/******** USER QUERY PROCESSING ********/

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  menuPrompt
//  Description:    presents menu to user and prompts for choice
//  Parameters:     none
//  Return Value:   char - option chosen (uppercase)
//
////////////////////////////////////////////////////////////////////////////////
char menuPrompt();

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  processChoice
//  Description:    throws user's menu choice to the relevant output function
//  Parameters:     char choice - user's choice of output
//                  WordRec words[]: array of WordRec objects - input
//                  int numElements: number of array elements - input
//  Return Value:   bool - false when the user chooses to quit
//
////////////////////////////////////////////////////////////////////////////////
bool processChoice(char choice, WordRec words[], int numElements);

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  printAllWords
//  Description:    prints all recorded words and how many times they appeared
//  Parameters:     WordRec words[]: array of WordRec objects - input
//                  int numElements: number of array elements - input
//  Return Value:   none
//
////////////////////////////////////////////////////////////////////////////////
void printAllWords(WordRec words[], int numElements);

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  printWordsWithCount
//  Description:    prints all words which have a certain count specified by
//                  the user
//  Parameters:     WordRec words[]: array of WordRec objects - input
//                  int numElements: number of array elements - input
//  Return Value:   none
//
////////////////////////////////////////////////////////////////////////////////
void printWordsWithCount(WordRec words[], int numElements);

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  printFirstChars
//  Description:    prints all recorded words and how many times they appeared,
//                  truncated to the number of characters specified by the user
//  Parameters:     WordRec words[]: array of WordRec objects - input
//                  int numElements: number of array elements - input
//  Return Value:   none
//
////////////////////////////////////////////////////////////////////////////////
void printFirstChars(WordRec words[], int numElements);

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  findWord
//  Description:    prompts the user for a word and tells them if there is a
//                  matching WordRec in the array
//  Parameters:     WordRec words[]: array of WordRec objects - input
//                  int numElements: number of array elements - input
//  Return Value:   none
//
////////////////////////////////////////////////////////////////////////////////
void findWord(WordRec words[], int numElements);

/******** SUPPLEMENTARY ********/

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  findLongestWord
//  Description:    finds the length of the longest word in the WordRec array
//  Parameters:     WordRec words[]: array of WordRec objects - input
//                  int numElements: number of array elements - input
//  Return Value:   int - length of the longest word in the array
//
////////////////////////////////////////////////////////////////////////////////
int findLongestWord(WordRec words[], int numElements);

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  getPositiveInput
//  Description:    prompts the user for a positive number and pester them until
//                  they enter one
//  Parameters:     string prompt - prompt to display to the user
//  Return Value:   int - the user's input
//
////////////////////////////////////////////////////////////////////////////////
int getPositiveInput(string prompt);

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  printHeader
//  Description:    prints the header for use with other print functions
//  Parameters:     WordRec words[]: array of WordRec objects - input
//                  int numElements: number of array elements - input
//                  int staticWidth: overrides dynamic width if positive. must
//                      be between MIN_WIDTH and the longest word length - input
//  Return Value:   int - width words should be printed with
//
////////////////////////////////////////////////////////////////////////////////
int printHeader(WordRec words[], int numElements, int staticWidth = 0);

/**************** END PROTOTYPES ****************/





/**************** MAIN PROGRAM ****************/

int main() {
    WordRec words[MAX_WORDS];
    ifstream wordFile;
    int numElements;
    
    // prompt user for filename and open the file
    // exit if the file is not found
    if (!openFile(wordFile)) {
        cout << endl << "File failed to open." << endl;
        return 1;
    }
    
    // read until array is full or EOF. elements will be unique and sorted
    numElements = readSort(wordFile, words);
    
    // check if any words were read
    if (numElements == 0) {
        cout << endl << "No words were found in the file." << endl << endl;
        return 1;
    }
    
    // if there is more to read, only process duplicates of existing members
    if (numElements == MAX_WORDS && !wordFile.fail())
        findDuplicates(wordFile, words, numElements);
    
    // prompt for and print desired output until user chooses to quit
    while (processChoice(menuPrompt(), words, numElements));
    
    return 0;
}

/**************** END MAIN PROGRAM ****************/





/**************** FUNCTION DEFINITIONS ****************/

/******** FILE PROCESSING ********/

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
//  Function name:  readSort
//  Description:    reads the file and fills a WordRec in the array for each
//                  word encountered, then calls sortWords(). repeats this
//                  process until EOF or the array is full of unique elements
//  Parameters:     ifstream &wordFile: the filestream to read - input/output
//                  WordRec words[]: array of empty WordRec objects - output
//  Return Value:   int - size of the array
//
////////////////////////////////////////////////////////////////////////////////
int readSort(ifstream &wordFile, WordRec words[]) {
    int numElements = 0;
    string next; // next word read from the file
    
    // keep reading and sorting until no space is available or read fail
    while (numElements < MAX_WORDS && !wordFile.fail()) {
        int i;
        for (i = numElements; i < MAX_WORDS; i++) {
            if (wordFile >> next) {
                words[i].setWord(next);
                numElements++;
            }
        }
        
        sortWords(words, numElements);
    }
    // need another sort
    sortWords(words, numElements);
    
    return numElements;
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
        indexMin = spot;
        // nb: spot+1 because we don't need to compare the word with itself
        // we do however need to check the last element for duplicate
        for (int i = spot+1; i < numElements; i++)
            if (words[i] < words[indexMin]) indexMin = i;
        
        // duplicate check
        // nb: first condition ensures the first word is not checked
        //     and also prevents a segfault
        if (spot > 0 && words[indexMin] == words[spot-1]) {
            removeDuplicate(words, spot, indexMin, numElements);
        }
        else if (indexMin != spot)
            swap(words[spot], words[indexMin]);
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
    WordRec temp = a;
    a = b;
    b = temp;
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  removeDuplicate
//  Description:    for use with sortWords(): removes encountered duplicate
//                  from the array by swapping with the last element and
//                  decreasing numElements; increments count for existing word
//  Parameters:     WordRec words[]: array of WordRec objects - input/output
//                  int &spot: index of first unsorted element - input
//                  int &indexMin: index of minimum unsorted word - input
//                  int &numElements: number of array elements - output
//  Return Value:   none
//
////////////////////////////////////////////////////////////////////////////////
void removeDuplicate(WordRec words[],int &spot,int &indexMin,int &numElements) {
    swap(words[indexMin], words[numElements-1]);
    words[spot-1]++;
    numElements--;
    spot--; // offset increment done by for loop
}

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
        location = searchWords(words, numElements, next);
        if (location != -1) words[location]++;
    }
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
        if (words[midpoint] == key) return midpoint;
        if (words[midpoint] < key) min = midpoint + 1;
        else max = midpoint - 1;
    }
    return (words[min] == key) ? min : -1;
}

/******** END FILE PROCESSING ********/



/******** USER QUERY PROCESSING ********/

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  menuPrompt
//  Description:    presents menu to user and prompts user for one of five
//                  options for use with processChoice()
//  Parameters:     none
//  Return Value:   char - option chosen (uppercase)
//
////////////////////////////////////////////////////////////////////////////////
char menuPrompt() {
    string input;
    char choice;
    
    cout << endl
         << "Please choose an option:" << endl
         << "A)ll words with number of appearances" << endl
         << "P)rint words that appeared a specified number of times" << endl
         << "S)how first N characters of all words" << endl
         << "F)ind a word" << endl
         << "Q)uit" << endl << "> ";
    
    cin >> choice;
    
    // clear the cin buffer (thanks stackoverflow)
    // program acts weird without this
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    return toupper(choice);
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
        case OPT_ALLWORDS:
            printAllWords(words, numElements);
            break;
        case OPT_COUNT:
            printWordsWithCount(words, numElements);
            break;
        case OPT_FIRST:
            printFirstChars(words, numElements);
            break;
        case OPT_FIND:
            findWord(words, numElements);
            break;
        case OPT_QUIT:
            return false;
        default:
            cout << "Invalid option chosen." << endl;
    }
    
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  printAllWords
//  Description:    prints all recorded words and how many times they appeared
//  Parameters:     WordRec words[]: array of WordRec objects - input
//                  int numElements: number of array elements - input
//  Return Value:   none
//
////////////////////////////////////////////////////////////////////////////////
void printAllWords(WordRec words[], int numElements) {
    int width = printHeader(words, numElements);
    
    for (int i = 0; i < numElements; i++)
        cout << setw(width) << left << words[i] << endl;
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  printWordsWithCount
//  Description:    prints all words which have a certain count specified by
//                  the user
//  Parameters:     WordRec words[]: array of WordRec objects - input
//                  int numElements: number of array elements - input
//  Return Value:   none
//
////////////////////////////////////////////////////////////////////////////////
void printWordsWithCount(WordRec words[], int numElements) {
    int input = getPositiveInput("Enter the count you are looking for: ");
    WordRec matching[MAX_WORDS];
    
    // find words
    int i, numWords = 0;
    for (i = 0; i < numElements; i++) {
        if (words[i].getCount() == input)
            matching[numWords++] = words[i];
    }
    
    // are there matching words?
    if (numWords == 0) {
        cout << endl << "No words appeared " << input << " times." << endl
             << endl;
        return;
    }
    
    // use static header
    cout << endl << "Words (" << input << " appearances)" << endl;
    cout << "-----" << endl;
    
    // print words
    for (i = 0; i < numWords; i++)
        cout << matching[i].getWord() << endl;
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  printFirstChars
//  Description:    prints all recorded words and how many times they appeared,
//                  truncated to the number of characters specified by the user
//  Parameters:     WordRec words[]: array of WordRec objects - input
//                  int numElements: number of array elements - input
//  Return Value:   none
//
////////////////////////////////////////////////////////////////////////////////
void printFirstChars(WordRec words[], int numElements) {
    int input = getPositiveInput("Number of characters to display: ");
    
    cout << "Showing the first " << input << " characters of each word" << endl;
    int width = printHeader(words, numElements, input);
    
    for (int i = 0; i < numElements; i++)
        cout << setw(width) << left << words[i].getWord().substr(0,input)
             << ' ' << words[i].getCount() << endl;
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  findWord
//  Description:    prompts the user for a word and tells them if there is a
//                  matching WordRec in the array
//  Parameters:     WordRec words[]: array of WordRec objects - input
//                  int numElements: number of array elements - input
//  Return Value:   none
//
////////////////////////////////////////////////////////////////////////////////
void findWord(WordRec words[], int numElements) {
    string input;
    cout << "Enter word to search for: ";
    cin >> input;
    
    int index = searchWords(words, numElements, input);
    cout << endl;
    if (index > -1)
        cout << "The word \"" << input << "\" was found "
             << words[index].getCount() << " times.";
    else cout << "The word \"" << input << "\" was not found in the file.";
    cout << endl << endl;
}

/******** END USER QUERY PROCESSING ********/



/******** SUPPLEMENTARY ********/

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  findLongestWord
//  Description:    finds the length of the longest word in the WordRec array
//  Parameters:     WordRec words[]: array of WordRec objects - input
//                  int numElements: number of array elements - input
//  Return Value:   int - length of the longest word in the array
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
//  Function name:  getPositiveInput
//  Description:    prompts the user for a positive number and pester them until
//                  they enter one
//  Parameters:     string prompt - prompt to display to the user
//  Return Value:   int - the user's input
//
////////////////////////////////////////////////////////////////////////////////
int getPositiveInput(string prompt) {
    int input;
    cout << prompt;
    
    while (!(cin >> input) || input <= 0) {
        cout << "Please enter a positive number: ";
        // clear the cin buffer (thanks stackoverflow)
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    return input;
}

////////////////////////////////////////////////////////////////////////////////
//
//  Function name:  printHeader
//  Description:    prints the header for use with other print functions
//  Parameters:     WordRec words[]: array of WordRec objects - input
//                  int numElements: number of array elements - input
//                  int staticWidth: overrides dynamic width if positive. must
//                      be between MIN_WIDTH and the longest word length - input
//  Return Value:   int - width words should be printed with
//
////////////////////////////////////////////////////////////////////////////////
int printHeader(WordRec words[], int numElements, int staticWidth) {
    int width; // width of word header
    int longest = findLongestWord(words, numElements);
    
    // dynamically determine width of the word header
    if (staticWidth <= 0)
        width = max(longest, MIN_WIDTH);
    else if (staticWidth < MIN_WIDTH) width = MIN_WIDTH;
    else width = min(staticWidth, longest);
    
    // print header
    cout << endl << setw(width+1) << left << "Word" << "Count" << endl;
    for (int i = 0; i < width; i++)
        cout << '-';
    cout << " -----" << endl;
    
    return width;
}

/******** END SUPPLEMENTARY ********/

/**************** END FUNCTION DEFINITIONS ****************/