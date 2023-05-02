#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include <string>
#include <vector>
#include <fstream>
#include "Command.h"

class CommandProcessor {
public:
    CommandProcessor();
    static void processCommand(const std::string& command);
};


#endif //COMMANDPROCESSOR_H
