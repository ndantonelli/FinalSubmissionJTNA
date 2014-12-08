/*Owner: Josh Thomas
 * Revision History:
 * 11/12 - file created
 * 11/13 - addWord functionality written
 * 12/1 - converted containers from vectors to unordered_maps
 * 12/1 - Added the reading writing and clearing capabilities
 *        all virtual so every subclass can implement their own
 * 12/3 - added a type indicator and made printing the index virtual
 * 12/7 - manipulated the AVL Tree to give bette searching results
*/
#ifndef AVL_INDEXER_H
#define AVL_INDEXER_H

#include <vector>
#include <string>
#include <unordered_set>
#include "AVL_Tree.h"
#include "document.h"
using namespace std;

// ******************PUBLIC OPERATIONS*********************
// int type()                                    --> Returns what type of Index it is (0 for HashTable, 1 for AVL Tree)
// void addWord(unordered_multiset, Document*)   --> Inserts the words stored in the multiset into the Avl Tree.
//                                                   Adds the document to the document list for each word
//unordered_multimap searchIndex(wstring)        --> Searches the Avl Tree for the wstring.
//                                                   Returns a multimap of the documents stored in the word object
//word* basicSearch(wsting)                      --> Searches the Avl Tree for the wstring.
//                                                   Returns the word object that is found
//void clearIndex()                              --> Clears the index
//void printIndex()                              --> Prints the contents of the Avl Tree to the terminal
//void readInindex(Parser*)                      --> Re-Creates a new Avl Tree from a file
//void writeIndex()                              --> Writes the contents of the Avl Tree out to a file


class AvlIndexer: public Indexer{
public:
    AvlIndexer();
    ~AvlIndexer();
    int type();
    void addWord(unordered_multiset<wstring> &, Document*);
    unordered_multimap<double, Document *> searchIndex(wstring);
    word* basicSearch(wstring);
    void clearIndex();
    void printIndex();
    void readInIndex(Parser*);
    void writeIndex();
private:
    AvlTree<word>* tree;
};

#endif // AVL_INDEXER_H
