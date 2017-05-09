cmake_minimum_required(VERSION 3.7)
project(Heap)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -fno-exceptions -Wno-format -fno-rtti -std=c++17")

set(SOURCE_FILES
        src/PoolAllocator.cpp
        src/Main.cpp

        include/PoolAllocator.h)
add_executable(Heap ${SOURCE_FILES})