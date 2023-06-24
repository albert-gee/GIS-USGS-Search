#include <fstream>
#include <sstream>
#include "../include/Logger.h"

Logger::Logger(const string& logFileLocation){
    Logger::logFileLocation = logFileLocation;
}

void Logger::logCommand(const int& cmdNumber, const string& function, list<string>& args, const char& delimiter) {
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

void Logger::logComment(const string& comment){
    string line = comment;
    logLine(line);
}

void Logger::logLine(const string& line){
    ofstream logFile(logFileLocation, ios_base::app);
    logFile << line << endl;
    logFile.close();
}

void Logger::logImportStats(const int &numOfImportsByName, const int &longestProbe, const int &importedLocations,
                            const int &avgNameLength) {
    ostringstream  os;
    os << "Imported Features by name:\t" << numOfImportsByName << endl;
    os << "Longest probe sequence:\t" << longestProbe << endl;
    os << "Imported Locations:\t" << importedLocations << endl;
    os << "Average name length:\t" << avgNameLength <<endl;
    logLine(os.str());
    logLine(string(90, '-'));
}