//
// Created by micgo on 6/10/2023.
//

#ifndef GIS_LINEUTILITY_H
#define GIS_LINEUTILITY_H

#include <string>
#include <list>
#include <vector>

using namespace std;

class LineUtility {
public:
    static vector<string> * extractParametersFromLine(const string &line);
private:
    static const char DELIMITER = '|';
};


#endif //GIS_LINEUTILITY_H
