#ifndef GIS_POINT_H
#define GIS_POINT_H

#include <iostream>

// This struct describes a geographic coordinate.
struct Point {
    double latitude;
    double longitude;

    Point(double lat, double lng) : latitude{lat}, longitude{lng} {}

    // Print the point
    void print() const {
        std::cout << "(" << latitude << ", " << longitude << ")" << std::endl;
    }
};

#endif //GIS_POINT_H
