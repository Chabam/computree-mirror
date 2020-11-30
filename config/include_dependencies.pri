# We really setup dependancies, not only check
CHECK_LIBS_ONLY = false

isEmpty(LIB_PATH) : LIB_PATH = # empty

# Check if we can or if we must use the dependency
!isEmpty(CHECK_CAN_USE_EIGEN)|!isEmpty(MUST_USE_EIGEN) {
    include(default_path_eigen.pri)
    exists(user_path_eigen.pri) : include(user_path_eigen.pri)
    include(include_necessary_eigen.pri)
    message("DEPENDENCY SETUP - EIGEN        will be used in $$TARGET")
}
!isEmpty(CHECK_CAN_USE_MUPARSER)|!isEmpty(MUST_USE_MUPARSER) {
    include(default_path_muparser.pri)
    exists(user_path_muparser.pri) : include(user_path_muparser.pri)
    include(include_necessary_muparser.pri)
    message("DEPENDENCY SETUP - MUPARSER     will be used in $$TARGET")
}
!isEmpty(CHECK_CAN_USE_LIBQGLVIEWER)|!isEmpty(MUST_USE_LIBQGLVIEWER) {
    include(default_path_libqglviewer.pri)
    exists(user_path_libqglviewer.pri) : include(user_path_libqglviewer.pri)
    include(include_necessary_libqglviewer.pri)
    message("DEPENDENCY SETUP - LIBQGLVIEWER will be used in $$TARGET")
}
!isEmpty(CHECK_CAN_USE_GDAL)|!isEmpty(MUST_USE_GDAL)|contains( COMPUTREE, ctlibgdal) {
    include(default_path_gdal.pri)
    exists(user_path_gdal.pri) : include(user_path_gdal.pri)
    include(include_necessary_gdal.pri)
    message("DEPENDENCY SETUP - GDAL         will be used in $$TARGET")
}
!isEmpty(CHECK_CAN_USE_OPENCV)|!isEmpty(MUST_USE_OPENCV)|contains( COMPUTREE, ctlibopencv) {
    include(default_path_opencv.pri)
    exists(user_path_opencv.pri) : include(user_path_opencv.pri)
    include(include_necessary_opencv.pri)
    message("DEPENDENCY SETUP - OPENCV       will be used in $$TARGET")
}
!isEmpty(CHECK_CAN_USE_PCL)|!isEmpty(MUST_USE_PCL)|contains( COMPUTREE, ctlibpcl) {
    include(default_path_pcl.pri)
    exists(user_path_pcl.pri) : include(user_path_pcl.pri)
    include(include_necessary_pcl.pri)
    message("DEPENDENCY SETUP - PCL          will be used in $$TARGET")
}
!isEmpty(CHECK_CAN_USE_BOOST)|!isEmpty(MUST_USE_BOOST) {
    include(default_path_boost.pri)
    exists(user_path_boost.pri) : include(user_path_boost.pri)
    include(include_necessary_boost.pri)
    message("DEPENDENCY SETUP - BOOST        will be used in $$TARGET")
}
!isEmpty(CHECK_CAN_USE_GSL)|!isEmpty(MUST_USE_GSL)|contains( COMPUTREE, ctlibgsl) {
    include(default_path_gsl.pri)
    exists(user_path_gsl.pri) : include(user_path_gsl.pri)
    include(include_necessary_gsl.pri)
    message("DEPENDENCY SETUP - GSL          will be used in $$TARGET")
}
!isEmpty(CHECK_CAN_USE_FLANN)|!isEmpty(MUST_USE_FLANN) {
    include(default_path_flann.pri)
    exists(user_path_flann.pri) : include(user_path_flann.pri)
    include(include_necessary_flann.pri)
    message("DEPENDENCY SETUP - FLANN        will be used in $$TARGET")
}
!isEmpty(CHECK_CAN_USE_QHULL)|!isEmpty(MUST_USE_QHULL) {
    include(default_path_qhull.pri)
    exists(user_path_qhull.pri) : include(user_path_qhull.pri)
    include(include_necessary_qhull.pri)
    message("DEPENDENCY SETUP - QHULL        will be used in $$TARGET")
}

### To avoid compilation error with GCC 7+
linux {
    INCLUDEPATH -= /usr/include
}
