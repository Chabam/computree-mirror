# Check dependencies for Computree plugins (paths, 3rd party libraries, compiler & options)
include(config/check_dependencies_plugins.pri)

TEMPLATE = subdirs
SUBDIRS = plugins/pluginbase

defineTest(addPlugin) {
    exists(plugins/$${1}//$${1}.pro) {
        SUBDIRS += plugins//$${1}
        message("Plugin $${1} will be used.")
    } else {
        warning("Plugin $${1} can't be found!")
    }
}

# Uncomment/add you own plugin here
# addPlugin(pluginartsfree)
# addPlugin(pluginonf)
# addPlugin(pluginonflsis)
# addPlugin(pluginlvox)
# addPlugin(plugingenerate)
# addPlugin(plugintoolkit)
