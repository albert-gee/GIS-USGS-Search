#ifndef FINAL_SYSTEMMANAGER_H
#define FINAL_SYSTEMMANAGER_H

#include <string>
#include "../DMS.h"
#include "../nameIndex/NameIndex.h"
#include "../bufferPool/BufferPool.h"
#include "../coordinateIndex/QuadTree.h"
#include "../log/LogService.h"
#include "../database/DbService.h"

using namespace std;

// This class describes the system manager.
// The SystemManager uses system components to import, index, store, and retrieve data.
class SystemManager {
public:
    // Constants related to the format of the records in the databaseService file
    static const int FEATURE_NAME_COL = 1;
    static const int STATE_ALPHA_COL = 3;
    static const char DELIM = '|';
    static const int LATITUDE_COL = 9;
    static const int LONGITUDE_COL = 10;


    // ToDo: implement the following methods
/*    void whatIsAt(DMS coordinates);
    void whatIs(string featureName, string state);
    void whatIsIn(DMS coordinates,int halfHeight, int halfWidth);*/

    // The "world" command.
    // Set boundaries for the coordinate index
    void setCoordinateIndexBoundaries(double north, double south, double east, double west);

    // The "import" command.
    // Add all the valid records from the file recordsDataSetFileLocation to the databaseService file.
    void import (const string& recordsDataSetFileLocation);

    // Find GIS records that match the given coordinates.
    list<GISRecord> findGISRecordsByCoordinates(double latitude, double longitude);
    void whatIs(string featureName, string stateAbrv);
    SystemManager(NameIndex& nameIndex, const QuadTree& coordinateIndex, BufferPool& bufferPool, DbService& databaseService, LogService& logService);


    void logCommand(int cmdNumber, string function, list <string> args, char delimiter);
    void logComment(string comment);

    void debugHash();

private:
    // System components: the name index, the coordinate index, and the buffer pool
    NameIndex nameIndex;
    QuadTree coordinateIndex;
    BufferPool bufferPool;


    // Database service
    DbService& databaseService;

    // Log serviced
    LogService& logService;

    // Index the records in the databaseService file by feature name and state
    list<int> * indexDatabaseByName();

    // Index the records in the databaseService file by location
    unsigned int indexDatabaseByCoordinates();

    void logLine(string text);
};


#endif //FINAL_SYSTEMMANAGER_H
