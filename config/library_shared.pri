# this is the common file for all library

CT_PREFIX = ..
CT_PREFIX_LIB = ..
CT_PREFIX_INSTALL = ../../..

exists(../library/$$TARGET/libdependencies.pri) {
    include(../library/$$TARGET/libdependencies.pri)
}

LIB_PATH = ../
MUST_USE_EIGEN = 1

include(destdir.pri)
include(library_include_ct.pri)
include(include_dependencies.pri)

INCLUDEPATH += .
INCLUDEPATH += ..
INCLUDEPATH += ../library

DESTDIR = $$LIBRARY_DESTDIR

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
    # QMAKE_MACOSX_DEPLOYMENT_TARGET = 11.0
    DEFINES += _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
}
