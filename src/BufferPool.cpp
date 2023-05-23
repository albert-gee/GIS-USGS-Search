#include "../include/BufferPool.h"

GISRecord* BufferPool::searchForRecord(int featureId)
{
    int bufferIndex = 0;
    for (GISRecord &gisRecord : *buffer) {
        if (gisRecord.getFeatureId() == featureId) {
            if (bufferIndex > 0) {
                GISRecord *recent = &gisRecord;
                list<GISRecord>::iterator it = buffer->begin();
                std::advance(it, ++bufferIndex);
                buffer->erase(it);
                buffer->push_front(*recent);
            }
            return &gisRecord;
        }
        ++bufferIndex;
    }



    GISRecord* gis = NULL;
    //Insert code to search nameindex or coordinate index


    //If buffer limit is hit, pop the back and insert new record
    if (buffer->size() == maxSize) {
        buffer->pop_back();
        buffer->push_front(*gis);
    }
    return gis;
}