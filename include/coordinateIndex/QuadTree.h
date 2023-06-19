#ifndef GIS_QUADTREE_H
#define GIS_QUADTREE_H

#include "Point.h"
#include "QuadTreeQuadrant.h"

// This class describes a QuadTree data structure. It is used to indexDatabaseByName the GIS records by their geographic coordinates.
class QuadTree {
public:
    QuadTree(Point northWestPoint, Point southEastPoint, unsigned long bucketCapacity);

    ~QuadTree();

    // Insert an offset of a GIS record into the QuadTree
    void insert(Point point, int offsetOfGISRecord);

    // Get the offsets of the GIS records in the QuadTree that are within the given bounding box
    std::vector<int> getOffsetsOfGISRecords(Point northWestPoint, Point southEastPoint);

    // Print the QuadTree
    void print();

private:
    unsigned long bucketCapacity;
    QuadTreeQuadrant *root;
};


#endif //GIS_QUADTREE_H
