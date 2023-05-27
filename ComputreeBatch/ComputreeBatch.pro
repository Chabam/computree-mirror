
CT_PREFIX = ..
CT_PREFIX_INSTALL = ../..
CT_PREFIX_LIB = ../library
CT_LIB_PREFIX = ..

COMPUTREE = ctlibcore

MUST_USE_EIGEN = 1

include(../config/destdir.pri)
include(../config/library_include_ct.pri)
include(../config/include_dependencies.pri)

##### TODO : remove it to use eigen from pcl or from 3rdparty ######

INCLUDEPATH += $$CT_LIB_PREFIX/3rdparty/eigen
TR_EXCLUDE  += $$CT_LIB_PREFIX/3rdparty/eigen/*

INCLUDEPATH += .
INCLUDEPATH += $$CT_PREFIX_LIB

CONFIG -= plugin

TARGET = CompuTreeBatch
TEMPLATE = app
QT += widgets network
CONFIG += console

# pour que l'application recherche les librairies dans son propre dossier
linux|win32 {
    QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN
}

DESTDIR = $${EXECUTABLE_DESTDIR}

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
    # LIBS += $${DESTDIR}/libCompuTreeCore*.dylib
} else {
    unix{
        # LIBS += $${DESTDIR}/libCompuTreeCore*.so
    }

    win32 {
        win32-g++ {
        # LIBS += $${DESTDIR}/libCompuTreeCore*.a
        }

        win32-msvc* {
        # LIBS += $${DESTDIR}/CompuTreeCore*.lib
        }
    }
}

macx {
    QMAKE_CXXFLAGS += -Wno-deprecated-declarations
    QMAKE_RANLIB += -no_warning_for_no_symbols
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 12.0
    CONFIG += sdk_no_version_check
}
