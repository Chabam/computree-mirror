TEMPLATE      = subdirs
SUBDIRS       = ctlibmodels ctliblog ctlibwidget ctlibclouds ctlibstructure ctlibstep ctlibmath ctlibstructureaddon ctlibstepaddon ctlibfilters ctlibstructurewidget ctlibexporter ctlibreader ctlibaction ctlibplugin ctlibio ctliblas
CONFIG        += ordered

win32-msvc2013 {
    message( "msvc 2013 detected" )
    QMAKE_CXXFLAGS += /FS
}
