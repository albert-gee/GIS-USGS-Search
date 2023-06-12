#ifndef GIS_QUADTREEQUADRANT_H
#define GIS_QUADTREEQUADRANT_H

#include <vector>
#include "Point.h"
#include "Entry.h"

class QuadTreeQuadrant {
public:
    QuadTreeQuadrant(Point northWestPoint, Point southEastPoint);
    ~QuadTreeQuadrant();

    // Insert an entry into the quadrant
    void insert(const Entry& entry, int bucketCapacity);

    // Getters and setters

    // Getters and setters for Borders
    const Point &getNorthWestPoint() const;

    const Point &getSouthEastPoint() const;

    // Getters and setters for Entries
    const std::vector<Entry> &getEntries() const;

    // Getters and setters for sub-quadrants
    QuadTreeQuadrant *getNorthWest() const;

    void setNorthWest(QuadTreeQuadrant *northWest);

    QuadTreeQuadrant *getNorthEast() const;

    void setNorthEast(QuadTreeQuadrant *northEast);

    QuadTreeQuadrant *getSouthWest() const;

    void setSouthWest(QuadTreeQuadrant *southWest);

    QuadTreeQuadrant *getSouthEast() const;

    void setSouthEast(QuadTreeQuadrant *southEast);

private:
    // Bounding box
    Point northWestPoint;
    Point southEastPoint;

    // Entries in quadrant
    std::vector<Entry> entries;

    // Children quadrants
    QuadTreeQuadrant *northWest;
    QuadTreeQuadrant *northEast;
    QuadTreeQuadrant *southWest;
    QuadTreeQuadrant *southEast;

    void clearPoints();

};

#endif //GIS_QUADTREEQUADRANT_H
