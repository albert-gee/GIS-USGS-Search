#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include "../../include/bufferPool/BufferPool.h"

BufferPool::BufferPool(DbService& dbService)
    : databaseService{dbService} {}

// ToDo: Implement this function
const list<GISRecord *> BufferPool::findGISRecordsByCoordinates(double latitude, double longitude) {
    for(BufferedRecord* bufferRecord : buffer) {
//        if (bufferRecord->gisRecordPtr->
    }
    return list<GISRecord *>();
}


// Search and return GISRecords from the databaseService
list<GISRecord *> BufferPool::getRecordsByKey(string key, NameIndex &nameIndex)
{
    list<int> lineNums = nameIndex.getLineNumsByKey(key);
    list<GISRecord*> foundRecords;
    for(int l : lineNums){
        GISRecord * gisRecordPtr = searchBuffer(l);
        if(gisRecordPtr != nullptr) {
            foundRecords.push_front(gisRecordPtr);
        }
    }
    return foundRecords;
}

// Search the buffer for a line number
GISRecord * BufferPool::searchBuffer(int lineNum){
    auto b = buffer.begin();
    GISRecord *gisRecordptr = nullptr;

    // Loop until line is found in buffer or at the end of buffer
    while(b != buffer.end()){
        if(lineNum == (*b)->lineNum){
            BufferedRecord *recent = *b;
            buffer.erase(b);
            buffer.push_front(recent);
            return (*b)->gisRecordPtr;
        }
        ++b;
    }

    // Line not found
    // Get line from databaseService and create GIS record
    string line = databaseService.getLineByNumber(lineNum);
    GISRecord* gisRecord = LineUtility::createGISRecordFromLine(line, '|');
    if(buffer.size() >= MAX_SIZE){
        buffer.pop_back();
    }
    BufferedRecord * newRecord =  new BufferedRecord(lineNum, gisRecord);
    buffer.push_front(newRecord);
    return gisRecordptr;
}



// Print contents of buffer
void BufferPool::printBuffer() {
    int index = 0;
    if(buffer.empty()){
        cout << "Buffer is empty" << endl;
    }
    else {
        for (BufferedRecord *b: buffer) {
            cout << "Buffer " << index++ << ": " << b->lineNum << ", " << b->gisRecordPtr->getFeatureName() << endl;
        }
    }
}

string BufferPool::str() {
    int index = 0;
    ostringstream os;
    os << "MRU" << endl;
    if (!buffer.empty()) {
        for (BufferedRecord *b: buffer) {
            os.width(4);
            os << b->lineNum << ": " << b->gisRecordPtr->getFeatureName() << endl;
        }
    }
    os << "LRU";
    return os.str();
}

