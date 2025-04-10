#ifndef GIS_QUADTREEQUADRANT_H
#define GIS_QUADTREEQUADRANT_H

#include <vector>
#include "Point.h"
#include "Entry.h"

// This class describes a quadrant in the QuadTree.
// Quadrant is a leaf of a tree. In a Bucket PR QuadTree, each leaf stores up to $bucketCapacity data objects of the
// type Entry. If a leaf's bucket is full, then the leaf will be partitioned into 4 quadrants: northWest, northEast,
// southWest, southEast. Then. all the data objects as well as the new data object will be inserted into those
// quadrants.
class QuadTreeQuadrant {
public:
    QuadTreeQuadrant(Point northWestPoint, Point southEastPoint, unsigned long bucketCapacity = 0);

    ~QuadTreeQuadrant();

    // Insert an entry into the quadrant. The entry is inserted into the bucket. If the bucket is full, the quadrant is
    // divided into four sub-quadrants and the entry and all entries from the bucket are inserted into them.
    void insert(Point location, int offsetOfGISRecord);

    // Returns a string representation of the structure of this quadrant and its sub-quadrants
    std::string str(unsigned int indent);

    // Returns a string representation of the content of this quadrant and its sub-quadrants
    std::string getContent();

    // Get the offsets of the GIS records in the quadrant
    [[nodiscard]] std::vector<int> getOffsetsOfGISRecordsWithin(Point offsetNorthWestPoint, Point offsetSouthEastPoint) const;

    // Get the offsets of the GIS records at specific location in the quadrant
    [[nodiscard]] std::vector<int> getOffsetsOfGISRecordsByLocation(Point location) const;

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
    const unsigned long bucketCapacity;

    // Children quadrants
    QuadTreeQuadrant *northWest;
    QuadTreeQuadrant *northEast;
    QuadTreeQuadrant *southWest;
    QuadTreeQuadrant *southEast;

    // Get bucket available capacity.
    // It is the number of entries that can be inserted into the bucket.
    // It is also the difference between the bucket capacity and the number of entries in the bucket.
    [[nodiscard]] unsigned long getBucketAvailableCapacity() const;

    // Check if the point is within the quadrant
    [[nodiscard]] bool isPointWithinQuadrant(Point point) const;

    // Insert an offset and its location into the bucket
    void insertIntoBucket(Point location, int offsetOfGISRecord);

    // Insert an entry into the sub-quadrants if the bucket is full (bucketCapacity == 0)
    void insertIntoSubQuadrants(Point location, int offsetOfGISRecord);

    // If the bucket is full, divide the quadrant into sub-quadrants
    void divideQuadrantIntoSubQuadrants();

    // Move entries from the bucket into the sub-quadrants
    void moveEntriesFromBucketIntoSubQuadrants();

    // Clear the bucket
    void clearBucket();

};

#endif //GIS_QUADTREEQUADRANT_H
