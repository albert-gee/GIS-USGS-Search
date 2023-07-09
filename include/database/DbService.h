#ifndef GIS_DBSERVICE_H
#define GIS_DBSERVICE_H

#include <string>

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

private:
    std::basic_string<char> databaseFileLocation;
    std::fstream databaseFile;

};


#endif //GIS_DBSERVICE_H
