QT = core gui

contains ( QT_VERSION, "^5.*" ) {
    QT *= widgets
}

MUST_USE_LASZIP = 1
include(../../config/library_shared.pri)
include(../../config/include_dependencies.pri)

QT += concurrent
QT += xml

TARGET = ctliblaz

DEFINES += CTLIBLAZ_LIBRARY

HEADERS += \
    ctliblaz_global.h \
    exporters/ct_exporter_laz.h \
    exporters/private/ct_lazpiecebypieceprivateexporter.h \
    readers/ct_laszip_reader.h \
    readers/ct_reader_laz.h \
    readers/headers/ct_lazheader.h

SOURCES += \
    exporters/ct_exporter_laz.cpp \
    exporters/private/ct_lazpiecebypieceprivateexporter.cpp \
    readers/ct_laszip_reader.cpp \
    readers/ct_reader_laz.cpp \
    readers/headers/ct_lazheader.cpp


TRANSLATIONS += languages/ctliblaz_fr.ts \
                languages/ctliblaz_en.ts

# c++14 for LASZIP
CONFIG -= c++17
CONFIG += c++14
