/* Owner: Nick Antonelli
 * Revision History:
 * 12/4 - created the search engine control, augmented the capabilities within the functions
 * but overall it has stayed very much the same*/
#ifndef SEARCHENGINECONTROL_H
#define SEARCHENGINECONTROL_H
#include "indexer.h"
#include "parser.h"
#include "userinterface.h"

// ******************PUBLIC OPERATIONS*********************
// runTheEngine()           --> run the user interface and interact with index
// closingOperations()      --> closes the engine by clearing and persisting the index


class SearchEngineControl
{
public:
    SearchEngineControl();
    void runTheEngine();
    void closingOperations();
private:
    Indexer* theIndex; //inheritance based version of the index allowing for 2 types with one declaration
    Parser* theParser; //one parser for all functions
    UserInterface* theInterface; //inheritance based user interface so multiple modes can be called with one name
};

#endif // SEARCHENGINECONTROL_H
