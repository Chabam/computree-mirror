# Optional use of Point Cloud Library (PCL)
MUST_USE_PCL = 1

# Check dependencies for Computree base (paths, 3rd party libraries, compiler & options)
include(config/check_dependencies_base.pri)

TEMPLATE = subdirs
SUBDIRS += AMKgl library ComputreeGui

library.depends = 3rdparty/libQGLViewer
AMKgl.depends = 3rdparty/libQGLViewer
ComputreeGui.depends = AMKgl 3rdparty/muparser
