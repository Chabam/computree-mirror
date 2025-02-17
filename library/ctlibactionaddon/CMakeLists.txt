project(ctlibactionaddon)

set(CTLIBACTIONADDON_HEADERS
    ctlibactionaddon_global.h
    actions/pb_actionselectitemdrawablegv.h
    views/actions/pb_actionselectitemdrawablegvoptions.h
    actions/tools/math.h
    actions/tools/polygonforpicking.h
    actions/tools/rectangleforpicking.h
)

set(CTLIBACTIONADDON_SOURCES
    actions/pb_actionselectitemdrawablegv.cpp
    views/actions/pb_actionselectitemdrawablegvoptions.cpp
    actions/tools/math.cpp
    actions/tools/polygonforpicking.cpp
    actions/tools/rectangleforpicking.cpp
)

set(CTLIBACTIONADDON_FORMS
    views/actions/pb_actionselectitemdrawablegvoptions.ui
)

add_library(ctlibactionaddon SHARED
    ${CTLIBACTIONADDON_HEADERS}
    ${CTLIBACTIONADDON_SOURCES}
    ${CTLIBACTIONADDON_FORMS}
)

target_include_directories(ctlibactionaddon PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    $<INSTALL_INTERFACE:include/ctlibs/ctlibactionaddon>
)

target_compile_definitions(ctlibactionaddon PRIVATE CTLIBACTIONADDON_LIBRARY)
target_link_libraries(ctlibactionaddon
    ctlibcommon
    ctlibaction
    ctlibstructure
    ctlibclouds
    ctliblog
    Qt5::Core
    Qt5::Widgets
)
