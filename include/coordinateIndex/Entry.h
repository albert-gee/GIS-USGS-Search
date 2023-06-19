#ifndef GIS_ENTRY_H
#define GIS_ENTRY_H

#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include "Point.h"

// This struct describes an indexDatabaseByName entry held in QuadTree.
// It stores a geographic coordinate of the type Point and a collection of the database file offsets of the matching GIS
// records. One entry within one location can contain infinite GIS records offsets.
struct Entry {
    Point location;

    std::vector<int> offsetsOfGISRecords;

    Entry(Point location, std::vector<int> offsetsOfGISRecords) : location{location}, offsetsOfGISRecords{std::move(offsetsOfGISRecords)} {}

    // Print the entry
    void print() const {
        std::cout << "Entry" << std::endl;
        std::cout << "Location: ";
        location.print();
        std::cout << "Offsets of GIS records: ";
        for (int offsetOfGISRecord : offsetsOfGISRecords) {
            std::cout << offsetOfGISRecord << " ";
        }
        std::cout << std::endl;
    }
};

#endif //GIS_ENTRY_H
