#include "../../include/coordinateIndex/QuadTreeQuadrant.h"

QuadTreeQuadrant::QuadTreeQuadrant(Point northWestPoint, Point southEastPoint) :
        northWestPoint{northWestPoint}, southEastPoint{southEastPoint}, points() {
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

void QuadTreeQuadrant::insert(Point point) {
    points.push_back(point);
}

void QuadTreeQuadrant::clearPoints() {
    points.clear();
}

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

const std::vector<Point> &QuadTreeQuadrant::getPoints() const {
    return points;
}

void QuadTreeQuadrant::setPoints(const std::vector<Point> &points) {
    QuadTreeQuadrant::points = points;
}

QuadTreeQuadrant *QuadTreeQuadrant::getNorthWest() const {
    return northWest;
}

void QuadTreeQuadrant::setNorthWest(QuadTreeQuadrant *northWest) {
    QuadTreeQuadrant::northWest = northWest;
}

QuadTreeQuadrant *QuadTreeQuadrant::getNorthEast() const {
    return northEast;
}

void QuadTreeQuadrant::setNorthEast(QuadTreeQuadrant *northEast) {
    QuadTreeQuadrant::northEast = northEast;
}

QuadTreeQuadrant *QuadTreeQuadrant::getSouthWest() const {
    return southWest;
}

void QuadTreeQuadrant::setSouthWest(QuadTreeQuadrant *southWest) {
    QuadTreeQuadrant::southWest = southWest;
}

QuadTreeQuadrant *QuadTreeQuadrant::getSouthEast() const {
    return southEast;
}

void QuadTreeQuadrant::setSouthEast(QuadTreeQuadrant *southEast) {
    QuadTreeQuadrant::southEast = southEast;
}
