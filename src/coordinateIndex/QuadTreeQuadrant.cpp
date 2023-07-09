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
    os << std::string(indent * 2, indentCharacter) << "NW: (" << northWestPoint.latitude.toDmsString() << ", "
       << northWestPoint.longitude.toDmsString() << ")" << std::endl;
    os << std::string(indent * 2, indentCharacter) << "SE: (" << southEastPoint.latitude.toDmsString() << ", "
       << southEastPoint.longitude.toDmsString() << ")" << std::endl;

    // Check if the bucket is not empty
    // If it is not empty, add the offsets of the GIS records from the bucket to output
    if (!bucket.empty()) {
        os << std::string(indent * 2, indentCharacter) << "Bucket (" << bucket.size() << "): ";

        os << "[";
        for (auto &i: bucket) {

            if (!i.offsetsOfGISRecords.empty()) {

                os << "(" << i.location.latitude.toDmsString() << ", " << i.location.longitude.toDmsString() << ")";

                os << ": ";
                for (int offsetOfGISRecord: i.offsetsOfGISRecords) {
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

        for (auto &i: bucket) {

            if (!i.offsetsOfGISRecords.empty()) {

                os << "(" << i.location.latitude.toDmsString() << ", " << i.location.longitude.toDmsString() << ")";

                os << ":  ";
                for (int offsetOfGISRecord: i.offsetsOfGISRecords) {
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
QuadTreeQuadrant::getOffsetsOfGISRecordsWithin(Point offsetNorthWestPoint, Point offsetSouthEastPoint) const {

    // This vector will contain the offsetsVector of the GIS records and be returned by the method
    std::vector<int> offsetsVector;
    // Check if the boundaries cover the quadrant
    if (offsetNorthWestPoint.latitude.toDecimal() >= southEastPoint.latitude.toDecimal() &&
        offsetNorthWestPoint.longitude.toDecimal() <= southEastPoint.longitude.toDecimal() &&
        offsetSouthEastPoint.latitude.toDecimal() <= northWestPoint.latitude.toDecimal() &&
        offsetSouthEastPoint.longitude.toDecimal() >= northWestPoint.longitude.toDecimal()) {

        // If the quadrant is a leaf node, return the offsets that satisfy the bounding box
        if (northWest == nullptr) {
            for (const Entry &entry: bucket) {

                // Check if the point is within the bounding box
                if (entry.location.latitude.toDecimal() <= offsetNorthWestPoint.latitude.toDecimal() &&
                    entry.location.longitude.toDecimal() >= offsetNorthWestPoint.longitude.toDecimal() &&
                    entry.location.latitude.toDecimal() >= offsetSouthEastPoint.latitude.toDecimal() &&
                    entry.location.longitude.toDecimal() <= offsetSouthEastPoint.longitude.toDecimal()) {

                    if (!entry.offsetsOfGISRecords.empty()) {
                        for (auto offset: entry.offsetsOfGISRecords) {
                            offsetsVector.push_back(offset);
                        }
                    }
                    return offsetsVector;
                }
            }
        } else {
            // It turns out the quadrant is not a leaf node and has sub-quadrants
            // Try to find the intersection of the bounding box and the sub-quadrant and recursively call the method

            // This vector will contain the offsetsVector of the GIS records and be returned by the method
            std::vector<int> newOffsetsVector;


            // NW sub-quadrant
            // We create a new bounding box for the NW sub-quadrant
            DMS newNwLat = (offsetNorthWestPoint.latitude.toDecimal() >= northWestPoint.latitude.toDecimal())
                           ? northWestPoint.latitude : offsetNorthWestPoint.latitude;
            DMS newNwLon = (offsetNorthWestPoint.longitude.toDecimal() <= northWestPoint.longitude.toDecimal())
                           ? northWestPoint.longitude : offsetNorthWestPoint.longitude;
            Point newNwPoint = {newNwLat, newNwLon};

            DMS newSeLat = (offsetSouthEastPoint.latitude.toDecimal() <= southEastPoint.latitude.toDecimal())
                           ? southEastPoint.latitude : offsetSouthEastPoint.latitude;
            DMS newSeLon = (offsetSouthEastPoint.longitude.toDecimal() >= southEastPoint.longitude.toDecimal())
                           ? southEastPoint.longitude : offsetSouthEastPoint.longitude;
            Point newSePoint = {newSeLat, newSeLon};

            // Try to find the offsets within the new bounding box
            newOffsetsVector = northWest->getOffsetsOfGISRecordsWithin(newNwPoint, newSePoint);
            for (int element: newOffsetsVector) {
                offsetsVector.push_back(element);
            }

            // NE sub-quadrant
            newNwLat = (offsetNorthWestPoint.latitude.toDecimal() >= northWestPoint.latitude.toDecimal())
                       ? northWestPoint.latitude : offsetNorthWestPoint.latitude;
            newNwLon = (offsetNorthWestPoint.longitude.toDecimal() <= northWestPoint.longitude.toDecimal())
                       ? northWestPoint.longitude : offsetNorthWestPoint.longitude;
            newNwPoint = {newNwLat, newNwLon};

            newSeLat = (offsetSouthEastPoint.latitude.toDecimal() <= southEastPoint.latitude.toDecimal())
                       ? southEastPoint.latitude : offsetSouthEastPoint.latitude;
            newSeLon = (offsetSouthEastPoint.longitude.toDecimal() >= southEastPoint.longitude.toDecimal())
                       ? southEastPoint.longitude : offsetSouthEastPoint.longitude;
            newSePoint = {newSeLat, newSeLon};

            // Try to find the offsets within the new bounding box
            newOffsetsVector = northEast->getOffsetsOfGISRecordsWithin(newNwPoint, newSePoint);
            for (int element: newOffsetsVector) {
                offsetsVector.push_back(element);
            }

            // SW sub-quadrant
            newNwLat = (offsetNorthWestPoint.latitude.toDecimal() >= northWestPoint.latitude.toDecimal())
                       ? northWestPoint.latitude : offsetNorthWestPoint.latitude;
            newNwLon = (offsetNorthWestPoint.longitude.toDecimal() <= northWestPoint.longitude.toDecimal())
                       ? northWestPoint.longitude : offsetNorthWestPoint.longitude;
            newNwPoint = {newNwLat, newNwLon};

            newSeLat = (offsetSouthEastPoint.latitude.toDecimal() <= southEastPoint.latitude.toDecimal())
                       ? southEastPoint.latitude : offsetSouthEastPoint.latitude;
            newSeLon = (offsetSouthEastPoint.longitude.toDecimal() >= southEastPoint.longitude.toDecimal())
                       ? southEastPoint.longitude : offsetSouthEastPoint.longitude;
            newSePoint = {newSeLat, newSeLon};

            // Try to find the offsets within the new bounding box
            newOffsetsVector = southWest->getOffsetsOfGISRecordsWithin(newNwPoint, newSePoint);
            for (int element: newOffsetsVector) {
                offsetsVector.push_back(element);
            }

            // SE sub-quadrant
            newNwLat = (offsetNorthWestPoint.latitude.toDecimal() >= northWestPoint.latitude.toDecimal())
                       ? northWestPoint.latitude : offsetNorthWestPoint.latitude;
            newNwLon = (offsetNorthWestPoint.longitude.toDecimal() <= northWestPoint.longitude.toDecimal())
                       ? northWestPoint.longitude : offsetNorthWestPoint.longitude;
            newNwPoint = {newNwLat, newNwLon};

            newSeLat = (offsetSouthEastPoint.latitude.toDecimal() <= southEastPoint.latitude.toDecimal())
                       ? southEastPoint.latitude : offsetSouthEastPoint.latitude;
            newSeLon = (offsetSouthEastPoint.longitude.toDecimal() >= southEastPoint.longitude.toDecimal())
                       ? southEastPoint.longitude : offsetSouthEastPoint.longitude;
            newSePoint = {newSeLat, newSeLon};

            // Try to find the offsets within the new bounding box
            newOffsetsVector = southEast->getOffsetsOfGISRecordsWithin(newNwPoint, newSePoint);
            for (int element: newOffsetsVector) {
                offsetsVector.push_back(element);
            }

//                // Try to find the offsets within the new bounding box
//                newOffsetsVector = northWest->getOffsetsOfGISRecordsWithin(newNwPoint, newSePoint);
//                for (int element: newOffsetsVector) {
//                    offsetsVector.push_back(element);
//                }
//
//
//                if (southWest->isPointWithinQuadrant(offsetNorthWestPoint) ||
//                    southWest->isPointWithinQuadrant(offsetSouthEastPoint)) {
//                    // We create a new bounding box for the NW sub-quadrant
//                    DMS newNwLat = (offsetNorthWestPoint.latitude.toDecimal() >= northWestPoint.latitude.toDecimal()) ? northWestPoint.latitude : offsetNorthWestPoint.latitude;
//                    DMS newNwLon = (offsetNorthWestPoint.longitude.toDecimal() <= northWestPoint.longitude.toDecimal()) ? northWestPoint.longitude : offsetNorthWestPoint.longitude;
//                    Point newNwPoint = {newNwLat, newNwLon};
//
//                    DMS newSeLat = (offsetSouthEastPoint.latitude.toDecimal() <= southEastPoint.latitude.toDecimal()) ? southEastPoint.latitude : offsetSouthEastPoint.latitude;
//                    DMS newSeLon = (offsetSouthEastPoint.longitude.toDecimal() >= southEastPoint.longitude.toDecimal()) ? southEastPoint.longitude : offsetSouthEastPoint.longitude;
//                    Point newSePoint = {newSeLat, newSeLon};
//
//                    // Try to find the offsets within the new bounding box
//                    newOffsetsVector = northWest->getOffsetsOfGISRecordsWithin(newNwPoint, newSePoint);
//                    for (int element: newOffsetsVector) {
//                        offsetsVector.push_back(element);
//                    }
//                }
//
//                if (southEast->isPointWithinQuadrant(offsetNorthWestPoint) ||
//                    southEast->isPointWithinQuadrant(offsetSouthEastPoint)) {
//                    // We create a new bounding box for the NW sub-quadrant
//                    DMS newNwLat = (offsetNorthWestPoint.latitude.toDecimal() >= northWestPoint.latitude.toDecimal()) ? northWestPoint.latitude : offsetNorthWestPoint.latitude;
//                    DMS newNwLon = (offsetNorthWestPoint.longitude.toDecimal() <= northWestPoint.longitude.toDecimal()) ? northWestPoint.longitude : offsetNorthWestPoint.longitude;
//                    Point newNwPoint = {newNwLat, newNwLon};
//
//                    DMS newSeLat = (offsetSouthEastPoint.latitude.toDecimal() <= southEastPoint.latitude.toDecimal()) ? southEastPoint.latitude : offsetSouthEastPoint.latitude;
//                    DMS newSeLon = (offsetSouthEastPoint.longitude.toDecimal() >= southEastPoint.longitude.toDecimal()) ? southEastPoint.longitude : offsetSouthEastPoint.longitude;
//                    Point newSePoint = {newSeLat, newSeLon};
//
//                    // Try to find the offsets within the new bounding box
//                    newOffsetsVector = northWest->getOffsetsOfGISRecordsWithin(newNwPoint, newSePoint);
//                    for (int element: newOffsetsVector) {
//                        offsetsVector.push_back(element);
//                    }
//                }


        }

    }
    return offsetsVector;

}

std::vector<int> QuadTreeQuadrant::getOffsetsOfGISRecordsByLocation(Point location) const {

    // Check if the given location is contained in the quadrant
    // If not, throw an exception
    if (!(location.latitude.toDecimal() < getNorthWestPoint().latitude.toDecimal() &&
          location.longitude.toDecimal() < getSouthEastPoint().longitude.toDecimal())) {
        throw std::invalid_argument("The given bounding box is not contained in the quadrant.");
    }

    // This vector will contain the offsetsVector of the GIS records and be returned by the method
    std::vector<int> offsetsVector;

    // If the quadrant is a leaf node, try to find an entry with the same location in the bucket
    if (northWest == nullptr) {

        for (const Entry &entry: bucket) {

            if (entry.location.latitude.equals(location.latitude) &&
                entry.location.longitude.equals(location.longitude)) {
                if (!entry.offsetsOfGISRecords.empty()) {
                    for (auto offset: entry.offsetsOfGISRecords) {
                        offsetsVector.push_back(offset);
                    }
                }
                return offsetsVector;
            }
        }

    } else {
        // Otherwise, recursively try to find it in the sub-quadrants
        if (northWest->isPointWithinQuadrant(location)) {
            offsetsVector = northWest->getOffsetsOfGISRecordsByLocation(location);
        } else if (northEast->isPointWithinQuadrant(location)) {
            offsetsVector = northEast->getOffsetsOfGISRecordsByLocation(location);
        } else if (southWest->isPointWithinQuadrant(location)) {
            offsetsVector = southWest->getOffsetsOfGISRecordsByLocation(location);
        } else if (southEast->isPointWithinQuadrant(location)) {
            offsetsVector = southEast->getOffsetsOfGISRecordsByLocation(location);
        } else {
            throw std::invalid_argument("The point is not within any of the sub-quadrants.");
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
