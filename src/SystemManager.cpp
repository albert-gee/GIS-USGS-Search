#include <fstream>
#include <iostream>
#include "../include/SystemManager.h"

SystemManager::SystemManager(NameIndex& nameIndex, const QuadTree& coordinateIndex, BufferPool& bufferPool, const string& databaseFileLocation, const string& logFileLocation)
        : nameIndex{nameIndex}, coordinateIndex{coordinateIndex}, bufferPool{bufferPool}, databaseFileLocation{databaseFileLocation}, logFileLocation{logFileLocation} {}

// Add all the valid records from the file recordsDataSetFileLocation to the database file databaseFileLocation.
void SystemManager::import(const string& recordsDataSetFileLocation){
    // The recordsDataSetFileLocation file is created and opened for reading
    ifstream recordsFile (recordsDataSetFileLocation);
    // The databaseFileLocation file is created and opened for appending
    ofstream databaseFile(databaseFileLocation, ios_base::app);

    if(!recordsFile.is_open()) {
        cerr << "Error: Failed to create records file." << std::endl;
    } else if (!databaseFile.is_open()) {
        cerr << "Error: Failed to create database file." << std::endl;
    } else {
        // Both the records file and the database file are open
        // Parse the records file line by line and add the valid records to the database file
        string line;
        int lineNum = 0;

        while (getline(recordsFile, line)) {
            if(lineNum > 0){
                databaseFile << line << endl;
            }
            ++lineNum;
        }
        databaseFile.close();
        recordsFile.close();

        // Index the records in the database file by feature name and state

        // Index the records in the database file by location

        // Log the number of records added to the database file
    }
    //Michael's test
/*    indexDatabaseByName(databaseFileLocation);

    const list<GISRecord*> records = bufferPool.getRecordsByKey("Alkali Creek CO", nameIndex, databaseFileLocation);
    const list<GISRecord*> records2 = bufferPool.getRecordsByKey("Bob Creek CO", nameIndex, databaseFileLocation);
    const list<GISRecord*> records4 = bufferPool.getRecordsByKey("Nipple Peak Trail CO", nameIndex, databaseFileLocation);
    const list<GISRecord*> records5 = bufferPool.getRecordsByKey("Nipple Peak Trail CO", nameIndex, databaseFileLocation);
    const list<GISRecord*> records3 = bufferPool.getRecordsByKey("Bob Lake CO", nameIndex, databaseFileLocation);
    bufferPool.printBuffer();*/

}

// Index the records in the database file by feature name and state
void SystemManager::indexDatabaseByName(){
    ifstream databaseFile(databaseFileLocation);
    if(!databaseFile.is_open()){
        cerr << "Error: Failed to open database file." << endl;
    } else {
        string line;
        int lineNum = 0;
        while(getline(databaseFile, line)){
            ++lineNum;
            string indexKey = LineUtility::extractFeatureNameAndStateFromLine(line, FEATURE_NAME_COL, STATE_ALPHA_COL, DELIM);
            nameIndex.indexLine(indexKey, lineNum);
        }
    }

    databaseFile.close();
}

// Index the records in the database file by location
void SystemManager::indexDatabaseByCoordinates(){
    ifstream databaseFile(databaseFileLocation);
    if(!databaseFile.is_open()){
        cerr << "Error: Failed to open database file." << endl;
    } else {
        string line;
        int lineNum = 0;
        while(getline(databaseFile, line)){
            ++lineNum;

            Point location = LineUtility::extractLocationFromLine(line, LONGITUDE_COL, LATITUDE_COL, DELIM);
            coordinateIndex.insert(location, lineNum);
        }
    }

    databaseFile.close();
}

// ToDo: implement the following method
list<GISRecord> SystemManager::findGISRecordsByCoordinates(double latitude, double longitude) {
    list<GISRecord> offsets;

    // Find in buffer
    //offsets = bufferPool.findGISRecordsByCoordinates(northWestPoint, southEastPoint);

    // If not found in buffer, find in coordinate index
    //offsets = coordinateIndex.getOffsetsOfGISRecords(northWestPoint, southEastPoint);

    return offsets;
}



