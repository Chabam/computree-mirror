# Mandatory dependency check function
defineTest(checkMandatoryDependence) {
    include(default_path_$${1}.pri)
    include_file = "default_path_$${1}.pri"

    exists(user_path_$${1}.pri) {
        include(user_path_$${1}.pri)
        include_file = "user_path_$${1}.pri"
    }

    include(check_$${1}.pri)

    !isEmpty(CHECK_ERROR_MSG) {
        error("DEPENDENCY CHECK - $$upper($${1}) - Invalid path specified ($$include_file)")
        error($$CHECK_ERROR_MSG)
    }

    CHECK_LIBS_ONLY = true
    include(include_necessary_$${1}.pri)
    CHECK_LIBS_ONLY = false

    !isEmpty(CHECK_ERROR_MSG) {
        error("DEPENDENCY CHECK - $$upper($${1}) - Libraries not found in specified path ($$include_file)")
        error($$CHECK_ERROR_MSG)
    } else {
        message("DEPENDENCY CHECK - $$upper($${1}) - OK ($$include_file)")
    }
}

# Default relative path for libs
isEmpty(LIB_PATH) : LIB_PATH = # empty

# Check mandatory libraries
checkMandatoryDependence(eigen)
checkMandatoryDependence(muparser)
checkMandatoryDependence(libqglviewer)
checkMandatoryDependence(opencv)
checkMandatoryDependence(gdal)

# Optional check for PCL
!isEmpty(MUST_USE_PCL) : checkMandatoryDependence(pcl)

# Include sub-projects from dependencies
exists (../3rdparty/libQGLViewer/libQGLViewer.pro) {
    SUBDIRS += 3rdparty/libQGLViewer
} else {
    error("libQGLViewer.pro can't be found! It is mandatory to compile it with Computree.")
}

exists (../3rdparty/muparser/muparser.pro) {
    SUBDIRS += 3rdparty/muparser
} else {
    error("muparser.pro can't be found! It is mandatory to compile it with Computree.")
}