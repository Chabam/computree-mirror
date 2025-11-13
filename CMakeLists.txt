cmake_minimum_required(VERSION 3.26)

execute_process(
    COMMAND git rev-parse HEAD
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE COMPUTREE_REVISION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND git branch --show-current
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE COMPUTREE_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND git rev-list --count HEAD
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE COMPUTREE_REVISION_COUNT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

project(computree VERSION 6.0.${COMPUTREE_REVISION_COUNT} LANGUAGES CXX C)

include(${CMAKE_CURRENT_SOURCE_DIR}/external/dependencies.cmake)

add_subdirectory(library)
add_subdirectory(AMKgl)
add_subdirectory(ComputreeGui)
add_subdirectory(ComputreeBatch)
add_subdirectory(plugins)

include(InstallRequiredSystemLibraries)
include(GNUInstallDirs)

set(CMAKE_INSTALL_RPATH $ORIGIN $ORIGIN/${CMAKE_INSTALL_BINDIR})

set(CMAKE_INSTALL_LIBDIR lib)

if (WIN32)
    set(COMPUTREE_EXECS ComputreeGui ComputreeBatch)

    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/BundleQTDlls.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/BundleQTDlls.cmake
        @ONLY
    )

    install(SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/BundleQTDlls.cmake)
endif()

set(CPACK_PACKAGE_NAME "Computree")
set(CPACK_PACKAGE_VENDOR "Computree")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Computree a forestry computing platform")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

include(CPack)
