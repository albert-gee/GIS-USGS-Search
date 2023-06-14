#ifndef GIS_QUADTREE_H
#define GIS_QUADTREE_H


#include "Point.h"
#include "QuadTreeQuadrant.h"

class QuadTree {
public:
    QuadTree(Point northWestPoint, Point southEastPoint, int bucketCapacity);
    ~QuadTree();

    void insert(const Entry& entry);
    std::vector<int> getOffsetsOfGISRecords(Point northWestPoint, Point southEastPoint);

private:
    int bucketCapacity;
    QuadTreeQuadrant* root;
};


#endif //GIS_QUADTREE_H
