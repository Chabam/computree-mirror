MUST_USE_PCL = 1
MUST_USE_BOOST = 1
MUST_USE_FLANN = 1
include(../../config/library_shared.pri)
include(../../config/include_dependencies.pri)

TARGET = ctlibpcl

DEFINES += CTLIBPCL_LIBRARY

HEADERS += \
    tools/ct_pcltools.h \
    ct_pcldefines.h

SOURCES += \ 
    tools/ct_pcltools.cpp


TRANSLATIONS += languages/ctlibpcl_fr.ts \
                languages/ctlibpcl_en.ts

LUPDATE = $$system($$[QT_INSTALL_BINS]/lupdate -ts $$TRANSLATIONS)

CONFIG += lrelease
QMAKE_LRELEASE_FLAGS += -removeidentical
LRELEASE_DIR = $$DESTDIR/../../languages


# c++14 for PCL (for the moment, because of FLANN issues with C++17)
CONFIG -= c++17
CONFIG += c++14

macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 11.0
}
