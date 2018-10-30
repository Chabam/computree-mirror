##### SHADOW BUILD ####
!equals(PWD, $${OUT_PWD}) {
    error("Shadow build seems to be activated, please desactivated it !")
}

##### QGLVIEWER ####
include(qglviewer_default_path.pri)
!exists($$QGL_VIEWER_PATH/qglviewer.h) {
    error("QGLViewer.h not found in $$QGL_VIEWER_PATH ! Check your QGLViewer directory or the path in qglviewer_default_path.pri.")
} else {
    warning("DEPENDENCY CHECK - QGLVIEWER    - OK (qglviewer_default_path.pri)")
}

##### GEOS ####
include(geos_default_path.pri)
exists(geos_user_path.pri) {
    include(geos_user_path.pri)
    GEOS_USER_MODE = "geos_user_path.pri"
} else {
    GEOS_USER_MODE = "geos_default_path.pri"
}
include(geos_check.pri)

#if GEOS can be used
isEmpty(USE_GEOS_ERROR_MSG) {

    CHECK_LIBS_ONLY = true
    include(include_geos_necessary.pri)
    CHECK_LIBS_ONLY = false

    isEmpty(USE_GEOS_ERROR_MSG) {
        warning("DEPENDENCY CHECK - GEOS   - OK ($$GEOS_USER_MODE)")
    } else {
        warning("DEPENDENCY CHECK - GEOS   - Libraries not found in specified path ($$GEOS_USER_MODE)")
        warning($$USE_GEOS_ERROR_MSG)
    }
} else {
    warning("DEPENDENCY CHECK - GEOS   - invalid path specified ($$GEOS_USER_MODE)")
}


##### GDAL ####
include(gdal_default_path.pri)
exists(gdal_user_path.pri) {
    include(gdal_user_path.pri)
    GDAL_USER_MODE = "gdal_user_path.pri"
} else {
    GDAL_USER_MODE = "gdal_default_path.pri"
}
include(gdal_check.pri)

#if GDAL can be used
isEmpty(USE_GDAL_ERROR_MSG) {
    CHECK_LIBS_ONLY = true
    include(include_gdal_necessary.pri)
    CHECK_LIBS_ONLY = false

    isEmpty(USE_GDAL_ERROR_MSG) {
        warning("DEPENDENCY CHECK - GDAL   - OK ($$GDAL_USER_MODE)")
    } else {
        warning("DEPENDENCY CHECK - GDAL   - Libraries not found in specified path ($$GDAL_USER_MODE)")
        warning($$USE_GDAL_ERROR_MSG)
    }
} else {
        warning("DEPENDENCY CHECK - GDAL   - invalid path specified ($$GDAL_USER_MODE)")
}


##### GSL ####
include(gsl_default_path.pri)
exists(gsl_user_path.pri) {
    include(gsl_user_path.pri)
    GSL_USER_MODE = "gsl_user_path.pri"
} else {
    GSL_USER_MODE = "gsl_default_path.pri"
}
include(gsl_check.pri)

#if GSL can be used
isEmpty(USE_GSL_ERROR_MSG) {
    CHECK_LIBS_ONLY = true
    include(include_gsl_necessary.pri)
    CHECK_LIBS_ONLY = false

    isEmpty(USE_GSL_ERROR_MSG) {
        warning("DEPENDENCY CHECK - GSL    - OK ($$GSL_USER_MODE)")
    } else {
        warning("DEPENDENCY CHECK - GSL    - Libraries not found in specified path ($$GSL_USER_MODE)")
        warning($$USE_GSL_ERROR_MSG)
    }
} else {
        warning("DEPENDENCY CHECK - GSL    - invalid path specified ($$GSL_USER_MODE)")
}


##### PCL ####
include(pcl_default_path.pri)
exists(pcl_user_path.pri) {
    include(pcl_user_path.pri)
    PCL_USER_MODE = "pcl_user_path.pri"
} else {
    PCL_USER_MODE = "pcl_default_path.pri"
}
include(pcl_check.pri)

#if PCL can be used
isEmpty(USE_PCL_ERROR_MSG) {
    CHECK_LIBS_ONLY = true
    include(include_pcl_necessary.pri)
    CHECK_LIBS_ONLY = false

    isEmpty(USE_PCL_ERROR_MSG) {
        warning("DEPENDENCY CHECK - PCL    - OK ($$PCL_USER_MODE)")
    } else {
        warning("DEPENDENCY CHECK - PCL    - Libraries not found in specified path ($$PCL_USER_MODE)")
        warning($$USE_PCL_ERROR_MSG)
    }
} else {
        warning("DEPENDENCY CHECK - PCL    - invalid path specified ($$PCL_USER_MODE)")
}


##### OPENCV ####
include(opencv_default_path.pri)
exists(opencv_user_path.pri) {
    include(opencv_user_path.pri)
    OPENCV_USER_MODE = "opencv_user_path.pri"
} else {
    OPENCV_USER_MODE = "opencv_default_path.pri"
}
include(opencv_check.pri)

#if OPENCV can be used
isEmpty(USE_OPENCV_ERROR_MSG) {
    CHECK_LIBS_ONLY = true
    include(include_opencv_necessary.pri)
    CHECK_LIBS_ONLY = false

    isEmpty(USE_OPENCV_ERROR_MSG) {
        warning("DEPENDENCY CHECK - OPENCV - OK ($$OPENCV_USER_MODE)")
    } else {
        warning("DEPENDENCY CHECK - OPENCV - Libraries not found in specified path ($$OPENCV_USER_MODE)")
        warning($$USE_OPENCV_ERROR_MSG)
    }
} else {
        warning("DEPENDENCY CHECK - OPENCV - invalid path specified ($$OPENCV_USER_MODE)")
}
