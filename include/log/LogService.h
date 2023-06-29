#ifndef FINAL_LOGGER_H
#define FINAL_LOGGER_H

#include <string>
#include <list>

using namespace std;

class LogService {
public:
    explicit LogService(const string& logFileLocation);

    void logCommand(const int& cmdNumber, const string& function, list<string>& args, const char& delimiter);
    void logImportStats(const int& numOfImportsByName, const int& longestProbe, const int& importedLocations, const int& avgNameLength);
    void logComment(const string& comment);

    void logLine(const string& line);

private:
    std::ofstream logFile;

};


#endif //FINAL_LOGGER_H
