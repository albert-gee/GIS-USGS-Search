#include <stdexcept>
#include "../../include/coordinateIndex/QuadTreeQuadrant.h"

QuadTreeQuadrant::QuadTreeQuadrant(Point northWestPoint, Point southEastPoint) :
        northWestPoint{northWestPoint}, southEastPoint{southEastPoint}, entries{} {
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

void QuadTreeQuadrant::insert(const Entry& newEntry, int bucketCapacity) {

    // Check if the bucket has space
    if (getEntries().size() < bucketCapacity) {
        // Add the point to the bucket if it has space
        entries.push_back(newEntry);

    } else {
        // Otherwise, divide the quadrant into four sub-quadrants:

        // 1. Find the center coordinates of the quadrant
        double centerX = (getNorthWestPoint().x + getSouthEastPoint().x) / 2.0;
        double centerY = (getNorthWestPoint().y + getSouthEastPoint().y) / 2.0;

        // 2. Create the four sub-quadrants
        setNorthWest(new QuadTreeQuadrant(
                getNorthWestPoint(),
                Point(centerX, centerY)));
        setNorthEast(new QuadTreeQuadrant(
                Point(centerX, getNorthWestPoint().y),
                Point(getSouthEastPoint().x, centerY)));
        setSouthWest(new QuadTreeQuadrant(
                Point(getNorthWestPoint().x, centerY),
                Point(centerX, getSouthEastPoint().y)));
        setSouthEast(new QuadTreeQuadrant(
                Point(centerX, centerY),
                getSouthEastPoint()));

        // 3. Insert the points into the sub-quadrants
        entries.push_back(newEntry);
        for (const Entry& entry : getEntries()) {

            // Check if the point is in the north-west quadrant
            if (entry.location.x <= centerX && entry.location.y <= centerY) {
                getNorthWest()->insert(entry, bucketCapacity);
                // Check if the point is in the north-east quadrant
            } else if (entry.location.x > centerX && entry.location.y <= centerY) {
                getNorthEast()->insert(entry, bucketCapacity);
                // Check if the point is in the south-west quadrant
            } else if (entry.location.x <= centerX && entry.location.y > centerY) {
                getSouthWest()->insert(entry, bucketCapacity);
                // Check if the point is in the south-east quadrant
            } else if (entry.location.x > centerX && entry.location.y > centerY) {
                getSouthEast()->insert(entry, bucketCapacity);
            }
        }
        clearPoints();
    }
}

std::vector<int> QuadTreeQuadrant::getOffsetsOfGISRecords(Point northWestPoint, Point southEastPoint) const {

    // Check if the given bounding box is contained in the quadrant
    // If not, throw an exception
    if (northWestPoint.x < getNorthWestPoint().x && southEastPoint.x > getSouthEastPoint().x) {
        throw std::invalid_argument("The given bounding box is not contained in the quadrant.");
    }

    // This vector will contain the offsetsVector of the GIS records and be returned by the method
    std::vector<int> offsetsVector;

    // If the quadrant is a leaf node, return the offsets Vector of the GIS records
    if (getNorthWest() == nullptr) {
        for (const Entry& entry : getEntries()) {

            if (entry.location.x >= northWestPoint.x &&
                entry.location.y >= northWestPoint.y &&
                entry.location.x <= southEastPoint.x &&
                entry.location.y <= southEastPoint.y) {
                // If the point is in the bounding box, add the offsetsVector of the GIS records to the vector
                offsetsVector.insert(offsetsVector.end(), entry.offsetsOfGISRecords.begin(), entry.offsetsOfGISRecords.end());
            }
        }
    } else {
        // Otherwise, recursively get the offsetsVector of the GIS records from the sub-quadrants

        std::vector<int> offsets;
        // Check if the north-west quadrant is contained in the bounding box
        if (northWestPoint.x <= getNorthWest()->getNorthWestPoint().x &&
            northWestPoint.y <= getNorthWest()->getNorthWestPoint().y &&
            southEastPoint.x >= getNorthWest()->getSouthEastPoint().x &&
            southEastPoint.y >= getNorthWest()->getSouthEastPoint().y) {
            // Get the offsetsVector of the GIS records from the north-west quadrant
            std::vector<int> offsetsFromNorthWest = getNorthWest()->getOffsetsOfGISRecords(northWestPoint, southEastPoint);
            // Insert the offsetsVector from the north-west quadrant into the vector
            offsets.insert(offsets.end(), offsetsFromNorthWest.begin(), offsetsFromNorthWest.end());
        }
        // Check if the north-east quadrant is contained in the bounding box
        if (northWestPoint.x <= getNorthEast()->getNorthWestPoint().x &&
            northWestPoint.y <= getNorthEast()->getNorthWestPoint().y &&
            southEastPoint.x >= getNorthEast()->getSouthEastPoint().x &&
            southEastPoint.y >= getNorthEast()->getSouthEastPoint().y) {
            // Get the offsetsVector of the GIS records from the north-east quadrant
            std::vector<int> offsetsFromNorthEast = getNorthEast()->getOffsetsOfGISRecords(northWestPoint, southEastPoint);
            // Insert the offsetsVector from the north-east quadrant into the vector
            offsets.insert(offsets.end(), offsetsFromNorthEast.begin(), offsetsFromNorthEast.end());
        }
        // Check if the south-west quadrant is contained in the bounding box
        if (northWestPoint.x <= getSouthWest()->getNorthWestPoint().x &&
            northWestPoint.y <= getSouthWest()->getNorthWestPoint().y &&
            southEastPoint.x >= getSouthWest()->getSouthEastPoint().x &&
            southEastPoint.y >= getSouthWest()->getSouthEastPoint().y) {
            // Get the offsetsVector of the GIS records from the south-west quadrant
            std::vector<int> offsetsFromSouthWest = getSouthWest()->getOffsetsOfGISRecords(northWestPoint, southEastPoint);
            // Insert the offsetsVector from the south-west quadrant into the vector
            offsets.insert(offsets.end(), offsetsFromSouthWest.begin(), offsetsFromSouthWest.end());
        }
        // Check if the south-east quadrant is contained in the bounding box
        if (northWestPoint.x <= getSouthEast()->getNorthWestPoint().x &&
            northWestPoint.y <= getSouthEast()->getNorthWestPoint().y &&
            southEastPoint.x >= getSouthEast()->getSouthEastPoint().x &&
            southEastPoint.y >= getSouthEast()->getSouthEastPoint().y) {
            // Get the offsetsVector of the GIS records from the south-east quadrant
            std::vector<int> offsetsFromSouthEast = getSouthEast()->getOffsetsOfGISRecords(northWestPoint, southEastPoint);
            // Insert the offsetsVector from the south-east quadrant into the vector
            offsets.insert(offsets.end(), offsetsFromSouthEast.begin(), offsetsFromSouthEast.end());
        }
    }

    return offsetsVector;
}

void QuadTreeQuadrant::clearPoints() {
    entries.clear();
}

// Getters and setters
const Point &QuadTreeQuadrant::getNorthWestPoint() const {
    return northWestPoint;
}

const Point &QuadTreeQuadrant::getSouthEastPoint() const {
    return southEastPoint;
}

const std::vector<Entry> &QuadTreeQuadrant::getEntries() const {
    return entries;
}

// Getters and setters for sub-quadrants
QuadTreeQuadrant *QuadTreeQuadrant::getNorthWest() const {
    return northWest;
}

void QuadTreeQuadrant::setNorthWest(QuadTreeQuadrant *newNorthWest) {
    QuadTreeQuadrant::northWest = newNorthWest;
}

QuadTreeQuadrant *QuadTreeQuadrant::getNorthEast() const {
    return northEast;
}

void QuadTreeQuadrant::setNorthEast(QuadTreeQuadrant *newNorthEast) {
    QuadTreeQuadrant::northEast = newNorthEast;
}

QuadTreeQuadrant *QuadTreeQuadrant::getSouthWest() const {
    return southWest;
}

void QuadTreeQuadrant::setSouthWest(QuadTreeQuadrant *newSouthWest) {
    QuadTreeQuadrant::southWest = newSouthWest;
}

QuadTreeQuadrant *QuadTreeQuadrant::getSouthEast() const {
    return southEast;
}

void QuadTreeQuadrant::setSouthEast(QuadTreeQuadrant *newSouthEast) {
    QuadTreeQuadrant::southEast = newSouthEast;
}
