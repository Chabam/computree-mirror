contains ( QT_VERSION, "^5.*" ) {
    QT *= widgets
}

CT_PREFIX = ..
CT_PREFIX_INSTALL = ../..
CT_LIB_PREFIX = ../library
DEFAULT_EIGEN_INC_PATH = "$$CT_LIB_PREFIX/3rdparty/eigen"

COMPUTREE = ctlibcore

# c++14
greaterThan(QT_MAJOR_VERSION, 4) {
    CONFIG += c++14
} else {
    QMAKE_CXXFLAGS += -std=c++14
}

win32-msvc {
    CONFIG(debug, debug|release) {
        LIBS += -lQGLViewerd
    } else {
        LIBS += -lQGLViewer
    }

    OTHER_FILES +=

    LIBS += -lopengl32 -lglu32
}

LIBS += -lAMKgl

include($${CT_PREFIX}/destdir.pri)
include($${CT_PREFIX}/include_ct_library.pri)
include($${CT_LIB_PREFIX}/library_include_eigen.pri)
include(../amkgl_default_path.pri)
include(../qglviewer_default_path.pri)

INCLUDEPATH += .
INCLUDEPATH += $$CT_LIB_PREFIX
#TR_EXCLUDE  += $${CT_PREFIX}/ComputreeCore/*

CONFIG -= plugin

TARGET = CompuTreeGui
TEMPLATE = app
QT += opengl
QT += xml

DESTDIR = $${EXECUTABLE_DESTDIR}

greaterThan(QT_MAJOR_VERSION, 4): QT += concurrent

HEADERS += dm_graphicsviewsynchronizedgroup.h \
    dm_graphicsviewsynchronizedgroupoptions.h \
    dm_itemdrawablemodelmanager.h \
    dm_multipleitemdrawablemodelmanager.h \
    dm_itemmodelviewsynchronizedgroup.h \
    dm_abstractviewsynchronizedgroup.h \
    dm_itemmodelviewsynchronizedgroupoptions.h \
    dm_actionsmanager.h \
    dm_actionshandler.h \
    imainprogressdialog.h \
    dm_abstractinfo.h \
    dm_iteminfoforgraphics.h \
    dm_iprogresslistener.h \
    dm_progresslistenermanager.h \
    dm_stepsfrompluginsmodelconstructor.h \
    dm_domutils.h \
    dm_itemdrawableconfigurationmanagerview.h \
    $${CT_LIB_PREFIX}/documentinterface.h \
    $${CT_LIB_PREFIX}/graphicsviewinterface.h \
    $${CT_LIB_PREFIX}/indocumentviewinterface.h \
    $${CT_LIB_PREFIX}/treeviewinterface.h

# OTHER
SOURCES += main.cpp \
    dm_document.cpp \
    dm_graphicsview.cpp \
    dm_graphicsviewoptions.cpp \
    dm_documentview.cpp \
    dm_documentmanagerview.cpp \
    dm_documentmanager.cpp \
    dm_guimanager.cpp \
    dm_mainwindow.cpp \
    dm_asynchroneprogress.cpp \
    dm_itemdrawablemanageroptions.cpp \
    dm_graphicsviewcamera.cpp \
    dm_graphicsviewsynchronizedgroup.cpp \
    dm_graphicsviewsynchronizedgroupoptions.cpp \
    dm_itemdrawablemodelmanager.cpp \
    dm_multipleitemdrawablemodelmanager.cpp \
    dm_itemmodelviewsynchronizedgroup.cpp \
    dm_abstractviewsynchronizedgroup.cpp \
    dm_itemmodelviewsynchronizedgroupoptions.cpp \
    dm_actionsmanager.cpp \
    dm_actionshandler.cpp \
    dm_abstractinfo.cpp \
    dm_iteminfoforgraphics.cpp \
    dm_progresslistenermanager.cpp \
    dm_stepsfrompluginsmodelconstructor.cpp \
    dm_itemdrawableconfigurationmanagerview.cpp

HEADERS += \
    dm_document.h \
    dm_graphicsview.h \
    dm_graphicsviewoptions.h \
    dm_documentview.h \
    dm_documentmanagerview.h \
    dm_documentmanager.h \
    dm_guimanager.h \
    dm_mainwindow.h \
    dm_asynchroneprogress.h \
    dm_itemdrawablemanageroptions.h \
    dm_graphicsviewcamera.h \
    dm_context.h
    
TRANSLATIONS += languages/computreegui_fr.ts \
                languages/computreegui_en.ts

include(view/view.pri)
include(qtcolorpicker/qtcolorpicker.pri)
include(muParser/muparser.pri)
include(tools/tools.pri)

TR_EXCLUDE  += ./qtcolorpicker/*
TR_EXCLUDE  += ./muParser/*

RESOURCES += resource/icones.qrc

unix {
    # add your own with quoting gyrations to make sure $ORIGIN gets to the command line unexpanded
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\'"
}

##### AMKGL #####
AMKGL_DIR = ../$$AMKGL_PATH
AMKGL_LIB_DIR = $$AMKGL_DIR/compiled
QGLVIEWER_DIR = ../$$QGL_VIEWER_PATH

DEFINES += QGLVIEWER_STATIC

INCLUDEPATH += $$AMKGL_DIR
INCLUDEPATH += $$QGLVIEWER_DIR
INCLUDEPATH += ../amkgl_defines_for_use_with_computree

LIBS += -L$$AMKGL_LIB_DIR
LIBS += -L$$QGLVIEWER_DIR

win32-g++ {
    CONFIG(debug, debug|release) {
        LIBS += -lQGLViewerd
    } else {
        LIBS += -lQGLViewer
    }

    OTHER_FILES +=

    LIBS += -lopengl32 -lglu32
}

unix {
    LIBS += $$QGLVIEWER_DIR/libQGLViewer-qt5.a
}

!equals(PWD, $${OUT_PWD}) {
    error("Shadow build seems to be activated, please desactivated it !")
}
