AMKGL_DEFINES_DIR = ../amkgl_defines_for_use_with_computree

INCLUDEPATH = ../AMKglTest

DEFINES += ENVIRONMENT64

INCLUDEPATH += ../pluginshared
INCLUDEPATH += ../ComputreeGui/tools/amkgl

CONFIG(debug, debug|release) {
    LIBS += -L../ComputreeInstallDebug
} else {
    LIBS += -L../ComputreeInstallRelease
}

LIBS += -lpluginShared
