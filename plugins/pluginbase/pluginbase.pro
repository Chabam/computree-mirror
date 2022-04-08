COMPUTREE += ctlibplugin ctlibio ctlibaction ctlibstdactions ctliblas ctliblaz ctlibstepaddon ctlibfilters ctlibmetrics

CHECK_CAN_USE_GDAL = 1
MUST_USE_LASZIP = 1
MUST_USE_OPENCV = 1

INCLUDED_PLUGIN = yes

include(../../config/plugin_shared.pri)

TARGET = plug_base

QT += concurrent

HEADERS += $$CT_PREFIX_LIB/ctlibplugin/pluginentryinterface.h\
    pb_pluginentry.h \
    pb_steppluginmanager.h \
    step/pb_stepapplypointfilters.h \
    step/pb_stepcomputepointmetrics.h \
    step/pb_stepcomputerastermetrics.h \
    step/pb_stepexportitemlist.h \
    step/pb_stepgenericexporter.h \
    step/pb_stepgenericloadfile.h \
    step/pb_stepuseritemselection.h \
    step/pb_stepexportattributesinloop.h \
    step/pb_stepexportpointsbyxyarea.h \
    step/pb_stepbeginloopthroughgroups02.h \
    step/pb_steplooponfiles.h \
    step/pb_steplooponfilesets.h \
    step/pb_steploadfilebyname.h \
    step/pb_stepusereadertoloadfiles.h \
    step/pb_stepcreatereaderlist.h \
    step/pb_testbug.h

SOURCES += \
    pb_pluginentry.cpp \
    pb_steppluginmanager.cpp \
    step/pb_stepapplypointfilters.cpp \
    step/pb_stepcomputepointmetrics.cpp \
    step/pb_stepcomputerastermetrics.cpp \
    step/pb_stepexportitemlist.cpp \
    step/pb_stepgenericexporter.cpp \
    step/pb_stepgenericloadfile.cpp \
    step/pb_stepuseritemselection.cpp \
    step/pb_stepexportattributesinloop.cpp \
    step/pb_stepexportpointsbyxyarea.cpp \
    step/pb_stepbeginloopthroughgroups02.cpp \
    step/pb_steplooponfiles.cpp \
    step/pb_steplooponfilesets.cpp \
    step/pb_steploadfilebyname.cpp \
    step/pb_stepusereadertoloadfiles.cpp \
    step/pb_stepcreatereaderlist.cpp \
    step/pb_testbug.cpp

RESOURCES += resource.qrc

TRANSLATIONS += languages/pluginbase_fr.ts \
                languages/pluginbase_en.ts

LUPDATE = $$system($$[QT_INSTALL_BINS]/lupdate -ts $$TRANSLATIONS)

CONFIG += lrelease
QMAKE_LRELEASE_FLAGS += -removeidentical
LRELEASE_DIR = $$DESTDIR/../languages
