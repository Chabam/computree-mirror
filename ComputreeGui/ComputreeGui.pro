contains ( QT_VERSION, "^5.*" ) {
    QT *= widgets
}

CT_PREFIX = ..
CT_PREFIX_INSTALL = ../..
CT_PREFIX_CONFIG = ../config
CT_PREFIX_LIB = ../library
DEFAULT_EIGEN_INC_PATH = "../3rdparty/eigen/"

COMPUTREE = ctlibcore

# c++17
greaterThan(QT_MAJOR_VERSION, 4) {
    CONFIG += c++17
} else {
    QMAKE_CXXFLAGS += -std=c++17
}

LIBS += -lAMKgl

include($${CT_PREFIX_CONFIG}/destdir.pri)
include($${CT_PREFIX_LIB}/library_include_ct.pri)
include($${CT_PREFIX_CONFIG}/user_path_eigen.pri)
include($${CT_PREFIX_CONFIG}/default_path_amkgl.pri)
include($${CT_PREFIX_CONFIG}/default_path_qglviewer.pri)

INCLUDEPATH += .
INCLUDEPATH += $$CT_PREFIX_LIB
#TR_EXCLUDE  += $${CT_PREFIX}/ComputreeCore/*

CONFIG -= plugin
CONFIG += windows

TARGET = CompuTreeGui
TEMPLATE = app
RC_ICONS = Computree.ico
QT += opengl
QT += xml

DESTDIR = $${EXECUTABLE_DESTDIR}

# FOR FINAL INSTALLATION (make install)
# (done for Windows+MSVC, to be done for the other platforms)
win32-msvc* {
    lib_qt.files += $$[QT_INSTALL_LIBS]/../bin/Qt5Concurrent.dll
    lib_qt.files += $$[QT_INSTALL_LIBS]/../bin/Qt5Core.dll
    lib_qt.files += $$[QT_INSTALL_LIBS]/../bin/Qt5Gui.dll
    lib_qt.files += $$[QT_INSTALL_LIBS]/../bin/Qt5OpenGL.dll
    lib_qt.files += $$[QT_INSTALL_LIBS]/../bin/Qt5Widgets.dll
    lib_qt.files += $$[QT_INSTALL_LIBS]/../bin/Qt5Xml.dll
    lib_qt.path = $$DESTDIR/libraries/Qt

    lib_qt_platforms.files += $$[QT_INSTALL_LIBS]/../plugins/platforms/qwindows.dll
    lib_qt_platforms.path = $$DESTDIR/platforms

    OPENCV_BASE_PATH = $$PWD/../3rdparty/opencv/build/
    lib_opencv.files += $${OPENCV_BASE_PATH}bin/opencv_world440.dll
    lib_opencv.path = $$DESTDIR/libraries/opencv

    GDAL_BASE_PATH = $$PWD/../3rdparty/gdal/
    lib_gdal.files += $${GDAL_BASE_PATH}bin/*.dll
    lib_gdal.path = $$DESTDIR/libraries/gdal

    lib_pcl.files += $${PCL_BASE_PATH}bin/*[^d].dll
    lib_pcl.path = $$DESTDIR/libraries/pcl

    setenv.files += ../tools/win_setenv.cmd
    setenv.path = $$DESTDIR

    lib_opengl.files += ../3rdparty/opengl/opengl32.dll
    lib_opengl.path = $$DESTDIR

    INSTALLS += lib_qt lib_qt_platforms lib_opencv lib_gdal lib_pcl setenv lib_opengl
}


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
    $${CT_PREFIX_LIB}/documentinterface.h \
    $${CT_PREFIX_LIB}/graphicsviewinterface.h \
    $${CT_PREFIX_LIB}/indocumentviewinterface.h \
    $${CT_PREFIX_LIB}/treeviewinterface.h

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
# include(../3rdparty/muparser/muparser.pri)
include(tools/tools.pri)

TR_EXCLUDE  += ./qtcolorpicker/*
# TR_EXCLUDE  += ./muParser/*

RESOURCES += resource/icones.qrc

linux {
    # add your own with quoting gyrations to make sure $ORIGIN gets to the command line unexpanded
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN\'"
    QMAKE_CXXFLAGS += -Wno-c++11-compat
}

win32-g++ {
    QMAKE_CXXFLAGS += -Wno-c++11-compat
}

macx {
    QMAKE_RANLIB += -no_warning_for_no_symbols
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15
    # Silent OpenGL warning for MacOS > 10.14
    DEFINES += GL_SILENCE_DEPRECATION
}

##### AMKGL #####
AMKGL_DIR = ../$$AMKGL_PATH
AMKGL_LIB_DIR = $$AMKGL_DIR/compiled
QGLVIEWER_DIR = ../$$QGL_VIEWER_PATH

DEFINES += QGLVIEWER_STATIC
DEFINES += MUPARSER_STATIC

INCLUDEPATH += $$EIGEN_INC_PATH
INCLUDEPATH += $$AMKGL_DIR
INCLUDEPATH += $$QGLVIEWER_DIR
INCLUDEPATH += ../AMKgl/defines_computree
INCLUDEPATH += ../3rdparty/muparser/include

LIBS += -L$$AMKGL_LIB_DIR
LIBS += -L$$QGLVIEWER_DIR
LIBS += -lmuparser -L../3rdparty/muparser

win32 {
    CONFIG(debug, debug|release) {
        LIBS += -lQGLViewerd
    } else {
        LIBS += -lQGLViewer
    }

    LIBS += -lopengl32 -lglu32
}


linux {
    LIBS += $$QGLVIEWER_DIR/libQGLViewer-qt5.a

    LIBS += -lGL -lGLU -lm
}

macx {
    LIBS += $$QGLVIEWER_DIR/libQGLViewer.a

    LIBS += -framework OpenGL

    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15
    # Silent OpenGL warning for MacOS > 10.14
    DEFINES += GL_SILENCE_DEPRECATION
}

!equals(PWD, $${OUT_PWD}) {
    error("Shadow build seems to be activated, please desactivated it !")
}
