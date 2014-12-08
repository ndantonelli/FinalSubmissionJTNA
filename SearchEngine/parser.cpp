#include "Parser.h"
#include "english_stem.h"
#include "locale"
#include "codecvt"
#include "document.h"
#include <string>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "avlindexer.h"
#include "hashindexer.h"

using namespace std;

Parser::Parser(Indexer* theIndex){
    avl = theIndex;
    docsParsed = 0;
    //reads the list of stop words into an unordered set
    //used to compare read in word to stop words
    string word;
    ifstream fin("stopwords.txt");
    while (!fin.eof()){
        fin >> word;
        stopWords.insert(word);
    }
    fin.close();

    //reads in the list of english words
    //used to ensure only english words are present
    fin.open("englishWords.txt");
    while(!fin.eof()){
        fin >> word;
        englishWords.insert(word);
    }
    fin.close();

    return;
}
void Parser :: changeIndex(Indexer* newIndex){
    //clears the index and enters the new index type
    avl->clearIndex();
    avl = newIndex;
    return;
}

void Parser :: writeDocs(){
    ofstream fout("/Users/ndantonelli/Desktop/EngineFiles/DocumentList/list");
    fout << docs.size();
    for (int i = 0; i < docs.size(); i++){
        fout << '\n' << docs[i]->getTitle() << '~' << docs[i]->getAuthor() << '~' << docs[i]->getFileName() << '~' << docs[i]->getDate();
    }
    fout.close();
}

//Called to get the dump of XML formatted documents from disk
//puts finds the important text that makes up a single document
//sends the single document to be parsed
void Parser :: parseDump(string& files){
    //reads in all of the XML Documents from a single file at once
    string text;
    //ifstream fin("/Users/Josh/Documents/School/Data\ Structures/Search\ Engine/XMLParser/WikiDump/" + files); //josh
    ifstream fin(files.c_str()); //nick
    if(!fin.is_open()){
        cout << "issue opening input file\n";
        return;
    }
    getline(fin, text, '\0');
    fin.close();

    //finds the starting and ending position of the first page of usable text
    size_t pos = text.find("<page>");
    size_t pos2 = text.find("</page>");

    //continues finding the position of files in the string until there are no more <page> delimeters in the string
    while(pos != string :: npos){

        //creates a string of a single documents worth of words and sends the text to be parsed
        string page = text.substr(pos, (pos2 - pos) + 7);
        parsePage(page);

        //finds the positions of the next page
        pos = text.find("<page>", pos2);
        pos2 = text.find("</page>", pos);
    }

    return;
}

void Parser :: parsePage(string& thePage){
    if (docsParsed != 0){
        cout << '\r';
    }
    cout << docsParsed << "/178000";
    //sets up all needed data containers for the function
    string title, author, date, text;
    unordered_multiset<wstring> inputWords;

    //determines the position of the starting and ending tags for the title
    //addes the title to the string and parses it to be inputted into the index
    size_t pos = thePage.find("<title>");
    size_t pos2 = thePage.find("</title>");
    if (pos != string::npos){
        title = thePage.substr(pos + 7, pos2 - (pos + 7));
        parseSpace(title, inputWords);
    }
    else
        title = "Unkown";

    //determines the position of the starting and ending tags for the timestamp
    //addes the timestamp to the string
    pos = thePage.find("<timestamp>");
    pos2 = thePage.find("</timestamp>");
    if (pos != string::npos){
        date = thePage.substr(pos + 11, pos2-(pos+11));
    }
    else
        date = "Unkown";

    //determines the position of the starting and ending tags for the author
    //addes the author to the string
    pos = thePage.find("<username>");
    pos2 = thePage.find("</username>");
    if (pos != string::npos){
        author = thePage.substr(pos + 10, pos2 - (pos+10));
    }
    else
        author = "Unkown";

    //locations the position of document text and parses the text
    //if the document is usable then a document object is created
    //all of the words are added to the index
    pos = thePage.find("<text xml:space=\"preserve\">");
    if (pos != string :: npos){
        pos2 = thePage.find("</text>");
        text = thePage.substr(pos + 27, pos2 - (pos + 27));
        if (parseSpace(text, inputWords)){
            string fileName = "/Users/ndantonelli/Desktop/EngineFiles/DocText/docobj" + to_string(docsParsed);
            Document* obj = new Document(title, author, date, fileName, docsParsed);
            ofstream fout(fileName.c_str());
            fout << thePage;
            fout.close();
            docs.push_back(obj);
            avl ->addWord(inputWords, obj);
            docsParsed++;
        }
    }
    return;
}
//allows for super precise word extraction
bool my_predicate(char& c){
    if(isupper(c)){
        c = tolower(c);
    }
    return !isalpha(c) && c != '\'' && c != ' ';
}

bool Parser :: parseSpace(string& theText, unordered_multiset<wstring>& inputWords){
    int counter = 0;
    bool newWord = true;
    string word;
    //replace_if(begin(theText), end(theText), my_predicate, ' ');
    //counts through every character in the text of a document and parses out the important words
    while (counter < theText.size()){
        if(theText[counter] != ' '){
            word += tolower(theText[counter]);
            newWord = false;
        }
        else{
            newWord = true;
        }
        if(newWord){
            //continues only if the word is present in the english dictionary
            auto got = englishWords.find(word);
            if(got != englishWords.end()){
                //continues only if the word is not a stop word
                auto get = stopWords.find(word);
                if(get == stopWords.end()){
                    //stems if the word has not been stemmed before
                    //if it has been stemmed before then the stemmed word is input to be added to the index again
                    int c = stemmedWords.count(word);
                    if(c==0){
                        wstring temp;
                        wchar_t* UnicodeTextBuffer = new wchar_t[word.length()+1];
                        wmemset(UnicodeTextBuffer, 0, word.length()+1);
                        mbstowcs(UnicodeTextBuffer, word.c_str(), word.length());
                        temp = UnicodeTextBuffer;
                        stemming::english_stem<> englishStemmer;
                        englishStemmer(temp);
                        stemmedWords.emplace(word, temp);
                        inputWords.insert(temp);
                    }
                    else{
                        inputWords.insert(stemmedWords.find(word) -> second);
                    }
                }
            }
            word.clear();
        }
        counter++;
    }
    return true;
}

void Parser :: pullIndex(){
    //reads in documents first so that the index can pull pointers of the object
    readInDocs();
    avl -> readInIndex(this);
    return;
}

void Parser :: clearDocs(){
    //resets the document vector and the amount of parsed documents
    docsParsed = 0;
    docs.clear();
    return;
}


Parser :: ~Parser(){
    stemmedWords.clear();
    englishWords.clear();
    stopWords.clear();
    return;
}
//returns the pointer to a specific document object
Document* Parser :: getDocPointer(int location){
    return docs[location];
}

void Parser :: readInDocs(){
    ifstream fin("/Users/ndantonelli/Desktop/EngineFiles/DocumentList/list");
    if (!fin.is_open()){
        cout << "Error reading in document list";
        return;
    }
    //reads in the specific number of document objects and recreates the vector
    int counter;
    fin >> counter;
    for (int i = 0; i < counter; i++){
        string title, author, date, filename;
        getline(fin, title, '~');
        title.erase(0,1);
        getline(fin, author, '~');
        getline(fin, filename, '~');
        fin >> date;
        Document* obj = new Document(title, author, date, filename, i);
        docs.push_back(obj);
    }
    fin.close();
    docsParsed = counter;
    return;
}
