#include "Indexer.h"
#include "word.h"
#include "parser.h"
#include "document.h"
#include "hashindexer.h"
#include "avlindexer.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;


Indexer::Indexer(){

}
Indexer::~Indexer(){

}
void Indexer :: writeIndex(){

}
void Indexer :: clearIndex(){

}

int Indexer :: type(){
    return 1000000;
}

void Indexer::addWord(unordered_multiset<wstring>&, Document*){
}
unordered_multimap<double, Document *> Indexer::searchIndex(wstring target){

}
word* Indexer::basicSearch(wstring target){

}

Indexer* Indexer::getDS(string type) {
    if(type == "hash"){
        idx = new HashIndexer();
    }
    else{
        idx = new AvlIndexer();
    }
    return idx;
}

void Indexer :: printIndex(){
    return;
}

void Indexer :: readInIndex(Parser*){

}
