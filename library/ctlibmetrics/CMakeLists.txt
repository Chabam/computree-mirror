project(ctlibmetrics)

set(CTLIBMETRICS_HEADERS
    ctlibmetrics_global.h
    ct_metric/abstract/ct_abstractmetric.h
    ct_metric/abstract/ct_abstractmetric_raster.h
    ct_metric/abstract/ct_abstractmetric_xyz.h
    ct_metric/points/ct_cloudmetrics.h
    tools/ct_valueandbool.h
    ct_metric/abstract/ct_abstractmetricgeneric.h
    ct_metric/tools/ct_metricoutmodelstructuremanager.h
)

set(CTLIBMETRICS_SOURCES
    ct_metric/abstract/ct_abstractmetric.cpp
    ct_metric/abstract/ct_abstractmetric_raster.cpp
    ct_metric/abstract/ct_abstractmetric_xyz.cpp
    ct_metric/points/ct_cloudmetrics.cpp
    ct_metric/abstract/ct_abstractmetricgeneric.cpp
)

set(CTLIBMETRICS_TRANSLATIONS
    languages/ctlibmetrics_fr.ts
    languages/ctlibmetrics_en.ts
)

add_library(ctlibmetrics SHARED
    ${CTLIBMETRICS_HEADERS}
    ${CTLIBMETRICS_SOURCES}
    ${CTLIBMETRICS_TRANSLATIONS}
)

target_include_directories(ctlibmetrics PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    $<INSTALL_INTERFACE:include/ctlibs/ctlibmetrics>
    # TODO: fix the actual include
    $<BUILD_INTERFACE:${computree_SOURCE_DIR}/library>
    $<INSTALL_INTERFACE:include/ctlibs/>
)

target_compile_definitions(ctlibmetrics PRIVATE CTLIBMETRICS_LIBRARY)
target_link_libraries(ctlibmetrics
    ctlibcommon
    ctlibwidget
    ctlibstructureaddon
    ctliblog
    Qt5::Core
    Qt5::Widgets
)

set_target_properties(ctlibmetrics
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

if (WIN32)
    install(FILES
        $<TARGET_RUNTIME_DLLS:ctlibmetrics>
        DESTINATION bin
    )

    add_custom_command(TARGET ctlibmetrics POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:ctlibmetrics> $<TARGET_RUNTIME_DLLS:ctlibmetrics>
        COMMAND_EXPAND_LISTS
    )
endif()

install(TARGETS
    ctlibmetrics
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
