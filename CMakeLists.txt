cmake_minimum_required(VERSION 3.25)
project(GIS)

set(CMAKE_CXX_STANDARD 17)

add_executable(GIS
        src/GIS.cpp
        src/GISRecord.cpp
        include/GISRecord.h
        src/NameIndex.cpp
        include/NameIndex.h
        src/bufferPool/BufferPool.cpp
        include/bufferPool/BufferPool.h
        src/Logger.cpp
        include/Logger.h
        src/SystemManager.cpp
        include/SystemManager.h
        src/CommandProcessor.cpp
        include/CommandProcessor.h
        src/coordinateIndex/QuadTreeQuadrant.cpp
        include/coordinateIndex/QuadTreeQuadrant.h
        src/coordinateIndex/QuadTree.cpp
        include/coordinateIndex/QuadTree.h
        include/coordinateIndex/Point.h
        include/coordinateIndex/Entry.h
        src/bufferPool/BufferRecord.h)
