file(GLOB COMPUTREE_PLUGINS "plugin*/")

foreach(PLUGIN ${COMPUTREE_PLUGINS})
    message(STATUS "Plugin found at ${PLUGIN}, adding to the build")
    add_subdirectory(${PLUGIN})
endforeach()
