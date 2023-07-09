#ifndef GISRECORD_H
#define GISRECORD_H

#include <string>

class GISRecord {
public:
    GISRecord(int feature_id, std::string feature_name, std::string feature_class, std::string state_alpha,
              std::string state_numeric, std::string county_name, std::string county_numeric,
              std::string primary_latitude_DMS, std::string primary_longitude_DMS, std::string primary_latitude_DEC,
              std::string primary_longitude_DEC, std::string source_latitude_DMS, std::string source_longitude_DMS,
              std::string source_latitude_DES, std::string source_longitude_DES, int elevation_meters,
              int elevation_feet, std::string map_name, std::string date_created, std::string date_edited);

    [[nodiscard]] int getFeatureId() const;

    void setFeatureId(int featureId);

    [[nodiscard]] const std::string &getFeatureName() const;

    void setFeatureName(const std::string &featureName);

    [[nodiscard]] const std::string &getFeatureClass() const;

    void setFeatureClass(const std::string &featureClass);

    [[nodiscard]] const std::string &getStateAlpha() const;

    void setStateAlpha(const std::string &stateAlpha);

    [[nodiscard]] const std::string &getStateNumeric() const;

    void setStateNumeric(const std::string &stateNumeric);

    [[nodiscard]] const std::string &getCountyName() const;

    void setCountyName(const std::string &countyName);

    [[nodiscard]] const std::string &getCountyNumeric() const;

    void setCountyNumeric(const std::string &countyNumeric);

    [[nodiscard]] const std::string &getPrimaryLatitudeDms() const;

    void setPrimaryLatitudeDms(const std::string &primaryLatitudeDms);

    [[nodiscard]] const std::string &getPrimaryLongitudeDms() const;

    void setPrimaryLongitudeDms(const std::string &primaryLongitudeDms);

    [[nodiscard]] const std::string &getPrimaryLatitudeDec() const;

    void setPrimaryLatitudeDec(const std::string &primaryLatitudeDec);

    [[nodiscard]] const std::string &getPrimaryLongitudeDec() const;

    void setPrimaryLongitudeDec(const std::string &primaryLongitudeDec);

    [[nodiscard]] const std::string &getSourceLatitudeDms() const;

    void setSourceLatitudeDms(const std::string &sourceLatitudeDms);

    [[nodiscard]] const std::string &getSourceLongitudeDms() const;

    void setSourceLongitudeDms(const std::string &sourceLongitudeDms);

    [[nodiscard]] const std::string &getSourceLatitudeDes() const;

    void setSourceLatitudeDes(const std::string &sourceLatitudeDes);

    [[nodiscard]] const std::string &getSourceLongitudeDes() const;

    void setSourceLongitudeDes(const std::string &sourceLongitudeDes);

    [[nodiscard]] int getElevationMeters() const;

    void setElevationMeters(int elevationMeters);

    [[nodiscard]] int getElevationFeet() const;

    void setElevationFeet(int elevationFeet);

    [[nodiscard]] const std::string &getMapName() const;

    void setMapName(const std::string &mapName);

    [[nodiscard]] const std::string &getDateCreated() const;

    void setDateCreated(const std::string &dateCreated);

    [[nodiscard]] const std::string &getDateEdited() const;

    void setDateEdited(const std::string &dateEdited);
    std::string str();

    std::string detailStr();

private:
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

    std::string convertDECtoDMS(double dec);
};


#endif //GISRECORD_H
