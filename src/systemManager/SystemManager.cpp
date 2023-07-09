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
        //databaseService.import(recordsDataSetFileLocation);
        Point nwpoint = coordinateIndex.getNWPoint();
        Point sepoint = coordinateIndex.getSEPoint();
        databaseService.import(recordsDataSetFileLocation, nwpoint.latitude.toDecimal(), sepoint.latitude.toDecimal(), nwpoint.longitude.toDecimal(), sepoint.longitude.toDecimal());
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
    const string& structure = coordinateIndex.str();
    logService.logString(structure);
    logService.logLineBreak();
//    Point nwpoint = coordinateIndex.getNWPoint();
//    Point sepoint = coordinateIndex.getSEPoint();
//
//    double totalLat = abs(nwpoint.latitude.toDecimal() - sepoint.latitude.toDecimal());
//    double totalLon = abs(sepoint.longitude.toDecimal() - nwpoint.longitude.toDecimal());
//    double latDecrements = totalLat / mapHeight;
//    double lonIncrements = totalLon / mapWidth;
//    int map[mapHeight][mapWidth] = {0};
//    string* line = new string();
//    // Open the database file and index each line
//    databaseService.open();
//    while (databaseService.getNextLine(*line)) {
//        GISRecord *gisRecord = LineUtility::extractGISRecordFromLine(*line, DELIM);
//        DMS lat (gisRecord->getPrimaryLatitudeDms());
//        DMS lon (gisRecord->getPrimaryLongitudeDms());
//        int i = 0;
//        double north = nwpoint.latitude.toDecimal();
//        while(north > lat.toDecimal()){
//            north -= latDecrements;
//            if(north > lat.toDecimal()){
//                ++i;
//            }
//        }
//
//        int j = 0;
//        double west = nwpoint.longitude.toDecimal();
//        while(west < lon.toDecimal()){
//            west += lonIncrements;
//            if(west < lon.toDecimal()){
//                ++j;
//            }
//        }
//        ++(map[i][j]);
//    }
//    databaseService.close();
//    ostringstream os;
//    os << '+' << string(mapWidth, '-')  << '+' << endl;
//    for(int i = 0; i < mapHeight; ++i){
//        for(int j = 0; j <= mapWidth + 1; ++j){
//           if(j == 0 || j == mapWidth + 1){
//               os << '|';
//           } else {
//               if(map[i][j-1] > 0){
//                   os << map[i][j-1];
//               } else {
//                   os << ' ';
//               }
//           }
//           if(j == mapWidth + 1){
//                os << endl;
//           }
//        }
//    }
//    os << '+' << string(mapWidth, '-')  << '+';
//    logService.logString(os.str());
//    logService.logLineBreak();
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

void SystemManager::whatIsIn(bool isFiltered, bool isDetailed, string &filter, string &latitude, string &longitude,
                             string &halfHeight, string &halfWidth) {
    int hlfh = stoi(halfHeight);
    int hlfw = stoi(halfWidth);

    DMS centerLat = DMS(latitude);
    DMS centerLng = DMS(longitude);

    // Set the boundaries of the search - NW and SE corners
    DMS nwLat{centerLat.degrees, centerLat.minutes, centerLat.seconds, centerLat.direction};
    nwLat.addSeconds(hlfh);
    DMS nwLng{centerLng.degrees, centerLng.minutes, centerLng.seconds, centerLng.direction};
    nwLng.addSeconds(hlfw);

    DMS seLat{centerLat.degrees, centerLat.minutes, centerLat.seconds, centerLat.direction};
    seLat.addSeconds(-hlfh);
    DMS seLng{centerLng.degrees, centerLng.minutes, centerLng.seconds, centerLng.direction};
    seLng.addSeconds(-hlfw);

    // Get the records from the buffer pool and coordinate index
    auto records = bufferPool.getRecordsByCoordinateRange(isFiltered, filter, {nwLat, nwLng}, {seLat, seLng}, coordinateIndex);

    ostringstream os;
    os.width(2);
    os << "";
    if (!records.empty()) {
        if (isFiltered) {
            os << "The following features matching your criteria were ";
        } else {
            os << "The following " << records.size() << " feature(s) were ";
        }

    } else {
        os << "Nothing was ";
    }

    Point centerPoint = {centerLat, centerLng};
    os << "found in (" << centerPoint.latitude.toDmsString() << " +/- " << halfHeight << ", " << centerPoint.longitude.toDmsString() << " +/- "
       << halfWidth << ")\n\n";

    for (auto r: records) {
        if (isDetailed) {
            os << r->gisRecordPtr->str();
        } else {
            os.width(4);
            os << "" << r->lineNum << ':'
               << " \"" << r->gisRecordPtr->getFeatureName() << "\" "
               << " \"" << r->gisRecordPtr->getCountyName() << "\" "
               << " \"" << r->gisRecordPtr->getStateAlpha() << "\" "
               << "(" << r->gisRecordPtr->getPrimaryLatitudeDms() << ", " << r->gisRecordPtr->getPrimaryLongitudeDms() << ")";
        }
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

void SystemManager::quit(){
    logService.logString("Terminating exeuction of commands.");
    logService.logLineBreak();
    time_t now = time(0);
    ostringstream os;
    os << "End Time: " << std::put_time(std::localtime(&now), "%a %b %d %X %Z %Y");
    logService.logString(os.str());
}






