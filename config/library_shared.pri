# this is the common file for all library

CT_PREFIX = ..
CT_PREFIX_LIB = ..
CT_PREFIX_INSTALL = ../../..
CT_PREFIX_CONFIG = ../config

exists(../library/$$TARGET/libdependencies.pri) {
    include(../library/$$TARGET/libdependencies.pri)
}
include(destdir.pri)
include(library_include_ct.pri)
LIB_PATH = ../
include(include_dependencies.pri)

INCLUDEPATH += .
INCLUDEPATH += ..
INCLUDEPATH += ../library

DESTDIR = $${EXECUTABLE_DESTDIR}/libraries/core

TEMPLATE = lib
CONFIG += plugin

# c++17
greaterThan(QT_MAJOR_VERSION, 4) {
    CONFIG += c++17
} else {
    QMAKE_CXXFLAGS += -std=c++17
}

linux {
    QMAKE_CXXFLAGS += -Wno-c++11-compat
}

mingw {
    QMAKE_CXXFLAGS += -Wno-c++11-compat -Wno-deprecated-copy
}

macx {
    QMAKE_CXXFLAGS += -Wno-inconsistent-missing-override
    QMAKE_RANLIB += -no_warning_for_no_symbols
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15
}
