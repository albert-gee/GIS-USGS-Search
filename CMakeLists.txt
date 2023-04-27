cmake_minimum_required(VERSION 3.25)
project(final)

set(CMAKE_CXX_STANDARD 17)

add_executable(final
        main.cpp
        src/GIS.cpp
        src/GIS.h
        src/GISRecord.cpp
        src/GISRecord.h
        src/NameIndex.cpp
        src/NameIndex.h
        src/CoordinateIndex.cpp
        src/CoordinateIndex.h
        src/BufferPool.cpp
        src/BufferPool.h
        src/Logger.cpp
        src/Logger.h
        src/SystemManager.cpp
        src/SystemManager.h
        src/CommandProcessor.cpp
        src/CommandProcessor.h
        )
