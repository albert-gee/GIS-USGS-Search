#include <fstream>
#include <sstream>
#include <iostream>
#include "../../include/log/LogService.h"

LogService::LogService(const string& logFileLocation){
    // Delete the log file if it already exists
    if (std::remove(logFileLocation.c_str()) != 0) {
        std::perror("Error deleting the log file");
    }

    // Create the log file as an empty file
    logFile.open(logFileLocation);
    if (!logFile.is_open()) {
        std::cerr << "Error: Failed to open log file." << std::endl;
    }
}

void LogService::logCommand(const int& cmdNumber, const string& function, list<string>& args, const char& delimiter) {
    ostringstream os;
    string commandHeader = "Command ";
    if(function != "world"){
        os << commandHeader << cmdNumber << ": ";
    }
    os << function;
    for(string &arg : args){
        os << delimiter << arg;
    }
    os << endl;
    string line = os.str();
    logLine(line);
}

void LogService::logComment(const string& comment){
    logLine(comment);
}

void LogService::logLine(const string& line){
    logFile << line << endl;
    logFile.close();
}

void LogService::logImportStats(const int &numOfImportsByName, const int &longestProbe, const int &importedLocations,
                                const int &avgNameLength) {
    stringstream  os;
    os.width(27);
    os.setf(ios::left);
    os << "Imported Features by name:";
    os << numOfImportsByName << endl;
    os.width(27);
    os << "Longest probe sequence:\t";

    os << longestProbe << endl;
    os.width(27);
    os << "Imported Locations:" << importedLocations << endl;
    os.width(27);
    os << "Average name length:" << avgNameLength;
    logLine(os.str());
    logLine(string(90, '-'));
}