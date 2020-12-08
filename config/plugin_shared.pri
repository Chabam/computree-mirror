# this is the common file for all plugins

# to make qmake more faster :
CONFIG -= depend_includepath

isEmpty(CT_PREFIX) {
    isEmpty(INCLUDED_PLUGIN) {
        CT_PREFIX = ../../computreev6
    } else {
        CT_PREFIX = ../..
    }
}

isEmpty(CT_PREFIX_INSTALL) {
    CT_PREFIX_INSTALL = $$CT_PREFIX/..
}

isEmpty(CT_PREFIX_LIB) {
    CT_PREFIX_LIB = $$CT_PREFIX/library
}

isEmpty(LIB_PATH) {
    isEmpty(INCLUDED_PLUGIN) {
        LIB_PATH = ../../computreev6/3rdparty/
    } else {
        LIB_PATH = ../
    }
}

# all plugins needs this library
COMPUTREE += ctlibplugin

MUST_USE_EIGEN = 1

include(destdir.pri)
include(library_include_ct.pri)
include(include_dependencies.pri)

QT *= widgets

DESTDIR = $${PLUGIN_DESTDIR}

TEMPLATE = lib
CONFIG += plugin

CONFIG(debug, debug|release) {
    OBJECTS_DIR = debug/.obj
    MOC_DIR = debug/.moc
    UI_DIR = debug/.ui
    RCC_DIR = debug/.rcc
} else {
    OBJECTS_DIR = release/.obj
    MOC_DIR = release/.moc
    UI_DIR = release/.ui
    RCC_DIR = release/.rcc
}

macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 11.0
}

msvc:DEFINES += _USE_MATH_DEFINES

# c++17
greaterThan(QT_MAJOR_VERSION, 4) {
    CONFIG += c++17
} else {
    QMAKE_CXXFLAGS += -std=c++17
}
