#include <iostream>
#include <fstream>
#include "../include/CommandProcessor.h"

// This function takes the names of three files from the command line, like this:
// ./GIS <database file name> <command script file name> <log file name>
// Example: ./cmake-build-debug/GIS db_file.txt data/script01.txt log_file.txt
int main(int argc, char *argv[]) {
    // Check if exactly 3 arguments were provided
    if (argc != 4) {
        std::string programName(argv[0]);
        std::cout << "Error: Invalid number of arguments. Usage: " << programName << " name1 name2 name3" << std::endl;

        return 1; // Exit with error code 1
    }

    // Get the names of the files from the command line
    std::string databaseFileName = argv[1];
    std::string commandScriptFileName = argv[2];
    std::string logFileName = argv[3];

    /// DATABASE
    // Delete the database file if it already exists
    if (std::remove(databaseFileName.c_str()) != 0) {
        // If the file doesn't exist, std::remove returns a non-zero value
        // indicating that the file could not be deleted. We can ignore this
        // error if it occurs.
    }

    // Create the database file as an empty file
    std::ofstream databaseFile(databaseFileName);
    if (!databaseFile.is_open()) {
        std::cerr << "Error: Failed to create database file." << std::endl;
        return 1;
    }

    /// COMMAND SCRIPT
    // Open the command script file
    std::ifstream commandScriptFile(commandScriptFileName);
    //std::cout << commandScriptFileName << " ";
    if (!commandScriptFile.is_open()) {
        std::cerr << "Error: Failed to open command script file." << std::endl;
        return 1;
    }

    /// LOG
    // Delete the log file if it already exists
    if (std::remove(logFileName.c_str()) != 0) {
        std::perror("Error deleting the log file");
    }

    // Create the log file as an empty file
    std::ofstream logFile(logFileName);
    if (!logFile.is_open()) {
        std::cerr << "Error: Failed to open log file." << std::endl;
        return 1;
    }

    CommandProcessor cmdProcessor = CommandProcessor(databaseFileName, logFileName);
    // read each line from the file and pass it to the command processor
    std::string line;
    while (std::getline(commandScriptFile, line)) {
        cmdProcessor.processCommand(line);
    }
    return 0;
}