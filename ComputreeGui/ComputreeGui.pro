CT_PREFIX = ..
CT_PREFIX_INSTALL = ../..
CT_PREFIX_LIB = ../library

COMPUTREE = ctlibcore
MUST_USE_EIGEN = 1
MUST_USE_MUPARSER = 1
MUST_USE_LIBQGLVIEWER = 1

# c++17
greaterThan(QT_MAJOR_VERSION, 4) {
    CONFIG += c++17
} else {
    QMAKE_CXXFLAGS += -std=c++17
}

include(../config/destdir.pri)
include(../config/library_include_ct.pri)
include(../config/include_dependencies.pri)

INCLUDEPATH += .
INCLUDEPATH += $$CT_PREFIX_LIB

CONFIG -= plugin
CONFIG += windows

TARGET = CompuTreeGui
TEMPLATE = app
RC_ICONS = Computree.ico
QT *= opengl xml concurrent widgets

DESTDIR = $${EXECUTABLE_DESTDIR}

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

RESOURCES += resource/icones.qrc

include(view/view.pri)
include(tools/tools.pri)
include(qtcolorpicker/qtcolorpicker.pri)

TR_EXCLUDE  += ./qtcolorpicker/*

# AMKgl
DEFINES *= AMKGL_NO_TODO_WARNINGS
INCLUDEPATH += ../AMKgl ../AMKgl/defines_computree
LIBS += -lAMKgl -L../AMKgl/compiled

# Opengl
win32 : LIBS += -lopengl32 -lglu32
linux : LIBS += -lGL -lGLU -lm
macx  : LIBS += -framework OpenGL

# Other specific compiler options
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

# For final deployment of binaries to make a standalone package (this avoid to do the usual 'make install')
win32 {
    # Usefull definitions of paths
    LIB = $$DESTDIR/libraries
    WIN_PATH = $$PWD/$$DESTDIR
    WIN_PATH ~= s,/,\\,g

    # Common part for deployment
    include(../config/default_path_opencv.pri)
    CONFIG(release, debug|release) : lib_opencv.files += $$OPENCV_BASE_PATH/x64/vc15/bin/opencv_world450.dll
    CONFIG(debug,   debug|release) : lib_opencv.files += $$OPENCV_BASE_PATH/x64/vc15/bin/opencv_world450d.dll
    lib_opencv.path = $$LIB/opencv

    include(../config/default_path_gdal.pri)
    lib_gdal.files += $$GDAL_BASE_PATH/bin/*.dll
    lib_gdal.path = $$LIB/gdal

    include(../config/default_path_pcl.pri)
    exists($$PCL_BASE_PATH) {
        CONFIG(release, debug|release) : lib_pcl.files += $$PCL_BASE_PATH/bin/*[^d].dll
        CONFIG(debug,   debug|release) : lib_pcl.files += $$PCL_BASE_PATH/bin/*d.dll
        lib_pcl.path = $$LIB/pcl
    }

    INSTALLS += lib_opencv lib_gdal lib_pcl

    # Specific part for deployment
    qt_deploy_options = --force --no-translations --angle --compiler-runtime --plugindir $$LIB/Qt --libdir $$LIB/Qt
    CONFIG(release, debug|release) : qt_deploy_options += --release
    CONFIG(debug,   debug|release) : qt_deploy_options += --debug

    qt_deploy_cmd1 = $$[QT_INSTALL_LIBS]/../bin/windeployqt.exe $$DESTDIR $$qt_deploy_options
    qt_deploy_cmd2 = copy "..\tools\win_setenv.cmd" $$WIN_PATH && cmd /C $$PWD/$$DESTDIR/win_setenv.cmd && del $$WIN_PATH\win_setenv.cmd
    qt_deploy_cmd3 = move $$WIN_PATH\libraries\Qt\opengl32sw.dll $$WIN_PATH\opengl32.dll
    qt_deploy_cmd4 = move $$WIN_PATH\libraries\Qt\vc_redist.x64.exe $$WIN_PATH

    qt_deploy.path = $$DESTDIR
    qt_deploy.extra = $$qt_deploy_cmd1 && $$qt_deploy_cmd2 && $$qt_deploy_cmd3 && $$qt_deploy_cmd4

    INSTALLS += qt_deploy
}
