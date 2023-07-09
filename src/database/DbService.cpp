#include <fstream>
#include <iostream>
#include <limits>
#include "../../include/database/DbService.h"
#include "../../include/coordinateIndex/Point.h"
#include "../../include/database/LineUtility.h"

DbService::DbService(const std::string &databaseFileLocation) {

    // Delete the databaseService file if it already exists
    if (std::remove(databaseFileLocation.c_str()) != 0) {
        // If the file doesn't exist, std::remove returns a non-zero value
        // indicating that the file could not be deleted. We can ignore this
        // error if it occurs.
    }

    this->databaseFileLocation = databaseFileLocation;
}

// Destructor
DbService::~DbService() {
    databaseFile.close();
}

void DbService::open() {
    databaseFile.open(databaseFileLocation, std::ios::in | std::ios::out | std::ios::app);
    if (!databaseFile.is_open()) {
        std::cerr << "Error: Failed to create databaseService file." << std::endl;
    }
}

void DbService::close() {
    databaseFile.close();
}

void DbService::import(const std::string& recordsDataSetFileLocation) {

    // The recordsDataSetFileLocation file is created and opened for reading
    std::ifstream recordsFile (recordsDataSetFileLocation);

    if(!recordsFile.is_open()) {
        std::cerr << "Error: Failed to create records file." << std::endl;
    } else {
        // The records file is open. Open the database file
        open();

        // Parse the records file line by line and add the valid records to the database file
        std::string line;
        // Clear the first line as it has the headings
        getline(recordsFile, line);
        while (getline(recordsFile, line)) {
            databaseFile << line << std::endl;
        }

        // Close the database file
        close();

        // Close the records file
        recordsFile.close();
    }
}

void
DbService::import(const std::string &recordsDataSetFileLocation, double north, double south, double west, double east) {

    // The recordsDataSetFileLocation file is created and opened for reading
    std::ifstream recordsFile (recordsDataSetFileLocation);

    if(!recordsFile.is_open()) {
        std::cerr << "Error: Failed to create records file." << std::endl;
    } else {
        // The records file is open. Open the database file
        open();

        // Parse the records file line by line and add the valid records to the database file
        std::string line;
        // Clear the first line as it has the headings
        getline(recordsFile, line);
        while (getline(recordsFile, line)) {
            GISRecord* gis = LineUtility::extractGISRecordFromLine(line, '|');
            double lat = stod(gis->getPrimaryLatitudeDec());
            double lon = stod(gis->getPrimaryLongitudeDec());
            delete gis;
            if(lat <= north && lat >= south){
                if(lon <= east && lon >= west){
                    databaseFile << line << std::endl;
                }
            }
        }

        // Close the database file
        close();

        // Close the records file
        recordsFile.close();
    }
}

// Get the next line from databaseService
// Returns true if the end of the file has been reached
bool DbService::getNextLine(std::string & line) {
/*    bool endOfFile = false;

    if (!databaseFile.eof()) {
        std::getline(databaseFile, line);
    } else {
        endOfFile = true;
    }
    return endOfFile;*/
    if(std::getline(databaseFile, line)){
        return true;
    } else {
        return false;
    }
}

// Get the line from databaseService
std::string DbService::getLineByNumber(int lineNum){
    std::string line;
    int index = 1;
    open();
    while (index < lineNum && !databaseFile.eof()) {
        databaseFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        ++index;
    }
    if(!databaseFile.eof()){
        getline(databaseFile,line);

    }
    close();
    return line;
}
