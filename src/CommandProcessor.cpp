#include "../include/CommandProcessor.h"

#include <iostream>
#include <list>
#include <utility>

using namespace std;

enum Commands{
    import = 0
};

CommandProcessor::CommandProcessor(string databaseFileName, string logFileName){
    CommandProcessor::databaseFileName = std::move(databaseFileName);
    CommandProcessor::logFileName = std::move(logFileName);
    CommandProcessor::systemManager = SystemManager();
}

// Process a command from the command file.
// Lines beginning with a semicolon and blank lines are ignored.
// Other lines consist of tokens separated by single Tab characters. A line terminator follow the last token on a line.
void CommandProcessor::processCommand(const string &command) {
    // Special Characters
    char commentIndicator = ';';
    char delim1 = '\t';
    char delim2 = ' ';
    char end = '\0';

    list<string> args;
    string temp;
    int i = 0;

    // Check if the command is not a comment
    if(command[0] != commentIndicator) {

        // Parse the command character by character until the end of the command
        while (command[i] != end) {

            // Check if the character is not a delimiter and add it to the temp string.
            // If the character is a delimiter, add the temp string to the args list and clear the temp string.
            if (command[i] != delim1 && command[i] != delim2) {
                temp += command[i];
            } else {
                if (!temp.empty()) {
                    args.push_back(temp);
                    temp.clear();
                }
            }
            i++;
            if (command[i] == end) {
                if (!temp.empty()) {
                    args.push_back(temp);
                }
            }
        }

        // The first argument is the function name
        string function = args.front();
        args.pop_front();

        // Execute the function
        if(function == "import"){
            string recordFileName = args.front();
            cout << "import " << recordFileName << endl;
            systemManager.import(recordFileName, databaseFileName);
        }

        /*cout << command << endl;
        for(string s : args) {
            cout << s << " ";
        }
        cout << endl;*/
    }

    //std::cout << "Processing command: " << command << std::endl;

}
