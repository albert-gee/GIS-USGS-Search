//
// Created by micgo on 6/10/2023.
//
#include <sstream>
#include <iostream>
#include "../include/LineUtility.h"

vector<string> * LineUtility::extractParametersFromLine(const string &line) {
    int index = 0;
    vector<string> *parameters = new vector<string>();
    ostringstream os;
    while (line[index] != '\0') {
        if (line[index] != DELIMITER) {
            os << line[index];
        } else {
            parameters->push_back(os.str());
            os.str("");
            os.clear();
        }
        ++index;
    }
    if(parameters->size() < 20){
        parameters->push_back(os.str());
    }
    return parameters;
}