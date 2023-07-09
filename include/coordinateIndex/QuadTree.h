#ifndef GIS_QUADTREE_H
#define GIS_QUADTREE_H

#include "Point.h"
#include "QuadTreeQuadrant.h"

// This class describes a QuadTree data structure. It is used to indexDatabaseByName the GIS records by their geographic coordinates.
class QuadTree {
public:
    explicit QuadTree(unsigned long bucketCapacity);

    ~QuadTree();

    // Set the boundaries of the QuadTree
    void setBoundingBox(Point northWestPoint, Point southEastPoint);

    // Insert location and offset of a GIS record into the QuadTree
    //
    // The offset is used to retrieve the GIS record from the database
    // The location is expressed as latitude and longitude in decimal degrees provided as strings
    void insert(const std::string& latitude, const std::string& longitude, int offsetOfGISRecord);

    // Get the offsets of the GIS records in the QuadTree that are within the given bounding box
    std::vector<int> getOffsetsOfGISRecords(Point northWestPoint, Point southEastPoint);

    std::vector<int> getOffsetsOfGISRecordsByLocation(Point location);

    // Returns a string representation of the structure of QuadTree
    [[nodiscard]] std::string str() const;

    // Returns a string representation of the content of QuadTree
    [[nodiscard]] std::string getContent() const;

private:
    unsigned long bucketCapacity;
    QuadTreeQuadrant *root;

    Point getNWPoint();

    Point getSEPoint();
};


#endif //GIS_QUADTREE_H
