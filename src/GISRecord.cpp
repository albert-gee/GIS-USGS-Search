//
// Created by albert on 26/04/23.
//

#include "GISRecord.h"

class GISRecord {
public:
    int feature_id;
    std::string feature_name;

    std::string feature_class;

    std::string state_alpha;
    std::string state_numeric;
    std::string county_name;
    std::string county_numeric;

    std::string primary_latitude_DMS;
    std::string primary_longitude_DMS;
    std::string primary_latitude_DEC;
    std::string primary_longitude_DEC;

    std::string source_latitude_DMS;
    std::string source_longitude_DMS;
    std::string source_latitude_DES;
    std::string source_longitude_DES;

    int elevation_meters;

    int elevation_feet;

    std::string map_name;

    std::string date_created;

    std::string date_edited;

};