project(ctlibexporter)

set(CTLIBEXPORTER_HEADERS
    ct_exporter/abstract/ct_abstractexporter.h
    ctlibexporter_global.h
    ct_exporter/tools/ct_exporterinmodelstructuremanager.h
    ct_exporter/abstract/ct_abstractpiecebypieceexporter.h
)

set(CTLIBEXPORTER_SOURCES
    ct_exporter/abstract/ct_abstractexporter.cpp
    ct_exporter/tools/ct_exporterinmodelstructuremanager.cpp
    ct_exporter/abstract/ct_abstractpiecebypieceexporter.cpp
)

set(CTLIBEXPORTER_TRANSLATIONS
    languages/ctlibexporter_fr.ts
    languages/ctlibexporter_en.ts
)

add_library(ctlibexporter SHARED
    ${CTLIBEXPORTER_HEADERS}
    ${CTLIBEXPORTER_SOURCES}
    ${CTLIBEXPORTER_TRANSLATIONS}
)

target_include_directories(ctlibexporter PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    $<INSTALL_INTERFACE:include/ctlibs/ctlibexporter>
)

target_compile_definitions(ctlibexporter PRIVATE CTLIBEXPORTER_LIBRARY)
target_link_libraries(ctlibexporter
	ctlibcommon
    ctlibwidget
    ctlibstructureaddon
    ctlibmodelsextraviews
    Eigen3::Eigen
	Qt5::Core
	Qt5::Widgets
)

if (WIN32)
    install(FILES
        $<TARGET_RUNTIME_DLLS:ctlibexporter>
        DESTINATION bin
    )

    add_custom_command(TARGET ctlibexporter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:ctlibexporter> ${CMAKE_BINARY_DIR}
        COMMAND_EXPAND_LISTS
    )

    set_target_properties(ctlibexporter
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()

install(TARGETS
    ctlibexporter
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
