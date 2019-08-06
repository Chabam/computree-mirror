QT = core gui

contains ( QT_VERSION, "^5.*" ) {
    QT *= widgets
}

include(../library_shared.pri)
include(../../include_all.pri)

TARGET = ctlibstdactions

DEFINES += CTLIBSTDACTIONS_LIBRARY

HEADERS += action/tools/math.h \
    action/tools/polygonforpicking.h \
    action/tools/rectangleforpicking.h \
    action/tools/ct_actionselecttool.h \
    views/actions/ct_actionselectitemdrawablegvoptions.h \
    action/ct_actionselectitemdrawablegv.h

SOURCES += \ 
    action/tools/math.cpp \
    action/tools/polygonforpicking.cpp \
    action/tools/rectangleforpicking.cpp \
    action/tools/ct_actionselecttool.cpp \
    views/actions/ct_actionselectitemdrawablegvoptions.cpp \
    action/ct_actionselectitemdrawablegv.cpp

FORMS += \
    views/actions/ct_actionselectitemdrawablegvoptions.ui

RESOURCES += \
    icons.qrc
