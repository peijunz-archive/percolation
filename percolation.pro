TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++0x -O3
INCLUDEPATH += ./lib
INCLUDEPATH += /usr/include/opencv

LIBS += -L/usr/lib64 -lopencv_core -lopencv_imgcodecs -lopencv_highgui
SOURCES += main.cpp \
    lib/16807.cpp \

HEADERS += \
    lib/16807.h \
    lib/ndarray.h \
    lib/zonebond.h \
    lib/save2dimg.h\
    percolation.h
