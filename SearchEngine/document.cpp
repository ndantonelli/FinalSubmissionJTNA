#include "document.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

Document::Document(string& theTitle, string& theAuthor, string& theDate, string& theFile, int doc){
    filename=theFile;
    title = theTitle;
    author = theAuthor;
    date = theDate;
    docNum = doc;
    return;
}

string& Document :: getFileName(){
    return filename;
}

string& Document :: getAuthor(){
    return author;
}

string& Document :: getTitle(){
    return title;
}

string& Document :: getDate(){
    return date;
}

int Document::getDocNum(){
    return docNum;
}

Document :: ~Document(){
    title.clear();
    author.clear();
    date.clear();
    return;
}
