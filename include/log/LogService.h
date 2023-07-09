#ifndef FINAL_LOGGER_H
#define FINAL_LOGGER_H

#include <string>
#include <list>
#include "IndexStats.h"

using namespace std;

class LogService {
public:
    ~LogService();
    LogService(const string& logFileLocation, int logLineWidth, char lineBreakChar);

    void logCommand(const int& cmdNumber, const string& function, list<string>& args, const char& delimiter);
    void logImportStats(const IndexStats& indexStats);
    void logComment(const string& comment);

    void logString(const string& s);
    void logLineBreak();

    int getLogLineWidth() const;

private:
    std::ofstream logFile;
    std::string logFileLocation;
    const int logLineWidth = 90;
    const char lineBreakChar = '-';
};


#endif //FINAL_LOGGER_H
