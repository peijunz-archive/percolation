cmake_minimum_required(VERSION 2.8)
project(Percolation)
include_directories(./lib/)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    main.cpp
    lib/test.cpp
    lib/test.h
    lib/16807.h
    lib/16807.cpp
    lib/iter.h
    lib/iter.cpp
    lib/linklist.h
    lib/deque.h
    lib/ndarray.h)

add_executable(Percolation ${SOURCE_FILES})
