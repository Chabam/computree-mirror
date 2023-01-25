QT = core gui

contains ( QT_VERSION, "^5.*" ) {
    QT *= widgets
}

MUST_USE_LASZIP = 1
include(../../config/library_shared.pri)
include(../../config/include_dependencies.pri)

TARGET = ctlibbuffer

DEFINES += CTLIBBUFFER_LIBRARY

HEADERS += \
    ctlibbuffer_global.h \
    exporter/ct_exporter_buffer.h \
    reader/ct_reader_buffer.h \
    reader/headers/ct_ctiheader.h

SOURCES += \
    exporter/ct_exporter_buffer.cpp \
    reader/ct_reader_buffer.cpp

TRANSLATIONS += languages/ctlibbuffer_fr.ts \
                languages/ctlibbuffer_en.ts
