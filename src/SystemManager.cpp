#include <fstream>
#include <iostream>
#include "../include/SystemManager.h"

SystemManager::SystemManager() {
    nameIndex = NameIndex();
    bufferPool = BufferPool();
}

void SystemManager::import(string recordFileName, string databaseFileName){
    ifstream recordFile (recordFileName);
    ofstream databaseFile(databaseFileName, ios_base::app);
    if(!recordFile.is_open()) {
        cerr << "Error: Failed to create record file." << std::endl;
    } else {
        string line;
        int lineNum = 0;
        if(databaseFile.is_open()) {
            while (getline(recordFile, line)) {
                if(lineNum > 0){
                    databaseFile << line << endl;
                }
                ++lineNum;
            }
            databaseFile.close();
            recordFile.close();
        } else {
            cerr << "Error: Failed to create database file." << std::endl;
        }
    }
    //Michael's test
/*    index(databaseFileName);

    const list<GISRecord*> records = bufferPool.getRecordsByKey("Alkali Creek CO", nameIndex, databaseFileName);
    const list<GISRecord*> records2 = bufferPool.getRecordsByKey("Bob Creek CO", nameIndex, databaseFileName);
    const list<GISRecord*> records4 = bufferPool.getRecordsByKey("Nipple Peak Trail CO", nameIndex, databaseFileName);
    const list<GISRecord*> records5 = bufferPool.getRecordsByKey("Nipple Peak Trail CO", nameIndex, databaseFileName);
    const list<GISRecord*> records3 = bufferPool.getRecordsByKey("Bob Lake CO", nameIndex, databaseFileName);
    bufferPool.printBuffer();*/

}

void SystemManager::index(string databaseFileName){
    ifstream databaseFile(databaseFileName);
    if(!databaseFile.is_open()){
        cerr << "Error: Failed to open database file." << endl;
    } else {
        string line;
        int lineNum = 0;
        while(getline(databaseFile, line)){
            ++lineNum;
            string indexKey = extractKey(line);
            nameIndex.indexLine(indexKey, lineNum);
        }
    }

    databaseFile.close();
}

string SystemManager::extractKey(string line) {
    int index = 0;
    string featureName;
    string stateAlpha;
    int column = 0;
    while(line[index] != '\0' && column <= STATE_ALPHA_COL){
        if(line[index] != DELIM){
            if(column == FEATURE_NAME_COL) {
                featureName += line[index];
            } else if(column == STATE_ALPHA_COL){
                stateAlpha += line[index];
            }
        } else {
            ++column;
        }
        ++index;
    }
    return featureName + " " + stateAlpha;
}

