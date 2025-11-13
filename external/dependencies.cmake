# NOTE: using find_package for the moment, may be fallback to
# fetchContent in the future?

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

# Qt settings
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS REQUIRED
    Widgets
    Core
    Gui
    Xml
    OpenGL
    Concurrent
)

find_package(Eigen3 REQUIRED)
find_package(GDAL REQUIRED)
find_package(muparser REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED)
find_package(Qhull REQUIRED)
find_package(FLANN REQUIRED)
find_package(GSL REQUIRED)
find_package(LASzip REQUIRED)
find_package(OpenGL REQUIRED)

include(FetchContent)

# NOTE: The original authors of Computree did some changes to this fork...
set(NodeEditorPatch git apply ${CMAKE_CURRENT_LIST_DIR}/computree-custom.patch)
FetchContent_Declare(NodeEditor
  GIT_REPOSITORY https://github.com/Daguerreo/NodeEditor
  GIT_TAG 9bf1549e9bf1f4bf75c2a381402593c4a8b21a27
  PATCH_COMMAND ${NodeEditorPatch}
  UPDATE_DISCONNECTED TRUE
)

set(pclPatch ${CMAKE_CURRENT_LIST_DIR}/pcl-fix-boost.patch)
set(PCL_INSTALL_PATH "${CMAKE_BINARY_DIR}/pcl-install")
FetchContent_Declare(pcl
  GIT_REPOSITORY https://github.com/PointCloudLibrary/pcl
  GIT_TAG pcl-1.15.1
  GIT_SHALLOW TRUE
  UPDATE_DISCONNECTED TRUE
  PATCH_COMMAND git apply --check ${pclPatch} && git apply ${pclPatch}
  CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${PCL_INSTALL_PATH}
        -DWITH_VTK=OFF
        -DWITH_QT=NO
        -DBUILD_visualization=OFF
)

FetchContent_MakeAvailable(NodeEditor pcl)

find_package(nodes)
install(TARGETS nodes
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
