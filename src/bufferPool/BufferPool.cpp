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

list<GISRecord *> BufferPool::getRecordsByKey(string key, NameIndex &nameIndex, const string& databaseFileName)
{
    list<int> lineNums = nameIndex.getLineNumsByKey(key);
    list<GISRecord*> foundRecords;

    for(int l : lineNums){

        bool bufferFound = false;
        unsigned int bufferIndex = 0;
        auto b = buffer.begin();

        while(!bufferFound && b != buffer.end()){
            if(l == (*b)->lineNum){
                BufferedRecord *recent = *b;
                buffer.erase(b);
                buffer.push_front(recent);
                foundRecords.push_front((*b)->gisRecordPtr);
                bufferFound = true;
            }
            ++b;
        }
        if(!bufferFound){
            string line = getLineFromDB(l, databaseFileName);

            // ToDo: set the delimiter in createGISRecordFromLine()
            GISRecord* gisRecord = LineUtility::createGISRecordFromLine(line, '\t');

            if(buffer.size() >= MAX_SIZE){
                cout << "buffer popped" << endl;
                buffer.pop_back();
            }
            BufferedRecord * newRecord =  new BufferedRecord(l, gisRecord);
            buffer.push_front(newRecord);
            foundRecords.push_front(gisRecord);
        }
        cout << "buffer size: " << buffer.size() << endl;
    }
    return foundRecords;
}

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

void BufferPool::printBuffer() {
    int index = 0;
    for(BufferedRecord * b: buffer){
        cout << "Buffer " << index++ << ": " << b->lineNum << ", " << b->gisRecordPtr->getFeatureName() <<endl;
    }
}
