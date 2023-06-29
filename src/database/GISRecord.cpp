#include "../../include/database/GISRecord.h"

GISRecord::GISRecord(int feature_id, std::string feature_name, std::string feature_class, std::string state_alpha,
                     std::string state_numeric, std::string county_name, std::string county_numeric,
                     std::string primary_latitude_DMS, std::string primary_longitude_DMS,
                     std::string primary_latitude_DEC, std::string primary_longitude_DEC,
                     std::string source_latitude_DMS, std::string source_longitude_DMS, std::string source_latitude_DES,
                     std::string source_longitude_DES, int elevation_meters, int elevation_feet, std::string map_name,
                     std::string date_created, std::string date_edited): feature_id{feature_id},
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

void GISRecord::setFeatureId(int featureId) {
    feature_id = featureId;
}

const std::string &GISRecord::getFeatureName() const {
    return feature_name;
}

void GISRecord::setFeatureName(const std::string &featureName) {
    feature_name = featureName;
}

const std::string &GISRecord::getFeatureClass() const {
    return feature_class;
}

void GISRecord::setFeatureClass(const std::string &featureClass) {
    feature_class = featureClass;
}

const std::string &GISRecord::getStateAlpha() const {
    return state_alpha;
}

void GISRecord::setStateAlpha(const std::string &stateAlpha) {
    state_alpha = stateAlpha;
}

const std::string &GISRecord::getStateNumeric() const {
    return state_numeric;
}

void GISRecord::setStateNumeric(const std::string &stateNumeric) {
    state_numeric = stateNumeric;
}

const std::string &GISRecord::getCountyName() const {
    return county_name;
}

void GISRecord::setCountyName(const std::string &countyName) {
    county_name = countyName;
}

const std::string &GISRecord::getCountyNumeric() const {
    return county_numeric;
}

void GISRecord::setCountyNumeric(const std::string &countyNumeric) {
    county_numeric = countyNumeric;
}

const std::string &GISRecord::getPrimaryLatitudeDms() const {
    return primary_latitude_DMS;
}

void GISRecord::setPrimaryLatitudeDms(const std::string &primaryLatitudeDms) {
    primary_latitude_DMS = primaryLatitudeDms;
}

const std::string &GISRecord::getPrimaryLongitudeDms() const {
    return primary_longitude_DMS;
}

void GISRecord::setPrimaryLongitudeDms(const std::string &primaryLongitudeDms) {
    primary_longitude_DMS = primaryLongitudeDms;
}

const std::string &GISRecord::getPrimaryLatitudeDec() const {
    return primary_latitude_DEC;
}

void GISRecord::setPrimaryLatitudeDec(const std::string &primaryLatitudeDec) {
    primary_latitude_DEC = primaryLatitudeDec;
}

const std::string &GISRecord::getPrimaryLongitudeDec() const {
    return primary_longitude_DEC;
}

void GISRecord::setPrimaryLongitudeDec(const std::string &primaryLongitudeDec) {
    primary_longitude_DEC = primaryLongitudeDec;
}

const std::string &GISRecord::getSourceLatitudeDms() const {
    return source_latitude_DMS;
}

void GISRecord::setSourceLatitudeDms(const std::string &sourceLatitudeDms) {
    source_latitude_DMS = sourceLatitudeDms;
}

const std::string &GISRecord::getSourceLongitudeDms() const {
    return source_longitude_DMS;
}

void GISRecord::setSourceLongitudeDms(const std::string &sourceLongitudeDms) {
    source_longitude_DMS = sourceLongitudeDms;
}

const std::string &GISRecord::getSourceLatitudeDes() const {
    return source_latitude_DES;
}

void GISRecord::setSourceLatitudeDes(const std::string &sourceLatitudeDes) {
    source_latitude_DES = sourceLatitudeDes;
}

const std::string &GISRecord::getSourceLongitudeDes() const {
    return source_longitude_DES;
}

void GISRecord::setSourceLongitudeDes(const std::string &sourceLongitudeDes) {
    source_longitude_DES = sourceLongitudeDes;
}

int GISRecord::getElevationMeters() const {
    return elevation_meters;
}

void GISRecord::setElevationMeters(int elevationMeters) {
    elevation_meters = elevationMeters;
}

int GISRecord::getElevationFeet() const {
    return elevation_feet;
}

void GISRecord::setElevationFeet(int elevationFeet) {
    elevation_feet = elevationFeet;
}

const std::string &GISRecord::getMapName() const {
    return map_name;
}

void GISRecord::setMapName(const std::string &mapName) {
    map_name = mapName;
}

const std::string &GISRecord::getDateCreated() const {
    return date_created;
}

void GISRecord::setDateCreated(const std::string &dateCreated) {
    date_created = dateCreated;
}

const std::string &GISRecord::getDateEdited() const {
    return date_edited;
}

void GISRecord::setDateEdited(const std::string &dateEdited) {
    date_edited = dateEdited;
}
