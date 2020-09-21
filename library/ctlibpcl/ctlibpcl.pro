MUST_USE_PCL = 1

include(../library_shared.pri)
include(../../include_all.pri)
include(../../plugin_shared.pri)

TARGET = ctlibpcl

DEFINES += CTLIBPCL_LIBRARY

HEADERS += \
    tools/ct_pcltools.h \
    ct_pcldefines.h

SOURCES += \ 
    tools/ct_pcltools.cpp
