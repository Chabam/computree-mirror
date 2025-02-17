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
