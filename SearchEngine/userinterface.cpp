#include "userinterface.h"
#include "parser.h"
#include "indexer.h"
#include "hashindexer.h"
#include "avlindexer.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "QueryEngine.h"
#include "word.h"
#include "document.h"
#include <dirent.h>
#include <chrono>

using namespace std;

UserInterface::UserInterface(Indexer* someIndex, Parser* parse){
    theIndex = someIndex;
    p = parse;
    return;
}

Indexer* UserInterface :: execute(){
    cout << "Welcome to the basic User Interface\n";
    cout << "Unfortunately this mode is not available for use at this time/n\n";
    return theIndex;
}

Maintenance :: Maintenance(Indexer* someIndex, Parser* parse) : UserInterface(someIndex, parse){
    empty = false;
    return;
}

Indexer* Maintenance :: execute(){
    cout << "Welcome to Maintenance Mode:\n\n";
    bool loop = true;
    //loops through the options until the user wishes to exit
    while (loop){
        bool valid = false;
        int choice = 0;
        //loops until the user enters a valid choice, keeps the program from infinitely looping
        while (!valid){
            cout << "1) Add Documents to the Index\n";
            if (!empty){
                cout << "2) Clear the Index\n3) WriteIndex\n4) Exit Maintenance Mode\n";
            }
            else{
                cout << "2) WriteIndex\n3) Exit Maintenance Mode\n";
            }
            cout << "\n\nPlease enter the number of the action of your choice: ";
            cin >> choice;
            if(choice > 0 && choice < 5)
                valid = true;

            else
                cout << "Not a valid choice, please try again\n\n";
        }
        if (empty && choice == 3){
            choice = 4;
        }
        string newLocation;
        switch (choice){
            //allows the user to enter a new file location to add to the index
            case 1:
                cout << "\n\nPlease enter the File location: ";
                getline(cin, newLocation);
                getline(cin, newLocation);
                p->parseDump(newLocation);
                cout << "\n\nThe Index was Successfully Augmented!\n\n";
                empty = false;
                break;
            //allows the user to clear the index completely
            case 2:
                p->clearDocs();
                theIndex -> clearIndex();
                cout << "Successfully cleared the Index!\n";
                cout << "The Data Structure is now empty\n\n";
                empty = true;
                break;
            //the user chose to write the current index to disk
            case 3:
                p -> writeDocs();
                theIndex -> writeIndex();
                break;
            //exits maintenance mode
            case 4:
                return theIndex;
        }
    }
    return theIndex;
}

Interactive :: Interactive(Indexer* newIndex, Parser* parse) : UserInterface(newIndex, parse){

    return;
}

Indexer* Interactive :: execute(){
    cout << "Welcome to Interactive Mode:\n\n";
    bool loop = true;
    //loops through the options until the user wishes to exit
    while (loop){
        int choice = 0;
        bool valid = false;
        //keeps the user from entering impossible response
        while (!valid){
            cout << "1) Choose/Change Index Type\n2) Search the Index\n3) Exit Interactive Mode\n\n";
            cout << "\nPlease enter the number of the action of your choice: ";
            cin >> choice;
            if(choice >0 && choice < 4)
                valid = true;
            else
                cout << "Not a valid entry, please try again\n\n";
        }
        wstring searcher;
        switch (choice){
            case 1:
                valid = false;
                while (!valid){
                    cout << "\n\nPlease enter 1 for a Hash Table or 2 for an AVL Tree: ";
                    cin >> choice;
                    if(choice >0 && choice <3)
                        valid = true;
                    else
                        cout << "Not a valid option, please try again\n\n";
                }
                //allows the user to change the index type, but wont change type
                //if already in the type desired
                if (choice == 1){
                    if(theIndex->type() == 0){
                        cout << "Already held in a Hash Table\n\n";
                    }
                    else{
                        cout << "Changing Indexes, please wait...";
                        theIndex -> clearIndex();
                        theIndex = new HashIndexer();
                        p->clearDocs();
                        p->changeIndex(theIndex);
                        p -> pullIndex();
                        theIndex ->printIndex();
                        cout << "Successfully changed index type to a Hash Table\n\n";
                    }
                }
                else{
                    if(theIndex->type() == 1){
                        cout << "Already held in an AVL Tree\n\n";
                    }
                    else{
                        cout << "Changing Indexes, please wait...";
                        theIndex -> clearIndex();
                        theIndex = new AvlIndexer();
                        p->clearDocs();
                        p->changeIndex(theIndex);
                        p -> pullIndex();
                        theIndex ->printIndex();
                        cout << "Successfully changed index type to an AVL Tree\n\n";
                    }
                }
                break;
            //Allows the user to enter a query and see the result of their wish
            case 2:{
                QueryEngine* engine = new QueryEngine();
                cout << endl << "You may enter a search with the following boolean prefixes:" << endl;
                cout << "AND, OR, NOT    (Booleans MUST be in all Capital Letters to be recognized)" << endl << endl;
                cout << "Please enter your search criteria: ";
                getline(wcin, searcher);
                getline(wcin, searcher);
                wcout<< endl << endl << "searched for: " << searcher << endl;
                //searches the index for the word and the conditions
                unordered_multimap<double, Document*> result = engine->search(searcher, theIndex);

                cout << "THE RESULTS ARE IN:\n\n";
                auto c = result.begin();
                int i = 0;
                for (; i < 15; i++){
                    if(c == result.end())
                        break;
                    cout.precision(5);
                    cout << "\n\n" << i+1 << ")\nTitle: " << c->second -> getTitle() << "\nAuthor: " << c->second -> getAuthor() << "\nDatePublished: " << c->second -> getDate() << "\nRelevancy Ranking: "<< fixed << c->first << "\n";
                    c++;
                }
                bool valid = false;
                int bob = 0;
                //allows a user to choose and display one document of their search results
                while (!valid){
                    cout << "\nPlease Choose the Document you wish to view: ";
                    cin >> bob;
                    if (bob > 0 && bob < i + 1)
                        break;
                    else
                        cout<< "You cant fool me, enter a real response\n\n";
                }
                cout << "\n\nHere is the XML text, thank you for waiting!\n\n\n";
                int counter = 1;
                auto h = result.begin();
                for (;h != result.end() ; h++){
                    if (counter == bob)
                        break;
                    counter++;
                }
                ifstream fin((h->second->getFileName()).c_str());
                string text;
                getline(fin, text, '\0');
                cout << text << endl;
                break;
        }
            //exits the interactive mode
            case 3:
                return theIndex;
        }
    }
    return theIndex;
}

StressTest :: StressTest(Indexer* newInd, Parser* parse) : UserInterface(newInd, parse){
    return;
}

Indexer* StressTest :: execute(){
    //allows the user to specify a test file
    cout << "Please enter the file name for the properly formatted text file: ";
    string commandFile;
    cin >> commandFile;
    ifstream fin(commandFile.c_str());
    if(!fin.is_open()){
        cout << "Issue opening stress test command file\n\n";
        return theIndex;
    }
    int counter;
    fin >> counter;
    //starts a timer to measure how long a single function takes to execute with an index
    std::chrono::time_point<std::chrono::system_clock> startAll, startOne, end;
    startAll = std::chrono::system_clock::now();
    //reads in the commands needed from a file made by the user
    for(int i = 0; i < counter; i++){
        string command;
        getline(fin, command, '~');
        command.erase(0,1);
        startOne = std::chrono::system_clock::now();
        executeCommand(command, fin);
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-startOne;
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        std::cout << "elapsed time Command " << i << ": " << elapsed_seconds.count() << "seconds\n";
    }
    //adds up all of the time it took to do all of the functions
    std::chrono::duration<double> elapsed_seconds = end-startAll;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << "Total elapsed time: " << elapsed_seconds.count() << "seconds\n";
    return theIndex;
}
//function used to get all of the firles in the directory holding the separated dump
int getFiles(string& dir, vector<string>& files){
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

void StressTest :: executeCommand(string& command, ifstream& fin){//command will be the first sequence of characters on a line, all arguments after are comma delimeted, and a new command is started at the carriage return
    if (command == "WID"){ //write existing index to disk, has no following arguments
        p -> writeDocs();
        theIndex -> writeIndex();
    }
    else if (command == "PID"){ //pull current index from disk, followed by one argument (index type - 0 for hash, 1 for tree)
        int type;
        fin >> type;
        if(!theIndex->type() == type){
            theIndex->clearIndex();
            if(type == 0){
                theIndex = new HashIndexer();
                p->changeIndex(theIndex);
            }
            else{
                theIndex = new AvlIndexer();
                p->changeIndex(theIndex);
            }
        }
        p->clearDocs();
        p ->pullIndex();
    }
    else if (command == "RCI"){ //recreate index, 1 argument the filename needed to recreate the index
        string filename;
        getline(fin,filename, '~');
        p->clearDocs();
        theIndex ->clearIndex();
        p->parseDump(filename);
    }
    else if (command == "Q"){ //query the index, followed by 1 arguments the properly formatted boolean and the number 1-15 desired to view
        string boolean;
        getline(fin, boolean, '~');
        wstring temp;
        wchar_t* UnicodeTextBuffer = new wchar_t[boolean.length()+1];
        wmemset(UnicodeTextBuffer, 0, boolean.length()+1);
        mbstowcs(UnicodeTextBuffer, boolean.c_str(), boolean.length());
        temp = UnicodeTextBuffer;
        QueryEngine* engine = new QueryEngine();
        unordered_multimap<double, Document*> result = engine->search(temp, theIndex);
    }
    else if (command == "CI"){ //clear index, no arguments
        p->clearDocs();
        theIndex->clearIndex();
    }
    else if (command == "AI"){//add to index, 1 argument filename with xml docs to be added to index
        string filename;
        getline(fin, filename, '~');
        p->parseDump(filename);
    }
    else if (command == "BI"){//build the index, no arguments because it uses the normal dump location

        string dir = string("/Users/ndantonelli/Desktop/XMLfiles/WikiDump"); //nick
        vector <string> file = vector<string>();
        getFiles(dir, file);
        for (int i = 0; i < 2; i++){
            string name = "/Users/ndantonelli/Desktop/XMLfiles/WikiDump/" + file[i];
            p -> parseDump(name);
        }
    }
    else if (command == "SIT"){//switch the index type, no arguments it merely changes from hash to AVL or avl to Hash
        int type = theIndex->type();
        theIndex->clearIndex();
        if(type == 0){
            theIndex = new AvlIndexer();
        }
        else{
            theIndex = new HashIndexer();
        }
        p->clearDocs();
        p->changeIndex(theIndex);
    }
    else{//if no viable command was written then it cannot be executed
        cout << "Command not recognizable, execution may not work as desired\n\n";
    }
    return;
}
