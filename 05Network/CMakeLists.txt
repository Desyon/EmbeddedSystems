cmake_minimum_required(VERSION 3.7)
project(05Network)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        src/Main.cpp
        src/Endpoint.cpp
        src/Communicator.cpp
        src/PoolAllocator.cpp
        include/Endpoint.h
        include/Communicator.h
        include/PoolAllocator.h
        )
add_executable(05Network ${SOURCE_FILES})