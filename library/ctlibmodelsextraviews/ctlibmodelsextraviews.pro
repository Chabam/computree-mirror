QT = core gui widgets

include(../../config/library_shared.pri)
include(../../3rdparty/NodeEditor/NodeEditor.pri)

TARGET = ctlibmodelsextraviews

DEFINES += CTLIBMODELSEXTRAVIEWS_LIBRARY

HEADERS += \
    ctlibmodelsextraviews_global.h \
    ctg_modelslinkconfigurationflowview.h \
    tools/ct_modelflowdata.h \
    tools/ct_modelflowdatamodel.h \
    tools/ct_indatatypetooutdatatypeconverter.h \
    tools/ct_helpgraphicsitem.h

SOURCES += \
    ctg_modelslinkconfigurationflowview.cpp \
    tools/ct_modelflowdata.cpp \
    tools/ct_modelflowdatamodel.cpp \
    tools/ct_indatatypetooutdatatypeconverter.cpp \
    tools/ct_helpgraphicsitem.cpp

FORMS += ctg_modelslinkconfigurationflowview.ui


TRANSLATIONS += languages/ctlibmodelsextraviews_fr.ts \
                languages/ctlibmodelsextraviews_en.ts

LUPDATE = $$system($$[QT_INSTALL_BINS]/lupdate -ts $$TRANSLATIONS)

CONFIG += lrelease
QMAKE_LRELEASE_FLAGS += -removeidentical
LRELEASE_DIR = $$DESTDIR/../../languages
