project(ctlibmath)

set(CTLIBMATH_HEADERS
    ct_math/ct_math2dlines.h
    ct_math/ct_mathboundingshape.h
    ct_math/ct_mathfittedline2d.h
    ct_math/ct_mathpoint.h
    ct_math/ct_mathstatistics.h
    ctlibmath_global.h
)

set(CTLIBMATH_SOURCES
    ct_math/ct_math2dlines.cpp
    ct_math/ct_mathboundingshape.cpp
    ct_math/ct_mathfittedline2d.cpp
    ct_math/ct_mathpoint.cpp
    ct_math/ct_mathstatistics.cpp
)

set(CTLIBMATH_TRANSLATIONS
    languages/ctlibmath_fr.ts
    languages/ctlibmath_en.ts
)

add_library(ctlibmath SHARED
    ${CTLIBMATH_HEADERS}
    ${CTLIBMATH_SOURCES}
    ${CTLIBMATH_TRANSLATIONS}
)

target_include_directories(ctlibmath PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    $<INSTALL_INTERFACE:include/ctlibs/ctlibmath>
)

target_compile_definitions(ctlibmath PRIVATE CTLIBMATH_LIBRARY)
target_link_libraries(ctlibmath
     ctlibcommon
     Eigen3::Eigen
     Qt5::Core
     Qt5::Widgets
)

if (WIN32)
    install(FILES
        $<TARGET_RUNTIME_DLLS:ctlibmath>
        DESTINATION bin
    )

    add_custom_command(TARGET ctlibmath POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:ctlibmath> ${CMAKE_BINARY_DIR}
        COMMAND_EXPAND_LISTS
    )

    set_target_properties(ctlibmath
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()

install(TARGETS
    ctlibmath
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
