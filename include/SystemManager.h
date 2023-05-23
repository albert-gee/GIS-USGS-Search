#ifndef FINAL_SYSTEMMANAGER_H
#define FINAL_SYSTEMMANAGER_H

#include <string>
#include "DMS.h"

using namespace std;

class SystemManager {
public:
/*    void whatIsAt(DMS coordinates);
    void whatIs(string featureName, string state);
    void whatIsIn(DMS coordinates,int halfHeight, int halfWidth);*/
    void import (string recordFileName, string databaseFileName);
    SystemManager() = default;
};


#endif //FINAL_SYSTEMMANAGER_H
