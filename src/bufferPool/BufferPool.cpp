#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include "../../include/bufferPool/BufferPool.h"

// ToDo: Implement this function
const list<GISRecord *> BufferPool::findGISRecordsByCoordinates(double latitude, double longitude) {
    for(BufferedRecord* bufferRecord : buffer) {
//        if (bufferRecord->gisRecordPtr->
    }
    return list<GISRecord *>();
}


// Search and return GISRecords from the database
list<GISRecord *> BufferPool::getRecordsByKey(string key, NameIndex &nameIndex, const string& databaseFileName)
{
    list<int> lineNums = nameIndex.getLineNumsByKey(key);
    list<GISRecord*> foundRecords;
    for(int l : lineNums){
        GISRecord * gisRecordPtr = searchBuffer(l, databaseFileName);
        if(gisRecordPtr != nullptr) {
            foundRecords.push_front(gisRecordPtr);
        }
    }
    return foundRecords;
}

// Search the buffer for a line number
GISRecord * BufferPool::searchBuffer(int lineNum, string databaseFileName){
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
    // Get line from database and create GIS record
    string line = getLineFromDB(lineNum, databaseFileName);
    GISRecord* gisRecord = LineUtility::createGISRecordFromLine(line, '|');
    if(buffer.size() >= MAX_SIZE){
        buffer.pop_back();
    }
    BufferedRecord * newRecord =  new BufferedRecord(lineNum, gisRecord);
    buffer.push_front(newRecord);
    return gisRecordptr;
}

// Get the line from database
string BufferPool::getLineFromDB(int lineNum, string databaseFileName){
    ifstream databaseFile(databaseFileName);
    string line;
    if(!databaseFile.is_open()){
        cerr << "Error: Failed to open database file." << endl;
    } else {
        int index = 1;
        while (index < lineNum && !databaseFile.eof()) {
            databaseFile.ignore(numeric_limits<streamsize>::max(), '\n');
            ++index;
        }
        if(!databaseFile.eof()){
            getline(databaseFile,line);

        }
    }
    databaseFile.close();

    return line;
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

