AMKGL_DEFINES_DIR = ../AMKgl/defines_computree

INCLUDEPATH = ../AMKglTest

DEFINES += ENVIRONMENT64

CT_PREFIX = ..
CT_PREFIX_INSTALL = ../..
CT_PREFIX_LIB = ../library

COMPUTREE = ctlibplugin

include(destdir.pri)
include($${CT_PREFIX_LIB}/library_include_ct.pri)

INCLUDEPATH += ../ComputreeGui/tools/amkgl

CONFIG(debug, debug|release) {
    LIBS += -L../ComputreeInstallDebug
} else {
    LIBS += -L../ComputreeInstallRelease
}

LIBS += -lpluginShared
