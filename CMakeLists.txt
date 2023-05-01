cmake_minimum_required(VERSION 3.25)
project(final)

set(CMAKE_CXX_STANDARD 17)

add_executable(final
        src/GIS.cpp
        src/GISRecord.cpp
        include/GISRecord.h
        src/NameIndex.cpp
        include/NameIndex.h
        src/CoordinateIndex.cpp
        include/CoordinateIndex.h
        src/BufferPool.cpp
        include/BufferPool.h
        src/Logger.cpp
        include/Logger.h
        src/SystemManager.cpp
        include/SystemManager.h
        src/CommandProcessor.cpp
        include/CommandProcessor.h
        )
