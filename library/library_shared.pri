# this is the common file for all library

CT_LIB_PREFIX = ..
CT_PREFIX = ..
CT_PREFIX_INSTALL = ../../..

exists($$TARGET/libdependencies.pri) {
    include($$TARGET/libdependencies.pri)
}

include($${CT_PREFIX}/destdir.pri)
include($${CT_PREFIX}/include_ct_library.pri)
include(library_include_eigen.pri)

INCLUDEPATH += .
INCLUDEPATH += ..

DESTDIR = $${EXECUTABLE_DESTDIR}

TEMPLATE = lib
CONFIG += plugin

# c++11
greaterThan(QT_MAJOR_VERSION, 4) {
    CONFIG += c++11
} else {
    QMAKE_CXXFLAGS += -std=c++11
}

LIBRARY_PWD = $$PWD/$$TARGET

!equals(LIBRARY_PWD, $${OUT_PWD}) {
    error("Shadow build seems to be activated, please desactivated it !")
}
