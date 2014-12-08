/*Owner: Josh Thomas
 * Revision Histor:
 * 11/27 - file created
 * 12/2 - converted containers from vectors to unordered_maps
 * 12/7 - converted containers from unorderd_maps to unordered_multimaps
*/
#ifndef QUERYENGINE_H
#define QUERYENGINE_H

#include "document.h"
#include "indexer.h"

#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

// ******************PUBLIC OPERATIONS*********************
// unordered_multimap search(wsting, Indexer*)   --> Accepts the origional search phrase (eg."AND boston seattle")
//                                                   Calls other Query Engine functions to rank and search for the query
//void searchSingle(wsting, Indexer*)            --> Handles searches that are one word long
//                                                   Accepts the word that is searched for as a wstring
//unordered_multimap searchDouble(...)           --> Accepts the two search terms and a boolean
//                                                   Calls AND, OR, NOT functions based on boolean value
//unordered_multimap searchMultiple(...)         --> Accepts a vector of search terms, a boolean, and an Indexer*
//                                                   Performs boolean prefixed searches for all words in the vector
//void basicSearch(wstring, Indexer*)            --> Prints out whether or not the searched term was found in the index
//unordered_multimap rankDoc(...)                --> Orders the documents in the multimap according to their Relevancy Ranking
//void AND(multimap, multimap&)                  --> Manipulates the second multimap so that it contains only the documents
//                                                   that are common to both sets of documents
//void OR(multimap, multimap&)                   --> Manupulates the second multimap so that it contains a list of the
//                                                   documents witht the highest relevancy rankings between the two sets
//void NOT(multimap&, multimap)                  --> Manipulates the first multimap so that it contains only the documents
//                                                   that are found in the first set, but are not in both sets
//void outputDocuments()                         --> Prints the result list of documents to the terminal


class QueryEngine{
public:
    QueryEngine();
    ~QueryEngine();
    unordered_multimap<double, Document*> search(wstring, Indexer*);
    void searchSingle(wstring, Indexer*);
    unordered_multimap<double, Document *> searchDouble(wstring, wstring, wstring, Indexer*);
    unordered_multimap<double, Document *> searchMultiple(vector<wstring>, wstring, Indexer*);
    void basicSearch(wstring, Indexer*);
    unordered_multimap<double, Document *> rankDoc(unordered_multimap<double, Document *>, word*);
    void AND(unordered_multimap<double, Document *>, unordered_multimap<double, Document *> &);
    void OR(unordered_multimap<double, Document *>, unordered_multimap<double, Document *> &);
    void NOT(unordered_multimap<double, Document *> &, unordered_multimap<double, Document *>);
    void outputDocuments();
private:
    unordered_multimap<double, Document*> output; //Holds the ordered list of ranked documents
                                                  //that are the result of the search
                                                  //The double is the relRank of each document
                                                  //The Document* is a pointer to each document
};

#endif // QUERYENGINE_H
