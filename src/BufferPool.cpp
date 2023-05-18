#include "../include/BufferPool.h"

GISRecord* BufferPool::searchForRecord(int featureId)
{
    int bufferIndex = 0;
    for (GISRecord gisRecord : buffer) {
        if (gisRecord.getFeatureId() == featureId) {
            if (bufferIndex > 0) {
                buffer.push_front(gisRecord);
                list<GISRecord>::iterator it = buffer.begin();
                std::advance(it, ++bufferIndex);
                buffer.erase(it);
            }

            return &gisRecord;
        }
        ++bufferIndex;
    }



    GISRecord* gis = NULL;
    //Insert code to search database and create a GISrecord and assign to gis variable


    //If buffer limit is hit, pop the back and insert new record
    if (buffer.size() == maxSize) {
        buffer.pop_back();
        buffer.push_front(*gis);
    }
    return gis;
}