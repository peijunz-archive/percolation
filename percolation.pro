TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11 -O3
INCLUDEPATH += /usr/include/opencv

LIBS += -L/usr/lib64 -lopencv_core -lopencv_imgcodecs -lopencv_highgui
SOURCES += main.cpp \

HEADERS += \
    ndarray.h \
    zonebond.h \
    save2dimg.h\
    percolation.h
