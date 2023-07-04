#include "../../include/commandProcessor/CommandProcessor.h"

#include <iostream>
#include <list>
#include <utility>

using namespace std;

enum Commands {
    import = 0
};

CommandProcessor::CommandProcessor(SystemManager systemManager, const std::string& commandScriptFileLocation)
        : systemManager(std::move(systemManager)) {

    // Open the command script file
    commandScriptFile.open(commandScriptFileLocation, std::ios::in | std::ios::out | std::ios::app);

    //std::cout << commandScriptFileLocation << " ";
    if (!commandScriptFile.is_open()) {
        std::cerr << "Error: Failed to open command script file." << std::endl;
    }
}

void CommandProcessor::processCommandsFromScriptFile() {
    // read each line from the file and pass it to the command processor
    std::string line;
    while (std::getline(commandScriptFile, line)) {
        processCommand(line);
    }
}

// Process a line from the line file.
// Lines beginning with a semicolon and blank lines are ignored.
// Other lines consist of tokens separated by single Tab characters. A line terminator follow the last token on a line.
void CommandProcessor::processCommand(const string &line) {
    // Special Characters
    char commentIndicator = ';';
    char delim1 = '\t';
    //char delim2 = ' ';
    char end = '\0';

    list<string> args;
    string temp;
    int i = 0;
    // Check if the line is not a comment
    if (line[0] != commentIndicator) {

        // Parse the line character by character until the end of the line
        while (line[i] != end) {

            // Check if the character is not a delimiter and add it to the temp string.
            // If the character is a delimiter, add the temp string to the args list and clear the temp string.
            if (line[i] != delim1) {
                temp += line[i];
            } else {
                if (!temp.empty()) {
                    args.push_back(temp);
                    temp.clear();
                }
            }
            i++;
            if (line[i] == end) {
                if (!temp.empty()) {
                    args.push_back(temp);
                }
            }
        }

        // The first argument is the function name
        string function = args.front();
        args.pop_front();

        // Execute the function
        // Import: load records into the databaseService from external files

        if (function != "world") {
            ++commandsProcessed;
        }
        systemManager.logCommand(commandsProcessed, function, args, delim1);
        //systemManager.logComment(string(90, '-'));

        if (function == "world") {
            // Specify boundaries of coordinate space
            string westLong = args.front();
            args.pop_front();
            string eastLong = args.front();
            args.pop_front();
            string southLat = args.front();
            args.pop_front();
            string northLat = args.front();
            args.pop_front();

            // The coordinates are in DMS format, so create DMS objects
            DMS westLongDMS(westLong);
            DMS eastLongDMS(eastLong);
            DMS southLatDMS(southLat);
            DMS northLatDMS(northLat);

            // Set the boundaries of the coordinate space
            systemManager.setCoordinateIndexBoundaries(northLatDMS.toDecimal(), southLatDMS.toDecimal(),
                                                       eastLongDMS.toDecimal(), westLongDMS.toDecimal());
        } else if (function == "import") {

            // The next argument is the name of the file containing the records
            string recordsDataSetFileName = args.front();


            // Import the records into the databaseService
            systemManager.import(recordsDataSetFileName);
        } else if (function == "what_is_at") {
            // The next argument is the latitude of the location
            string latitude = args.front();
            cout << "lat: " << latitude << endl;
            args.pop_front();

            // The next argument is the latitude of the location
            string longitude = args.front();
            cout << "lon: " << longitude << endl;

            // Import the records into the databaseService
            systemManager.findGISRecordsByCoordinates(std::stod(latitude), std::stod(longitude));
        } else if (function == "what_is") {
            whatIs(args);
  /*          string featureName = args.front();
            args.pop_front();
            string stateAbrv = args.front();
            systemManager.whatIs(featureName, stateAbrv);*/

        } else if (function == "what_is_in") {

        } else if (function == "debug") {
            string debugTarget = args.front();
            if (debugTarget == "hash") {
                systemManager.debugHash();
            } else if(debugTarget == "pool"){
                systemManager.debugPool();
            }

        } else if (function == "quit") {

        }

        /*cout << line << endl;
        for(string s : args) {
            cout << s << " ";
        }
        cout << endl;*/
    } else {
        systemManager.logComment(line);
    }
}

void CommandProcessor::whatIs(list<string> args) {
    string featureName = args.front();
    args.pop_front();
    string stateAbrv = args.front();
    systemManager.whatIs(featureName, stateAbrv);
}
