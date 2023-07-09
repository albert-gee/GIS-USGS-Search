#include <stdexcept>
#include <iostream>
#include <cmath>
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
    return northWest == nullptr ? (bucketCapacity - bucket.size()) : 0;
}

bool QuadTreeQuadrant::isPointWithinQuadrant(Point point) const {
    return point.latitude.toDecimal() <= this->northWestPoint.latitude.toDecimal() &&
           point.latitude.toDecimal() >= this->southEastPoint.latitude.toDecimal() &&
           point.longitude.toDecimal() >= this->northWestPoint.longitude.toDecimal() &&
           point.longitude.toDecimal() <= this->southEastPoint.longitude.toDecimal();
}

std::string QuadTreeQuadrant::str(unsigned int indent) {

    char indentCharacter = '.';

    std::ostringstream os;
    os << "\n";
    os << std::string(indent * 2, indentCharacter) << "NW: (" << northWestPoint.latitude.toDmsString() << ", " << northWestPoint.longitude.toDmsString() << ")" << std::endl;
    os << std::string(indent * 2, indentCharacter) << "SE: (" << southEastPoint.latitude.toDmsString() << ", " << southEastPoint.longitude.toDmsString() << ")" << std::endl;

    // Check if the bucket is not empty
    // If it is not empty, add the offsets of the GIS records from the bucket to output
    if (!bucket.empty()) {
        os << std::string(indent * 2, indentCharacter) << "Bucket (" << bucket.size() << "): ";

        os << "[";
        for (auto & i : bucket) {

            if (!i.offsetsOfGISRecords.empty()) {

                os << "(" << i.location.latitude.toDmsString() << ", " << i.location.longitude.toDmsString() << ")";

                os << ": ";
                for (int offsetOfGISRecord : i.offsetsOfGISRecords) {
                    os << std::to_string(offsetOfGISRecord);
                    os << " ";
                }
            }
        }
        os << "]" << std::endl;
    }

    // Check if the quadrant is divided
    if (northWest != nullptr) {

        // The sub-quadrants
        indent += 3;
        os << northWest->str(indent);
        os << northEast->str(indent);

        os << southWest->str(indent);
        os << southEast->str(indent);

    }

    return os.str();
}

std::string QuadTreeQuadrant::getContent() {
    std::ostringstream os;

    // Check if the bucket is not empty
    // If it is not empty, add the offsets of the GIS records from the bucket to output
    if (!bucket.empty()) {

        for (auto & i : bucket) {

            if (!i.offsetsOfGISRecords.empty()) {

                os << "(" << i.location.latitude.toDmsString() << ", " << i.location.longitude.toDmsString() << ")";

                os << ":  ";
                for (int offsetOfGISRecord : i.offsetsOfGISRecords) {
                    os << std::to_string(offsetOfGISRecord);
                    os << " ";
                }
            }
            os << std::endl;
        }

    }

    // Check if the quadrant is divided
    if (northWest != nullptr) {

        os << northWest->getContent();
        os << northEast->getContent();

        os << southWest->getContent();
        os << southEast->getContent();
    }

    return os.str();
}


void QuadTreeQuadrant::insert(const Point location, int offsetOfGISRecord) {

    // Check if the point is within the quadrant
    if (!isPointWithinQuadrant(location)) {
        throw std::invalid_argument("The point is not within the quadrant.");
    }

    // Check if the bucket is not full
    if (getBucketAvailableCapacity() > 0) {

        // Insert the offset and its location into the bucket
        insertIntoBucket(location, offsetOfGISRecord);
    } else {
        // If the bucket is full, insert the entry into the sub-quadrants

        // But first, check if the quadrant is not divided into sub-quadrants
        if (northWest == nullptr) {

            // If so, divide the quadrant into four sub-quadrants
            divideQuadrantIntoSubQuadrants();

            // Then, move the entries from the bucket into the sub-quadrants
            moveEntriesFromBucketIntoSubQuadrants();

            // Clear the points from the bucket
            clearBucket();
        }

        // And then, insert the new entry into the sub-quadrants
        try {
            insertIntoSubQuadrants(location, offsetOfGISRecord);
        } catch (const std::invalid_argument &e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
}

void QuadTreeQuadrant::divideQuadrantIntoSubQuadrants() {
    // 1. Find the center coordinates of the quadrant
    DMS latitudeCenter = getNorthWestPoint().latitude.findCenterBetweenThisAndAnotherDMS(getSouthEastPoint().latitude);
    DMS longitudeCenter = getNorthWestPoint().longitude.findCenterBetweenThisAndAnotherDMS(
            getSouthEastPoint().longitude);

    // 2. Create the four sub-quadrants
    northWest = new QuadTreeQuadrant(
            getNorthWestPoint(),
            Point(latitudeCenter, longitudeCenter),
            bucketCapacity);
    northEast = new QuadTreeQuadrant(
            Point(getNorthWestPoint().latitude, longitudeCenter),
            Point(latitudeCenter, getSouthEastPoint().longitude),
            bucketCapacity);
    southWest = new QuadTreeQuadrant(
            Point(latitudeCenter, getNorthWestPoint().longitude),
            Point(getSouthEastPoint().latitude, longitudeCenter),
            bucketCapacity);
    southEast = new QuadTreeQuadrant(
            Point(latitudeCenter, longitudeCenter),
            getSouthEastPoint(),
            bucketCapacity);
}

void QuadTreeQuadrant::moveEntriesFromBucketIntoSubQuadrants() {
    for (const auto &entry: bucket) {

        // Insert all offsets of the entry into the sub-quadrants
        for (const auto &bucketOffsetOfGISRecord: entry.offsetsOfGISRecords) {

            try {
                insertIntoSubQuadrants(entry.location, bucketOffsetOfGISRecord);
            } catch (const std::invalid_argument &e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
        }
    }
}

void QuadTreeQuadrant::insertIntoBucket(const Point location, const int offsetOfGISRecord) {
    // Check if an entry with the same location already exists in the bucket
    bool exists = false;
    for (Entry &entry: bucket) {

        // If so, add the offset of the GIS record to the entry and return
        if (entry.location.latitude.equals(location.latitude) && entry.location.longitude.equals(location.longitude)) {
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
    if (northWest->isPointWithinQuadrant(location)) {
        northWest->insert(location, offsetOfGISRecord);
    } else if (northEast->isPointWithinQuadrant(location)) {
        northEast->insert(location, offsetOfGISRecord);
    } else if (southWest->isPointWithinQuadrant(location)) {
        southWest->insert(location, offsetOfGISRecord);
    } else if (southEast->isPointWithinQuadrant(location)) {
        southEast->insert(location, offsetOfGISRecord);
    } else {
        throw std::invalid_argument("The point is not within any of the sub-quadrants.");
    }
}

std::vector<int>
QuadTreeQuadrant::getOffsetsOfGISRecords(Point offsetNorthWestPoint, Point offsetSouthEastPoint) const {

    // Check if the given bounding box is contained in the quadrant
    // If not, throw an exception
    if (offsetNorthWestPoint.latitude.toDecimal() < getNorthWestPoint().latitude.toDecimal() &&
        offsetSouthEastPoint.latitude.toDecimal() > getSouthEastPoint().latitude.toDecimal()) {
        throw std::invalid_argument("The given bounding box is not contained in the quadrant.");
    }

    // This vector will contain the offsetsVector of the GIS records and be returned by the method
    std::vector<int> offsetsVector;

    // If the quadrant is a leaf node, return the offsets Vector of the GIS records
    if (northWest == nullptr) {
        for (const Entry &entry: bucket) {

            if (entry.location.latitude.toDecimal() >= offsetNorthWestPoint.latitude.toDecimal() &&
                entry.location.longitude.toDecimal() >= offsetNorthWestPoint.longitude.toDecimal() &&
                entry.location.latitude.toDecimal() <= offsetSouthEastPoint.latitude.toDecimal() &&
                entry.location.longitude.toDecimal() <= offsetSouthEastPoint.longitude.toDecimal()) {
                // If the point is in the bounding box, add the offsetsVector of the GIS records to the vector
                offsetsVector.insert(offsetsVector.end(), entry.offsetsOfGISRecords.begin(),
                                     entry.offsetsOfGISRecords.end());
            }
        }
    } else {
        // Otherwise, recursively get the offsetsVector of the GIS records from the sub-quadrants

        std::vector<int> offsets;
        // Check if the north-west quadrant is contained in the bounding box
        if (offsetNorthWestPoint.latitude.toDecimal() <= northWest->getNorthWestPoint().latitude.toDecimal() &&
            offsetNorthWestPoint.longitude.toDecimal() <= northWest->getNorthWestPoint().longitude.toDecimal() &&
            offsetSouthEastPoint.latitude.toDecimal() >= northWest->getSouthEastPoint().latitude.toDecimal() &&
            offsetSouthEastPoint.longitude.toDecimal() >= northWest->getSouthEastPoint().longitude.toDecimal()) {
            // Get the offsetsVector of the GIS records from the north-west quadrant
            std::vector<int> offsetsFromNorthWest = northWest->getOffsetsOfGISRecords(offsetNorthWestPoint,
                                                                                      offsetSouthEastPoint);
            // Insert the offsetsVector from the north-west quadrant into the vector
            offsets.insert(offsets.end(), offsetsFromNorthWest.begin(), offsetsFromNorthWest.end());
        }
        // Check if the north-east quadrant is contained in the bounding box
        if (offsetNorthWestPoint.latitude.toDecimal() <= northEast->getNorthWestPoint().latitude.toDecimal() &&
            offsetNorthWestPoint.longitude.toDecimal() <= northEast->getNorthWestPoint().longitude.toDecimal() &&
            offsetSouthEastPoint.latitude.toDecimal() >= northEast->getSouthEastPoint().latitude.toDecimal() &&
            offsetSouthEastPoint.longitude.toDecimal() >= northEast->getSouthEastPoint().longitude.toDecimal()) {
            // Get the offsetsVector of the GIS records from the north-east quadrant
            std::vector<int> offsetsFromNorthEast = northEast->getOffsetsOfGISRecords(offsetNorthWestPoint,
                                                                                      offsetSouthEastPoint);
            // Insert the offsetsVector from the north-east quadrant into the vector
            offsets.insert(offsets.end(), offsetsFromNorthEast.begin(), offsetsFromNorthEast.end());
        }
        // Check if the south-west quadrant is contained in the bounding box
        if (offsetNorthWestPoint.latitude.toDecimal() <= southWest->getNorthWestPoint().latitude.toDecimal() &&
            offsetNorthWestPoint.longitude.toDecimal() <= southWest->getNorthWestPoint().longitude.toDecimal() &&
            offsetSouthEastPoint.latitude.toDecimal() >= southWest->getSouthEastPoint().latitude.toDecimal() &&
            offsetSouthEastPoint.longitude.toDecimal() >= southWest->getSouthEastPoint().longitude.toDecimal()) {
            // Get the offsetsVector of the GIS records from the south-west quadrant
            std::vector<int> offsetsFromSouthWest = southWest->getOffsetsOfGISRecords(offsetNorthWestPoint,
                                                                                      offsetSouthEastPoint);
            // Insert the offsetsVector from the south-west quadrant into the vector
            offsets.insert(offsets.end(), offsetsFromSouthWest.begin(), offsetsFromSouthWest.end());
        }
        // Check if the south-east quadrant is contained in the bounding box
        if (offsetNorthWestPoint.latitude.toDecimal() <= southEast->getNorthWestPoint().latitude.toDecimal() &&
            offsetNorthWestPoint.longitude.toDecimal() <= southEast->getNorthWestPoint().longitude.toDecimal() &&
            offsetSouthEastPoint.latitude.toDecimal() >= southEast->getSouthEastPoint().latitude.toDecimal() &&
            offsetSouthEastPoint.longitude.toDecimal() >= southEast->getSouthEastPoint().longitude.toDecimal()) {
            // Get the offsetsVector of the GIS records from the south-east quadrant
            std::vector<int> offsetsFromSouthEast = southEast->getOffsetsOfGISRecords(offsetNorthWestPoint,
                                                                                      offsetSouthEastPoint);
            // Insert the offsetsVector from the south-east quadrant into the vector
            offsets.insert(offsets.end(), offsetsFromSouthEast.begin(), offsetsFromSouthEast.end());
        }
    }

    return offsetsVector;
}

std::vector<int> QuadTreeQuadrant::getOffsetsOfGISRecordsByLocation(Point location) const {

    // Check if the given location is contained in the quadrant
    // If not, throw an exception
    if (!(location.latitude.toDecimal() < getNorthWestPoint().latitude.toDecimal() && location.longitude.toDecimal() < getSouthEastPoint().longitude.toDecimal())) {
        throw std::invalid_argument("The given bounding box is not contained in the quadrant.");
    }

    // This vector will contain the offsetsVector of the GIS records and be returned by the method
    std::vector<int> offsetsVector;

    // If the quadrant is a leaf node, try to find an entry with the same location in the bucket
    if (northWest == nullptr) {

        for (const Entry &entry: bucket) {

            double epsilon = 0.01;  // Define the precision (two digits after the decimal point)
            double latDiff = std::fabs(entry.location.latitude.toDecimal() - location.latitude.toDecimal());  // Calculate the absolute difference between the numbers
            double lngDiff = std::fabs(entry.location.longitude.toDecimal() - location.longitude.toDecimal());  // Calculate the absolute difference between the numbers

            // Check if the absolute difference is within the desired precision
            if (latDiff < epsilon && lngDiff < epsilon) {
                std::cout << "2FINDING BY COORDINATES: (" << location.latitude.toDecimalString() << ", " << location.longitude.toDecimalString() << ")" << std::endl;

                offsetsVector = entry.offsetsOfGISRecords;
            }

//            if (entry.location.latitude == location.latitude &&
//                entry.location.longitude == location.longitude) {
//
//                offsetsVector = entry.offsetsOfGISRecords;
//            }
        }
    } else {
        // Otherwise, recursively try to find it in the sub-quadrants

        // Check if the location is within the north-west quadrant
        if (location.latitude.toDecimal() <= northWest->getNorthWestPoint().latitude.toDecimal() &&
            location.longitude.toDecimal() <= northWest->getSouthEastPoint().longitude.toDecimal()) {

            offsetsVector = northWest->getOffsetsOfGISRecordsByLocation(location);
        }
        // Check if the location is within the north-east quadrant
        if (location.latitude.toDecimal() <= northEast->getNorthWestPoint().latitude.toDecimal() &&
            location.longitude.toDecimal() <= northEast->getSouthEastPoint().longitude.toDecimal()) {

            offsetsVector = northEast->getOffsetsOfGISRecordsByLocation(location);
        }
        // Check if the location is within the south-east quadrant
        if (location.latitude.toDecimal() <= southEast->getNorthWestPoint().latitude.toDecimal() &&
            location.longitude.toDecimal() <= southEast->getSouthEastPoint().longitude.toDecimal()) {

            offsetsVector = southEast->getOffsetsOfGISRecordsByLocation(location);
        }
        // Check if the location is within the south-west quadrant
        if (location.latitude.toDecimal() <= southWest->getNorthWestPoint().latitude.toDecimal() &&
            location.longitude.toDecimal() <= southWest->getSouthEastPoint().longitude.toDecimal()) {

            offsetsVector = southWest->getOffsetsOfGISRecordsByLocation(location);
        }
    }

    return offsetsVector;
}

void QuadTreeQuadrant::clearBucket() {
    bucket.clear();
}

// Getters
const Point &QuadTreeQuadrant::getNorthWestPoint() const {
    return northWestPoint;
}

const Point &QuadTreeQuadrant::getSouthEastPoint() const {
    return southEastPoint;
}
