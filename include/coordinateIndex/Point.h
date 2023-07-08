#ifndef GIS_POINT_H
#define GIS_POINT_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>
#include "DMS.h"

// This struct describes a geographic coordinate.
struct Point {
    DMS latitude;
    DMS longitude;

    // Create a point from a latitude and longitude
    Point(const DMS& lat, const DMS& lng)
        : latitude(lat), longitude(lng) {}

    // Print the point
    void print() const {
        std::cout << std::fixed << std::setprecision(7);
        std::cout << "(" << latitude.toDecimalString() << ", " << longitude.toDecimalString() << ")";
    }
};

#endif //GIS_POINT_H
