#include "../include/CommandProcessor.h"

#include <iostream>
#include <list>

using namespace std;

enum Commands{
    import = 0
};

CommandProcessor::CommandProcessor(string databaseFileName, string logFileName){
    CommandProcessor::databaseFileName = databaseFileName;
    CommandProcessor::logFileName = logFileName;
    CommandProcessor::systemManager = SystemManager();
}


void CommandProcessor::processCommand(const string &command) {
    char commentIndicator = ';';
    char delim1 = '\t';
    char delim2 = ' ';
    char end = '\0';
    list<string> args;
    string temp;
    int i = 0;
    if(command[0] != commentIndicator) {
        while (command[i] != end) {
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

        string function = args.front();
        args.pop_front();
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
