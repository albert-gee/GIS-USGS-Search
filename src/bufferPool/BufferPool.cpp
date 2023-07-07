#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include "../../include/bufferPool/BufferPool.h"
#include "../../include/coordinateIndex/QuadTree.h"

BufferPool::BufferPool(DbService& dbService)
    : databaseService{dbService} {}

// ToDo: Implement this function
// get line numbers from coordinateIndex and assign to lineNums
list<BufferedRecord*> BufferPool::getRecordsByCoordinates(double latitude, double longitude,
                                                          QuadTree coordinateIndex) {
    list<int> lineNums;

    list<BufferedRecord*> foundRecords;
    for(int l : lineNums){
        BufferedRecord* bufRecordPtr = searchBuffer(l);
        if(bufRecordPtr != nullptr){
            foundRecords.push_front(bufRecordPtr);
        }
    }
    return foundRecords;
}

list<BufferedRecord *> BufferPool::getRecordsByKey(string key, NameIndex &nameIndex)
{
    list<int> lineNums = nameIndex.getLineNumsByKey(key);
    list<BufferedRecord*> foundRecords;
    for(int l : lineNums){
        BufferedRecord * bufRecordPtr = searchBuffer(l);
        if(bufRecordPtr != nullptr) {
            foundRecords.push_front(bufRecordPtr);
        }
    }
    return foundRecords;
}

BufferedRecord * BufferPool::searchBuffer(int lineNum){
    auto b = buffer.begin();
    BufferedRecord *bufRecordptr = nullptr;

    // Loop until line is found in buffer or at the end of buffer
    while(b != buffer.end()){
        if(lineNum == (*b)->lineNum){
            BufferedRecord *recent = *b;
            buffer.erase(b);
            buffer.push_front(recent);
            return recent;
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
    return newRecord;
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
            os.width(2);
            os << "" << b->lineNum << ": " << b->gisRecordPtr->str() << endl;
        }
    }
    os << "LRU";
    return os.str();
}

