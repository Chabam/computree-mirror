#!/bin/bash

echo "# Check dependencies for Computree plugins (paths, 3rd party libraries, compiler, options)
include(config/check_dependencies_plugins.pri)

TEMPLATE = subdirs
SUBDIRS = plugins/pluginbase

# Comment/uncomment the plugins you want to use/compile. You can also add you own plugin here
#SUBDIRS += ../plugingenerate/plugingenerate
SUBDIRS += ../pluginmk/pluginmk
SUBDIRS += ../pluginonf/pluginonf
SUBDIRS += ../pluginsegma/pluginsegma
#SUBDIRS += ../pluginignlif/pluginignlif
SUBDIRS += ../plugintoolkit/plugintoolkit
" > plugins.pro
