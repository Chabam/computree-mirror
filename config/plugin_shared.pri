# this is the common file for all plugins

# to make qmake more faster :
CONFIG -= depend_includepath

isEmpty(CT_PREFIX) {
    CT_PREFIX = ../..
}

isEmpty(CT_PREFIX_INSTALL) {
    CT_PREFIX_INSTALL = $$CT_PREFIX/..
}

isEmpty(CT_PREFIX_LIB) {
    CT_PREFIX_LIB = $$CT_PREFIX/library
}

# all plugins needs this library
COMPUTREE += ctlibplugin

LIB_PATH = ../

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
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15
}

msvc:DEFINES += _USE_MATH_DEFINES

# c++17
greaterThan(QT_MAJOR_VERSION, 4) {
    CONFIG += c++17
} else {
    QMAKE_CXXFLAGS += -std=c++17
}
