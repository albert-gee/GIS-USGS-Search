#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include <string>
#include <vector>
#include <fstream>
#include "Command.h"
#include "../systemManager/SystemManager.h"
#include "../nameIndex/NameIndex.h"

using namespace std;

enum Commands {
    WORLD, IMPORT, DEBUG, QUIT, WHAT_IS_AT, WHAT_IS, WHAT_IS_IN
};

class CommandProcessor {
public:
    explicit CommandProcessor(SystemManager systemManager);

    // Process a script file with commands.
    void processCommandsFromScriptFile(const std::string &commandFileLocation);


private:
    // Special Characters
    char COMMENT_INDICATOR = ';';
    const char DELIM1 = '\t';
    const char END_CHAR = '\0';

    SystemManager systemManager;

    int commandsProcessed = 0;

    // Get the command name from the command enum.
    string& getCommandName(Commands command) {
        string function;

        switch (command)
        {
            case WORLD:
                function = "world";
                break;
            case IMPORT:
                function = "import";
                break;
            case DEBUG:
                function = "debug";
                break;
            case QUIT:
                function = "quit";
                break;
            case WHAT_IS_AT:
                function = "what_is_at";
                break;
            case WHAT_IS_IN:
                function = "what_is_in";
                break;
            case WHAT_IS:
                function = "what_is";
                break;
            default:
                function = "quit";
        }

        return function;
    }

    // Process an individual line from the script file.
    // The line is parsed into a function and a list of arguments. The function and arguments are then passed to
    // processCommand().
    // Lines beginning with a semicolon and blank lines are ignored.
    // Other lines consist of tokens separated by single Tab characters. A line terminator follow the last token on a line.
    void processLine(const string &line);

    // Process a command by function name and arguments.
    void processCommand(const string &function, list<string> &args);

    // Specifies the boundaries of the coordinate space.
    // Takes longitude and latitude in DMS format, representing the vertical and horizontal boundaries of the coordinate
    // space. Records outside the coordinate space are ignored (not indexed).
    void world(string &westLong, string &eastLong, string &southLat, string &northLat);

    // Add all valid records from the specified file to the existing database file, index them, add the number of
    // entries added to each index, and the longest probe sequence that was needed for inserting into the hash table.
    // A valid record is the one that specified within the specified world boundaries.
    void import(string &recordsDataSetFileName);

    // Log the contents of the specified index structure in a fashion that makes the internal structure and contents of
    // the index clear. Includes information like key values and file offsets.
    void debug(string& debugTarget);

    // Terminates the execution of the program.
    void quit();

    void whatIsAt(string &latitude, string &longitude);

    void whatIs(string &featureName, string &stateAbrv);

    void
    whatIsIn(bool isFiltered, bool isDetailed, string &filter, string &latitude, string &longitude, string &halfHeight,
             string &halfWidth);


};


#endif //COMMANDPROCESSOR_H
