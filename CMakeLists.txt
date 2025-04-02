cmake_minimum_required(VERSION 3.26)

project(computree VERSION 0.0.0 LANGUAGES CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/external/dependencies.cmake)

execute_process(
    COMMAND git rev-parse HEAD
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE COMPUTREE_REVISION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

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
set(CPACK_PACAKGE_VERSION_MAJOR "6")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "${COMPUTREE_REVISION}")

include(CPack)
