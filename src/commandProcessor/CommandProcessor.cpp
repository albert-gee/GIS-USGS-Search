#include "../../include/commandProcessor/CommandProcessor.h"

#include <iostream>
#include <list>
#include <utility>

using namespace std;

CommandProcessor::CommandProcessor(SystemManager systemManager)
        : systemManager(std::move(systemManager)) {}

void CommandProcessor::processCommandsFromScriptFile(const std::string &commandScriptFileLocation) {
    std::ifstream commandScriptFile;

    // Open the command script file
    commandScriptFile.open(commandScriptFileLocation, std::ios::in | std::ios::out | std::ios::app);

    //std::cout << commandScriptFileLocation << " ";
    if (!commandScriptFile.is_open()) {
        std::cerr << "Error: Failed to open command script file." << std::endl;
    }

    // read each line from the file and pass it to the command processor
    std::string line;
    while (std::getline(commandScriptFile, line) && !is_quit) {
        processLine(line);
    }

    commandScriptFile.close();
}

void CommandProcessor::processLine(const string &line) {
    list<string> args;
    string temp;
    int i = 0;

    // Check if the line is not a comment
    if (line[0] != COMMENT_INDICATOR) {

        // Parse the line character by character until the end of the line
        while (line[i] != END_CHAR) {

            // Check if the character is not a delimiter and add it to the temp string.
            // If the character is a delimiter, add the temp string to the args list and clear the temp string.
            if (line[i] != DELIM1) {
                temp += line[i];
            } else {
                if (!temp.empty()) {
                    args.push_back(temp);
                    temp.clear();
                }
            }
            i++;
            if (line[i] == END_CHAR) {
                if (!temp.empty()) {
                    args.push_back(temp);
                }
            }
        }

        // The first argument is the function name
        string function = args.front();
        args.pop_front();

        // Pass the function name and arguments to processCommand()
        processCommand(function, args);
    } else {
        systemManager.logComment(line);
    }
}

void CommandProcessor::processCommand(const string &function, list<string> &args) {

    // Log the command and arguments EXCEPT for the world command
    if (function != getCommandName(Command::WORLD)) {
        ++commandsProcessed;
    }
    systemManager.logCommand(commandsProcessed, function, args, DELIM1);

    // Call the appropriate function based on the function name
    if (function == getCommandName(Command::WORLD)) {

        string westLong = args.front();
        args.pop_front();
        string eastLong = args.front();
        args.pop_front();
        string southLat = args.front();
        args.pop_front();
        string northLat = args.front();
        args.pop_front();

        world(westLong, eastLong, southLat, northLat);

    } else if (function == getCommandName(Command::IMPORT)) {

        string recordsDataSetFileName = args.front();
        import(recordsDataSetFileName);

    } else if (function == getCommandName(Command::WHAT_IS_AT)) {
        string latitude = args.front();
        args.pop_front();

        string longitude = args.front();
        args.pop_front();

        whatIsAt(latitude, longitude);

    } else if (function == getCommandName(Command::WHAT_IS)) {

        string featureName = args.front();
        args.pop_front();
        string stateAbrv = args.front();

        whatIs(featureName, stateAbrv);

    } else if (function == getCommandName(Command::WHAT_IS_IN)) {
        bool isFiltered = false;
        bool isDetailed = false;
        string filter;
        if (args.front() == "-filter") {
            isFiltered = true;
            args.pop_front();
            filter = args.front();
            args.pop_front();
        } else if (args.front() == "-long") {
            isDetailed = true;
            args.pop_front();
        }

        string latitude = args.front();
        args.pop_front();

        string longitude = args.front();
        args.pop_front();

        string halfHeight = args.front();
        args.pop_front();

        string halfWidth = args.front();
        args.pop_front();

        whatIsIn(isFiltered, isDetailed, filter, latitude, longitude, halfHeight, halfWidth);

    } else if (function == getCommandName(Command::DEBUG)) {
        string debugTarget = args.front();
        debug(debugTarget);

    } else if (function == getCommandName(Command::QUIT)) {
        systemManager.quit();
        quit();
    }
}

void CommandProcessor::world(string &westLong, string &eastLong, string &southLat, string &northLat) {

    // The coordinates are in DMS format, so create DMS objects
    DMS westLongDMS(westLong);
    DMS eastLongDMS(eastLong);
    DMS southLatDMS(southLat);
    DMS northLatDMS(northLat);

    // Set the boundaries of the coordinate space
    systemManager.setCoordinateIndexBoundaries(northLatDMS, southLatDMS, eastLongDMS, westLongDMS);
}

void CommandProcessor::import(string &recordsDataSetFileName) {
    systemManager.import(recordsDataSetFileName);
}

void CommandProcessor::debug(string &debugTarget) {
    if(debugTarget == "quad") {
        systemManager.debugQuad();
    } else if (debugTarget == "hash") {
        systemManager.debugHash();
    } else if (debugTarget == "pool") {
        systemManager.debugPool();
    } else if (debugTarget == "world") {
        systemManager.debugWorld();
    } else {
        // Invalid debugTarget argument
        std::cout << "Invalid debug target!" << std::endl;
    }
}

void CommandProcessor::quit() {
    is_quit = true;
}

void CommandProcessor::whatIsAt(string &latitude, string &longitude) {
    Point point = {DMS(latitude), DMS(longitude)};
    systemManager.whatIsAt(point);
}

void CommandProcessor::whatIs(string &featureName, string &stateAbrv) {
    systemManager.whatIs(featureName, stateAbrv);
}

void CommandProcessor::whatIsIn(bool isFiltered, bool isDetailed, string &filter, string &latitude, string &longitude,
                                string &halfHeight, string &halfWidth) {
    systemManager.whatIsIn(isFiltered, isDetailed, filter, latitude, longitude, halfHeight, halfWidth);
}
