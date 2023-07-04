#include <iostream>
#include "../../include/coordinateIndex/QuadTree.h"

QuadTree::QuadTree(unsigned long bucketCapacity) {
    this->bucketCapacity = bucketCapacity;
}

QuadTree::~QuadTree() = default;

void QuadTree::setBoundingBox(Point northWestPoint, Point southEastPoint) {
    this->root = new QuadTreeQuadrant{northWestPoint, southEastPoint, bucketCapacity};
}

void QuadTree::print() {
    std::cout << "\n\n=== QuadTree ===" << std::endl;

    if(this->root != nullptr) {
        this->root->print();
    } else {
        std::cout << "The QuadTree is empty" << std::endl;
    }
}

void QuadTree::insert(Point location, int offsetOfGISRecord) {
    // This method goes through the tree recursively and inserts the point into the correct quadrant
    try {
        this->root->insert(location, offsetOfGISRecord);
        std::cout << "Inserted " << offsetOfGISRecord << " at " << location.latitude << ", " << location.longitude << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cout << " ! Failed to insert " << offsetOfGISRecord << " at " << location.latitude << ", "
                  << location.longitude << std::endl;
        std::cout << e.what() << std::endl;
    }
}

std::vector<int> QuadTree::getOffsetsOfGISRecords(Point northWestPoint, Point southEastPoint) {
    return this->root->getOffsetsOfGISRecords(northWestPoint, southEastPoint);
}

