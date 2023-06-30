#include "../include/commandProcessor/CommandProcessor.h"

namespace Constants {
    // The maximum number of records that can be stored in a bucket.
    // If a bucket is full, the corresponding quadrant must be split into four sub-quadrants with their own buckets.
    const unsigned long BUCKET_CAPACITY{4};

    // These specific values encompass the latitude range from approximately 24.396308 (southernmost point) to
    // 49.384358 (northernmost point), and the longitude range from approximately -125.000000 (westernmost point) to
    // -66.934570 (easternmost point) of the contiguous United States.
    const Point NORTH_WEST_POINT(49.384358, -125.000000);    // Top-left corner of the QuadTree's region
    const Point SOUTH_EAST_POINT(24.396308, -66.934570);     // Bottom-right corner of the QuadTree's region
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

    /// BufferPool, NameIndex, QuadTree
    BufferPool bufferPool{dbService};
    NameIndex nameIndex{};
    QuadTree coordinateIndex = QuadTree(Constants::NORTH_WEST_POINT, Constants::SOUTH_EAST_POINT,
                                        Constants::BUCKET_CAPACITY);

    /// SystemManager
    SystemManager systemManager = SystemManager(nameIndex, coordinateIndex, bufferPool, dbService, logService);

    /// CommandProcessor
    CommandProcessor cmdProcessor = CommandProcessor(systemManager, commandScriptFileLocation);
    // Process the commands from the script file
    cmdProcessor.processCommandsFromScriptFile();

    return 0;
}