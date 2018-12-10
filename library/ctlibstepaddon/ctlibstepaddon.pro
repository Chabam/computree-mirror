QT = core gui

contains ( QT_VERSION, "^5.*" ) {
    QT *= widgets
}

include(../library_shared.pri)

TARGET = ctlibstepaddon

DEFINES += CTLIBSTEPADDON_LIBRARY

HEADERS += \
    ctlibstepaddon_global.h \
    ct_step/abstract/ct_abstractsteploadfileinscene.h \
    ct_step/ct_stepbeginloop.h \
    ct_step/ct_stependloop.h

SOURCES += \
    ct_step/abstract/ct_abstractsteploadfileinscene.cpp \
    ct_step/ct_stepbeginloop.cpp \
    ct_step/ct_stependloop.cpp
