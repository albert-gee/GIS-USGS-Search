#include <stdexcept>
#include <iostream>
#include "../../include/coordinateIndex/QuadTreeQuadrant.h"

QuadTreeQuadrant::QuadTreeQuadrant(Point northWestPoint, Point southEastPoint, unsigned long bucketCapacity)
        : northWestPoint{northWestPoint}, southEastPoint{southEastPoint}, bucket{}, bucketCapacity{bucketCapacity} {
    this->northWest = nullptr;
    this->northEast = nullptr;
    this->southWest = nullptr;
    this->southEast = nullptr;
}

QuadTreeQuadrant::~QuadTreeQuadrant() {
    delete this->northWest;
    delete this->northEast;
    delete this->southWest;
    delete this->southEast;
}

unsigned long QuadTreeQuadrant::getBucketAvailableCapacity() const {
    return bucketCapacity - bucket.size();
}

void QuadTreeQuadrant::print() const {
    std::cout << "QuadTreeQuadrant" << std::endl;
    std::cout << "northWestPoint: ";
    this->northWestPoint.print();
    std::cout << "southEastPoint: ";
    this->southEastPoint.print();
    std::cout << "bucketCapacity: " << this->bucketCapacity << std::endl;
    std::cout << "bucket.size(): " << this->bucket.size() << std::endl;
    std::cout << "bucketCapacity - bucket.size(): " << this->getBucketAvailableCapacity() << std::endl;
    std::cout << "bucket: " << std::endl;
    for (const auto &entry: bucket) {
        entry.print();
    }
    std::cout << std::endl;

    if (this->northWest != nullptr) {
        this->northWest->print();
    }
    if (this->northEast != nullptr) {
        this->northEast->print();
    }
    if (this->southWest != nullptr) {
        this->southWest->print();
    }
    if (this->southEast != nullptr) {
        this->southEast->print();
    }
}

// Insert an entry into the quadrant. The entry is inserted into the bucket. If the bucket is full, the quadrant is
// divided into four sub-quadrants and the entry and all entries from the bucket are inserted into them.
void QuadTreeQuadrant::insert(const Point location, int offsetOfGISRecord) {

    // Check if the bucket is not full
    if (getBucketAvailableCapacity() > 0) {

        // Insert the offset and its location into the bucket
        insertIntoBucket(location, offsetOfGISRecord);
    } else {

        // Otherwise, check if the quadrant is not divided into sub-quadrants
        if (northWest == nullptr) {
            // If so, divide the quadrant into four sub-quadrants
            divideQuadrantIntoSubQuadrants();

            // Then, move the entries from the bucket into the sub-quadrants
            for (const auto &entry: bucket) {
                insertIntoSubQuadrants(location, offsetOfGISRecord);
            }

            // Clear the points from the bucket
            clearBucket();
        }

        // Insert the entry into the sub-quadrants since the bucket is full
        insertIntoSubQuadrants(location, offsetOfGISRecord);
    }
}

void QuadTreeQuadrant::divideQuadrantIntoSubQuadrants() {
    // 1. Find the center coordinates of the quadrant
    double centerX = (getNorthWestPoint().x + getSouthEastPoint().x) / 2.0;
    double centerY = (getNorthWestPoint().y + getSouthEastPoint().y) / 2.0;

    // 2. Create the four sub-quadrants
    northWest = new QuadTreeQuadrant(
            getNorthWestPoint(),
            Point(centerX, centerY));
    northEast = new QuadTreeQuadrant(
            Point(centerX, getNorthWestPoint().y),
            Point(getSouthEastPoint().x, centerY));
    southWest = new QuadTreeQuadrant(
            Point(getNorthWestPoint().x, centerY),
            Point(centerX, getSouthEastPoint().y));
    southEast = new QuadTreeQuadrant(
            Point(centerX, centerY),
            getSouthEastPoint());

}

void QuadTreeQuadrant::insertIntoBucket(const Point location, const int offsetOfGISRecord) {
    // Check if an entry with the same location already exists in the bucket
    bool exists = false;
    for (Entry &entry: bucket) {

        // If so, add the offset of the GIS record to the entry and return
        if (entry.location.x == location.x && entry.location.y == location.y) {
            entry.offsetsOfGISRecords.push_back(offsetOfGISRecord);
            exists = true;
        }
    }

    // If an entry with the same location doesn't exist, create a new entry and add it to the bucket
    if (!exists) {
        Entry newEntry{location, {offsetOfGISRecord}};
        bucket.push_back(newEntry);
    }
}

void QuadTreeQuadrant::insertIntoSubQuadrants(Point location, int offsetOfGISRecord) {

    // Check if the point is in the north-west quadrant
    if (location.x < northEast->getNorthWestPoint().x &&
        location.y < southWest->getNorthWestPoint().y) {
        northWest->insert(location, offsetOfGISRecord);
        // Check if the point is in the north-east quadrant
    } else if (location.x >= northEast->getNorthWestPoint().x &&
               location.y < southWest->getNorthWestPoint().y) {
        northEast->insert(location, offsetOfGISRecord);
        // Check if the point is in the south-west quadrant
    } else if (location.x < northEast->getNorthWestPoint().x &&
               location.y >= southWest->getNorthWestPoint().y) {
        southWest->insert(location, offsetOfGISRecord);
        // Check if the point is in the south-east quadrant
    } else if (location.x >= northEast->getNorthWestPoint().x &&
               location.y >= southWest->getNorthWestPoint().y) {
        southEast->insert(location, offsetOfGISRecord);
    }
}

std::vector<int> QuadTreeQuadrant::getOffsetsOfGISRecords(Point offsetNorthWestPoint, Point offsetSouthEastPoint) const {

    // Check if the given bounding box is contained in the quadrant
    // If not, throw an exception
    if (offsetNorthWestPoint.x < getNorthWestPoint().x && offsetSouthEastPoint.x > getSouthEastPoint().x) {
        throw std::invalid_argument("The given bounding box is not contained in the quadrant.");
    }

    // This vector will contain the offsetsVector of the GIS records and be returned by the method
    std::vector<int> offsetsVector;

    // If the quadrant is a leaf node, return the offsets Vector of the GIS records
    if (northWest == nullptr) {
        for (const Entry &entry: bucket) {

            if (entry.location.x >= offsetNorthWestPoint.x &&
                entry.location.y >= offsetNorthWestPoint.y &&
                entry.location.x <= offsetSouthEastPoint.x &&
                entry.location.y <= offsetSouthEastPoint.y) {
                // If the point is in the bounding box, add the offsetsVector of the GIS records to the vector
                offsetsVector.insert(offsetsVector.end(), entry.offsetsOfGISRecords.begin(),
                                     entry.offsetsOfGISRecords.end());
            }
        }
    } else {
        // Otherwise, recursively get the offsetsVector of the GIS records from the sub-quadrants

        std::vector<int> offsets;
        // Check if the north-west quadrant is contained in the bounding box
        if (offsetNorthWestPoint.x <= northWest->getNorthWestPoint().x &&
            offsetNorthWestPoint.y <= northWest->getNorthWestPoint().y &&
            offsetSouthEastPoint.x >= northWest->getSouthEastPoint().x &&
            offsetSouthEastPoint.y >= northWest->getSouthEastPoint().y) {
            // Get the offsetsVector of the GIS records from the north-west quadrant
            std::vector<int> offsetsFromNorthWest = northWest->getOffsetsOfGISRecords(offsetNorthWestPoint,
                                                                                      offsetSouthEastPoint);
            // Insert the offsetsVector from the north-west quadrant into the vector
            offsets.insert(offsets.end(), offsetsFromNorthWest.begin(), offsetsFromNorthWest.end());
        }
        // Check if the north-east quadrant is contained in the bounding box
        if (offsetNorthWestPoint.x <= northEast->getNorthWestPoint().x &&
            offsetNorthWestPoint.y <= northEast->getNorthWestPoint().y &&
            offsetSouthEastPoint.x >= northEast->getSouthEastPoint().x &&
            offsetSouthEastPoint.y >= northEast->getSouthEastPoint().y) {
            // Get the offsetsVector of the GIS records from the north-east quadrant
            std::vector<int> offsetsFromNorthEast = northEast->getOffsetsOfGISRecords(offsetNorthWestPoint,
                                                                                           offsetSouthEastPoint);
            // Insert the offsetsVector from the north-east quadrant into the vector
            offsets.insert(offsets.end(), offsetsFromNorthEast.begin(), offsetsFromNorthEast.end());
        }
        // Check if the south-west quadrant is contained in the bounding box
        if (offsetNorthWestPoint.x <= southWest->getNorthWestPoint().x &&
            offsetNorthWestPoint.y <= southWest->getNorthWestPoint().y &&
            offsetSouthEastPoint.x >= southWest->getSouthEastPoint().x &&
            offsetSouthEastPoint.y >= southWest->getSouthEastPoint().y) {
            // Get the offsetsVector of the GIS records from the south-west quadrant
            std::vector<int> offsetsFromSouthWest = southWest->getOffsetsOfGISRecords(offsetNorthWestPoint,
                                                                                           offsetSouthEastPoint);
            // Insert the offsetsVector from the south-west quadrant into the vector
            offsets.insert(offsets.end(), offsetsFromSouthWest.begin(), offsetsFromSouthWest.end());
        }
        // Check if the south-east quadrant is contained in the bounding box
        if (offsetNorthWestPoint.x <= southEast->getNorthWestPoint().x &&
            offsetNorthWestPoint.y <= southEast->getNorthWestPoint().y &&
            offsetSouthEastPoint.x >= southEast->getSouthEastPoint().x &&
            offsetSouthEastPoint.y >= southEast->getSouthEastPoint().y) {
            // Get the offsetsVector of the GIS records from the south-east quadrant
            std::vector<int> offsetsFromSouthEast = southEast->getOffsetsOfGISRecords(offsetNorthWestPoint,
                                                                                           offsetSouthEastPoint);
            // Insert the offsetsVector from the south-east quadrant into the vector
            offsets.insert(offsets.end(), offsetsFromSouthEast.begin(), offsetsFromSouthEast.end());
        }
    }

    return offsetsVector;
}

void QuadTreeQuadrant::clearBucket() {
    bucket.clear();
}

// Getters and setters
const Point &QuadTreeQuadrant::getNorthWestPoint() const {
    return northWestPoint;
}

const Point &QuadTreeQuadrant::getSouthEastPoint() const {
    return southEastPoint;
}

void QuadTreeQuadrant::setNorthWestPoint(const Point &northWestPoint) {
    QuadTreeQuadrant::northWestPoint = northWestPoint;
}

void QuadTreeQuadrant::setSouthEastPoint(const Point &southEastPoint) {
    QuadTreeQuadrant::southEastPoint = southEastPoint;
}
