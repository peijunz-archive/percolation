cmake_minimum_required(VERSION 2.8)
project(Percolation)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp lib/16807.h lib/16807.cpp lib/ndarray.h lib/deque.h lib/graph.cpp lib/graph.h lib/linklist.h)
add_executable(Percolation ${SOURCE_FILES})
