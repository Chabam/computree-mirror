cmake_minimum_required(VERSION 3.26)

project(computree VERSION 0.0.0 LANGUAGES CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/external/dependencies.cmake)

add_subdirectory(AMKgl)
add_subdirectory(ComputreeGui)
add_subdirectory(ComputreeBatch)
add_subdirectory(plugins/pluginbase)
