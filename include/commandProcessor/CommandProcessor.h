#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include <string>
#include <vector>
#include <fstream>
#include "Command.h"
#include "../systemManager/SystemManager.h"
#include "../nameIndex/NameIndex.h"

using namespace std;

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

    bool is_quit = false;

    SystemManager systemManager;

    int commandsProcessed = 0;

    // Get the command name from the command enum.
    static string& getCommandName(Command command) {
        static std::string functionName;

        switch (command)
        {
            case WORLD:
                functionName = "world";
                break;
            case IMPORT:
                functionName = "import";
                break;
            case DEBUG:
                functionName = "debug";
                break;
            case QUIT:
                functionName = "quit";
                break;
            case WHAT_IS_AT:
                functionName = "what_is_at";
                break;
            case WHAT_IS_IN:
                functionName = "what_is_in";
                break;
            case WHAT_IS:
                functionName = "what_is";
                break;
            default:
                functionName = "quit";
        }

        return functionName;
    }

    // Process an individual line from the script file.
    // The line is parsed into a function and a list of arguments. The function and arguments are then passed to
    // processCommand().
    // Lines beginning with a semicolon and blank lines are ignored.
    // Other lines consist of tokens separated by single Tab characters. A line terminator follow the last token on a line.
    void processLine(const string &line);

    // Call the appropriate function by function name and passes its arguments.
    void processCommand(const string &function, list<string> &args);

    // Specifies the boundaries of the coordinate space.
    //
    // Takes longitude and latitude in DMS format, representing the vertical and horizontal boundaries of the coordinate
    // space. Records outside the coordinate space are ignored (not indexed).
    void world(string &westLong, string &eastLong, string &southLat, string &northLat);

    // Add all valid records from the specified file to the existing database file, index them, add the number of
    // entries added to each index, and the longest probe sequence that was needed for inserting into the hash table.
    // A valid record is the one that specified within the specified world boundaries.
    void import(string &recordsDataSetFileName);

    // Log the contents of the specified index structure in a fashion that makes the internal structure and contents of
    // the index clear. Includes information like key values and file offsets.
    //
    // The debugTarget argument specifies the index structure to be logged. It can be one of the following:
    // 1. "quad" - the coordinate index
    // 2. "hash" - the name index
    // 3. "pool" - the buffer pool
    // 4. "world" - the world boundaries
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
