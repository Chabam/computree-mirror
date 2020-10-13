TEMPLATE      = subdirs
SUBDIRS       = ctlibmodels \
                ctlibmodelsextraviews \
                ctliblog \
                ctlibwidget \
                ctlibclouds \
                ctlibstructure \
                ctlibstep \
                ctlibmath \
                ctlibstructureaddon \
                ctlibstepaddon \
                ctlibfilters \
                ctlibmetrics \
                ctlibstructurewidget \
                ctlibexporter \
                ctlibreader \
                ctlibaction \
                ctlibstdactions \
                ctlibplugin \
                ctlibio \
                ctliblas \
                ctlibcore \
                ctlibpcl

ctlibmodelsextraviews.depends = ctlibmodels
ctlibstructure.depends = ctlibmodels
ctlibstep.depends = ctlibmodelsextraviews ctliblog ctlibwidget ctlibstructure
ctlibstructureaddon.depends = ctlibclouds ctliblog ctlibmath ctlibstructure
ctlibstepaddon.depends = ctlibstep ctlibstructureaddon
ctlibfilters.depends = ctliblog ctlibwidget ctlibstructureaddon
ctlibmetrics.depends = ctliblog ctlibwidget ctlibstructureaddon
ctlibstructurewidget.depends = ctlibstructure ctlibstep
ctlibexporter.depends = ctlibmodelsextraviews ctlibwidget ctlibstructureaddon
ctlibreader.depends = ctlibstructure ctlibwidget
ctlibstdactions.depends = ctliblog ctlibclouds ctlibmath ctlibstructure ctlibaction
ctlibplugin.depends = ctlibstep ctlibfilters ctlibmetrics ctlibexporter ctlibreader ctlibaction
ctlibio.depends = ctlibstructureaddon ctlibexporter ctlibreader
ctliblas.depends = ctlibstructureaddon ctlibfilters ctlibmetrics ctlibexporter ctlibreader
ctlibcore.depends = ctlibplugin
ctlibpcl.depends = ctlibclouds

win32-msvc2013 {
    message( "msvc 2013 detected" )
    QMAKE_CXXFLAGS += /FS
}
