#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include <string>
#include <vector>
#include <fstream>
//#include <wsman.h>
#include "Command.h"
#include "../systemManager/SystemManager.h"
#include "../nameIndex/NameIndex.h"

using namespace std;

class CommandProcessor {
public:
    CommandProcessor(SystemManager systemManager, const std::string& commandFileLocation);
    void processCommandsFromScriptFile();
    //static void processCommand(const std::string&, const std::string&, const std::string&);

    std::ifstream commandScriptFile;

private:
    SystemManager systemManager;
    int commandsProcessed = 0;
    //NameIndex nameIndex;

    void processCommand(const std::string& line);

    void whatIs(list<string> args);
};


#endif //COMMANDPROCESSOR_H
