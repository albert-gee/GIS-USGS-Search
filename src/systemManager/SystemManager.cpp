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

    // Import the records from the recordsDataSetFileLocation file to the databaseService file
    try {
        databaseService.import(recordsDataSetFileLocation);

        // Index the records in the databaseService file by feature name and state
        list<int>* nameImportStats = indexDatabaseByName();
        int numofIndexedLines = nameImportStats->front();
        nameImportStats->pop_front();
        int longestProbeSeq = nameImportStats->front();

        nameImportStats->pop_front();
        int avgNameLength = nameImportStats->front();
        nameImportStats->pop_front();

        indexDatabaseByCoordinates();

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

/*list<int> * SystemManager::indexDatabaseByNameAndCoordinates(){
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
        cout << *line << endl;
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
}*/


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

string SystemManager::convertLatDECtoDMS(double dec){
    string direction = dec < 0 ? "South" : "North";
    double absoluteDEC = abs(dec);
    unsigned int degrees = absoluteDEC;
    unsigned int minutes = absoluteDEC -= degrees;
    unsigned int seconds = absoluteDEC -= minutes;
    ostringstream os;
    os << degrees << "d " << minutes << "m " << seconds << "s " << direction;
    return os.str();
}

string SystemManager::convertLongDECtoDMS(double dec){
    string direction = dec >= 0 ? "East" : "West";
    double absoluteDEC = abs(dec);
    unsigned int degrees = absoluteDEC;
    unsigned int minutes = absoluteDEC -= degrees;
    unsigned int seconds = absoluteDEC -= minutes;
    ostringstream os;
    os << degrees << "d " << minutes << "m " << seconds << "s " << direction;
    return os.str();
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

void SystemManager::debugHash() {
    const string stats = nameIndex.str();
    logLine(stats);
    logLine(string(90, '-'));
    //nameIndex.printIndex();
}

void SystemManager::debugPool() {
    const string pool = bufferPool.str();
    logLine(pool);
}



