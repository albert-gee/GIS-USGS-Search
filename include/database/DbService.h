#ifndef GIS_DBSERVICE_H
#define GIS_DBSERVICE_H

#include <string>
#include "../coordinateIndex/Point.h"

// This class describes the service which allows to work with the databaseService file
class DbService {
public:
    explicit DbService(const std::string& databaseFileLocation);
    ~DbService();

    void open();
    void close();

    void import(const std::string& recordsDataSetFileLocation);

    bool getNextLine(std::string& line);

    std::string getLineByNumber(int lineNum);

    void import(const std::string &recordsDataSetFileLocation, double north, double south, double west, double east);

private:
    std::basic_string<char> databaseFileLocation;
    std::fstream databaseFile;

};


#endif //GIS_DBSERVICE_H
