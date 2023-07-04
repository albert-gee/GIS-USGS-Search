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

bool QuadTreeQuadrant::isPointWithinQuadrant(Point point) const {
    return point.latitude <= this->northWestPoint.latitude &&
           point.latitude >= this->southEastPoint.latitude &&
           point.longitude >= this->northWestPoint.longitude &&
           point.longitude <= this->southEastPoint.longitude;
}

void QuadTreeQuadrant::print() const {
    if (northWest != nullptr) {
        northWest->print();
    }
    if (northEast != nullptr) {
        northEast->print();
    }

    // Print the information about the quadrant
    std::cout << "\nQuadTreeQuadrant - ";
    std::cout << "northWestPoint: ";
    northWestPoint.print();
    std::cout << ", southEastPoint: ";
    southEastPoint.print();
    std::cout << std::endl;
    std::cout << (northWest == nullptr ? " not divided" : " divided") << std::endl;

    std::cout << " bucket size: " << bucket.size() << std::endl;
    for (const auto &entry: bucket) {
        entry.print();
    }

    if (southWest != nullptr) {
        southWest->print();
    }
    if (southEast != nullptr) {
        southEast->print();
    }
}

// Insert an entry into the quadrant. The entry is inserted into the bucket. If the bucket is full, the quadrant is
// divided into four sub-quadrants and the entry and all entries from the bucket are inserted into them.
void QuadTreeQuadrant::insert(const Point location, int offsetOfGISRecord) {
    std::cout << "\nINSERT" << std::endl;

    // Check if the point is within the quadrant
    if (!isPointWithinQuadrant(location)) {
        throw std::invalid_argument("The point is not within the quadrant.");
    }

    // Check if the bucket is not full
    std::cout << "bucket available capacity: " << getBucketAvailableCapacity() << std::endl;
    if (getBucketAvailableCapacity() > 0 && northWest == nullptr) {
        std::cout << "Inserting into bucket: " << offsetOfGISRecord << std::endl;

        // Insert the offset and its location into the bucket
        insertIntoBucket(location, offsetOfGISRecord);
    } else {

        // Otherwise, check if the quadrant is not divided into sub-quadrants
        if (northWest == nullptr) {
            // If so, divide the quadrant into four sub-quadrants
            std::cout << "divideQuadrantIntoSubQuadrants " << std::endl;
            divideQuadrantIntoSubQuadrants();

            // Then, move the entries from the bucket into the sub-quadrants
            std::cout << " > move entries from bucket to subquadrants " << std::endl;
            std::cout << " bucket size: " << bucket.size() << std::endl;
            for (const auto &entry: bucket) {
                entry.location.print();

                // Insert all offsets of the entry into the sub-quadrants
                for (const auto &bucketOffsetOfGISRecord: entry.offsetsOfGISRecords) {
                    std::cout << " " << bucketOffsetOfGISRecord << std::endl;

                    try {
                        insertIntoSubQuadrants(entry.location, bucketOffsetOfGISRecord);
                    } catch (const std::invalid_argument &e) {
                        std::cout << "Error: " << e.what() << std::endl;
                    }
                }
            }
            std::cout << " bucket size: " << bucket.size() << std::endl;

            // Clear the points from the bucket
            clearBucket();
        } else {
            // Otherwise, insert the entry into the sub-quadrants since the bucket is full
            try {
                insertIntoSubQuadrants(location, offsetOfGISRecord);
            } catch (const std::invalid_argument &e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
        }
    }
}

void QuadTreeQuadrant::divideQuadrantIntoSubQuadrants() {
    // 1. Find the center coordinates of the quadrant
    double centerX = (getNorthWestPoint().latitude + getSouthEastPoint().latitude) / 2.0;
    double centerY = (getNorthWestPoint().longitude + getSouthEastPoint().longitude) / 2.0;

    // 2. Create the four sub-quadrants
    northWest = new QuadTreeQuadrant(
            getNorthWestPoint(),
            Point(centerX, centerY),
            bucketCapacity);
    northEast = new QuadTreeQuadrant(
            Point(centerX, getNorthWestPoint().longitude),
            Point(getSouthEastPoint().latitude, centerY),
            bucketCapacity);
    southWest = new QuadTreeQuadrant(
            Point(getNorthWestPoint().latitude, centerY),
            Point(centerX, getSouthEastPoint().longitude),
            bucketCapacity);
    southEast = new QuadTreeQuadrant(
            Point(centerX, centerY),
            getSouthEastPoint(),
            bucketCapacity);

}

void QuadTreeQuadrant::insertIntoBucket(const Point location, const int offsetOfGISRecord) {
    // Check if an entry with the same location already exists in the bucket
    bool exists = false;
    for (Entry &entry: bucket) {

        // If so, add the offset of the GIS record to the entry and return
        if (entry.location.latitude == location.latitude && entry.location.longitude == location.longitude) {
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
    std::cout << "Insert into sub quadrants: " << offsetOfGISRecord << std::endl;

    if (northWest->isPointWithinQuadrant(location)) {
        std::cout << " > inserted in northwest" << std::endl;
        northWest->insert(location, offsetOfGISRecord);
    } else if (northEast->isPointWithinQuadrant(location)) {
        std::cout << " > inserted in northeast" << std::endl;
        northEast->insert(location, offsetOfGISRecord);
    } else if (southWest->isPointWithinQuadrant(location)) {
        std::cout << " > inserted in southwest" << std::endl;
        southWest->insert(location, offsetOfGISRecord);
    } else if (southEast->isPointWithinQuadrant(location)) {
        std::cout << " > inserted in southeast" << std::endl;
        southEast->insert(location, offsetOfGISRecord);
    } else {
        throw std::invalid_argument("The point is not within any of the sub-quadrants.");
    }
}

std::vector<int> QuadTreeQuadrant::getOffsetsOfGISRecords(Point offsetNorthWestPoint, Point offsetSouthEastPoint) const {

    // Check if the given bounding box is contained in the quadrant
    // If not, throw an exception
    if (offsetNorthWestPoint.latitude < getNorthWestPoint().latitude && offsetSouthEastPoint.latitude > getSouthEastPoint().latitude) {
        throw std::invalid_argument("The given bounding box is not contained in the quadrant.");
    }

    // This vector will contain the offsetsVector of the GIS records and be returned by the method
    std::vector<int> offsetsVector;

    // If the quadrant is a leaf node, return the offsets Vector of the GIS records
    if (northWest == nullptr) {
        for (const Entry &entry: bucket) {

            if (entry.location.latitude >= offsetNorthWestPoint.latitude &&
                entry.location.longitude >= offsetNorthWestPoint.longitude &&
                entry.location.latitude <= offsetSouthEastPoint.latitude &&
                entry.location.longitude <= offsetSouthEastPoint.longitude) {
                // If the point is in the bounding box, add the offsetsVector of the GIS records to the vector
                offsetsVector.insert(offsetsVector.end(), entry.offsetsOfGISRecords.begin(),
                                     entry.offsetsOfGISRecords.end());
            }
        }
    } else {
        // Otherwise, recursively get the offsetsVector of the GIS records from the sub-quadrants

        std::vector<int> offsets;
        // Check if the north-west quadrant is contained in the bounding box
        if (offsetNorthWestPoint.latitude <= northWest->getNorthWestPoint().latitude &&
            offsetNorthWestPoint.longitude <= northWest->getNorthWestPoint().longitude &&
            offsetSouthEastPoint.latitude >= northWest->getSouthEastPoint().latitude &&
            offsetSouthEastPoint.longitude >= northWest->getSouthEastPoint().longitude) {
            // Get the offsetsVector of the GIS records from the north-west quadrant
            std::vector<int> offsetsFromNorthWest = northWest->getOffsetsOfGISRecords(offsetNorthWestPoint,
                                                                                      offsetSouthEastPoint);
            // Insert the offsetsVector from the north-west quadrant into the vector
            offsets.insert(offsets.end(), offsetsFromNorthWest.begin(), offsetsFromNorthWest.end());
        }
        // Check if the north-east quadrant is contained in the bounding box
        if (offsetNorthWestPoint.latitude <= northEast->getNorthWestPoint().latitude &&
            offsetNorthWestPoint.longitude <= northEast->getNorthWestPoint().longitude &&
            offsetSouthEastPoint.latitude >= northEast->getSouthEastPoint().latitude &&
            offsetSouthEastPoint.longitude >= northEast->getSouthEastPoint().longitude) {
            // Get the offsetsVector of the GIS records from the north-east quadrant
            std::vector<int> offsetsFromNorthEast = northEast->getOffsetsOfGISRecords(offsetNorthWestPoint,
                                                                                           offsetSouthEastPoint);
            // Insert the offsetsVector from the north-east quadrant into the vector
            offsets.insert(offsets.end(), offsetsFromNorthEast.begin(), offsetsFromNorthEast.end());
        }
        // Check if the south-west quadrant is contained in the bounding box
        if (offsetNorthWestPoint.latitude <= southWest->getNorthWestPoint().latitude &&
            offsetNorthWestPoint.longitude <= southWest->getNorthWestPoint().longitude &&
            offsetSouthEastPoint.latitude >= southWest->getSouthEastPoint().latitude &&
            offsetSouthEastPoint.longitude >= southWest->getSouthEastPoint().longitude) {
            // Get the offsetsVector of the GIS records from the south-west quadrant
            std::vector<int> offsetsFromSouthWest = southWest->getOffsetsOfGISRecords(offsetNorthWestPoint,
                                                                                           offsetSouthEastPoint);
            // Insert the offsetsVector from the south-west quadrant into the vector
            offsets.insert(offsets.end(), offsetsFromSouthWest.begin(), offsetsFromSouthWest.end());
        }
        // Check if the south-east quadrant is contained in the bounding box
        if (offsetNorthWestPoint.latitude <= southEast->getNorthWestPoint().latitude &&
            offsetNorthWestPoint.longitude <= southEast->getNorthWestPoint().longitude &&
            offsetSouthEastPoint.latitude >= southEast->getSouthEastPoint().latitude &&
            offsetSouthEastPoint.longitude >= southEast->getSouthEastPoint().longitude) {
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
