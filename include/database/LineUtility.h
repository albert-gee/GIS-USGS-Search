//
// Created by micgo on 6/10/2023.
//

#ifndef GIS_LINEUTILITY_H
#define GIS_LINEUTILITY_H

#include <string>
#include <list>
#include <vector>
#include "../coordinateIndex/Point.h"
#include "GISRecord.h"
#include <sstream>

using namespace std;

class LineUtility {
public:

    // Extract the fields from a line and create a GISRecord object from them.
    static GISRecord* extractGISRecordFromLine(const string& line, char delimiter) {

        vector<string> *fields = LineUtility::extractFieldsFromLine(line, delimiter);

        return new GISRecord(
            stoi((*fields)[0]),
            (*fields)[1],
            (*fields)[2],
            (*fields)[3],
            (*fields)[4],
            (*fields)[5],
            (*fields)[6],
            (*fields)[7],
            (*fields)[8],
            (*fields)[9],
            (*fields)[10],
            (*fields)[11],
            (*fields)[12],
            (*fields)[13],
            (*fields)[14],
            stoi((*fields)[15]),
            stoi((*fields)[16]),
            (*fields)[17],
            (*fields)[18],
            (*fields)[19]
        );
    }

private:

    // Extract the fields from a line, such as feature name, state alpha, etc.
    static vector<string> * extractFieldsFromLine(const string &line, char delimiter) {
        int index = 0;
        auto *fields = new vector<string>();

        ostringstream os;
        while (line[index] != '\0') {
            if (line[index] != delimiter) {
                os << line[index];
            } else {
                fields->push_back(os.str());
                os.str("");
                os.clear();
            }
            ++index;
        }
        if(fields->size() < 20){
            fields->push_back(os.str());
        }

        return fields;
    }
};


#endif //GIS_LINEUTILITY_H
