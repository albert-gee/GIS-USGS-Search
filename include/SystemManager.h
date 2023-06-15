#ifndef FINAL_SYSTEMMANAGER_H
#define FINAL_SYSTEMMANAGER_H

#include <string>
#include "DMS.h"
#include "NameIndex.h"
#include "BufferPool.h"

using namespace std;

class SystemManager {
public:
/*    void whatIsAt(DMS coordinates);
    void whatIs(string featureName, string state);
    void whatIsIn(DMS coordinates,int halfHeight, int halfWidth);*/
    void import (string, string);
    void index(string);
    string extractKey(string);
    static const int FEATURE_NAME_COL = 1;
    static const int STATE_ALPHA_COL = 3;
    static const char DELIM = '|';
    SystemManager();
private:
    NameIndex nameIndex;
    BufferPool bufferPool;
};


#endif //FINAL_SYSTEMMANAGER_H
