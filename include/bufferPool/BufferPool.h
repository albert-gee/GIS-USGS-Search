#ifndef FINAL_BUFFERPOOL_H
#define FINAL_BUFFERPOOL_H
#include "../database/GISRecord.h"
#include "../nameIndex/NameIndex.h"
#include "../database/LineUtility.h"
#include "BufferRecord.h"
#include "../database/DbService.h"
#include "../coordinateIndex/QuadTree.h"
#include <list>
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

private:
    // Buffering up to 15 records in the buffer pool
    static const int MAX_SIZE = 15;

    DbService& databaseService;

    BufferedRecord * searchBuffer(int lineNum);
    // List of buffered records in the buffer pool
    list<BufferedRecord*> buffer;
    string getLineFromDB(int, string);

    list<BufferedRecord *> getRecordsByCoordinates(double latitude, double longitude, QuadTree coordinateIndex);

    list<BufferedRecord *> getRecordsByCoordinateRange(Point nwPoint, Point sePoint, QuadTree coordinateIndex);
};



#endif //FINAL_BUFFERPOOL_H
