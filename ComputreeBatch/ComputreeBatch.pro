#include(../shared.pri)
#include($${PLUGIN_SHARED_DIR}/include.pri)

contains ( QT_VERSION, "^5.*" ) {
    QT *= widgets
}

CT_PREFIX = ..
CT_PREFIX_INSTALL = ../..
CT_LIB_PREFIX = ../library

COMPUTREE = ctlibplugin

include($${CT_PREFIX}/destdir.pri)
include($${CT_PREFIX}/include_ct_library.pri)

##### TODO : remove it to use eigen from pcl or from 3rdparty ######

INCLUDEPATH += $$CT_LIB_PREFIX/3rdparty/eigen
TR_EXCLUDE  += $$CT_LIB_PREFIX/3rdparty/eigen/*

INCLUDEPATH += .
INCLUDEPATH += $$CT_LIB_PREFIX
INCLUDEPATH += $${CT_PREFIX}/ComputreeCore
INCLUDEPATH += $${CT_PREFIX}/ComputreeCore/src

CONFIG -= plugin

TARGET = CompuTreeBatch
TEMPLATE = app
QT += network
CONFIG += console

# pour que l'application recherche les librairies dans son propre dossier
QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN

DESTDIR = $${PLUGINSHARED_DESTDIR}

SOURCES += \
    main.cpp \
    batchdialog.cpp \
    batch.cpp \
    batchpluginmanager.cpp \
    signalhandler.cpp \
    StepResultTreeView/gstepmanager2.cpp

HEADERS += \
    batchdialog.h \
    interfacesbatch.h \
    batch.h \
    batchpluginmanager.h \
    signalhandler.h \
    StepResultTreeView/gstepmanager2.h

FORMS += \
    batchdialog.ui \
    StepResultTreeView/gstepmanager2.ui

RESOURCES += \
    systray.qrc

macx {
    LIBS += $${DESTDIR}/libCompuTreeCore*.dylib
} else {
    unix{
        LIBS += $${DESTDIR}/libCompuTreeCore*.so
    }

    win32 {
        win32-g++ {
        LIBS += $${DESTDIR}/libCompuTreeCore*.a
        }

        win32-msvc* {
        LIBS += $${DESTDIR}/CompuTreeCore*.lib
        }
    }
}




































