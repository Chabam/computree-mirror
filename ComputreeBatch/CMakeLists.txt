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

set_target_properties(ComputreeBatch
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

if (WIN32)
    install(FILES
        $<TARGET_RUNTIME_DLLS:ComputreeBatch>
        DESTINATION bin
    )

    add_custom_command(TARGET ComputreeBatch POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:ComputreeBatch> ${CMAKE_BINARY_DIR}
        COMMAND_EXPAND_LISTS
    )

    find_file(WINDEPLOYQT_EXE
        "windeployqt.exe"
        PATHS "$ENV{QT_INSTALL_DIR}/bin" "C:/Qt/5.15.2/msvc2019_64/bin"
        REQUIRED
    )

    add_custom_command(TARGET ComputreeBatch POST_BUILD
        COMMAND ${WINDEPLOYQT_EXE} "ComputreeBatch${CMAKE_EXECUTABLE_SUFFIX}"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
endif()

install(TARGETS
    ComputreeBatch
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
