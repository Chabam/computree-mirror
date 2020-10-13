# Just verify if there is problems with dependencies (paths, 3rd party libraries, compiler & options)
include(config/check_dependencies_plugins.pri)

TEMPLATE = subdirs
SUBDIRS = plugins/pluginbase

defineReplace(addPlugin) {
    exists(plugins/$${1}//$${1}.pro) {
        SUBDIRS += plugins//$${1}
        plugins/$${1}.depends = plugins/pluginbase
        message("Plugin $${1} will be used.")
    } else {
        warning("Plugin $${1} can't be found!")
    }
}

var = addPlugin(pluginartsfree)
var = addPlugin(pluginonf)
var = addPlugin(pluginonflsis)
var = addPlugin(pluginlvox)
var = addPlugin(plugingenerate)
var = addPlugin(plugintoolkit)
