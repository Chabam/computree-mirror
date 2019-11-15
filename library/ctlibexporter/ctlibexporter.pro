QT = core gui

contains ( QT_VERSION, "^5.*" ) {
    QT *= widgets
}

include(../library_shared.pri)

TARGET = ctlibexporter

DEFINES += CTLIBEXPORTER_LIBRARY

HEADERS += \
    ct_exporter/abstract/ct_abstractexporter.h \
#    ct_exporter/abstract/ct_abstractexporterattributesselection.h \
#    ct_exporter/abstract/ct_abstractexporterpointattributesselection.h \
    ctlibexporter_global.h \
    ct_exporter/tools/ct_exporterinmodelstructuremanager.h \
    ct_exporter/abstract/ct_abstractpiecebypieceexporter.h

SOURCES += \
    ct_exporter/abstract/ct_abstractexporter.cpp \
#    ct_exporter/abstract/ct_abstractexporterattributesselection.cpp \
#    ct_exporter/abstract/ct_abstractexporterpointattributesselection.cpp
    ct_exporter/tools/ct_exporterinmodelstructuremanager.cpp \
    ct_exporter/abstract/ct_abstractpiecebypieceexporter.cpp
