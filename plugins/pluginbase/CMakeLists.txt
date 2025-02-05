project(plug_base)

set(PLUGBASE_HEADERS
    ${ctlibplugin_SOURCE_DIR}/pluginentryinterface.h
    pb_pluginentry.h
    pb_steppluginmanager.h
    step/pb_stepapplyfloatrasterfilters.h
    step/pb_stepapplypointfilters.h
    step/pb_stepapplyrgbirasterfilters.h
    step/pb_stepcomputepointmetrics.h
    step/pb_stepcomputerastermetrics.h
    step/pb_stepcreatepointfileindex.h
    step/pb_stepexportattributesasascii.h
    step/pb_stepexportattributesasraster.h
    step/pb_stepexportattributesasvector.h
    step/pb_stepgenericexporter.h
    step/pb_stepgenericloadfile.h
    step/pb_stepuseritemselection.h
    step/pb_stepexportattributesinloop.h
    step/pb_stepexportpointsbyxyarea.h
    step/pb_stepbeginloopthroughgroups02.h
    step/pb_steplooponfiles.h
    step/pb_steplooponfilesets.h
    step/pb_steploadfilebyname.h
    step/pb_stepusereadertoloadfiles.h
    step/pb_stepcreatereaderlist.h
    tools/pb_tools.h
)

set(PLUGBASE_SOURCES
    pb_pluginentry.cpp
    pb_steppluginmanager.cpp
    step/pb_stepapplyfloatrasterfilters.cpp
    step/pb_stepapplypointfilters.cpp
    step/pb_stepapplyrgbirasterfilters.cpp
    step/pb_stepcomputepointmetrics.cpp
    step/pb_stepcomputerastermetrics.cpp
    step/pb_stepcreatepointfileindex.cpp
    step/pb_stepexportattributesasascii.cpp
    step/pb_stepexportattributesasraster.cpp
    step/pb_stepexportattributesasvector.cpp
    step/pb_stepgenericexporter.cpp
    step/pb_stepgenericloadfile.cpp
    step/pb_stepuseritemselection.cpp
    step/pb_stepexportattributesinloop.cpp
    step/pb_stepexportpointsbyxyarea.cpp
    step/pb_stepbeginloopthroughgroups02.cpp
    step/pb_steplooponfiles.cpp
    step/pb_steplooponfilesets.cpp
    step/pb_steploadfilebyname.cpp
    step/pb_stepusereadertoloadfiles.cpp
    step/pb_stepcreatereaderlist.cpp
    tools/pb_tools.cpp
)

set(PLUGBASE_RESOURCES
    resource.qrc
)

set(PLUGBASE_TRANSLATIONS
    languages/pluginbase_fr.ts
    languages/pluginbase_en.ts
)

add_library(plug_base SHARED
    ${PLUGBASE_HEADERS}
    ${PLUGBASE_SOURCES}
    ${PLUGBASE_RESOURCES}
    ${PLUGBASE_TRANSLATIONS}
)

target_include_directories(plug_base PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(plug_base
    ctlibcommon
    ctlibplugin
    ctlibio
    ctlibpcl
    ctlibaction
    ctlibstdactions
    ctliblas
    ctliblaz
    ctlibstepaddon
    ctlibfilters
    ctlibmetrics
    Eigen3::Eigen
    Qt5::Concurrent
)

set_target_properties(plug_base
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)
