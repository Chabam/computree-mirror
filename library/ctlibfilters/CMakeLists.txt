project(ctlibfilters)

set(CTLIBFILTERS_HEADERS
    ctlibfilters_global.h
    ct_filter/abstract/ct_abstractfilter.h
    ct_filter/abstract/ct_abstractfilter_rasterfloat.h
    ct_filter/abstract/ct_abstractfilter_rasterrgbi.h
    ct_filter/abstract/ct_abstractfilter_xyz.h
)

set(CTLIBFILTERS_SOURCES
    ct_filter/abstract/ct_abstractfilter.cpp
    ct_filter/abstract/ct_abstractfilter_rasterfloat.cpp
    ct_filter/abstract/ct_abstractfilter_rasterrgbi.cpp
    ct_filter/abstract/ct_abstractfilter_xyz.cpp
)

set(CTLIBFILTERS_TRANSLATIONS
    languages/ctlibfilters_fr.ts
    languages/ctlibfilters_en.ts
)

add_library(ctlibfilters SHARED
    ${CTLIBFILTERS_HEADERS}
    ${CTLIBFILTERS_SOURCES}
    ${CTLIBFILTERS_TRANSLATIONS}
)

target_include_directories(ctlibfilters PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    $<INSTALL_INTERFACE:include/ctlibs/ctlibfilters>
)

target_compile_definitions(ctlibfilters PRIVATE CTLIBFILTERS_LIBRARY)
target_link_libraries(ctlibfilters
    ctlibcommon
    ctlibwidget
    ctlibstructureaddon
    ctliblog
    opencv_core
    Qt5::Core
    Qt5::Widgets
)

if (WIN32)
    install(FILES
        $<TARGET_RUNTIME_DLLS:ctlibfilters>
        DESTINATION bin
    )

    add_custom_command(TARGET ctlibfilters POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:ctlibfilters> $<TARGET_RUNTIME_DLLS:ctlibfilters>
        COMMAND_EXPAND_LISTS
    )
endif()

install(TARGETS
    ctlibfilters
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
