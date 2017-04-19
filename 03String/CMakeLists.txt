cmake_minimum_required(VERSION 3.7)
project(PreAllocString)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -fno-exceptions -Wno-format -fno-rtti -std=c++17")

set(SOURCE_FILES
        include/PreAllocString.h
        include/Printf.h
        src/Main.cpp
        src/Printf.cpp)

add_executable(03String ${SOURCE_FILES})