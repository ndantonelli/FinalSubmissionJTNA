/*Owner: Josh Thomas
 * Revision History:
 * 11/12 - file created
 * 11/13 - addWord functionality written
 * 12/1 - converted containers from vectors to unordered_maps
 * 12/1 - Added the reading writing and clearing capabilities
 *        all virtual so every subclass can implement their own
 * 12/3 - added a type indicator and made printing the index virtual
*/
#ifndef HASH_INDEXER_H
#define HASH_INDEXER_H
class word;
class Document;
#include "indexer.h"
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>

using namespace std;

// ******************PUBLIC OPERATIONS*********************
// void addWord(unordered_multiset, Document*)   --> Inserts the words stored in the multiset into the Hash Table.
//                                                   Adds the document to the document list for each word
//unordered_multimap searchIndex(wstring)        --> Searches the Hash Table for the wstring.
//                                                   Returns a multimap of the documents stored in the word object
//word* basicSearch(wsting)                      --> Searches the Hash Table for the wstring.
//                                                   Returns the word object that is found
//void clearIndex()                              --> Clears the index
//void printIndex()                              --> Prints the contents of the Hash Table to the terminal
//void readInindex(Parser*)                      --> Re-Creates a new Hash Table from a file
//void writeIndex()                              --> Writes the contents of the Hash Table out to a file
// int type()                                    --> Returns what type of Index it is (0 for HashTable, 1 for AVL Tree)


class HashIndexer: public Indexer{
public:
    HashIndexer();
    ~HashIndexer();
    void addWord(unordered_multiset<wstring> &, Document*);
    unordered_multimap<double, Document*> searchIndex(wstring);
    word* basicSearch(wstring);
    void printIndex();
    void writeIndex();
    void clearIndex();
    int type();
    void readInIndex(Parser*);
private:
    unordered_map<wstring, word*> table;
};

#endif // HASH_INDEXER_H
