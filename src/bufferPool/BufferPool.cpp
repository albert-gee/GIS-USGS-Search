#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include "../../include/bufferPool/BufferPool.h"
#include "../../include/coordinateIndex/QuadTree.h"

BufferPool::BufferPool(DbService& dbService)
    : databaseService{dbService} {}
/*
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
}*/

list<BufferedRecord*> BufferPool::getRecordsByCoordinate(Point point, QuadTree coordinateIndex) {

    vector<int> lineNums = coordinateIndex.getOffsetsOfGISRecordsByLocation(point);

    list<BufferedRecord*> foundRecords;
    for(int l : lineNums){
        BufferedRecord* bufRecordPtr = searchBuffer(l);
        if(bufRecordPtr != nullptr){
            foundRecords.push_front(bufRecordPtr);
        }
    }
    return foundRecords;
}

list<BufferedRecord *>
BufferPool::getRecordsByCoordinateRange(bool isFiltered, string filter, Point nwPoint, Point sePoint,
                                        QuadTree coordinateIndex) {
    vector<int> lineNums = coordinateIndex.getOffsetsOfGISRecords(nwPoint, sePoint);

    list<BufferedRecord*> foundRecords;
    for(int l : lineNums){
        BufferedRecord* bufRecordPtr = nullptr;
        if(isFiltered){
            bufRecordPtr = searchBufferWithFilter(l, filter);
        } else {
            bufRecordPtr = searchBuffer(l);
        }

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
BufferedRecord * BufferPool::searchBufferWithFilter(int lineNum, string filter){

    auto b = buffer.begin();
    // Loop until line is found in buffer or at the end of buffer
    while(b != buffer.end()){
        if(lineNum == (*b)->lineNum){
            if(matchFilter(filter, (*b)->gisRecordPtr->getFeatureName())){
                BufferedRecord *recent = *b;
                buffer.erase(b);
                buffer.push_front(recent);
                return recent;
            }
            return nullptr;
        }
        ++b;
    }

    // Line not found
    // Get line from databaseService and create GIS record
    string line = databaseService.getLineByNumber(lineNum);
    GISRecord* gisRecord = LineUtility::createGISRecordFromLine(line, '|');
    if(matchFilter(filter, gisRecord->getFeatureName())){
        if(buffer.size() >= MAX_SIZE){
            buffer.pop_back();
        }
        BufferedRecord * newRecord =  new BufferedRecord(lineNum, gisRecord);
        buffer.push_front(newRecord);
        return newRecord;
    }
    return nullptr;
}

bool BufferPool::matchFilter(string filter, string feature){
    bool matchFilter = false;
    if(filter == "water"){
        if(features.getWater().find(feature) != features.getWater().end()){
            matchFilter = true;
        }
    } else if(filter == "structure"){
        if(features.getStructure().find(feature) != features.getStructure().end()){
            matchFilter = true;
        }
    } else if(filter == "pop"){
        if(features.getPop().find(feature) != features.getPop().end()){
            matchFilter = true;
        }
    }
    return matchFilter;
}


BufferedRecord * BufferPool::searchBuffer(int lineNum){
    auto b = buffer.begin();

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

