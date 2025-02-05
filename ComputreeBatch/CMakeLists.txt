project(ComputreeBatch)

set(COMPUTREEBATCH_HEADERS
    batchdialog.h
    interfacesbatch.h
    batch.h
    batchpluginmanager.h
    signalhandler.h
    StepResultTreeView/gstepmanager2.h
)

set(COMPUTREEBATCH_SOURCES
    main.cpp
    batchdialog.cpp
    batch.cpp
    batchpluginmanager.cpp
    signalhandler.cpp
    StepResultTreeView/gstepmanager2.cpp
)

set(COMPUTREEBATCH_FORMS
    batchdialog.ui
    StepResultTreeView/gstepmanager2.ui
)
set(COMPUTREEBATCH_RESOURCES
    systray.qrc
)

add_executable(ComputreeBatch
    ${COMPUTREEBATCH_HEADERS}
    ${COMPUTREEBATCH_SOURCES}
    ${COMPUTREEBATCH_FORMS}
    ${COMPUTREEBATCH_RESOURCES}
)

target_include_directories(ComputreeBatch PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ComputreeBatch
    ctlibcommon
    ctlibcore
    ctliblog
    Eigen3::Eigen
)
