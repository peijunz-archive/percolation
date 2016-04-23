TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++0x -O3
INCLUDEPATH += ./lib
INCLUDEPATH += /usr/include/opencv

SOURCES += main.cpp \
    lib/16807.cpp \

HEADERS += \
    lib/16807.h \
    lib/ndarray.h \
    lib/singlelist.h \
    percolation.h
