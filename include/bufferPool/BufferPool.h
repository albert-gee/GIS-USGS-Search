#ifndef FINAL_BUFFERPOOL_H
#define FINAL_BUFFERPOOL_H
#include "../GISRecord.h"
#include "../NameIndex.h"
#include "../LineUtility.h"
#include "../../src/bufferPool/BufferRecord.h"
#include <list>
using namespace std;

// This class describes the buffer pool.
// The buffer pool buffers up to 15 records and uses LRU replacement policy.
class BufferPool {
public:
    BufferPool() = default;

    // ToDo: implement the following methods
    list<GISRecord *> getRecordsByKey(string key, NameIndex &nameIndex, const string& databaseFileName);
    const list<GISRecord *> findGISRecordsByCoordinates(double latitude, double longitude);
    void printBuffer();

    string str();

private:
    // Buffering up to 15 records in the buffer pool
    static const int MAX_SIZE = 15;

    GISRecord * searchBuffer(int lineNum, string databaseFileName);
    // List of buffered records in the buffer pool
    list<BufferedRecord*> buffer;
    string getLineFromDB(int, string);

};



#endif //FINAL_BUFFERPOOL_H
