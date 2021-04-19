# Check dependencies for Computree base (paths, 3rd party libraries, compiler & options)
include(config/check_dependencies_base.pri)

TEMPLATE = subdirs
SUBDIRS += AMKgl library ComputreeGui

library.depends = 3rdparty/libQGLViewer
AMKgl.depends = 3rdparty/libQGLViewer
ComputreeGui.depends = AMKgl library 3rdparty/muparser
