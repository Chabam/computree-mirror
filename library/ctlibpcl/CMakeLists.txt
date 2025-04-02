project(ctlibpcl)

set(CTLIBPCL_HEADERS
    ctlibpcl_global.h
    readers/ct_reader_pcd.h
    tools/ct_pcltools.h
    ct_pcldefines.h
)

set(CTLIBPCL_SOURCES
    readers/ct_reader_pcd.cpp
    tools/ct_pcltools.cpp
)

set(CTLIBPCL_TRANSLATIONS
    languages/ctlibpcl_fr.ts
    languages/ctlibpcl_en.ts
)

add_library(ctlibpcl SHARED
    ${CTLIBPCL_HEADERS}
    ${CTLIBPCL_SOURCES}
    ${CTLIBPCL_TRANSLATIONS}
)

target_include_directories(ctlibpcl PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    $<INSTALL_INTERFACE:include/ctlibs/ctlibpcl>
)

if (WIN32)
    if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # NOTE: Disable exceptions, it's causing linking error on Windows
        set(CMAKE_CXX_FLAGS "/EHsc")
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS-fno-cxx-exceptions)
    endif()
endif()

target_compile_definitions(ctlibpcl PRIVATE CTLIBPCL_LIBRARY)
target_link_libraries(ctlibpcl
    ctlibcommon
    ctlibplugin
    ctliblog
    ctlibclouds
    ctlibstructureaddon
    ctlibreader
    Qt5::Core
    Qt5::Widgets
    pcl_common
    pcl_io
)

if (WIN32)
    install(FILES
        $<TARGET_RUNTIME_DLLS:ctlibpcl>
        DESTINATION bin
    )

    add_custom_command(TARGET ctlibpcl POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:ctlibpcl> ${CMAKE_BINARY_DIR}
        COMMAND_EXPAND_LISTS
    )

    set_target_properties(ctlibpcl
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()

install(TARGETS
    ctlibpcl
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
