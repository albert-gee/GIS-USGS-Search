#include "../include/CommandProcessor.h"

#include <iostream>

using namespace std;

CommandProcessor::CommandProcessor() = default;

void CommandProcessor::processCommand(const string &command) {
    std::cout << "Processing command: " << command << std::endl;
}
