#include <fstream>
#include <iostream>
#include "../../include/systemManager/SystemManager.h"
#include "../../include/database/DbService.h"

SystemManager::SystemManager(NameIndex& nameIndex, const QuadTree& coordinateIndex, BufferPool& bufferPool, DbService& databaseService, LogService& logService)
        : nameIndex{nameIndex}, coordinateIndex{coordinateIndex}, bufferPool{bufferPool}, databaseService{databaseService}, logService{logService}{}

// Set boundaries for the coordinate index
void SystemManager::setCoordinateIndexBoundaries(double westLong, double eastLong, double southLat, double northLat) {
    coordinateIndex.setBoundingBox(Point(northLat, westLong), Point(southLat, eastLong));
}

// Add all the valid records from the file recordsDataSetFileLocation to the databaseService file databaseFileLocation.
void SystemManager::import(const string& recordsDataSetFileLocation){
    // The recordsDataSetFileLocation file is created and opened for reading
    ifstream recordsFile (recordsDataSetFileLocation);

    if(!recordsFile.is_open()) {
        cerr << "Error: Failed to create records file." << std::endl;
    } else {
        // Both the records file and the databaseService file are open
        // Parse the records file line by line and add the valid records to the databaseService file
        string line;

        // Clear the first line as it has the headings
        getline(recordsFile, line);
        while (getline(recordsFile, line)) {
            databaseService.insert(line);
        }

        recordsFile.close();

        // Index the records in the databaseService file by feature name and state

        // Index the records in the databaseService file by location

        // Log the number of records added to the databaseService file
    }

    list<int>* nameImportStats = indexDatabaseByName();
    int numofIndexedLinesByName = nameImportStats->front();
    nameImportStats->pop_front();
    int longestProbeSeq = nameImportStats->front();
    cout << longestProbeSeq;
    nameImportStats->pop_front();
    int avgNameLength = nameImportStats->front();
    nameImportStats->pop_front();
    //int numOfIndexedLinesByLocation = indexDatabaseByCoordinates();
    logService.logImportStats(numofIndexedLinesByName, longestProbeSeq, 0, avgNameLength);
    //nameIndex.printIndex();
    //Michael's test
/*    indexDatabaseByName();
    bufferPool.printBuffer();


    const list<GISRecord*> records = bufferPool.getRecordsByKey("Alkali Creek CO", nameIndex, databaseFileLocation);
    bufferPool.printBuffer();
    const list<GISRecord*> records2 = bufferPool.getRecordsByKey("Bob Creek CO", nameIndex, databaseFileLocation);
    bufferPool.printBuffer();
    const list<GISRecord*> records4 = bufferPool.getRecordsByKey("Nipple Peak Trail CO", nameIndex, databaseFileLocation);
    bufferPool.printBuffer();
    const list<GISRecord*> records3 = bufferPool.getRecordsByKey("Bob Lake CO", nameIndex, databaseFileLocation);
    bufferPool.printBuffer();
    const list<GISRecord*> records5 = bufferPool.getRecordsByKey("Nipple Peak Trail CO", nameIndex, databaseFileLocation);
    bufferPool.printBuffer();
    const list<GISRecord*> records6 = bufferPool.getRecordsByKey("Bob Creek CO", nameIndex, databaseFileLocation);
    bufferPool.printBuffer();*/
}

// Index the records in the databaseService file by feature name and state
list<int> * SystemManager::indexDatabaseByName(){
    unsigned int numOfIndexedLines = 0;
    unsigned int longestProbeSeq = 0;
    unsigned int totalNameLength = 0;
    unsigned int avgNameLength = 0;

    string line;
    int lineNum = 0;
    while(databaseService.getNextLine(line)){
        ++lineNum;
        string featureName = LineUtility::extractParamFromLine(line, FEATURE_NAME_COL, DELIM);
        string stateAbrv = LineUtility::extractParamFromLine(line, STATE_ALPHA_COL, DELIM);
        totalNameLength += featureName.length();
        //string indexKey = LineUtility::extractFeatureNameAndStateFromLine(line, FEATURE_NAME_COL, STATE_ALPHA_COL, DELIM);
        ostringstream os;
        os << featureName << " " << stateAbrv;
        unsigned int probes = nameIndex.indexLine(os.str(), lineNum);
        ++numOfIndexedLines;
        if(probes > longestProbeSeq){
            longestProbeSeq = probes;
        }
    }
    avgNameLength = totalNameLength / numOfIndexedLines;

    list<int> *nameImportStats = new list<int>();
    nameImportStats->push_back(numOfIndexedLines);
    nameImportStats->push_back(longestProbeSeq);
    nameImportStats->push_back(avgNameLength);
    return nameImportStats;
}

// Index the records in the databaseService file by location
unsigned int SystemManager::indexDatabaseByCoordinates(){
    unsigned int numOfIndexedLines = 0;

    string line;
    int lineNum = 0;
    while(databaseService.getNextLine(line)){
        ++lineNum;

        Point location = LineUtility::extractLocationFromLine(line, LONGITUDE_COL, LATITUDE_COL, DELIM);
        coordinateIndex.insert(location, lineNum);
        ++numOfIndexedLines;
    }

    return numOfIndexedLines;
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

void SystemManager::whatIs(string featureName, string stateAbrv){
    ostringstream os;
    os << featureName << " " << stateAbrv;
    const auto records = bufferPool.getRecordsByKey(os.str(), nameIndex);
}

void SystemManager::logCommand(int cmdNumber, std::string function, list<std::string> args, char delimiter) {
    logService.logCommand(cmdNumber, function, args, delimiter);
}

void SystemManager::logComment(string comment){
    logService.logComment(comment);
}

void SystemManager::logLine(string text){
    logService.logLine(text);
}

void SystemManager::debugHash() {
    const string stats = nameIndex.getIndexStats();
    logLine(stats);
    logLine(string(90, '-'));
}



