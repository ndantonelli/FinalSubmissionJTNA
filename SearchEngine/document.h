/* Owner: Nick Antonelli
 * Revision History:
 * 11/15 - created the basis for what a document needed (document text, title, author, date, and the document number)
 * 11/18 - added getters for all of the relevant information
 * 12/1 - decided that document text shouldnt be held in document object, write text to file and use docNum to find file later*/
#ifndef DOCUMENT_H
#define DOCUMENT_H
#include <string>

using std::string;


// ******************PUBLIC OPERATIONS*********************
// getDocNum()                  --> returns the specific document number
// string getFileName()                  --> returns the string location of the document text
// string getTitle()                  --> returns the title of the document
// string getAuthor()                  --> returns the author of the document
// string getDate()                  --> returns the date the document was published


class Document
{
public:
    Document(string&, string&, string&, string&, int);
    int getDocNum();
    string& getFileName();
    string& getTitle();
    string& getAuthor();
    string& getDate();
    ~Document();
private:
    int docNum;//specific number given by the location it was read in
    string filename; //location on disk of the document text
    string title;//title of document
    string author;//author of document
    string date;//date of publication
};

#endif // DOCUMENT_H
