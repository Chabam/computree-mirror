DEFAULT_EIGEN_INC_PATH = "../3rdparty/eigen"

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
}

!isEmpty(EIGEN_INC_PATH) {
    !exists($$EIGEN_INC_PATH) {
        EIGEN_INC_PATH = $$DEFAULT_EIGEN_INC_PATH
        message(Eigen path used for AMKGl ==> $$EIGEN_INC_PATH)
    }
} else {
    EIGEN_INC_PATH = $$DEFAULT_EIGEN_INC_PATH
}
