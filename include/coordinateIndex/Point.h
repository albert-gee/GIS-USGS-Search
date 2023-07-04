#ifndef GIS_POINT_H
#define GIS_POINT_H

#include <iostream>
#include <iomanip>

// This struct describes a geographic coordinate.
struct Point {
    double latitude;
    double longitude;

    Point(const std::string& lat, const std::string& lng){
        latitude = stod(lat);
        longitude = stod(lng);
    }

    Point(double lat, double lng) : latitude{lat}, longitude{lng} {}

    // Print the point
    void print() const {
        std::cout << std::fixed << std::setprecision(7);
        std::cout << "(" << latitude << ", " << longitude << ")";
    }
};

#endif //GIS_POINT_H
