#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include <string>
#include <vector>
#include <fstream>
//#include <wsman.h>
#include "Command.h"
#include "SystemManager.h"

using namespace std;

class CommandProcessor {
private:
    string databaseFileName;
    string logFileName;
    SystemManager systemManager;
public:
    CommandProcessor(string dataBaseFileName, string logFileName);
    void processCommand(const std::string& command);
};


#endif //COMMANDPROCESSOR_H
