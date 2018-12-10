AMKGL_DEFINES_DIR = ../amkgl_defines_for_use_with_computree

INCLUDEPATH = ../AMKglTest

DEFINES += ENVIRONMENT64

CT_PREFIX = ..
CT_PREFIX_INSTALL = ../..
CT_LIB_PREFIX = ../library

COMPUTREE = ctlibplugin

include(destdir.pri)
include(include_ct_library.pri)

INCLUDEPATH += ../ComputreeGui/tools/amkgl

CONFIG(debug, debug|release) {
    LIBS += -L../ComputreeInstallDebug
} else {
    LIBS += -L../ComputreeInstallRelease
}

LIBS += -lpluginShared
