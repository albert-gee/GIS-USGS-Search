#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include "../../include/bufferPool/BufferPool.h"
#include "../../include/coordinateIndex/QuadTree.h"

BufferPool::BufferPool(DbService& dbService)
    : databaseService{dbService} {}

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

list<BufferedRecord *> BufferPool::getRecordsByCoordinateRange(bool isFiltered, string filter, Point nwPoint, Point sePoint,
                                        QuadTree coordinateIndex) {
    vector<int> lineNums = coordinateIndex.getOffsetsOfGISRecordsWithin(nwPoint, sePoint);

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
            if(matchFilter(filter, (*b)->gisRecordPtr->getFeatureClass())){
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
    GISRecord* gisRecord = LineUtility::extractGISRecordFromLine(line, '|');
    if(matchFilter(filter, gisRecord->getFeatureClass())){
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
    GISRecord* gisRecord = LineUtility::extractGISRecordFromLine(line, '|');
    if(buffer.size() >= MAX_SIZE){
        buffer.pop_back();
    }
    BufferedRecord * newRecord =  new BufferedRecord(lineNum, gisRecord);
    buffer.push_front(newRecord);
    return newRecord;
}

string BufferPool::str() {

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

