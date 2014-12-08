#include <iostream>
#include <sstream>
#include <unordered_map>
#include <dirent.h>
#include <thread>
#include "english_stem.h"
#include "parser.h"
#include "avlindexer.h"
#include "hashindexer.h"
#include"QueryEngine.h"
#include "searchenginecontrol.h"
using namespace std;
int main()
{
    SearchEngineControl* s = new SearchEngineControl();
    s->runTheEngine();
    return 0;
}


