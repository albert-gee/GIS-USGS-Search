#ifndef GIS_POINT_H
#define GIS_POINT_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>

// This struct describes a geographic coordinate.
struct Point {
    double latitude;
    double longitude;

    Point(const std::string& lat, const std::string& lng){
        latitude = stod(lat);
        longitude = stod(lng);
    }

    Point(double lat, double lng) : latitude{lat}, longitude{lng} {}

    // Print the point
    void print() const {
        std::cout << std::fixed << std::setprecision(7);
        std::cout << "(" << latitude << ", " << longitude << ")";
    }

    std::string getLongToDMSStr() const{
        auto dmsParams = getDMSParams(longitude);
        std::string direction = longitude >= 0? "East" : "West";
        std::ostringstream os;
        os << dmsParams[0] <<"d " << dmsParams[1] << "m " << dmsParams[2] << "s " << direction;
        return os.str();
    }


    std::string getLatToDMSStr() const{
        auto dmsParams = getDMSParams(latitude);
        std::string direction = latitude >= 0? "North" : "South";
        std::ostringstream os;
        os << dmsParams[0] <<"d " << dmsParams[1] << "m " << dmsParams[2] << "s " << direction;
        return os.str();
    }
private:
    std::vector<double> & getDMSParams(double coordinate) const {
        coordinate = abs(coordinate);
        double degrees = floor(coordinate);
        coordinate -= degrees;
        coordinate *= 60;
        double minutes = floor(coordinate);
        coordinate -= minutes;
        coordinate *=60;
        double seconds = coordinate;

        std::vector<double>* dmsParams = new std::vector<double>();
        dmsParams->push_back(degrees);
        dmsParams->push_back(minutes);
        dmsParams->push_back(seconds);
        return *dmsParams;
    }
};

#endif //GIS_POINT_H
