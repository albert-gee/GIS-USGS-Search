#ifndef GIS_QUADTREEQUADRANT_H
#define GIS_QUADTREEQUADRANT_H

#include <vector>
#include "Point.h"
#include "Entry.h"

// This class describes a quadrant in the QuadTree
class QuadTreeQuadrant {
public:
    QuadTreeQuadrant(Point northWestPoint, Point southEastPoint, unsigned long bucketCapacity = 0);

    ~QuadTreeQuadrant();

    // Insert an entry into the quadrant
    void insert(const Entry &entry);

    // Get the offsets of the GIS records in the quadrant
    [[nodiscard]] std::vector<int> getOffsetsOfGISRecords(Point offsetNorthWestPoint, Point offsetSouthEastPoint) const;

    // Getters and setters for Borders
    [[nodiscard]] const Point &getNorthWestPoint() const;

    [[nodiscard]] const Point &getSouthEastPoint() const;

private:
    // Bounding box
    Point northWestPoint;
    Point southEastPoint;

    // Bucket contains entries in quadrant up to bucket capacity
    std::vector<Entry> bucket;
    // Bucket capacity.
    unsigned long bucketCapacity;

    // Children quadrants
    QuadTreeQuadrant *northWest;
    QuadTreeQuadrant *northEast;
    QuadTreeQuadrant *southWest;
    QuadTreeQuadrant *southEast;

    // Get bucket available capacity.
    // It is the number of entries that can be inserted into the bucket.
    // It is also the difference between the bucket capacity and the number of entries in the bucket.
    [[nodiscard]] unsigned long getBucketAvailableCapacity() const;

    // Insert an entry into the bucket
    void insertIntoBucket(const Entry &newEntry);

    // Insert an entry into the sub-quadrants if the bucket is full (bucketCapacity == 0)
    void insertIntoSubQuadrants(const Entry &newEntry);

    // If the bucket is full, divide the quadrant into sub-quadrants
    void divideQuadrantIntoSubQuadrants();

    // Clear the bucket
    void clearBucket();

};

#endif //GIS_QUADTREEQUADRANT_H
