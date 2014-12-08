#include "QueryEngine.h"
#include "locale"
#include "codecvt"
#include "document.h"
#include "english_stem.h"
#include "stemming.h"
#include "word.h"
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include "math.h"
using namespace std;

QueryEngine::QueryEngine(){
}

unordered_multimap<double, Document*> QueryEngine::search(wstring input, Indexer* idx){
    wstring boolean;
    wstring notBool;
    vector<wstring> words;
    vector<wstring> exclude;
    wstring temp;
    wistringstream iss(input);
    bool notExists = false;
    while(!iss.eof()){
        iss >> temp;
        if(temp == L"AND"){
            boolean = temp;
        }
        else if(temp == L"OR"){
            boolean = temp;
        }
        else if(temp == L"NOT"){
            notBool = temp;
            notExists = true;
        }
        else{
            if(!notExists){
                words.push_back(temp);
            }
            else {
                exclude.push_back(temp);
            }
        }
    }
    //stems the search words
    stemming::english_stem<> englishStemmer;
    for(int i=0; i<words.size(); i++){
        englishStemmer(words[i]);
    }
    //deals with searches of only one word
    if(words.size() == 1){
        searchSingle(words[0], idx);
    }
    else{
        //deals with searches that do not have a NOT component
        if(!notExists){
            output = searchMultiple(words, boolean, idx);

        }
        //deals with searches that have a NOT compnent
        else{
            //stems the words that follow the NOT command
            for(int i=0; i<exclude.size(); i++){
                englishStemmer(exclude[i]);
            }
            //performs the set operations on the searched words
            //vector<Document*> docs = searchMultiple(words, boolean, idx);
            //vector<Document*> notDocs = searchMultiple(exclude, notBool, idx);
            unordered_multimap<double, Document*> docs = searchMultiple(words, boolean, idx);
            unordered_multimap<double, Document*> notDocs = searchMultiple(words, notBool, idx);
            NOT(notDocs, docs);
            output = docs;
        }
    }
    //outputDocuments();
    return output;
}

unordered_multimap<double, Document*> QueryEngine::searchMultiple(vector<wstring> input, wstring modifier, Indexer* idx){
    unordered_multimap<double, Document*> result = searchDouble(input[0], input[1], modifier, idx);
    for(int i=2; i<input.size(); i++){
        if(modifier == L"AND"){
            AND(idx->basicSearch(input[i])->getDocList(), result);
        }
        else if(modifier == L"OR"){
            OR(idx->basicSearch(input[i])->getDocList(), result);
        }
        else{
            NOT(result, idx->basicSearch(input[i])->getDocList());
        }
    }
    return result;
}

void QueryEngine::searchSingle(wstring target, Indexer* idx){
    output = rankDoc(idx->searchIndex(target), idx->basicSearch(target));
}

unordered_multimap<double, Document*> QueryEngine::searchDouble(wstring first, wstring second, wstring modifier, Indexer* idx){
    unordered_multimap<double, Document*> resultFirst = rankDoc(idx->searchIndex(first), idx->basicSearch(first));
    unordered_multimap<double, Document*> resultSecond = rankDoc(idx->searchIndex(second), idx->basicSearch(second));
    if(modifier == L"AND"){
        AND(resultFirst, resultSecond);
        return resultSecond;
    }
    else if(modifier == L"OR"){
        OR(resultFirst, resultSecond);
        return resultSecond;
    }
    else if(modifier == L"NOT"){
        NOT(resultFirst, resultSecond);
        return resultFirst;
    }
}

void QueryEngine::basicSearch(wstring target, Indexer* idx){
    word* result = idx->basicSearch(target);
    if(result == NULL){
        cout << "Search Returned No Results" << endl;
    }
    else{
        wcout << "Search for \"" << target << "\" was successful" << endl;
        cout << "This word occured: " << result->getTotRefCount() << " times in total." << endl;
    }
}

unordered_multimap<double, Document*> QueryEngine::rankDoc(unordered_multimap<double, Document*> docList, word* target){
    /*unordered_map<int, Document*> docMap;
    int counter = 0;
    int pos = 0;
    auto iteratorPos = docList.begin();
    double greatest = -1.0;
    unordered_set<double> prevNum;

    while(counter < docList.size()){
        auto j = docList.begin();
        for(int i=0; i<docList.size(); i++){
            //checks to see if the ranking has already been put into the returnList
            if(prevNum.find(target->getRank(i)) == prevNum.end()){
                if(greatest < target->getRank(i)){
                    greatest = target->getRank(i);
                    pos = i;
                    iteratorPos = j;
                }
            }
            //accounts for different documents with the same ranking value by searching the docMap for a
            //document with the same docNum. If no such docment exists, the document that was searched for
            //is added to the docMap and rankMap
            else if(docMap.find(j->first) == docMap.end()){
                docMap.emplace(j->first, j->second);
                //returnList.push_back(docList[i]);
                rankMap.emplace(target->getRank(i), j->second);
            }
            j++;
        }
        docMap.emplace(iteratorPos->second->getDocNum(), iteratorPos->second);
        //returnList.push_back(docList[pos]);
        rankMap.emplace(target->getRank(pos), iteratorPos->second);
        cout << "Document: " << iteratorPos->second->getTitle() << endl;
        //cout << "rankMap: " << rankMap[target->getRank(pos)]->getTitle() << endl << endl;
        prevNum.emplace(greatest);
        greatest = 0;
        pos = 0;
        counter++;
    }*/

    unordered_multimap<double, Document*> rankMap;
    target->setRank();
    int j = 0;
    for(auto i=docList.begin(); i != docList.end(); i++){
        rankMap.emplace(target->getRank(j), i->second);
        j++;
    }

    return rankMap;
}

void QueryEngine::AND(unordered_multimap<double, Document*> input1, unordered_multimap<double, Document*>& input2){
    //removes all the elements from input2 that are not also in input1
    for(auto i=input2.begin(); i != input2.end(); i++){
        if(input1.find(i->first) == input1.end()){
            input2.erase(i);
        }
    }
}

void QueryEngine::OR(unordered_multimap<double, Document*> input1, unordered_multimap<double, Document*>& input2){
    unordered_multimap<double, Document*> result;
    auto temp1 = input1.begin();
    auto temp2 = input2.begin();
    for(int i=0; i<15;i++){
        if(temp1->first > temp2->first){
            result.emplace(temp1->first, temp1->second);
        }
        else{
            result.emplace(temp2->first, temp2->second);
        }

        temp1++;
        temp2++;
        if(temp1 == input1.end() || temp2 == input2.end()){
            break;
        }
    }
    //swaps the contents of result witht the contents of input 2
    //this makes input2 now contain the proper documents
    result.swap(input2);
}

void QueryEngine::NOT(unordered_multimap<double, Document*>& include, unordered_multimap<double, Document*> exclude){
    for(auto i=include.begin(); i != include.end(); i++){
        if(exclude.find(i->second->getDocNum()) != exclude.end()){
            include.erase(i);
        }
    }
}
void QueryEngine::outputDocuments(){
    cout << "Search Results: " << endl;
    if(output.size() == 0){
        cout << "No results found" << endl;
    }
    else{
        auto j = output.begin();
        for(int i=0; i<output.size() && i<15; i++){
            cout << i+1 << ") " << j->second->getTitle() << endl;
            j++;
        }
    }
}
QueryEngine::~QueryEngine(){
    output.clear();
}
