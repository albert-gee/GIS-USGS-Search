#include "../include/commandProcessor/CommandProcessor.h"

const string &generateLogStart(const string databaseFileLocation, const string logFileLocation, const string commandScriptFileLocation);

namespace Constants {
    // The maximum number of records that can be stored in a bucket.
    // If a bucket is full, the corresponding quadrant must be split into four sub-quadrants with their own buckets.
    const unsigned long BUCKET_CAPACITY{4};   // Bottom-right corner of the QuadTree's region


}

// This function takes the names of three files from the command line, like this:
// ./GIS <databaseService file name> <command script file name> <log file name>
// Example: ./cmake-build-debug/GIS db_file.txt data/script01.txt log_file.txt
int main(int argc, char *argv[]) {
    // Check if exactly 3 arguments were provided
    if (argc != 4) {
        std::string programName(argv[0]);
        std::cout << "Error: Invalid number of arguments. Usage: " << programName << " name1 name2 name3" << std::endl;

        return 1; // Exit with error code 1
    }

    // Get the names of the files from the command line
    const std::string databaseFileLocation = argv[1];
    const std::string commandScriptFileLocation = argv[2];
    const std::string logFileLocation = argv[3];

    /// DATABASE
    DbService dbService(databaseFileLocation);

    /// LOG
    LogService logService(logFileLocation);
    cout << generateLogStart(databaseFileLocation, logFileLocation, commandScriptFileLocation);
    logService.logString(generateLogStart(databaseFileLocation, logFileLocation, commandScriptFileLocation));

    /// BufferPool, NameIndex, QuadTree
    BufferPool bufferPool{dbService};
    NameIndex nameIndex{};
    QuadTree coordinateIndex = QuadTree(Constants::BUCKET_CAPACITY);

    /// SystemManager
    SystemManager systemManager = SystemManager(nameIndex, coordinateIndex, bufferPool, dbService, logService);

    /// CommandProcessor
    CommandProcessor cmdProcessor = CommandProcessor(systemManager);
    // Process the commands from the script file
    cmdProcessor.processCommandsFromScriptFile(commandScriptFileLocation);

    return 0;
}

const string &generateLogStart(const string databaseFileLocation, const string logFileLocation, const string commandScriptFileLocation) {
    time_t now = time(0);
    ostringstream os;
    os << "Course Project for COMP 8042\n";
    os << "Student Name: Albert Gainutdinov, Student Id:A01078557, & Michael Gou, Student Id: A00521595\n";
    os <<  "Begin of GIS Program log:\n";
    os << "dbFile:\t" + databaseFileLocation + "\n";
    os << "script:\t" + commandScriptFileLocation + "\n";
    os << "log:\t" + logFileLocation + "\n";
    os << "Start Time: " <<  std::put_time(std::localtime(&now), "%a %b %d %X %Z %Y");
    string *logHeading = new string();
    *logHeading  = os.str();
    return *logHeading;

}


