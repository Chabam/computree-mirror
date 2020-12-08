# Check dependencies for Computree plugins (paths, 3rd party libraries, compiler & options)
include(config/check_dependencies_plugins.pri)

TEMPLATE = subdirs
SUBDIRS = plugins/pluginbase

# defineReplace(addPlugin) {
#     exists(plugins/$${1}/$${1}.pro) {
#         message("Plugin $${1} will be used.")
#         return(plugins/$${1})
#     } else {
#         warning("Plugin $${1} can't be found!")
#         return()
#     }
# }

# Uncomment/add you own plugin here
# SUBDIRS += $$addPlugin(pluginartsfree)
# SUBDIRS += $$addPlugin(pluginonf)
# SUBDIRS += $$addPlugin(pluginonflsis)
# SUBDIRS += $$addPlugin(pluginlvox)
# SUBDIRS += $$addPlugin(plugingenerate)
# SUBDIRS += $$addPlugin(plugintoolkit)
# SUBDIRS += $$addPlugin(pluginmk)
# SUBDIRS += $$addPlugin(pluginsegma)

# Uncomment/add you own plugin here
SUBDIRS += ../pluginmk/pluginmk
SUBDIRS += ../pluginonf/pluginonf
SUBDIRS += ../pluginsegma/pluginsegma
SUBDIRS += ../plugintoolkit/plugintoolkit
