#ifndef GIS_ENTRY_H
#define GIS_ENTRY_H

#include <string>
#include <vector>
#include "Point.h"

struct Entry {
    Point location;

    std::vector<int> offsetsOfGISRecords;
};

#endif //GIS_ENTRY_H
