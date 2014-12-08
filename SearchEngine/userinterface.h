/*Owner: Nick Antonelli
 * Revision History:
 * 12/1 - created the Basic user interface and gave them all virtual functions
 * 12/3 - no longer need all of the classes origininally included
 * 12/3 - the only virtual function needed is one that alters the execution during each mode
 * 12/4 - added the execute command function for the stress test mode so we could interperet the input file*/

#ifndef USERINTERFACE_H
#define USERINTERFACE_H
#include "indexer.h"
#include "parser.h"
#include <string>
#include <fstream>

using namespace std;

// ******************PUBLIC OPERATIONS*********************
// virtual Indexer* execute()             --> polymorphic execution of different modes
// void executeCommand(string, ifstream)  --> executes a string based command for stress test mode

class UserInterface{
public:
    UserInterface(Indexer*, Parser*);
    virtual Indexer* execute();
protected:
    Indexer* theIndex;
    Parser* p;
};

class Maintenance : public UserInterface{
public:
    Maintenance(Indexer*, Parser*);
    Indexer* execute();
private:
    bool empty;
};

class Interactive : public UserInterface{
public:
    Interactive(Indexer*, Parser*);
    Indexer* execute();
};

class StressTest : public UserInterface{
public:
    StressTest(Indexer*, Parser*);
    Indexer* execute();
    void executeCommand(string&, ifstream&);
};

#endif // USERINTERFACE_H
