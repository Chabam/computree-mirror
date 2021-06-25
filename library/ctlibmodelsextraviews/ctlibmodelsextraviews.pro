QT = core gui widgets

include(../../config/library_shared.pri)

TARGET = ctlibmodelsextraviews

DEFINES += CTLIBMODELSEXTRAVIEWS_LIBRARY
DEFINES += NODE_EDITOR_STATIC

HEADERS += \
    ctlibmodelsextraviews_global.h \
    ctg_modelslinkconfigurationflowview.h \
    tools/ct_modelflowdata.h \
    tools/ct_modelflowdatamodel.h \
    tools/ct_indatatypetooutdatatypeconverter.h \
    tools/ct_helpgraphicsitem.h
HEADERS += $$files(nodeeditor/include/nodes/*, true)
HEADERS += $$files(nodeeditor/src/*.hpp, true)

SOURCES += \
    ctg_modelslinkconfigurationflowview.cpp \
    tools/ct_modelflowdata.cpp \
    tools/ct_modelflowdatamodel.cpp \
    tools/ct_indatatypetooutdatatypeconverter.cpp \
    tools/ct_helpgraphicsitem.cpp
SOURCES += $$files(nodeeditor/src/*.cpp, true)

INCLUDEPATH += nodeeditor/include
INCLUDEPATH += nodeeditor/include/nodes/
INCLUDEPATH += nodeeditor/include/nodes/internal/
INCLUDEPATH += nodeeditor/src/

RESOURCES += nodeeditor/resources/DefaultStyle.json \
             nodeeditor/resources/nodeeditor.qrc

FORMS += ctg_modelslinkconfigurationflowview.ui
