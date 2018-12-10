QT = core gui

contains ( QT_VERSION, "^5.*" ) {
    QT *= widgets
}

include(../library_shared.pri)

TARGET = ctlibreader

DEFINES += CTLIBREADER_LIBRARY

HEADERS += \
    ctlibreader_global.h \
    ct_reader/abstract/ct_abstractreader.h \
    ct_itemdrawable/ct_fileheader.h \
    ct_reader/tools/ct_readeroutmodelstructuremanager.h \
    ct_itemdrawable/ct_readeritem.h \
    ct_reader/extensions/ct_readerpointsfilteringextension.h

SOURCES += \
    ct_reader/abstract/ct_abstractreader.cpp \
    ct_itemdrawable/ct_fileheader.cpp \
    ct_reader/tools/ct_readeroutmodelstructuremanager.cpp \
    ct_itemdrawable/ct_readeritem.cpp \
    ct_reader/extensions/ct_readerpointsfilteringextension.cpp
