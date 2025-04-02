project(ctlibstepaddon)

set(CTLIBSTEPADDON_HEADERS
    ctlibstepaddon_global.h
    ct_step/abstract/ct_abstractsteploadfileinscene.h
    ct_step/ct_stepbeginloop.h
    ct_step/ct_stependloop.h
    ct_tools/ct_monitoredqthread.h
)

set(CTLIBSTEPADDON_SOURCES
    ct_step/abstract/ct_abstractsteploadfileinscene.cpp
    ct_step/ct_stepbeginloop.cpp
    ct_step/ct_stependloop.cpp
    ct_tools/ct_monitoredqthread.cpp
)

set(CTLIBSTEPADDON_TRANSLATIONS
    languages/ctlibstepaddon_fr.ts
    languages/ctlibstepaddon_en.ts
)

add_library(ctlibstepaddon SHARED
    ${CTLIBSTEPADDON_HEADERS}
    ${CTLIBSTEPADDON_SOURCES}
    ${CTLIBSTEPADDON_FORMS}
    ${CTLIBSTEPADDON_TRANSLATIONS}
)

target_include_directories(ctlibstepaddon PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    $<INSTALL_INTERFACE:include/ctlibs/ctlibstepaddon>
)

target_compile_definitions(ctlibstepaddon PRIVATE CTLIBSTEPADDON_LIBRARY)
target_link_libraries(ctlibstepaddon
    ctlibcommon
    ctlibstep
    ctlibstructureaddon
    Qt5::Core
    Qt5::Widgets
)

if (WIN32)
    install(FILES
        $<TARGET_RUNTIME_DLLS:ctlibstepaddon>
        DESTINATION bin
    )

    add_custom_command(TARGET ctlibstepaddon POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:ctlibstepaddon> ${CMAKE_BINARY_DIR}
        COMMAND_EXPAND_LISTS
    )

    set_target_properties(ctlibstepaddon
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()

install(TARGETS
    ctlibstepaddon
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
