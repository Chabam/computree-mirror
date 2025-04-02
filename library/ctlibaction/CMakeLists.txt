project(ctlibaction)

set(CTLIBACTION_HEADERS
    ctlibaction_global.h
    ct_actions/abstract/ct_abstractaction.h
    ct_actions/actionoptionsinterface.h
    ct_actions/view/abstract/ct_gabstractactionoptions.h
    ct_actions/abstract/ct_abstractactionforgraphicsview.h
    ct_actions/abstract/ct_abstractactionfortreeview.h
    ct_actions/view/abstract/ct_gabstractactionforgraphicsviewoptions.h
)

set(CTLIBACTION_SOURCES
    ct_actions/abstract/ct_abstractaction.cpp
    ct_actions/view/abstract/ct_gabstractactionoptions.cpp
    ct_actions/abstract/ct_abstractactionforgraphicsview.cpp
    ct_actions/abstract/ct_abstractactionfortreeview.cpp
    ct_actions/view/abstract/ct_gabstractactionforgraphicsviewoptions.cpp
)

set(CTLIBACTION_TRANSLATIONS
    languages/ctlibaction_fr.ts
    languages/ctlibaction_en.ts
)

add_library(ctlibaction SHARED
    ${CTLIBACTION_HEADERS}
    ${CTLIBACTION_SOURCES}
    ${CTLIBACTION_TRANSLATIONS}
)

target_include_directories(ctlibaction PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    $<INSTALL_INTERFACE:include/ctlibs/ctlibaction>
)

target_compile_definitions(ctlibaction PRIVATE CTLIBACTION_LIBRARY)
target_link_libraries(ctlibaction
    Eigen3::Eigen
    ctlibcommon
    Qt5::Core
    Qt5::Widgets
)

set_target_properties(ctlibaction
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

if (WIN32)
    install(FILES
        $<TARGET_RUNTIME_DLLS:ctlibaction>
        DESTINATION bin
    )

    add_custom_command(TARGET ctlibaction POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:ctlibaction> $<TARGET_RUNTIME_DLLS:ctlibaction>
        COMMAND_EXPAND_LISTS
    )
endif()

install(TARGETS
    ctlibaction
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
