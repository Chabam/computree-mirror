# Check dependencies for Computree base (paths, 3rd party libraries, compiler & options)
include(config/check_dependencies_base.pri)

TEMPLATE = subdirs
SUBDIRS += AMKgl library ComputreeGui ComputreeBatch

ComputreeGui.depends = AMKgl library
ComputreeBatch.depends = library
