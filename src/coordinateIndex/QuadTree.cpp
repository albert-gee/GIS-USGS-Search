#include <stdexcept>
#include "../../include/coordinateIndex/QuadTree.h"

QuadTree::QuadTree(Point northWestPoint, Point southEastPoint) {
    this->root = new QuadTreeQuadrant(northWestPoint, southEastPoint);
}

QuadTree::~QuadTree() = default;

void QuadTree::insert(Point point) {

}

void QuadTree::insertIntoQuadrant(QuadTreeQuadrant* quadrant, Point point) {
    // Check if quadrant is null. Throw an exception if it is.
    if (quadrant == nullptr) {
        throw std::runtime_error("QuadTreeQuadrant is null");
    }

    if (quadrant->getPoints().size() < BUCKET_CAPACITY) {
        // Add the point to the quadrant if it has space
        quadrant->insert(point);
    } else {
        // Find the center coordinates of the quadrant
        double centerX = (quadrant->getNorthWestPoint().x + quadrant->getSouthEastPoint().x) / 2.0;
        double centerY = (quadrant->getNorthWestPoint().y + quadrant->getSouthEastPoint().y) / 2.0;

        // Create the four sub-quadrants
        quadrant->setNorthWest(new QuadTreeQuadrant(
                quadrant->getNorthWestPoint(),
                Point(centerX, centerY)));
        quadrant->setNorthEast(new QuadTreeQuadrant(
                Point(centerX, quadrant->getNorthWestPoint().y),
                Point(quadrant->getSouthEastPoint().x, centerY)));
        quadrant->setSouthWest(new QuadTreeQuadrant(
                Point(quadrant->getNorthWestPoint().x, centerY),
                Point(centerX, quadrant->getSouthEastPoint().y)));
        quadrant->setSouthEast(new QuadTreeQuadrant(
                Point(centerX, centerY),
                quadrant->getSouthEastPoint()));

        for (const Point& p : quadrant->getPoints()) {
            // ToDo: Insert points into sub-quadrants
        }

        quadrant->clearPoints();
    }


}