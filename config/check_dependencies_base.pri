##### MICROSOFT VISUAL STUDIO FLAGS ####
# win32-msvc* {
#     MSVC_VER = $$(VisualStudioVersion)
#     # MSVC 2013
#     equals(MSVC_VER, 13.0) {
#         message( "msvc 2013 detected" )
#         QMAKE_CXXFLAGS += /FS
#     }
#     # MSVC 2015
#     equals(MSVC_VER, 14.0) {
#         # Nothing to do yet
#     }
#     # MSVC 2017
#     equals(MSVC_VER, 15.0) {
#         # Nothing to do yet
#     }
#     # MSVC 2019
#     equals(MSVC_VER, 16.0) {
#         # Nothing to do yet
#     }
# }

##### SHADOW BUILD ####
!equals(PWD, "$${OUT_PWD}/config") {
    error("Shadow build seems to be activated, please desactivated it !")
}

##### QGLVIEWER ####
include(default_path_qglviewer.pri)

!exists(../$$QGL_VIEWER_PATH/qglviewer.h) {
    error("QGLViewer.h not found in $$QGL_VIEWER_PATH ! Check your QGLViewer directory or the path in default_path_qglviewer.pri.")
} else {
    message("DEPENDENCY CHECK - QGLVIEWER - OK (default_path_qglviewer.pri)")
}

##### GEOS ####
include(default_path_geos.pri)

exists(user_path_geos.pri) {
    include(user_path_geos.pri)
    GEOS_USER_MODE = "user_path_geos.pri"
} else {
    GEOS_USER_MODE = "default_path_geos.pri"
}

include(check_geos.pri)

isEmpty(USE_GEOS_ERROR_MSG) {

    CHECK_LIBS_ONLY = true
    include(include_necessary_geos.pri)
    CHECK_LIBS_ONLY = false

    isEmpty(USE_GEOS_ERROR_MSG) {
        message("DEPENDENCY CHECK - GEOS      - OK ($$GEOS_USER_MODE)")
    } else {
        warning("DEPENDENCY CHECK - GEOS      - Libraries not found in specified path ($$GEOS_USER_MODE)")
        warning($$USE_GEOS_ERROR_MSG)
    }
} else {
        warning("DEPENDENCY CHECK - GEOS      - invalid path specified ($$GEOS_USER_MODE)")
}

##### GDAL ####
include(default_path_gdal.pri)

exists(user_path_gdal.pri) {
    include(user_path_gdal.pri)
    GDAL_USER_MODE = "user_path_gdal.pri"
} else {
    GDAL_USER_MODE = "default_path_gdal.pri"
}

include(check_gdal.pri)

isEmpty(USE_GDAL_ERROR_MSG) {
    CHECK_LIBS_ONLY = true
    include(include_necessary_gdal.pri)
    CHECK_LIBS_ONLY = false

    isEmpty(USE_GDAL_ERROR_MSG) {
        message("DEPENDENCY CHECK - GDAL      - OK ($$GDAL_USER_MODE)")
    } else {
        warning("DEPENDENCY CHECK - GDAL      - Libraries not found in specified path ($$GDAL_USER_MODE)")
        warning($$USE_GDAL_ERROR_MSG)
    }
} else {
        warning("DEPENDENCY CHECK - GDAL      - invalid path specified ($$GDAL_USER_MODE)")
}

##### GSL ####
include(default_path_gsl.pri)

exists(user_path_gsl.pri) {
    include(user_path_gsl.pri)
    GSL_USER_MODE = "user_path_gsl.pri"
} else {
    GSL_USER_MODE = "default_path_gsl.pri"
}

include(check_gsl.pri)

isEmpty(USE_GSL_ERROR_MSG) {
    CHECK_LIBS_ONLY = true
    include(include_necessary_gsl.pri)
    CHECK_LIBS_ONLY = false

    isEmpty(USE_GSL_ERROR_MSG) {
        message("DEPENDENCY CHECK - GSL       - OK ($$GSL_USER_MODE)")
    } else {
        warning("DEPENDENCY CHECK - GSL       - Libraries not found in specified path ($$GSL_USER_MODE)")
        warning($$USE_GSL_ERROR_MSG)
    }
} else {
        warning("DEPENDENCY CHECK - GSL       - invalid path specified ($$GSL_USER_MODE)")
}

##### PCL ####
include(default_path_pcl.pri)

exists(user_path_pcl.pri) {
    include(user_path_pcl.pri)
    PCL_USER_MODE = "user_path_pcl.pri"
} else {
    PCL_USER_MODE = "default_path_pcl.pri"
}

include(check_pcl.pri)

isEmpty(USE_PCL_ERROR_MSG) {
    CHECK_LIBS_ONLY = true
    include(include_necessary_pcl.pri)
    CHECK_LIBS_ONLY = false

    isEmpty(USE_PCL_ERROR_MSG) {
        message("DEPENDENCY CHECK - PCL       - OK ($$PCL_USER_MODE)")
    } else {
        warning("DEPENDENCY CHECK - PCL       - Libraries not found in specified path ($$PCL_USER_MODE)")
        warning($$USE_PCL_ERROR_MSG)
    }
} else {
        warning("DEPENDENCY CHECK - PCL       - invalid path specified ($$PCL_USER_MODE)")
}

##### OPENCV ####
include(default_path_opencv.pri)

exists(user_path_opencv.pri) {
    include(user_path_opencv.pri)
    OPENCV_USER_MODE = "user_path_opencv.pri"
} else {
    OPENCV_USER_MODE = "default_path_opencv.pri"
}

include(check_opencv.pri)

isEmpty(USE_OPENCV_ERROR_MSG) {
    CHECK_LIBS_ONLY = true
    include(include_necessary_opencv.pri)
    CHECK_LIBS_ONLY = false

    isEmpty(USE_OPENCV_ERROR_MSG) {
        message("DEPENDENCY CHECK - OPENCV    - OK ($$OPENCV_USER_MODE)")
    } else {
        warning("DEPENDENCY CHECK - OPENCV    - Libraries not found in specified path ($$OPENCV_USER_MODE)")
        warning($$USE_OPENCV_ERROR_MSG)
    }
} else {
        warning("DEPENDENCY CHECK - OPENCV    - invalid path specified ($$OPENCV_USER_MODE)")
}

# CHECK MANDATORY LIBRARIES
exists (../3rdparty/libQGLViewer/libQGLViewer.pro) {
    SUBDIRS += 3rdparty/libQGLViewer
    message("Library libQGLViewer is found and it will be used.")
} else {
    error("Library libQGLViewer can't be found! It is mandatory for Computree.")
}

exists (../3rdparty/muparser/muparser.pro) {
    SUBDIRS += 3rdparty/muparser
    message("Library muparser is found and it will be used.")
} else {
    error("Library muparser can't be found! It is mandatory for Computree.")
}
