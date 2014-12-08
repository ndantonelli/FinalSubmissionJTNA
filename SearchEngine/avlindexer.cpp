#include "avlindexer.h"
#include "AVL_Tree.h"
#include "Word.h"

#include <vector>
#include <string>
#include <codecvt>
using namespace std;

AvlIndexer::AvlIndexer():Indexer(){
    tree = new AvlTree<word>();
}

int AvlIndexer :: type(){
    return 1;
}
void AvlIndexer::addWord(unordered_multiset<wstring>& input, Document* doc){
    word* buff;
    auto i = input.begin();
    for(i; i != input.end(); i++){
        wstring inputWord = *i;
        int counter = input.count(inputWord);
        //check for duplicates
        buff = &tree->findNode(inputWord);
        if(buff != NULL){
            //add document pointer to corresponding word obj
            buff->duplicate(doc, counter);
        }
        else{
            //inserts the word obj into the tree
            buff = new word(inputWord, doc, counter);
            tree->insert(*buff);
        }
    }
}
word* AvlIndexer::basicSearch(wstring target){
    word t = tree->findNode(target);
    word* temp = new word(t.getWord(), t.getDocList(), t.getDocCounts(), t.getTotRefCount(), t.getDocRefCount());
    return temp;
}

unordered_multimap<double, Document*> AvlIndexer::searchIndex(wstring target){
    return tree->findNode(target).getDocList();
}
AvlIndexer::~AvlIndexer(){
    tree->makeEmpty();
}

void AvlIndexer :: clearIndex(){
    tree -> makeEmpty();
}

void AvlIndexer :: printIndex(){
    tree ->printTree();
    return;
}

void AvlIndexer :: writeIndex(){
    tree->writeIndex();

}

void AvlIndexer :: readInIndex(Parser* p){
    ifstream fin("/Users/ndantonelli/Desktop/EngineFiles/PersistentIndexes/Hash");
    if (!fin.is_open()){
        cout << "Error reading in the persistent index";
        return;
    }
    int count;
    fin >> count;
    for (int i = 0; i < count; i++){
        string someword;
        getline(fin, someword, '~');
        someword.erase(0,1);
        typedef std::codecvt_utf8<wchar_t> convert_typeX;
        std::wstring_convert<convert_typeX, wchar_t> converterX;
        wstring cool = converterX.from_bytes(someword);
        word* temp = new word(cool, fin, p);
        tree->insert(*temp);
    }
    return;
}
