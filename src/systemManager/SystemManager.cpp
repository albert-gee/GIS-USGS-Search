#include <fstream>
#include <iostream>
#include <iomanip>
#include "../../include/systemManager/SystemManager.h"

SystemManager::SystemManager(NameIndex &nameIndex, const QuadTree &coordinateIndex, BufferPool &bufferPool,
                             DbService &databaseService, LogService &logService)
        : nameIndex{nameIndex}, coordinateIndex{coordinateIndex}, bufferPool{bufferPool},
          databaseService{databaseService}, logService{logService} {
}

void SystemManager::setCoordinateIndexBoundaries(DMS northLat, DMS southLat, DMS eastLong, DMS westLong) {

    coordinateIndex.setBoundingBox(
            Point(northLat, westLong),
            Point(southLat, eastLong));

    ostringstream os;
    os << "\t\t\t\t\tWorld boundaries are set to:" << endl;
    os << "\t\t\t\t\t\t" << northLat.toDmsString() << " (" << northLat.toDecimalString() << ")" << endl;
    os << "\t\t" << westLong.toDmsString() << " (" << westLong.toDecimalString() << ")\t"
       << eastLong.toDmsString() << " (" << eastLong.toDecimalString() << ")" << endl;
    os << "\t\t\t\t\t\t" << southLat.toDmsString() << " (" << southLat.toDecimalString() << ")";
    logService.logLineBreak();
    logService.logString("Latitude/longitude values in index entries are shown as signed integers, in total seconds.");
    logService.logLineBreak();
    logService.logString(os.str());
    logService.logLineBreak();
}

void SystemManager::import(const string &recordsDataSetFileLocation) {

    // Import the records from the recordsDataSetFileLocation file to the databaseService file
    try {
        databaseService.import(recordsDataSetFileLocation);

        // Index the records in the databaseService file by
        // 1 - feature name and state
        // 2 - location
        IndexStats indexStats = indexDatabaseByNameAndCoordinates();

        // Log the import statistics
        logService.logImportStats(indexStats);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

IndexStats SystemManager::indexDatabaseByNameAndCoordinates() {

    // This struct is used to collect statistics about name index
    IndexStats indexStats{};

    // Line in the database file to be indexed
    auto *line = new string();
    int lineNum = 0;

    // Open the database file and index each line
    databaseService.open();
    while (databaseService.getNextLine(*line)) {
        ++lineNum;
        GISRecord *gisRecord = LineUtility::extractGISRecordFromLine(*line, DELIM);

        // Index the line by name and state abbreviation and collect statistics about the index
        indexDatabaseRecordByName(gisRecord, lineNum, indexStats);

        // Index the same line by location
        indexDatabaseRecordByLocation(gisRecord, lineNum);

        // Increment the number of indexed lines for statistics
        ++indexStats.numOfIndexedLines;
    }
    databaseService.close();

    return indexStats;
}

void SystemManager::indexDatabaseRecordByName(GISRecord *gisRecord, int lineNum, IndexStats &nameIndexStats) {
    // Calculate statistics about the import
    nameIndexStats.totalNameLength += gisRecord->getFeatureName().length();

    // Index the concatenated string of feature name and state
    ostringstream os;
    os << gisRecord->getFeatureName() << " " << gisRecord->getStateAlpha();
    unsigned int probes = nameIndex.indexLine(os.str(), lineNum);

    // Find the longest probe sequence for statistics
    if (probes > nameIndexStats.longestProbeSeq) {
        nameIndexStats.longestProbeSeq = probes;
    }
}

void SystemManager::indexDatabaseRecordByLocation(GISRecord *gisRecord, int lineNum) {
    // Index the location and line number
    try {
        coordinateIndex.insert(gisRecord->getPrimaryLatitudeDms(), gisRecord->getPrimaryLongitudeDms(), lineNum);
    } catch (const std::invalid_argument &e) {
        std::cout << "Invalid location in the database" << std::endl;
        std::cout << e.what() << std::endl;
    }
}

void SystemManager::debugWorld() {
    /*const string& structure = coordinateIndex.str();
    logService.logString(structure);
    logService.logLineBreak();*/


}

void SystemManager::debugQuad() {
    const string& content = coordinateIndex.getContent();
    logService.logString(content);
    logService.logLineBreak();
}

void SystemManager::debugHash() {
    const string& stats = nameIndex.str();
    logService.logString(stats);
    logService.logLineBreak();

}


void SystemManager::debugPool() {
    const string pool = bufferPool.str();
    logService.logString(pool);
    logService.logLineBreak();
}

//// ToDo: implement the following method
//list<GISRecord> SystemManager::findGISRecordsByCoordinates(Point location) {
//    std::cout << "FINDING BY COORDINATES: (" << location.latitude << ", " << location.longitude << ")" << std::endl;
//    list<GISRecord> offsets;
//
//    try {
//        auto lineNums = coordinateIndex.getOffsetsOfGISRecordsByLocation(location);
//
//        if (!lineNums.empty()) {
//            cout << "Found " << lineNums.size() << " records" << endl;
//        }
//
//        for(auto l: lineNums){
//            cout << "\n" << l << endl;
//        }
//
//    } catch (const std::invalid_argument& e) {
//        std::cout << e.what() << std::endl;
//        location.print();
//    }
//
//    // Find in buffer
//    //offsets = bufferPool.findGISRecordsByCoordinates(northWestPoint, southEastPoint);
//
//    // If not found in buffer, find in coordinate index
//    //offsets = coordinateIndex.getOffsetsOfGISRecords(northWestPoint, southEastPoint);
//
//    return offsets;
//}

//// ToDo:: this method is implemented incorrectly. Fix it.
//list<GISRecord> SystemManager::findGISRecordsByCoordinates(double latitude, double longitude, double halfHeight, double halfWidth) {
//    //cout << "what is in " << latitude + halfHeight << " " << longitude - halfWidth << " " << latitude - halfHeight << " " << longitude + halfWidth <<endl;
//    Point nw = {latitude + halfHeight, longitude - halfWidth };
//    Point se = {latitude - halfHeight, longitude + halfWidth };
////    auto lineNums = coordinateIndex.getOffsetsOfGISRecords(nw, se);
////    for(auto l: lineNums){
////        cout << l << endl;
////    }
//
//    list<GISRecord> offsets;
//
//    // Find in buffer
//    //offsets = bufferPool.findGISRecordsByCoordinates(northWestPoint, southEastPoint);
//
//    // If not found in buffer, find in coordinate index
//    //offsets = coordinateIndex.getOffsetsOfGISRecords(northWestPoint, southEastPoint);
//
//    return offsets;
//}

//void SystemManager::whatIsIn(bool isFiltered, bool isDetailed, string filter, double latitude, double longitude,
//                             double halfHeight, double halfWidth) {
//    Point point = {latitude, longitude};
//    Point nwPoint = {latitude + halfHeight / 3600, longitude - halfWidth / 3600};
//    Point sePoint = {latitude - halfHeight / 3600, longitude + halfWidth / 3600};
//
//    auto records = bufferPool.getRecordsByCoordinateRange(isFiltered, filter, nwPoint, sePoint, coordinateIndex);
//    ostringstream os;
//    os.width(2);
//    os << "";
//    if (!records.empty()) {
//        if (isFiltered) {
//            os << "The following features matching your criteria were ";
//        } else {
//            os << "The following " << records.size() << " feature(s) were ";
//        }
//
//    } else {
//        os << "Nothing was ";
//    }
//
//    os << "found in (" << point.latitude.toDmsString() << " +/- " << halfHeight << ", " << point.longitude.toDmsString() << " +/- "
//       << halfWidth << ")\n\n";
//
//    for (auto r: records) {
//        if (isDetailed) {
//            os << r->gisRecordPtr->detailStr();
//        } else {
//            os.width(4);
//            os << "" << r->lineNum << ':'
//               << " \"" << r->gisRecordPtr->getFeatureName() << "\" "
//               << " \"" << r->gisRecordPtr->getCountyName() << "\" "
//               << " \"" << r->gisRecordPtr->getStateAlpha() << "\" "
//               << "(" << r->gisRecordPtr->latDMSStr() << ", " << r->gisRecordPtr->longDMSStr() << ")";
//        }
//    }
//    logLine(os.str());
//    logService.logLineBreak();
//}

void SystemManager::whatIsAt(Point point) {
    auto records = bufferPool.getRecordsByCoordinate(point, coordinateIndex);
    ostringstream os;
    os.width(2);
    os << "";
    if (!records.empty()) {
        os << "The following feature(s) were ";
    } else {
        os << "Nothing was ";
    }

    os << "found at (\"" << point.latitude.toDmsString() << ", " << point.longitude.toDmsString() << "\")\n";

    for (auto r: records) {
        os.width(4);
        os << "" << r->lineNum << ':'
           << " \"" << r->gisRecordPtr->getFeatureName() << "\" "
           << " \"" << r->gisRecordPtr->getCountyName() << "\" "
           << " \"" << r->gisRecordPtr->getStateAlpha() << "\" ";
    }
    logService.logString(os.str());
    logService.logLineBreak();
}

void SystemManager::whatIs(const string& featureName, const string& stateAbrv) {
    ostringstream os;
    os << featureName << " " << stateAbrv;
    const auto records = bufferPool.getRecordsByKey(os.str(), nameIndex);
    os.str("");
    os.clear();

    for (auto record: records) {
        os.width(3);
        os << "" << record->lineNum << ": " << record->gisRecordPtr->getCountyName();

        DMS latDms{record->gisRecordPtr->getPrimaryLatitudeDms()};
        DMS lngDms{record->gisRecordPtr->getPrimaryLongitudeDms()};
        os << "(" << latDms.toDmsString() << ", " << lngDms.toDmsString() << ")";
    }
    if (records.empty()) {
        os.width(3);
        os << "" << "No records match \"" << featureName << "\" and \"" << stateAbrv << "\"";
    }
    logService.logString(os.str());
    logService.logLineBreak();
}

void SystemManager::logCommand(int cmdNumber, std::string function, list<std::string> args, char delimiter) {
    logService.logCommand(cmdNumber, function, args, delimiter);
}

void SystemManager::logComment(string comment) {
    logService.logComment(comment);
}







