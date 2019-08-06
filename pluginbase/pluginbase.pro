COMPUTREE += ctlibplugin ctlibio ctliblas #ctlibmetrics ctlibfilters ctlibstdactions

CHECK_CAN_USE_GDAL = 1
MUST_USE_OPENCV = 1
DONT_CHECK_CT_LIBRARY = 1

include(../plugin_shared.pri)

TARGET = plug_base

greaterThan(QT_MAJOR_VERSION, 4): QT += concurrent

HEADERS += $$CT_LIB_PREFIX/ctlibplugin/pluginentryinterface.h\
    pb_pluginentry.h \
    pb_steppluginmanager.h \
#    actions/pb_actionselectitemdrawablegv.h \
#    actions/pb_actionshowitemdatagv.h \
#    step/pb_stepapplypointfilters.h \
#    step/pb_stepcomputepointmetrics.h \
#    step/pb_stepcomputerastermetrics.h \
#    step/pb_stepexportitemlist.h \
    step/pb_stepgenericexporter.h \
    step/pb_stepgenericloadfile.h \
#    step/pb_stepuseritemselection.h \
#    views/actions/pb_actionselectitemdrawablegvoptions.h \
#    views/exporters/csv/pbg_csvconfigurationdialog.h \
#    views/exporters/csv/pbg_csvdatareflistwidget.h \
#    views/exporters/csv/pbg_csvpreviewwidget.h \
#    actions/pb_actionpickitemsinlist.h \
#    views/actions/pb_actionpickitemsinlistoptions.h \
#    step/pb_stepcreatereaderlist.h \
#    step/pb_stepusereadertoloadfiles.h \
#    step/pb_stepexportattributesinloop.h \
#    step/pb_stepexportpointsbyxyarea.h \
#    step/pb_stepbeginloopthroughgroups02.h \
#    actions/tools/polygonforpicking.h \
#    actions/tools/rectangleforpicking.h \
#    actions/tools/math.h \
#    step/pb_steplooponfiles.h \
#    step/pb_steplooponfilesets.h \
#    step/pb_steploadfilebyname.h \
#    tools/pb_configurableelementtools.h \
#    tools/pb_readerstools.h \
    tools/pb_exportertools.h \
    step/pb_stepcreatereaderlist.h \
#    step/pb_stepusereadertoloadfiles.h
    tools/pb_readerstools.h

SOURCES += \
    pb_pluginentry.cpp \
    pb_steppluginmanager.cpp \
#    actions/pb_actionselectitemdrawablegv.cpp \
#    actions/pb_actionshowitemdatagv.cpp \
#    step/pb_stepapplypointfilters.cpp \
#    step/pb_stepcomputepointmetrics.cpp \
#    step/pb_stepcomputerastermetrics.cpp \
#    step/pb_stepexportitemlist.cpp \
    step/pb_stepgenericexporter.cpp \
    step/pb_stepgenericloadfile.cpp \
#    step/pb_stepuseritemselection.cpp \
#    views/actions/pb_actionselectitemdrawablegvoptions.cpp \
#    views/exporters/csv/pbg_csvconfigurationdialog.cpp \
#    views/exporters/csv/pbg_csvdatareflistwidget.cpp \
#    views/exporters/csv/pbg_csvpreviewwidget.cpp \
#    actions/pb_actionpickitemsinlist.cpp \
#    views/actions/pb_actionpickitemsinlistoptions.cpp \
#    step/pb_stepcreatereaderlist.cpp \
#    step/pb_stepusereadertoloadfiles.cpp \
#    step/pb_stepexportattributesinloop.cpp \
#    step/pb_stepexportpointsbyxyarea.cpp \
#    step/pb_stepbeginloopthroughgroups02.cpp \
#    actions/tools/polygonforpicking.cpp \
#    actions/tools/rectangleforpicking.cpp \
#    actions/tools/math.cpp \
#    step/pb_steplooponfiles.cpp \
#    step/pb_steplooponfilesets.cpp \
#    step/pb_steploadfilebyname.cpp
    step/pb_stepcreatereaderlist.cpp \
#    step/pb_stepusereadertoloadfiles.cpp

RESOURCES += resource.qrc

#FORMS += \
#    views/actions/pb_actionselectitemdrawablegvoptions.ui \
#    views/exporters/csv/pbg_csvconfigurationdialog.ui \
#    views/exporters/csv/pbg_csvdatareflistwidget.ui \
#    views/exporters/csv/pbg_csvpreviewwidget.ui \
#    views/actions/pb_actionpickitemsinlistoptions.ui

TRANSLATIONS += languages/pluginbase_fr.ts \
                languages/pluginbase_en.ts
