echo # Check dependencies for Computree plugins (paths, 3rd party libraries, compiler, options)> plugins.pro
echo include(config/check_dependencies_plugins.pri)>> plugins.pro
echo.>> plugins.pro
echo TEMPLATE = subdirs>> plugins.pro
echo SUBDIRS = plugins/pluginbase>> plugins.pro
echo.>> plugins.pro
echo # Comment/uncomment the plugins you want to use/compile. You can also add you own plugin here>> plugins.pro
echo #SUBDIRS += ../plugingenerate/plugingenerate>> plugins.pro
echo SUBDIRS += ../pluginmk/pluginmk>> plugins.pro
echo SUBDIRS += ../pluginonf/pluginonf>> plugins.pro
echo SUBDIRS += ../pluginsegma/pluginsegma>> plugins.pro
echo SUBDIRS += ../pluginignlif/pluginignlif>> plugins.pro
echo SUBDIRS += ../plugintoolkit/plugintoolkit>> plugins.pro
echo.>> plugins.pro