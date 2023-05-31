#ifndef GIS_QUADTREEQUADRANT_H
#define GIS_QUADTREEQUADRANT_H

#include <vector>
#include "Point.h"

class QuadTreeQuadrant {
public:
    QuadTreeQuadrant(Point northWestPoint, Point southEastPoint);
    ~QuadTreeQuadrant();

    void insert(Point point);
    void clearPoints();

    const Point &getNorthWestPoint() const;

    const Point &getSouthEastPoint() const;

    void setNorthWestPoint(const Point &northWestPoint);

    void setSouthEastPoint(const Point &southEastPoint);

    const std::vector<Point> &getPoints() const;

    void setPoints(const std::vector<Point> &points);

    QuadTreeQuadrant *getNorthWest() const;

    void setNorthWest(QuadTreeQuadrant *northWest);

    QuadTreeQuadrant *getNorthEast() const;

    void setNorthEast(QuadTreeQuadrant *northEast);

    QuadTreeQuadrant *getSouthWest() const;

    void setSouthWest(QuadTreeQuadrant *southWest);

    QuadTreeQuadrant *getSouthEast() const;

    void setSouthEast(QuadTreeQuadrant *southEast);

private:
    // Bounding box
    Point northWestPoint;
    Point southEastPoint;

    // Points in quadrant
    std::vector<Point> points;

    // Children quadrants
    QuadTreeQuadrant *northWest;
    QuadTreeQuadrant *northEast;
    QuadTreeQuadrant *southWest;
    QuadTreeQuadrant *southEast;
};

#endif //GIS_QUADTREEQUADRANT_H
