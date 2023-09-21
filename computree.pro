#========================#
#                        #
#   C O M P U T R E E    #
#                        #
#========================#


# store svn revision
include(config/generate_revision_h.pri)

# Check environment configuration
include(config/check_environment.pri)

# Main Computree project files
TEMPLATE = subdirs
SUBDIRS = base plugins

base.file    = base.pro    # Sub-project file for Computree core
plugins.file = plugins.pro # Sub-project file for Computree base plugin and additional plugins

# Plugins need base to be compiled before
plugins.depends = base

# Run "qmake; make; make install" on this file to compile and install everything
# Dependencies are treated directly in sub-project files.

include(config/installation_packages.pri)
