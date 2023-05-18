#ifndef FINAL_BUFFERPOOL_H
#define FINAL_BUFFERPOOL_H
#include "GISRecord.h"
#include <list>
using namespace std;

class BufferPool {
public:
    GISRecord* searchForRecord(int featrureId);
private:
    const int maxSize = 15;
    list<GISRecord> buffer;
};



#endif //FINAL_BUFFERPOOL_H
