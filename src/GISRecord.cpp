#include "../include/GISRecord.h"

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
