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

void QuadTreeQuadrant::insert(const Entry& entry, int bucketCapacity) {

    // Check if the bucket has space
    if (getEntries().size() < bucketCapacity) {
        // Add the point to the bucket if it has space
        entries.push_back(entry);

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
        entries.push_back(entry);
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
