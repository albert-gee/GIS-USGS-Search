#ifndef GIS_BUFFERRECORD_H
#define GIS_BUFFERRECORD_H

#include "../../include/GISRecord.h"

struct BufferedRecord{
    int lineNum;
    GISRecord* gisRecordPtr;
    BufferedRecord(int lineNum, GISRecord* gisRecord){
        BufferedRecord::lineNum = lineNum;
        gisRecordPtr = gisRecord;
    }
};

#endif //GIS_BUFFERRECORD_H
