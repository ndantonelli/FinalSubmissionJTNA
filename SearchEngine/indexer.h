/*Owner: Josh Thomas
 * Revision History:
 * 11/12 - file created
 * 11/13 - addWord functionality written
 * 12/1 - converted containers from vectors to unordered_maps
 * 12/1 - Added the reading writing and clearing capabilities
 *  all virtual so every subclass can implement their own
 * 12/3 - added a type indicator and made printing the index virtual
*/
#ifndef INDEXER_H
#define INDEXER_H
class word;
class Document;
class Parser;
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
using namespace std;

// ******************PUBLIC OPERATIONS*********************
// void addWord(unordered_multiset, Document*)   --> Inserts the words stored in the multiset into the Index.
//                                                   Adds the document to the document list for each word
//unordered_multimap searchIndex(wstring)        --> Searches the Index for the wstring.
//                                                   Returns a multimap of the documents stored in the word object
//word* basicSearch(wsting)                      --> Searches the Index for the wstring.
//                                                   Returns the word object that is found
//void clearIndex()                              --> Clears the index
//void printIndex()                              --> Prints the contents of the Index to the terminal
//void readInindex(Parser*)                      --> Re-Creates a new Index from a file
//void writeIndex()                              --> Writes the contents of the Index out to a file
// int type()                                    --> Returns what type of Index it is (0 for HashTable, 1 for AVL Tree)
//Indexer* getDS(string)                         --> Determines which type of index to create based on the string input

class Indexer{
public:
    Indexer();
    ~Indexer();
    virtual void writeIndex();
    virtual void clearIndex();
    virtual void addWord(unordered_multiset<wstring>&, Document*);
    virtual unordered_multimap<double, Document*> searchIndex(wstring);
    virtual word* basicSearch(wstring);
    Indexer* getDS(string);
    virtual void printIndex();
    virtual void readInIndex(Parser*);
    virtual int type();
private:
    Indexer * idx;

};

#endif // INDEXER_H
