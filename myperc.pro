TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++0x
INCLUDEPATH += ./lib

SOURCES += main.cpp \
    lib/16807.cpp \
#    lib/test.cpp

HEADERS += \
    lib/16807.h \
    lib/ndarray.h \
#    lib/nditer.h \
    lib/singlelist.h \
#    lib/test.h \
    lib/wrapping.h
