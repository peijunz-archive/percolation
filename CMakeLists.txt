cmake_minimum_required(VERSION 2.8)
project(Percolation)
include_directories(./lib/)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    main.cpp
    lib/test.cpp
    lib/test.h
    lib/nditer.h
    lib/16807.h
    lib/16807.cpp
    lib/singlelist.h
    lib/ndarray.h
    lib/graph.cpp
    lib/graph.h
    )

add_executable(Percolation ${SOURCE_FILES})
