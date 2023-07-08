cmake_minimum_required(VERSION 3.25)
project(GIS)

set(CMAKE_CXX_STANDARD 17)

add_executable(GIS
        src/GIS.cpp
        src/database/GISRecord.cpp
        include/database/GISRecord.h
        src/nameIndex/NameIndex.cpp
        include/nameIndex/NameIndex.h
        src/bufferPool/BufferPool.cpp
        include/bufferPool/BufferPool.h
        src/log/LogService.cpp
        include/log/LogService.h
        src/systemManager/SystemManager.cpp
        include/systemManager/SystemManager.h
        src/commandProcessor/CommandProcessor.cpp
        include/commandProcessor/CommandProcessor.h
        src/coordinateIndex/QuadTreeQuadrant.cpp
        include/coordinateIndex/QuadTreeQuadrant.h
        src/coordinateIndex/QuadTree.cpp
        include/coordinateIndex/QuadTree.h
        include/coordinateIndex/Point.h
        include/coordinateIndex/Entry.h
        include/bufferPool/BufferRecord.h src/database/DbService.cpp include/database/DbService.h include/bufferPool/Features.h include/bufferPool/Features.h include/log/IndexStats.h)
