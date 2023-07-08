#ifndef FINAL_BUFFERPOOL_H
#define FINAL_BUFFERPOOL_H
#include "../database/GISRecord.h"
#include "../nameIndex/NameIndex.h"
#include "../database/LineUtility.h"
#include "BufferRecord.h"
#include "../database/DbService.h"
#include "../coordinateIndex/QuadTree.h"
#include "Features.h"
#include <list>
#include <set>

using namespace std;

// This class describes the buffer pool.
// The buffer pool buffers up to 15 records and uses LRU replacement policy.
class BufferPool {
public:
    explicit BufferPool(DbService& dbService);

    // ToDo: implement the following methods
    list<BufferedRecord *> getRecordsByKey(string key, NameIndex &nameIndex);
    void printBuffer();

    string str();

    list<BufferedRecord *> getRecordsByCoordinate(Point point, QuadTree coordinateIndex);

    list<BufferedRecord *>
    getRecordsByCoordinateRange(bool isFiltered, string filter, Point nwPoint, Point sePoint,
                                QuadTree coordinateIndex);

private:
    // Buffering up to 15 records in the buffer pool
    const int MAX_SIZE = 15;

    Features features;

    DbService& databaseService;

    BufferedRecord * searchBuffer(int lineNum);
    // List of buffered records in the buffer pool
    list<BufferedRecord*> buffer;
    string getLineFromDB(int, string);

    list<BufferedRecord *> getRecordsByCoordinates(double latitude, double longitude, QuadTree coordinateIndex);

    BufferedRecord *searchBufferWithFilter(int lineNum, string filter);

    bool matchFilter(string filter, string feature);
};



#endif //FINAL_BUFFERPOOL_H
