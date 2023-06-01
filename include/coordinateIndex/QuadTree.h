#ifndef GIS_QUADTREE_H
#define GIS_QUADTREE_H


#include "Point.h"
#include "QuadTreeQuadrant.h"

class QuadTree {
public:
    QuadTree(Point northWestPoint, Point southEastPoint);
    ~QuadTree();

    void insert(const Entry& entry);

private:
    const int BUCKET_CAPACITY{4};
    QuadTreeQuadrant* root;
};


#endif //GIS_QUADTREE_H
