cmake_minimum_required(VERSION 3.7)
project(01CmdLineParser)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -fno-exceptions -Wno-format -fno-rtti -std=c++1y")

set(SOURCE_FILES
        include/OptParser.h
        src/main.cpp
        src/OptParser.cpp)

add_executable(01CmdLineParser ${SOURCE_FILES})