/*Owner: Josh Thomas
 * 11/12 - file created
 * 11/30 - change from vectors to unordered maps for counts and document holders
 * 12/2 - added the ifstream constructor and the writing functions for writing to disk*/
#ifndef WORD_H
#define WORD_H
#include "document.h"
#include "parser.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

// ******************PUBLIC OPERATIONS*********************
// unordered_map getDocCounts()       --> returns docCounts
//wstring getWord()                   --> returns the word
//void setWord(wstring)               --> sets the value of wordName
//void setRank()                      --> sets the Relevancy Ranking of each document
//void addDocument(Document*, int)    --> adds a document to the list of referenced documents
//void duplicate(Document*, int)      --> handles dupplicate additions of a document
//unordered_multimap getDocList()     --> returns documents
//int getDocListSize()                --> retuns the size of documents
//int getTotRefCount()                --> returns totRefCount
//int getDocRefCount()                --> returns docRefCount
//double getRank(int)                 --> retuns the Relevancy Rank of a document in relRank[int]
//void writeDocs(ofstream&)           --> writes the documents list to a file
//void writeCounts(ofstream&)         --> writes docCounts to a file

class word{

public:
    word(wstring, Document*, int);
    word(wstring, ifstream&, Parser*);
    word(wstring, unordered_multimap <double, Document*>, unordered_map < int,int> , int, int);
    unordered_map<int,int> getDocCounts();
    ~word();
    wstring getWord();
    void setWord(wstring);
    void setRank();
    void addDocument(Document*, int);
    void duplicate(Document*, int);
    unordered_multimap<double, Document *> getDocList();
    int getDocListSize();
    int getTotRefCount();
    int getDocRefCount();
    double getRank(int);
    void writeDocs(ofstream&);
    void writeCounts(ofstream&);
    friend bool operator <(const word& left, const word& right);
private:
    wstring wordName;
    unordered_multimap<double, Document*> documents;//contains the list of documents that the word appears in
    unordered_map<int, int> docCounts;//contains the number of times the word appears in each document
    vector<double> relRank;//contains the relevancy ranking for each document
    int totRefcount; //number of times the word is referenced in the entire XML file
    int docRefCount; //keeps track of how many documents the word is referenced in
};

#endif // WORD_H
