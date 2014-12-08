class Document;
#include "Word.h"
#include "parser.h"
#include "math.h"
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <fstream>
using namespace std;

word::word(wstring inputName, Document* doc, int counter){
    totRefcount = counter;
    docRefCount = 1;
    wordName = inputName;
    documents.emplace(doc->getDocNum(), doc);
    docCounts.emplace(doc->getDocNum(), counter);
}

word::word(wstring words, ifstream& fin, Parser* p){
    wordName = words;
    string size;
    getline(fin, size, ',');
    int counter = atoi(size.c_str());
    for (int i = 0; i < counter; i++){
        if(counter - i == 1){
            getline(fin, size, '~');
            int docNum = atoi(size.c_str());
            Document* temp = p->getDocPointer(docNum);
            documents.emplace(docNum, temp);
        }
        else{
            getline(fin, size, ',');
            int docNum = atoi(size.c_str());
            Document* temp = p->getDocPointer(docNum);
            documents.emplace(docNum, temp);
        }
    }
    getline(fin, size, ',');
    counter = atoi(size.c_str());
    for (int i = 0; i < counter; i++){
        getline(fin, size, '-');
        int docNum = atoi(size.c_str());
        if (counter - i == 1){
            getline(fin, size, '~');
            int docCount = atoi(size.c_str());
            docCounts.emplace(docNum, docCount);
        }
        else{
            getline(fin, size, ',');
            int docCount = atoi(size.c_str());
            docCounts.emplace(docNum, docCount);
        }
    }
    getline(fin, size, '~');
    counter = atoi(size.c_str());
    totRefcount = counter;
    fin >> counter;
    docRefCount = counter;
    return;
}

word :: word(wstring name, unordered_multimap<double, Document*> docs, unordered_map <int, int> counts, int tot, int docRef){
    wordName = name;
    documents = docs;
    docCounts = counts;
    totRefcount = tot;
    docRefCount = docRef;
}

unordered_map<int,int> word :: getDocCounts(){
    return docCounts;
}

wstring word::getWord(){
    return wordName;
}
void word::setWord(wstring input){
    wordName = input;
}
void word::setRank(){
    auto j = docCounts.begin();
    for(int i=0; i<documents.size(); i++){
        double thes = log(178000.0/(1+documents.size()));
        //We were unable to calculate the total number of words in each document,
        //so we used a constant value instead
        thes*= 10.0*(j->second)/500;
        relRank.push_back( thes );
        j++;
    }
}
void word::addDocument(Document* doc, int counter){
    totRefcount += counter;
    documents.emplace(doc->getDocNum(), doc);
    docRefCount += counter;
    docCounts.emplace(doc->getDocNum(), counter);
}
void word::duplicate(Document* doc, int counter){
    totRefcount += counter;

    //if a document witht the same document number exists
    if(documents.count(doc->getDocNum()) != 0){
        docCounts[doc->getDocNum()] += counter;
    }
    else{
        documents.emplace(doc->getDocNum(), doc);
        docCounts.emplace(doc->getDocNum(), counter);
        docRefCount++;
    }
}
bool operator <(const word& left, const word& right){
    if(left.wordName.compare(right.wordName) == 0){
        //equal
        return false;
    }
    else if(left.wordName.compare(right.wordName) < 0){
        //left is smaller
        return true;
    }
    else{
        //left is bigger
        return false;
    }
}

unordered_multimap<double, Document *> word::getDocList(){
    return documents;
}
int word::getTotRefCount(){
    return totRefcount;
}
int word::getDocListSize(){
    return documents.size();
}
int word:: getDocRefCount(){
    return docRefCount;
}
double word::getRank(int i){
    return relRank[i];
}
void word :: writeDocs(ofstream& fout){
    fout << documents.size();
    for (auto i = documents.begin(); i != documents.end(); i++){
        fout << ',' << to_string(i->first);
    }
}
void word :: writeCounts(ofstream& fout){
    fout << docCounts.size();
    for (auto i = docCounts.begin(); i != docCounts.end(); i++){
        fout << ',' << i -> first << '-' << i -> second;
    }
}
word::~word(){
    documents.clear();
}
