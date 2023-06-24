//
// Created by micgo on 6/10/2023.
//

#ifndef GIS_LINEUTILITY_H
#define GIS_LINEUTILITY_H

#include <string>
#include <list>
#include <vector>
#include "coordinateIndex/Point.h"
#include "GISRecord.h"
#include <sstream>

using namespace std;

class LineUtility {
public:
    // Extract the parameters from a line
    static vector<string> * extractParametersFromLine(const string &line, char delimiter) {
        int index = 0;
        auto *parameters = new vector<string>();
        ostringstream os;
        while (line[index] != '\0') {
            if (line[index] != delimiter) {
                os << line[index];
            } else {
                parameters->push_back(os.str());
                os.str("");
                os.clear();
            }
            ++index;
        }
        if(parameters->size() < 20){
            parameters->push_back(os.str());
        }
        return parameters;
    }


    // Extract the location from a line
    static Point extractLocationFromLine(string line, int longitudeColumn, int latitudeColumn, char delimiter) {
        int index = 0;
        string latitude;
        string longitude;
        int column = 0;
        while(line[index] != '\0' && column <= longitudeColumn){
            if(line[index] != delimiter){
                if(column == latitudeColumn) {
                    latitude += line[index];
                } else if(column == latitudeColumn){
                    longitude += line[index];
                }
            } else {
                ++column;
            }
            ++index;
        }
        return {std::stod(latitude), std::stod(longitude)};
    }

    // Extract the feature name and state alpha from a line
    static string extractFeatureNameAndStateFromLine(string line, int featureNameColumn, int stateAlphaColumn, char delimiter) {
        int index = 0;
        string featureName;
        string stateAlpha;
        int column = 0;
        while(line[index] != '\0' && column <= stateAlphaColumn){
            if(line[index] != delimiter){
                if(column == featureNameColumn) {
                    featureName += line[index];
                } else if(column == stateAlphaColumn){
                    stateAlpha += line[index];
                }
            } else {
                ++column;
            }
            ++index;
        }
        return featureName + " " + stateAlpha;
    }

    static string extractFeatureName(string line, int featureNameColumn, char delimiter) {
        int index = 0;
        ostringstream os;
        int column = 0;
        while(line[index] != '\0' && column <= featureNameColumn){
            if(line[index] != delimiter){
                if(column == featureNameColumn) {
                    os << line[index];
                }
            } else {
                ++column;
            }
            ++index;
        }
        return os.str();
    }

    static string extractParamFromLine(const string line, const int paramColumn, const char delimiter) {
        int index = 0;
        ostringstream os;
        int column = 0;
        while(line[index] != '\0' && column <= paramColumn){
            if(line[index] != delimiter){
                if(column == paramColumn) {
                    os << line[index];
                }
            } else {
                ++column;
            }
            ++index;
        }
        return os.str();
    }



    static GISRecord * createGISRecordFromLine(const string& line, char delimiter) {

        vector<string> *params = LineUtility::extractParametersFromLine(line, delimiter);

        auto * gisRecord = new GISRecord(stoi((*params)[0]),
                                              (*params)[1],
                                              (*params)[2],
                                              (*params)[3],
                                              (*params)[4],
                                              (*params)[5],
                                              (*params)[6],
                                              (*params)[7],
                                              (*params)[8],
                                              (*params)[9],
                                              (*params)[10],
                                              (*params)[11],
                                              (*params)[12],
                                              (*params)[13],
                                              (*params)[14],
                                              stoi((*params)[15]),
                                              stoi((*params)[16]),
                                              (*params)[17],
                                              (*params)[18],
                                              (*params)[19]);
        return gisRecord;
    }

private:
};


#endif //GIS_LINEUTILITY_H
