#include "../../include/coordinateIndex/QuadTree.h"

QuadTree::QuadTree(Point northWestPoint, Point southEastPoint, int bucketCapacity)
    : bucketCapacity{bucketCapacity} {
    this->root = new QuadTreeQuadrant{northWestPoint, southEastPoint};
}

QuadTree::~QuadTree() = default;

void QuadTree::insert(const Entry& entry) {
    // This method goes through the tree recursively and inserts the point into the correct quadrant
    this->root->insert(entry, bucketCapacity);
}

std::vector<int> QuadTree::getOffsetsOfGISRecords(Point northWestPoint, Point southEastPoint) {
    return this->root->getOffsetsOfGISRecords(northWestPoint, southEastPoint);
}

