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