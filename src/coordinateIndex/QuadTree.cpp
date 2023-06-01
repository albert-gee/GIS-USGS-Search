#include "../../include/coordinateIndex/QuadTree.h"

QuadTree::QuadTree(Point northWestPoint, Point southEastPoint) {
    this->root = new QuadTreeQuadrant(northWestPoint, southEastPoint);
}

QuadTree::~QuadTree() = default;

void QuadTree::insert(const Entry& entry) {
    // This method goes through the tree recursively and inserts the point into the correct quadrant
    this->root->insert(entry, BUCKET_CAPACITY);
}
