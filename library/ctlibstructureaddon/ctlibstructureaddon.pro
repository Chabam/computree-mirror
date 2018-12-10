QT = core gui

contains ( QT_VERSION, "^5.*" ) {
    QT *= widgets
}

include(../library_shared.pri)

TARGET = ctlibstructureaddon

DEFINES += CTLIBSTRUCTUREADDON_LIBRARY

HEADERS += \
    ctlibstructureaddon_global.h \
    ct_itemdrawable/abstract/ct_abstractitemdrawablewithoutpointcloud.h \
    ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h \
    ct_itemdrawable/ct_scene.h \
    ct_itemdrawable/tools/drawmanager/ct_standardabstractitemdrawablewithoutpointclouddrawmanager.h \
    ct_itemdrawable/tools/drawmanager/ct_standardabstractitemdrawablewithpointclouddrawmanager.h \
    ct_itemdrawable/ct_loopcounter.h \
    ct_itemdrawable/tools/ct_counter.h \
    ct_itemdrawable/abstract/ct_abstractmeshmodel.h \
    ct_itemdrawable/tools/drawmanager/ct_standardmeshmodeldrawmanager.h \
    ct_itemdrawable/abstract/ct_abstractattributes.h \
    ct_itemdrawable/abstract/ct_abstractedgeattributes.h \
    ct_itemdrawable/abstract/ct_abstractedgeattributesscalar.h \
    ct_itemdrawable/abstract/ct_abstractfaceattributes.h \
    ct_itemdrawable/abstract/ct_abstractfaceattributesscalar.h \
    ct_itemdrawable/abstract/ct_abstractpointattributesscalar.h \
    ct_itemdrawable/abstract/ct_abstractpointsattributes.h \
    ct_itemdrawable/tools/drawmanager/ct_standardabstractpointsattributesdrawmanager.h \
    ct_itemdrawable/ct_itemattributelist.h \
    ct_itemdrawable/ct_pointsattributescolor.h \
    ct_itemdrawable/ct_pointsattributesnormal.h \
    ct_itemdrawable/ct_pointsattributesscalarmaskt.h \
    ct_itemdrawable/ct_pointsattributesscalarmaskt.hpp \
    ct_itemdrawable/ct_pointsattributesscalartemplated.h \
    ct_itemdrawable/ct_pointsattributesscalartemplated.hpp \
    ct_itemdrawable/tools/drawmanager/ct_standardpointsattributescolordrawmanager.h \
    ct_itemdrawable/tools/drawmanager/ct_standardpointsattributesnormaldrawmanager.h \
    ct_itemdrawable/tools/drawmanager/ct_standardscannerdrawmanager.h \
    ct_itemdrawable/ct_scanner.h \
    ct_itemdrawable/ct_beam.h \
    ct_itemdrawable/tools/drawmanager/ct_standardbeamdrawmanager.h \
    ct_itemdrawable/tools/scanner/ct_shot.h \
    ct_itemdrawable/tools/scanner/ct_shootingpattern.h \
    ct_itemdrawable/tools/scanner/ct_thetaphishootingpattern.h

SOURCES += \ 
    ct_itemdrawable/abstract/ct_abstractitemdrawablewithoutpointcloud.cpp \
    ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.cpp \
    ct_itemdrawable/ct_scene.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardabstractitemdrawablewithoutpointclouddrawmanager.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardabstractitemdrawablewithpointclouddrawmanager.cpp \
    ct_itemdrawable/ct_loopcounter.cpp \
    ct_itemdrawable/tools/ct_counter.cpp \
    ct_itemdrawable/abstract/ct_abstractmeshmodel.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardmeshmodeldrawmanager.cpp \
    ct_itemdrawable/abstract/ct_abstractattributes.cpp \
    ct_itemdrawable/abstract/ct_abstractedgeattributes.cpp \
    ct_itemdrawable/abstract/ct_abstractedgeattributesscalar.cpp \
    ct_itemdrawable/abstract/ct_abstractfaceattributes.cpp \
    ct_itemdrawable/abstract/ct_abstractfaceattributesscalar.cpp \
    ct_itemdrawable/abstract/ct_abstractpointattributesscalar.cpp \
    ct_itemdrawable/abstract/ct_abstractpointsattributes.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardabstractpointsattributesdrawmanager.cpp \
    ct_itemdrawable/ct_itemattributelist.cpp \
    ct_itemdrawable/ct_pointsattributescolor.cpp \
    ct_itemdrawable/ct_pointsattributesnormal.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardpointsattributescolordrawmanager.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardpointsattributesnormaldrawmanager.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardscannerdrawmanager.cpp \
    ct_itemdrawable/ct_scanner.cpp \
    ct_itemdrawable/ct_beam.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardbeamdrawmanager.cpp \
    ct_itemdrawable/tools/scanner/ct_shot.cpp \
    ct_itemdrawable/tools/scanner/ct_thetaphishootingpattern.cpp
