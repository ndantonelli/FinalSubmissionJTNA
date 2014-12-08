class Document;
#include "searchenginecontrol.h"
#include "userinterface.h"
#include "indexer.h"
#include "hashindexer.h"
#include "avlindexer.h"
#include "parser.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <dirent.h>

using namespace std;

SearchEngineControl::SearchEngineControl(){
    theIndex = new Indexer();
    theParser = new Parser(theIndex);
    return;
}
//function to get all of the file names in a directory
int getList(string& dir, vector<string>& files){
    DIR* dp;
    struct dirent* dirp;
    if ((dp = opendir(dir.c_str())) == NULL){
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }
    int counter = 0;
    while ((dirp = readdir(dp)) != NULL){
        if (counter > 2){
            files.push_back(string(dirp->d_name));
        }
        counter++;
    }
    closedir(dp);
    return 0;
}

void SearchEngineControl :: runTheEngine(){
    bool loop = true;
    bool valid = false;
    int choice;
    //prompts the user to choose the type of index they want to start with for any interface experience
    while (!valid){
        cout << "1) Hash Table\n2) AVL Tree\n\nPlease enter the number corresponding to the type of index you would like to use: ";
        cin >> choice;
        if(choice >0 && choice < 3)
            valid = true;
        else
            cout<< "Not a valid entry, try again\n\n";
    }
    if(choice == 1)
        theIndex = new HashIndexer();
    else
        theIndex = new AvlIndexer();
    theParser->changeIndex(theIndex);
    valid = false;
    while (!valid){
        cout << "\n1) Recreate the Index\n2) Pull a persisting index\n\nPlease enter the number corresponding to index creation method: ";
        cin >> choice;
        if(choice >0 && choice < 3)
            valid = true;
        else
            cout<< "Not a valid entry, try again\n\n";
    }
    if(choice == 1){
        //string dir = string("/Users/Josh/Documents/School/Data\ Structures/Search\ Engine/XMLParser/WikiDump"); //josh
        string dir = string("/Users/ndantonelli/Desktop/XMLfiles/WikiDump"); //nick
        vector <string> file = vector<string>();
        getList(dir, file);
        //gets all of the file names and then parses the dump using those files to add to the index
        for (int i = 0; i < file.size(); i++){
            string name = "/Users/ndantonelli/Desktop/XMLfiles/WikiDump/" + file[i];
            theParser -> parseDump(name);
        }
        //theIndex-> printIndex();
    }
    else{
        //if the user chose to pull a persistent index then the index is pulled
        theParser -> pullIndex();
        theIndex -> printIndex();
    }
    //loop that lets the user continually choose new exciting modes to play with until they wish to exit
    while (loop){
        valid = false;
        //prompts user to enter the type of mode they wish to enter, makes them choose a legitimate mode
        while (!valid){
            cout << "Please Choose Your Experience:\n1) Maintenance Mode\n2) Interactive Mode\n3) Stress Test Mode\n\nPlease enter the number corresponding to your choice: ";
            cin >> choice;
            if(choice >0 && choice < 4){
                valid = true;
            }
            else
                cout<< "Not a valid entry, try again\n\n";
        }
        //creates the experience of the users choosing for their first time
        switch(choice){
        case 1:
            theInterface = new Maintenance(theIndex, theParser);
            break;
        case 2:
            theInterface = new Interactive(theIndex, theParser);
            break;
        case 3:
            theIndex->clearIndex();
            theParser->clearDocs();
            theInterface = new StressTest(theIndex, theParser);
        }
        //executes the interface polymorphically so no matter what interface the user wants, its just one function call
        theIndex = theInterface -> execute();
        valid = false;
        //allows the user to either choose a new experience or quit the search engine
        while (!valid){
            cout << "Please choose your next action:\n1) Change Experience\n2) Exit\n\nPlease enter the number corresponding with your choice: ";
            cin >> choice;
            if(choice >0 && choice < 3)
                valid = true;
            else
                cout<< "Not a valid entry, try again\n\n";
        }
        switch(choice){
        case 1:
            break;
        case 2:
            //closingOperations();
            loop = false;
            return;
        }
    }
}

void SearchEngineControl :: closingOperations(){
    //creates a new persistent index
    theParser -> writeDocs();
    theIndex -> writeIndex();
}
