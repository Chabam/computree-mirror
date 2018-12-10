QT = core gui

contains ( QT_VERSION, "^5.*" ) {
    QT *= widgets
}

include(../library_shared.pri)

TARGET = ctlibexporter

DEFINES += CTLIBEXPORTER_LIBRARY

HEADERS += \
    ct_exporter/abstract/private/ct_abstractexporter_p.h \
    ct_exporter/abstract/ct_abstractexporter.h \
#    ct_exporter/abstract/ct_abstractexporterattributesselection.h \
#    ct_exporter/abstract/ct_abstractexporterpointattributesselection.h \
    ctlibexporter_global.h

SOURCES += \
    ct_exporter/abstract/ct_abstractexporter.cpp \
#    ct_exporter/abstract/ct_abstractexporterattributesselection.cpp \
#    ct_exporter/abstract/ct_abstractexporterpointattributesselection.cpp
