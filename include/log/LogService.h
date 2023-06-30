#ifndef FINAL_LOGGER_H
#define FINAL_LOGGER_H

#include <string>
#include <list>

using namespace std;

class LogService {
public:
    ~LogService();
    explicit LogService(const string& logFileLocation);

    void logCommand(const int& cmdNumber, const string& function, list<string>& args, const char& delimiter);
    void logImportStats(const int &numOfImports, const int &longestProbe, const int &avgNameLength);
    void logComment(const string& comment);

    void logString(const string& s);

private:
    std::ofstream logFile;
    std::string logFileLocation;
};


#endif //FINAL_LOGGER_H
