#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include "../include/BufferPool.h"



struct BufferPool::BufferedRecord{
    int lineNum;
    GISRecord* gisRecordPtr;
    BufferedRecord(int lineNum, GISRecord* gisRecord){
        BufferedRecord::lineNum = lineNum;
        gisRecordPtr = gisRecord;
    }
};

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

            GISRecord* gisRecord = createGISRecordFromLine(line);

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

GISRecord * BufferPool::createGISRecordFromLine(string line) {

    vector<string> *params = LineUtility::extractParametersFromLine(line);

    GISRecord * gisRecord = new GISRecord(stoi((*params)[0]),
                                    (*params)[1],
                                    (*params)[2],
                                    (*params)[3],
                                    (*params)[4],
                                    (*params)[5],
                                    (*params)[6],
                                    (*params)[7],
                                    (*params)[8],
                                    (*params)[9],
                                    (*params)[10],
                                    (*params)[11],
                                    (*params)[12],
                                    (*params)[13],
                                    (*params)[14],
                                    stoi((*params)[15]),
                                    stoi((*params)[16]),
                                    (*params)[17],
                                    (*params)[18],
                                    (*params)[19]);
    return gisRecord;
}

void BufferPool::printBuffer() {
    int index = 0;
    for(BufferedRecord * b: buffer){
        cout << "Buffer " << index++ << ": " << b->lineNum << ", " << b->gisRecordPtr->getFeatureName() <<endl;
    }
}
