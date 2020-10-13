CHECK_LIBS_ONLY = false

##### GEOS ####

#if we must check if we can use gdal
!isEmpty(CHECK_CAN_USE_GEOS) {
    # we check if GDAL can be used
    CHECK_GEOS = 1
}

#if we want absolutely use gdal (the GDAL check test must pass)
!isEmpty(MUST_USE_GEOS) {
    # we check if GDAL can be used
    CHECK_GEOS = 1
}

#if we must check if gdal can be used
!isEmpty(CHECK_GEOS) {
    include(default_path_geos.pri)

    exists(user_path_geos.pri) {
        include(user_path_geos.pri)
    }

    include(check_geos.pri)

    #if GEOS can be used
    isEmpty(USE_GEOS_ERROR_MSG) {
        include(include_necessary_geos.pri)

        isEmpty(USE_GEOS_ERROR_MSG) {
            message("GEOS found and it will be used in $$TARGET")
        }
    }
}

##### GDAL ####

#if we must check if we can use gdal
!isEmpty(CHECK_CAN_USE_GDAL) {
    # we check if GDAL can be used
    CHECK_GDAL = 1
}

#if we want absolutely use gdal library
contains( COMPUTREE, ctlibgdal ) {
    # we set that the GDAL check test must pass
    MUST_USE_GDAL = 1
}

#if we want absolutely use gdal (the GDAL check test must pass)
!isEmpty(MUST_USE_GDAL) {
    # we check if GDAL can be used
    CHECK_GDAL = 1
}

#if we must check if gdal can be used
!isEmpty(CHECK_GDAL) {
    include(default_path_gdal.pri)

    exists(user_path_gdal.pri) {
        include(user_path_gdal.pri)
    }

    include(check_gdal.pri)

    #if GDAL can be used
    isEmpty(USE_GDAL_ERROR_MSG) {
        include(include_necessary_gdal.pri)

        isEmpty(USE_GDAL_ERROR_MSG) {
            message("GDAL found and it will be used in $$TARGET")
        }
    }
}

#if we must absolutely use GDAL
!isEmpty(MUST_USE_GDAL) {
    !contains(DEFINES, USE_GDAL) {
        for(a, USE_GDAL_ERROR_MSG) {
            warning("Error when search GDAL : $${a}")
        }
        error("GDAL not found for $$TARGET, see warning above for more information")
    }
} else {
    !isEmpty(CHECK_CAN_USE_GDAL) {
        !contains(DEFINES, USE_GDAL) {
            warning($$TARGET can use GDAL but it was not found. The plugin will be compiled in a reduced mode.)

            !isEmpty(USE_GDAL_ERROR_MSG) {
                for(a, USE_GDAL_ERROR_MSG) {
                    warning("Error when search GDAL : $${a}")
                }
            }
        }
    }
}

##### OPENCV ####

#if we must check if we can use opencv
!isEmpty(CHECK_CAN_USE_OPENCV) {
    # we check if OPENCV can be used
    CHECK_OPENCV = 1
}

#if we want absolutely use opencv library
contains( COMPUTREE, ctlibopencv ) {
    # we set that the OPENCV check test must pass
    MUST_USE_OPENCV = 1
}

#if we want absolutely use opencv (the OPENCV check test must pass)
!isEmpty(MUST_USE_OPENCV) {
    # we check if OPENCV can be used
    CHECK_OPENCV = 1
}

#if we must check if opencv can be used
!isEmpty(CHECK_OPENCV) {
    include(default_path_opencv.pri)

    exists(user_path_opencv.pri) {
        include(user_path_opencv.pri)
    }

    include(check_opencv.pri)

    #if OPENCV can be used
    isEmpty(USE_OPENCV_ERROR_MSG) {
        include(include_necessary_opencv.pri)

        isEmpty(USE_OPENCV_ERROR_MSG) {
            message("OPENCV found and it will be used in $$TARGET")
        }
    }
}

#if we must absolutely use OPENCV
!isEmpty(MUST_USE_OPENCV) {
    !contains(DEFINES, USE_OPENCV) {
        for(a, USE_OPENCV_ERROR_MSG) {
            warning("Error when search OPENCV : $${a}")
        }
        error("OPENCV not found for $$TARGET, see warning above for more information")
    }
} else {
    !isEmpty(CHECK_CAN_USE_OPENCV) {
        !contains(DEFINES, USE_OPENCV) {
            warning($$TARGET can use OPENCV but it was not found. The plugin will be compiled in a reduced mode.)

            !isEmpty(USE_OPENCV_ERROR_MSG) {
                for(a, USE_OPENCV_ERROR_MSG) {
                    warning("Error when search OPENCV : $${a}")
                }
            }
        }
    }
}


##### GSL ####

#if we must check if we can use gsl
!isEmpty(CHECK_CAN_USE_GSL) {
    # we check if GSL can be used
    CHECK_GSL = 1
}

#if we want absolutely use gsl library
contains( COMPUTREE, ctlibgsl ) {
    # we set that the GSL check test must pass
    MUST_USE_GSL = 1
}

#if we want absolutely use gsl (the GSL check test must pass)
!isEmpty(MUST_USE_GSL) {
    # we check if GSL can be used
    CHECK_GSL = 1
}

#if we must check if gsl can be used
!isEmpty(CHECK_GSL) {
    include(default_path_gsl.pri)

    exists(user_pat_gslh.pri) {
        include(user_path_gsl.pri)
    }

    include(check_gsl.pri)

    #if GSL can be used
    isEmpty(USE_GSL_ERROR_MSG) {
        include(include_necessary_gsl.pri)

        isEmpty(USE_GSL_ERROR_MSG) {
            message("GSL found and it will be used in $$TARGET")
        }
    }
}

#if we must absolutely use GSL
!isEmpty(MUST_USE_GSL) {
    !contains(DEFINES, USE_GSL) {
        for(a, USE_GSL_ERROR_MSG) {
            warning("Error when search GSL : $${a}")
        }
        error("GSL not found for $$TARGET, see warning above for more information")
    }
} else {
    !isEmpty(CHECK_CAN_USE_GSL) {
        !contains(DEFINES, USE_GSL) {
            warning($$TARGET can use GSL but it was not found. The plugin will be compiled in a reduced mode.)

            !isEmpty(USE_GSL_ERROR_MSG) {
                for(a, USE_GSL_ERROR_MSG) {
                    warning("Error when search GSL : $${a}")
                }
            }
        }
    }
}

##### PCL : MUST BE DECLARED LAST IN THIS FILE (because includes usr/include on linux) ####

#if we must check if we can use pcl
!isEmpty(CHECK_CAN_USE_PCL) {
    # we check if PCL can be used
    CHECK_PCL = 1
}

#if we want absolutely use pcl library
contains( COMPUTREE, ctlibpcl ) {
    # we set that the PCL check test must pass
    MUST_USE_PCL = 1
}

#if we want absolutely use pcl (the PCL check test must pass)
!isEmpty(MUST_USE_PCL) {
    # we check if PCL can be used
    CHECK_PCL = 1
}

#if we must check if pcl can be used
!isEmpty(CHECK_PCL) {
    include(default_path_pcl.pri)

    exists(user_path_pcl.pri) {
        include(user_path_pcl.pri)
    }

    include(check_pcl.pri)

    #if PCL can be used
    isEmpty(USE_PCL_ERROR_MSG) {
        include(include_necessary_pcl.pri)

        isEmpty(USE_PCL_ERROR_MSG) {
            message("PCL found and it will be used in $$TARGET")
        }
    }
}

#if we must absolutely use PCL
!isEmpty(MUST_USE_PCL) {
    !contains(DEFINES, USE_PCL) {
        for(a, USE_PCL_ERROR_MSG) {
            warning("Error when search PCL : $${a}")
        }
        error("PCL not found for $$TARGET, see warning above for more information")
    }
} else {
    !isEmpty(CHECK_CAN_USE_PCL) {
        !contains(DEFINES, USE_PCL) {
            warning($$TARGET can use PCL but it was not found. The plugin will be compiled in a reduced mode.)

            !isEmpty(USE_PCL_ERROR_MSG) {
                for(a, USE_PCL_ERROR_MSG) {
                    warning("Error when search PCL : $${a}")
                }
            }
        }
    }
}

##### EIGEN IF PCL ######

!contains( DEFINES, USE_PCL ) {
    isEmpty(CT_PREFIX_LIB) {
        INCLUDEPATH += $$CT_PREFIX/3rdparty/eigen
        TR_EXCLUDE  += $$CT_PREFIX/3rdparty/eigen/*
    } else {
        INCLUDEPATH += $$CT_PREFIX/3rdparty/eigen
        TR_EXCLUDE  += $$CT_PREFIX/3rdparty/eigen/*
    }
}

### To avoid compilation error with GCC 7+
linux {
    INCLUDEPATH -= /usr/include
}
