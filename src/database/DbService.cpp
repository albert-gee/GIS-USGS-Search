#include <fstream>
#include <iostream>
#include <limits>
#include "../../include/database/DbService.h"

DbService::DbService(const std::string &databaseFileLocation) {

    // Delete the databaseService file if it already exists
    if (std::remove(databaseFileLocation.c_str()) != 0) {
        // If the file doesn't exist, std::remove returns a non-zero value
        // indicating that the file could not be deleted. We can ignore this
        // error if it occurs.
    }

    // Create the databaseService file as an empty file
    databaseFile.open(databaseFileLocation, std::ios::in | std::ios::out | std::ios::app);
    if (!databaseFile.is_open()) {
        std::cerr << "Error: Failed to create databaseService file." << std::endl;
    }
}

// Destructor
DbService::~DbService() {
    databaseFile.close();
}

// Insert a line into the databaseService file
void DbService::insert(const std::string &line) {
    databaseFile << line << std::endl;
}

bool DbService::getNextLine(std::string line) {
    bool endOfFile = true;

    if (!databaseFile.eof()) {
        std::getline(databaseFile, line);
        endOfFile = false;
    }

    return endOfFile;
}

// Get the line from databaseService
std::string DbService::getLineByNumber(int lineNum){
    std::string line;
    int index = 1;
    while (index < lineNum && !databaseFile.eof()) {
        databaseFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        ++index;
    }
    if(!databaseFile.eof()){
        getline(databaseFile,line);

    }

    return line;
}