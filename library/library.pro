TEMPLATE      = subdirs
SUBDIRS       = ctlibmodels ctlibmodelsextraviews ctliblog ctlibwidget ctlibclouds ctlibstructure ctlibstep ctlibmath ctlibstructureaddon ctlibstepaddon ctlibfilters ctlibmetrics ctlibstructurewidget ctlibexporter ctlibreader ctlibaction ctlibstdactions ctlibplugin ctlibio ctliblas ctlibcore

CONFIG        += ordered

win32-msvc2013 {
    message( "msvc 2013 detected" )
    QMAKE_CXXFLAGS += /FS
}
