#ifndef FINAL_SYSTEMMANAGER_H
#define FINAL_SYSTEMMANAGER_H

#include <string>
#include "../coordinateIndex/DMS.h"
#include "../nameIndex/NameIndex.h"
#include "../bufferPool/BufferPool.h"
#include "../coordinateIndex/QuadTree.h"
#include "../log/LogService.h"
#include "../database/DbService.h"
#include "../log/IndexStats.h"

using namespace std;

// This class describes the system manager.
// The SystemManager uses system components to import, index, store, and retrieve data.
class SystemManager {
public:
    static const char DELIM = '|';

    SystemManager(NameIndex &nameIndex, const QuadTree &coordinateIndex, BufferPool &bufferPool,
                  DbService &databaseService, LogService &logService);

    // The "world" command.
    // Set boundaries for the coordinate index
    void setCoordinateIndexBoundaries(DMS north, DMS south, DMS east, DMS west);

    // The "import" command.
    // Add all the valid records from the file recordsDataSetFileLocation to the databaseService file.
    void import(const string &recordsDataSetFileLocation);

    // The four variations of "debug" command.
    void debugQuad();
    void debugHash();
    void debugPool();
    void debugWorld();

    // The "what_is" command.
    void whatIs(const string &featureName, const string &stateAbrv);

    // The "what_is_in" command.
    void
    whatIsIn(bool isFiltered, bool isDetailed, string &filter, string &latitude, string &longitude,
             string &halfHeight, string &halfWidth);

    // The "what_is_at" command.
    void whatIsAt(Point point);

    void logCommand(int cmdNumber, string function, list <string> args, char delimiter);

    void logComment(string comment);

private:
    // System components: the name index, the coordinate index, and the buffer pool
    NameIndex nameIndex;
    QuadTree coordinateIndex;
    BufferPool bufferPool;

    static const int mapHeight = 40;
    static const int mapWidth = 150;

    // Database service
    DbService &databaseService;

    // Log service and its constants
    LogService &logService;

    // Index the records in the databaseService file by location and concatenated feature name and state.
    // Collect statistics about the index.
    // Uses the indexDatabaseByName() and indexDatabaseByCoordinates() methods.
    // Returns the statistics about the index.
    IndexStats indexDatabaseByNameAndCoordinates();

    // Index an individual GIS record from the database by name and state abbreviation and collect statistics about the
    // index
    void indexDatabaseRecordByName(GISRecord *gisRecord, int lineNum, IndexStats &nameIndexStats);

    // Index an individual GIS record from the database by location
    void indexDatabaseRecordByLocation(GISRecord *gisRecord, int lineNum);
};


#endif //FINAL_SYSTEMMANAGER_H
