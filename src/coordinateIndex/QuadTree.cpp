#include <sstream>
#include "../../include/coordinateIndex/QuadTree.h"

QuadTree::QuadTree(unsigned long bucketCapacity) {
    this->bucketCapacity = bucketCapacity;
}

QuadTree::~QuadTree() = default;

void QuadTree::setBoundingBox(Point northWestPoint, Point southEastPoint) {
    this->root = new QuadTreeQuadrant{northWestPoint, southEastPoint, bucketCapacity};
}

std::string QuadTree::str() const {
    std::ostringstream os;
    os << "Structure of the Coordinate Index:" << std::endl;

    if(this->root != nullptr) {
        os << this->root->str(1);
    } else {
        os << "The QuadTree is empty" << std::endl;
    }

    os << std::endl;

    return os.str();
}

std::string QuadTree::getContent() const {
    std::ostringstream os;
    os << "Content of the Coordinate Index:\n" << std::endl;

    if(this->root != nullptr) {
        os << this->root->getContent();
    } else {
        os << "The QuadTree is empty" << std::endl;
    }

    os << std::endl;

    return os.str();
}

void QuadTree::insert(const std::string& latitude, const std::string& longitude, int offsetOfGISRecord) {
    // This method goes through the tree recursively and inserts the point into the correct quadrant

    try {
        // The location point is created from the latitude and longitude provided as strings
        // The latitude and longitude are validated. An exceptions will be thrown if they are not valid
        Point location{DMS(latitude), DMS(longitude)};

        this->root->insert(location, offsetOfGISRecord);
    } catch (const std::invalid_argument& e) {
        std::cout << "! Failed to insert the record #" << offsetOfGISRecord << " - " << e.what() << std::endl;
    }
}

std::vector<int> QuadTree::getOffsetsOfGISRecords(Point northWestPoint, Point southEastPoint) {
    return this->root->getOffsetsOfGISRecords(northWestPoint, southEastPoint);
}

std::vector<int> QuadTree::getOffsetsOfGISRecordsByLocation(Point location) {
    return this->root->getOffsetsOfGISRecordsByLocation(location);
}

