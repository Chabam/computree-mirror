# this is the common file for all library

CT_LIB_PREFIX = ..
CT_PREFIX = ..
CT_PREFIX_INSTALL = ../../..
DEFAULT_EIGEN_INC_PATH = "../3rdparty/eigen"

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

LIBRARY_PWD = $$PWD/$$TARGET

!equals(LIBRARY_PWD, $${OUT_PWD}) {
    error("Shadow build seems to be activated, please desactivated it !")
}
