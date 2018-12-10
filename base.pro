include(all_check_dependencies.pri) # Just verify if there is problems with dependencies, do nothing else

TEMPLATE      = subdirs
SUBDIRS       = AMKgl/libQGLViewer-2.7.1 library AMKgl ComputreeCore ComputreeGui pluginbase
CONFIG       += console ordered

win32-msvc2013 {
    message( "msvc 2013 detected" )
    QMAKE_CXXFLAGS += /FS
}
