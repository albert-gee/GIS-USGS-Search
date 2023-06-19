#include <iostream>
#include "../../include/coordinateIndex/QuadTree.h"

QuadTree::QuadTree(Point northWestPoint, Point southEastPoint, unsigned long bucketCapacity)
    : bucketCapacity{bucketCapacity} {
    this->root = new QuadTreeQuadrant{northWestPoint, southEastPoint, bucketCapacity};
}

QuadTree::~QuadTree() = default;

void QuadTree::print() {
    std::cout << "QuadTree" << std::endl;
    this->root->print();
}

void QuadTree::insert(Point location, int offsetOfGISRecord) {
    // This method goes through the tree recursively and inserts the point into the correct quadrant
    this->root->insert(location, offsetOfGISRecord);
}

std::vector<int> QuadTree::getOffsetsOfGISRecords(Point northWestPoint, Point southEastPoint) {
    return this->root->getOffsetsOfGISRecords(northWestPoint, southEastPoint);
}

