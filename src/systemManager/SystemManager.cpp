#include <fstream>
#include <iostream>
#include <iomanip>
#include "../../include/systemManager/SystemManager.h"

SystemManager::SystemManager(NameIndex& nameIndex, const QuadTree& coordinateIndex, BufferPool& bufferPool, DbService& databaseService, LogService& logService)
        : nameIndex{nameIndex}, coordinateIndex{coordinateIndex}, bufferPool{bufferPool}, databaseService{databaseService}, logService{logService}{
}

// Set boundaries for the coordinate index
void SystemManager::setCoordinateIndexBoundaries(double northLat, double southLat, double eastLong, double westLong) {
    ostringstream os;
    os << "\t\t\t\t\t\tWorld boundaries are set to:" << endl;
    os << "\t\t\t\t\t\t\t\t\t" << northLat * 3600 << endl;
    os << "\t\t\t\t\t\t" << westLong * 3600;
    os.width(14);
    os << "" << eastLong * 3600 <<endl;
    os << "\t\t\t\t\t\t\t\t\t" << southLat * 3600;
    logLineBreak();
    logLine("Latitude/longitude values in index entries are shown as signed integers, in total seconds.");
    logLineBreak();
    logLine(os.str());
    logLineBreak();
    coordinateIndex.setBoundingBox(Point(northLat, westLong), Point(southLat, eastLong));
}

// Add all the valid records from the file recordsDataSetFileLocation to the databaseService file databaseFileLocation.
void SystemManager::import(const string& recordsDataSetFileLocation){

    // Import the records from the recordsDataSetFileLocation file to the databaseService file
    try {
        databaseService.import(recordsDataSetFileLocation);

        // Index the records in the databaseService file by feature name and state
        list<int>* nameImportStats = indexDatabaseByNameAndCoordinates();
        int numofIndexedLines = nameImportStats->front();
        nameImportStats->pop_front();
        int longestProbeSeq = nameImportStats->front();

        nameImportStats->pop_front();
        int avgNameLength = nameImportStats->front();
        nameImportStats->pop_front();
        //coordinateIndex.print();
        //indexDatabaseByCoordinates();

        stringstream  os;
        os.width(27);
        os.setf(ios::left);
        os << "Imported Features by name:";
        os << numofIndexedLines << endl;
        os.width(27);
        os << "Longest probe sequence:\t";

        os << longestProbeSeq << endl;
        os.width(27);
        os << "Imported Locations:" << numofIndexedLines << endl;
        os.width(27);
        os << "Average name length:" << avgNameLength;
        logLine(os.str());
        logLineBreak();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

// Index the records in the databaseService file by feature name and state
list<int> * SystemManager::indexDatabaseByName(){
    unsigned int numOfIndexedLines = 0;
    unsigned int longestProbeSeq = 0;
    unsigned int totalNameLength = 0;
    unsigned int avgNameLength = 0;

    string* line = new string();
    int lineNum = 0;
    databaseService.open();
    while(databaseService.getNextLine(*line)){
        ++lineNum;
        string featureName = LineUtility::extractParamFromLine(*line, FEATURE_NAME_COL, DELIM);
        string stateAbrv = LineUtility::extractParamFromLine(*line, STATE_ALPHA_COL, DELIM);
        totalNameLength += featureName.length();
        ostringstream os;
        os << featureName << " " << stateAbrv;
        unsigned int probes = nameIndex.indexLine(os.str(), lineNum);
        ++numOfIndexedLines;
        if(probes > longestProbeSeq){
            longestProbeSeq = probes;
        }
    }
    databaseService.close();
    avgNameLength = (numOfIndexedLines != 0) ? totalNameLength / numOfIndexedLines : 0;

    list<int> *nameImportStats = new list<int>();
    nameImportStats->push_back(numOfIndexedLines);
    nameImportStats->push_back(longestProbeSeq);
    nameImportStats->push_back(avgNameLength);
    return nameImportStats;
}

list<int> * SystemManager::indexDatabaseByNameAndCoordinates(){
    unsigned int numOfIndexedLines = 0;
    unsigned int longestProbeSeq = 0;
    unsigned int totalNameLength = 0;
    unsigned int avgNameLength = 0;

    string* line = new string();
    int lineNum = 0;
    databaseService.open();
    while(databaseService.getNextLine(*line)){

        ++lineNum;
        string featureName = LineUtility::extractParamFromLine(*line, FEATURE_NAME_COL, DELIM);
        string stateAbrv = LineUtility::extractParamFromLine(*line, STATE_ALPHA_COL, DELIM);
        totalNameLength += featureName.length();
        ostringstream os;
        os << featureName << " " << stateAbrv;
        unsigned int probes = nameIndex.indexLine(os.str(), lineNum);
        if(probes > longestProbeSeq){
            longestProbeSeq = probes;
        }

        Point location = LineUtility::extractLocationFromLine(*line, LONGITUDE_COL, LATITUDE_COL, DELIM);
        //cout << location.x << " " << location.y <<endl;
        coordinateIndex.insert(location, lineNum);
        ++numOfIndexedLines;

    }
    databaseService.close();
    avgNameLength = (numOfIndexedLines != 0) ? totalNameLength / numOfIndexedLines : 0;

    list<int> *nameImportStats = new list<int>();
    nameImportStats->push_back(numOfIndexedLines);
    nameImportStats->push_back(longestProbeSeq);
    nameImportStats->push_back(avgNameLength);
    return nameImportStats;
}


// Index the records in the databaseService file by location
unsigned int SystemManager::indexDatabaseByCoordinates(){
    unsigned int numOfIndexedLines = 0;

    string* line = new string();
    int lineNum = 0;
    databaseService.open();
    while(databaseService.getNextLine(*line)){

        ++lineNum;

        Point location = LineUtility::extractLocationFromLine(*line, LONGITUDE_COL, LATITUDE_COL, DELIM);
        coordinateIndex.insert(location, lineNum);
        ++numOfIndexedLines;
    }
    databaseService.close();
    return numOfIndexedLines;
}

// ToDo: implement the following method
list<GISRecord> SystemManager::findGISRecordsByCoordinates(Point location) {
    std::cout << "FINDING BY COORDINATES: (" << location.latitude << ", " << location.longitude << ")" << std::endl;
    list<GISRecord> offsets;

    try {
        auto lineNums = coordinateIndex.getOffsetsOfGISRecordsByLocation(location);

        if (!lineNums.empty()) {
            cout << "Found " << lineNums.size() << " records" << endl;
        }

        for(auto l: lineNums){
            cout << "\n" << l << endl;
        }

    } catch (const std::invalid_argument& e) {
        std::cout << e.what() << std::endl;
        location.print();
    }

    // Find in buffer
    //offsets = bufferPool.findGISRecordsByCoordinates(northWestPoint, southEastPoint);

    // If not found in buffer, find in coordinate index
    //offsets = coordinateIndex.getOffsetsOfGISRecords(northWestPoint, southEastPoint);

    return offsets;
}

// ToDo:: this method is implemented incorrectly. Fix it.
list<GISRecord> SystemManager::findGISRecordsByCoordinates(double latitude, double longitude, double halfHeight, double halfWidth) {
    //cout << "what is in " << latitude + halfHeight << " " << longitude - halfWidth << " " << latitude - halfHeight << " " << longitude + halfWidth <<endl;
    Point nw = {latitude + halfHeight, longitude - halfWidth };
    Point se = {latitude - halfHeight, longitude + halfWidth };
//    auto lineNums = coordinateIndex.getOffsetsOfGISRecords(nw, se);
//    for(auto l: lineNums){
//        cout << l << endl;
//    }

    list<GISRecord> offsets;

    // Find in buffer
    //offsets = bufferPool.findGISRecordsByCoordinates(northWestPoint, southEastPoint);

    // If not found in buffer, find in coordinate index
    //offsets = coordinateIndex.getOffsetsOfGISRecords(northWestPoint, southEastPoint);

    return offsets;
}

// ToDo: implement the following method
void SystemManager::whatIsIn(bool isFiltered, bool isDetailed, string filter, double latitude, double longitude,
                             double halfHeight, double halfWidth) {
    auto offsets = findGISRecordsByCoordinates(latitude, longitude, halfHeight, halfWidth);
}


void SystemManager::whatIs(string featureName, string stateAbrv){
    ostringstream os;
    os << featureName << " " << stateAbrv;
    const auto records = bufferPool.getRecordsByKey(os.str(), nameIndex);
    os.str("");
    os.clear();

    for(auto record: records){
        os.width(3);
        os << "" << record->lineNum << ": " << record->gisRecordPtr->getCountyName();
        os << "(" << record->gisRecordPtr->latDMSStr() << ", " << record->gisRecordPtr->longDMSStr() << ")";
    }
    if(records.empty()){
        os.width(3);
        os << "" << "No records match \"" << featureName << "\" and \"" << stateAbrv << "\"";
    }
    logLine(os.str());
    logLineBreak();
}

void SystemManager::logCommand(int cmdNumber, std::string function, list<std::string> args, char delimiter) {
    logService.logCommand(cmdNumber, function, args, delimiter);
}

void SystemManager::logComment(string comment){
    logService.logComment(comment);
}

void SystemManager::logLine(string text){
    logService.logString(text);
}


// ToDo: implement the following method
void SystemManager::debugQuad() {
}

void SystemManager::debugHash() {
    const string stats = nameIndex.str();
    logLine(stats);
    logLineBreak();

}

void SystemManager::debugPool() {
    const string pool = bufferPool.str();
    logLine(pool);
    logLineBreak();
}

// ToDo: implement the following method
void debugWorld() {
}

void SystemManager::logLineBreak(){
    int lineBreakLength = 90;
    char lineBreakChar = '-';
    logLine(string(lineBreakLength, lineBreakChar));
}


