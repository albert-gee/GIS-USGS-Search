#include <fstream>
#include <sstream>
#include <iostream>
#include "../../include/log/LogService.h"

LogService::LogService(const string& logFileLocation, int logLineWidth, char lineBreakChar)
    : logLineWidth(logLineWidth), lineBreakChar(lineBreakChar) {
    // Delete the log file if it already exists
    if (std::remove(logFileLocation.c_str()) != 0) {
        std::perror("Error deleting the log file");
    }
    LogService::logFileLocation = logFileLocation;
    // Create the log file as an empty file
    logFile.open(logFileLocation);
    if (!logFile.is_open()) {
        std::cerr << "Error: Failed to open log file." << std::endl;
    }
    logFile.close();
}

// Destructor
LogService::~LogService() {
    logFile.close();
}

void LogService::logCommand(const int& cmdNumber, const string& function, list<string>& args, const char& delimiter) {
    ostringstream os;
    string commandHeader = "\n\nCommand ";
    if(function != "world"){
        os << commandHeader << cmdNumber << ": ";
    }
    os << function;
    for(string &arg : args){
        os << delimiter << arg;
    }
    os << endl;
    string line = os.str();
    logString(line);
}

void LogService::logComment(const string& comment){
    logString(comment);
}

void LogService::logString(const string& s){

    if (!logFile.is_open()) {
        std::cerr << "Error: Failed to open log file." << std::endl;
    } else {
        logFile << s << endl;
    }
}

void LogService::logImportStats(const IndexStats& indexStats) {
    stringstream os;
    os.width(27);
    os.setf(ios::left);
    os << "Imported Features by name:";
    os << indexStats.numOfIndexedLines << endl;
    os.width(27);
    os << "Longest probe sequence:\t";

    os << indexStats.longestProbeSeq << endl;
    os.width(27);
    os << "Imported Locations:" << indexStats.numOfIndexedLines << endl;
    os.width(27);
    os << "Average name length:" << indexStats.getAvgNameLength();
    logString(os.str());
    logLineBreak();
}

void LogService::logLineBreak() {
    logString(string(logLineWidth, lineBreakChar));
}

int LogService::getLogLineWidth() const {
    return logLineWidth;
}