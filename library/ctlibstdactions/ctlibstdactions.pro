CT_PREFIX = ../..
CT_PREFIX_INSTALL = ../../..

DONT_CHECK_CT_LIBRARY = 1

include($${CT_PREFIX}/shared.pri)
include($${PLUGIN_SHARED_DIR}/include.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += concurrent

QT += xml

DESTDIR = $${PLUGINSHARED_DESTDIR}
TARGET = ctlibstdactions

DEFINES += CTLIBSTDACTIONS_LIBRARY

HEADERS += $${PLUGIN_SHARED_INTERFACE_DIR}/interfaces.h \
    action/tools/math.h \
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
