#include "hashindexer.h"
#include "document.h"
#include "word.h"
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <mutex>
#include <codecvt>
#include <fstream>
#include <parser.h>
using namespace std;

HashIndexer::HashIndexer(){
}
HashIndexer::~HashIndexer(){
    table.clear();
}
int HashIndexer :: type(){
    return 0;
}

void HashIndexer::addWord(unordered_multiset<wstring>& input, Document* doc){
    auto i = input.begin();
    for(i; i != input.end(); i++){
        wstring inputWord = *i;
        int counter = input.count(inputWord);
        for(int j=0; j<counter-1; j++){
            i++;
        }
        //check for duplicates
        if(table.find(inputWord) != table.end()){
            //duplicate exists
            table[inputWord]->duplicate(doc, counter);
        }
        else{
            word* temp = new word(inputWord, doc, counter);
            table.emplace(inputWord, temp);
        }
    }
}
unordered_multimap<double, Document *> HashIndexer::searchIndex(wstring target){
    return table.find(target)->second->getDocList();
}
word* HashIndexer::basicSearch(wstring target){
    auto searchResult =  table.find(target);
    if(searchResult == table.end()){
        return NULL;
    }
    else{
        return searchResult->second;
    }
}

void HashIndexer::printIndex(){
    auto current = table.begin();
    for(int i=0; i<table.size(); i++){
        wcout << current->first << " " << current->second->getTotRefCount() << endl;
        current++;
    }
}

void HashIndexer :: writeIndex(){
    ofstream fout("/Users/ndantonelli/Desktop/EngineFiles/PersistentIndexes/Hash");
    fout << table.size();
    for(auto i = table.begin(); i != table.end(); i++){
        wstring word = i-> first;
        typedef std::codecvt_utf8<wchar_t> convert_typeX;
        std::wstring_convert<convert_typeX, wchar_t> converterX;
        string someword = converterX.to_bytes(word);
        fout << '\n' << someword << '~';
        i->second -> writeDocs(fout);
        fout << '~';
        i->second->writeCounts(fout);
        fout << '~' << i -> second -> getTotRefCount() <<  '~' << i -> second -> getDocRefCount();
    }
    fout.close();
}

void HashIndexer :: readInIndex(Parser* p){
    ifstream fin("/Users/ndantonelli/Desktop/EngineFiles/PersistentIndexes/Hash");
    if (!fin.is_open()){
        cout << "Error reading in the persistent index";
        return;
    }
    int size;
    fin >> size;
    for (int i = 0; i < size; i++){
        string someword;
        getline(fin, someword, '~');
        someword.erase(0,1);
        typedef std::codecvt_utf8<wchar_t> convert_typeX;
        std::wstring_convert<convert_typeX, wchar_t> converterX;
        wstring cool = converterX.from_bytes(someword);
        word* temp = new word(cool, fin, p);
        table.emplace(cool, temp);
    }
    return;
}

void HashIndexer :: clearIndex(){
    table.clear();
    return;
}
