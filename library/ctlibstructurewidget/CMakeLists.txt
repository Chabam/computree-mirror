project(ctlibstructurewidget)

set(CTLIBSTRUCTUREWIDGET_HEADERS
    ctlibstructurewidget_global.h
    ct_itemdrawable/view/ct_itemdrawablehierarchycollectionmodel.h
    ct_itemdrawable/view/ct_itemdrawablehierarchycollectionselectionmodel.h
    ct_itemdrawable/view/ct_itemdrawablehierarchycollectionwidget.h
    ct_itemdrawable/tools/collection/abstract/ct_abstractitemdrawablecollectionbuilder.h
    ct_itemdrawable/tools/collection/ct_itemdrawablecollectionbuildert.h
    ct_itemdrawable/tools/collection/ct_itemdrawablecollectionbuildert.hpp
    ct_itemdrawable/tools/collection/ct_itemdrawablecollectionhierarchyresult.h
    ct_itemdrawable/tools/collection/ct_itemdrawablecollectionhierarchystep.h
)
set(CTLIBSTRUCTUREWIDGET_SOURCES
    ct_itemdrawable/view/ct_itemdrawablehierarchycollectionmodel.cpp
    ct_itemdrawable/view/ct_itemdrawablehierarchycollectionselectionmodel.cpp
    ct_itemdrawable/view/ct_itemdrawablehierarchycollectionwidget.cpp
    ct_itemdrawable/tools/collection/abstract/ct_abstractitemdrawablecollectionbuilder.cpp
    ct_itemdrawable/tools/collection/ct_itemdrawablecollectionhierarchyresult.cpp
    ct_itemdrawable/tools/collection/ct_itemdrawablecollectionhierarchystep.cpp
)

set(CTLIBSTRUCTUREWIDGET_TRANSLATIONS
    languages/ctlibstructurewidget_fr.ts
    languages/ctlibstructurewidget_en.ts
)

add_library(ctlibstructurewidget SHARED
    ${CTLIBSTRUCTUREWIDGET_HEADERS}
    ${CTLIBSTRUCTUREWIDGET_SOURCES}
    ${CTLIBSTRUCTUREWIDGET_TRANSLATIONS}
)

target_include_directories(ctlibstructurewidget PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    $<INSTALL_INTERFACE:include/ctlibs/ctlibstructurewidget>
)

target_compile_definitions(ctlibstructurewidget PRIVATE CTLIBSTRUCTUREWIDGET_LIBRARY)
target_link_libraries(ctlibstructurewidget
    ctlibcommon
    ctlibstructure
    ctlibstep
    Qt5::Core
    Qt5::Widgets
)

set_target_properties(ctlibstructurewidget
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

if (WIN32)
    install(FILES
        $<TARGET_RUNTIME_DLLS:ctlibstructurewidget>
        DESTINATION bin
    )

    add_custom_command(TARGET ctlibstructurewidget POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:ctlibstructurewidget> ${CMAKE_BINARY_DIR}
        COMMAND_EXPAND_LISTS
    )
endif()

install(TARGETS
    ctlibstructurewidget
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
