# this is the common file for all plugins

# to make qmake more faster :
CONFIG -= depend_includepath

isEmpty(CT_PREFIX) {
    CT_PREFIX = ..
}

isEmpty(CT_PREFIX_INSTALL) {
    CT_PREFIX_INSTALL = $$CT_PREFIX/..
}

isEmpty(CT_LIB_PREFIX) {
    CT_LIB_PREFIX = $$CT_PREFIX/library
}

# all plugins needs this library
COMPUTREE += ctlibplugin

include(destdir.pri)
include(include_ct_library.pri)
include(include_all.pri)

contains ( QT_VERSION, "^5.*" ) {
    QT *= widgets
}

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

msvc:DEFINES += _USE_MATH_DEFINES

# c++11
greaterThan(QT_MAJOR_VERSION, 4) {
    CONFIG += c++11
} else {
    QMAKE_CXXFLAGS += -std=c++11
}
