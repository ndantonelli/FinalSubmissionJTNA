/* OWNER:  Nick Antonelli
REVISION HISTORY:
11/16 - created the structure for the stop words and stemmed words to be compared
11/18 - set up first attempt at parsing the document, parse dump -> parse page -> parse text -> parse space
11/19 - added stemming abilities
11/20 - removed parsing of text and simply parsed everything based on spaces
11/21 - minor changes to the parsing algorithm to speed up the processing time
11/28 - added an unorderd set of words in the english dictionary to compare against
11/28 - added the read in functions
12/3 - added userinterface functionality aspects like change index and get doc pointer and clear docs and pull the index*/
#ifndef PARSER_H
#define PARSER_H
class Document;
#include "indexer.h"
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;


// ******************PUBLIC OPERATIONS*********************
// writeDocs( )                  --> write the vector of documents to disk
// pullIndex( )                  --> reads in the index from a persistent index
// parseDump(string)             --> parse entire XML dump
// parsePage(string)             --> determine important information on a single page
// parseSpace(string, multiset)  --> return list of words to be input into index
// changeIndex(Indexer)          --> change indexer to current indexer in engine control
// Document* getDocPointer(int)  --> return specific document in list
// void readInDocs()             --> read in the vector of docs from disk
// void clearDocs()              --> deletes all docs in the vector, resets parsed counter



class Parser
{
public:
    Parser(Indexer*);
    void writeDocs();
    void pullIndex();
    void parseDump(string&);
    void parsePage(string&);
    bool parseSpace(string&, unordered_multiset<wstring>&);
    void changeIndex(Indexer*);
    Document* getDocPointer(int);
    void readInDocs();
    void clearDocs();
    ~Parser();
private:
    Indexer* avl;
    vector<Document*> docs;//holds all document pointers needed by the index
    int docsParsed;
    unordered_map <string, wstring> stemmedWords;//map of words that have previously been stemmed so restemming is not necessary
    unordered_set <string> englishWords;//list of quickly referencable english words
    unordered_set <string> stopWords;//list of quickly referencable useless words not to be indexed
};

#endif // PARSER_H
