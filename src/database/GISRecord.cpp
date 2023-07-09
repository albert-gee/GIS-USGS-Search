#include <sstream>
#include <iostream>
#include <list>
#include <vector>
#include "../../include/database/GISRecord.h"
#include "../../include/coordinateIndex/DMS.h"

GISRecord::GISRecord(int feature_id, std::string feature_name, std::string feature_class, std::string state_alpha,
                     std::string state_numeric, std::string county_name, std::string county_numeric,
                     std::string primary_latitude_DMS, std::string primary_longitude_DMS,
                     std::string primary_latitude_DEC, std::string primary_longitude_DEC,
                     std::string source_latitude_DMS, std::string source_longitude_DMS, std::string source_latitude_DES,
                     std::string source_longitude_DES, int elevation_meters, int elevation_feet, std::string map_name,
                     std::string date_created, std::string date_edited) : feature_id{feature_id},
                                                                          feature_name{std::move(feature_name)},
                                                                          feature_class(std::move(feature_class)),
                                                                          state_alpha(std::move(state_alpha)),
                                                                          state_numeric(std::move(state_numeric)),
                                                                          county_name(std::move(county_name)),
                                                                          county_numeric(std::move(county_numeric)),
                                                                          primary_latitude_DMS(
                                                                                  std::move(primary_latitude_DMS)),
                                                                          primary_longitude_DMS(
                                                                                  std::move(primary_longitude_DMS)),
                                                                          primary_latitude_DEC(
                                                                                  std::move(primary_latitude_DEC)),
                                                                          primary_longitude_DEC(
                                                                                  std::move(primary_longitude_DEC)),
                                                                          source_latitude_DMS(
                                                                                  std::move(source_latitude_DMS)),
                                                                          source_longitude_DMS(
                                                                                  std::move(source_longitude_DMS)),
                                                                          source_latitude_DES(
                                                                                  std::move(source_latitude_DES)),
                                                                          source_longitude_DES(
                                                                                  std::move(source_longitude_DES)),
                                                                          elevation_meters{elevation_meters},
                                                                          elevation_feet{elevation_feet},
                                                                          map_name(std::move(map_name)),
                                                                          date_created(std::move(date_created)),
                                                                          date_edited(std::move(date_edited)) {}

int GISRecord::getFeatureId() const {
    return feature_id;
}

const std::string &GISRecord::getFeatureName() const {
    return feature_name;
}

const std::string &GISRecord::getFeatureClass() const {
    return feature_class;
}

const std::string &GISRecord::getStateAlpha() const {
    return state_alpha;
}

const std::string &GISRecord::getStateNumeric() const {
    return state_numeric;
}

const std::string &GISRecord::getCountyName() const {
    return county_name;
}

const std::string &GISRecord::getPrimaryLatitudeDms() const {
    return primary_latitude_DMS;
}

const std::string &GISRecord::getPrimaryLongitudeDms() const {
    return primary_longitude_DMS;
}

const std::string &GISRecord::getPrimaryLatitudeDec() const {
    return primary_latitude_DEC;
}

const std::string &GISRecord::getPrimaryLongitudeDec() const {
    return primary_longitude_DEC;
}

const std::string &GISRecord::getSourceLatitudeDms() const {
    return source_latitude_DMS;
}

const std::string &GISRecord::getSourceLongitudeDms() const {
    return source_longitude_DMS;
}

const std::string &GISRecord::getSourceLatitudeDes() const {
    return source_latitude_DES;
}

const std::string &GISRecord::getSourceLongitudeDes() const {
    return source_longitude_DES;
}

int GISRecord::getElevationMeters() const {
    return elevation_meters;
}

int GISRecord::getElevationFeet() const {
    return elevation_feet;
}

const std::string &GISRecord::getMapName() const {
    return map_name;
}

const std::string &GISRecord::getDateCreated() const {
    return date_created;
}

const std::string &GISRecord::getDateEdited() const {
    return date_edited;
}

std::string GISRecord::str() {
    char delimiter = '|';
    std::ostringstream os;

    os << feature_id << delimiter;
    os << feature_name << delimiter;
    os << feature_class << delimiter;

    os << state_alpha << delimiter;
    os << state_numeric << delimiter;
    os << county_name << delimiter;
    os << county_numeric << delimiter;

    os << primary_latitude_DMS << delimiter;
    os << primary_longitude_DMS << delimiter;
    os << primary_latitude_DEC << delimiter;
    os << primary_longitude_DEC << delimiter;

    os << source_latitude_DMS << delimiter;
    os << source_longitude_DMS << delimiter;
    os << source_latitude_DES << delimiter;
    os << source_longitude_DES << delimiter;

    os << elevation_meters << delimiter;
    os << elevation_feet << delimiter;
    os << map_name << delimiter;
    os << date_created << delimiter;
    os << date_edited << delimiter;
    return os.str();
}

std::string GISRecord::detailStr() {
    std::ostringstream os;
    int indent = 5;
    int indent2 = 13;
    std::string colon = ": ";
    DMS lat (primary_longitude_DMS);
    DMS lon (primary_latitude_DMS);


    os.setf(std::ios::left);

    std::vector<std::string> params = {std::to_string(feature_id), feature_name, feature_class, state_alpha,
                                       county_name, lon.toDmsString(), lat.toDmsString(),
                                       std::to_string(elevation_feet), map_name, date_created};

    std::vector<std::string> headers = {"Feature ID", "Feature Name", "Feature Cat", "State", "County", "Longitude",
                                        "Latitude", "Elev in ft", "USGS Quad", "Date Created"};

    for (int i = 0; i < params.size(); ++i) {
        os.width(indent);
        os << "";
        os.width(indent2);
        os << headers[i] << colon << params[i] << std::endl;
    }
    os << std::endl;
    return os.str();
}
