project(ctlibopencv)

set(CTLIBOPENCV_HEADERS
    ctlibopencv_global.h
)

add_library(ctlibopencv INTERFACE
    ${CTLIBOPENCV_HEADERS}
)

target_include_directories(ctlibopencv INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    $<INSTALL_INTERFACE:include/ctlibs/ctlibopencv>
)

target_compile_definitions(ctlibopencv INTERFACE CTLIBOPENCV_LIBRARY)
