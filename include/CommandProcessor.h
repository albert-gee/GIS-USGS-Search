#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include <string>
#include <vector>
#include <fstream>
//#include <wsman.h>
#include "Command.h"
#include "SystemManager.h"
#include "NameIndex.h"

using namespace std;

class CommandProcessor {
private:
    SystemManager systemManager;
    //NameIndex nameIndex;
public:
    CommandProcessor(SystemManager systemManager);
    void processCommand(const std::string& command);
    //static void processCommand(const std::string&, const std::string&, const std::string&);
};


#endif //COMMANDPROCESSOR_H
