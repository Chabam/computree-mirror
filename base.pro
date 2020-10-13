# Just verify if there is problems with dependencies (paths, 3rd party libraries, compiler & options)
include(config/check_dependencies_base.pri)

TEMPLATE = subdirs
SUBDIRS += library AMKgl ComputreeGui

library.depends = 3rdparty/libQGLViewer
AMKgl.depends = 3rdparty/libQGLViewer
ComputreeGui.depends = AMKgl 3rdparty/muparser

# Uncomment the following lines to remove libQGLViewer dependency
# NO_QGLVIEWER = True
# library.depends =
# AMKgl.depends =
