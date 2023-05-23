#include <fstream>
#include <iostream>
#include "../include/SystemManager.h"


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
                databaseFile << line << endl;
                ++lineNum;
            }
            databaseFile.close();
            recordFile.close();
        } else {
            cerr << "Error: Failed to create database file." << std::endl;
        }
    }
}
