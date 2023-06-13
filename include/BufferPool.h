#ifndef FINAL_BUFFERPOOL_H
#define FINAL_BUFFERPOOL_H
#include "GISRecord.h"
#include "NameIndex.h"
#include "LineUtility.h"
#include <list>
using namespace std;

class BufferPool {
public:
    list<GISRecord *> getRecordsByKey(string key, NameIndex &nameIndex, const string& databaseFileName);
    const list<GISRecord *> getRecordsByCoorindates();
    BufferPool() = default;
    void printBuffer();
private:
    static const int MAX_SIZE = 15;
    struct BufferedRecord;
    list<BufferedRecord*> buffer;
    string getLineFromDB(int, string);
    GISRecord * createGISRecordFromLine(string);
};



#endif //FINAL_BUFFERPOOL_H
