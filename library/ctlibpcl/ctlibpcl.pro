MUST_USE_PCL = 1

include(../library_shared.pri)
include(../../include_all.pri)
include(../../plugin_shared.pri)

macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15
}

# c++14 for PCL (for the moment, because of FLANN issues with C++17)
greaterThan(QT_MAJOR_VERSION, 4) {
    CONFIG -= c++17
    CONFIG += c++14
} else {
    QMAKE_CXXFLAGS -= -std=c++17
    QMAKE_CXXFLAGS += -std=c++14
}

TARGET = ctlibpcl

DEFINES += CTLIBPCL_LIBRARY

HEADERS += \
    tools/ct_pcltools.h \
    ct_pcldefines.h

SOURCES += \ 
    tools/ct_pcltools.cpp
