#ifndef GIS_POINT_H
#define GIS_POINT_H

#include <iostream>

// This struct describes a geographic coordinate.
struct Point {
    double x;
    double y;

    Point(double x, double y) : x{x}, y{y} {}

    // Print the point
    void print() const {
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }
};

#endif //GIS_POINT_H
