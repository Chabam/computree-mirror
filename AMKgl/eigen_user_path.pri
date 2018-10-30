DEFAULT_EIGEN_INC_PATH = "../pluginshared/3rdparty/eigen"

##### PCL ####
include(../pcl_default_path.pri)
exists(../pcl_user_path.pri) {
    include(../pcl_user_path.pri)
    PCL_USER_MODE = "../pcl_user_path.pri"
} else {
    PCL_USER_MODE = "../pcl_default_path.pri"
}
include(../pcl_check.pri)

isEmpty(USE_PCL_ERROR_MSG) {
    CHECK_LIBS_ONLY = true
    include(../include_pcl_necessary.pri)
    CHECK_LIBS_ONLY = false
}

!isEmpty(EIGEN_INC_PATH) {
    !exists($$EIGEN_INC_PATH) {
        EIGEN_INC_PATH = $$DEFAULT_EIGEN_INC_PATH
    }
} else {
    EIGEN_INC_PATH = $$DEFAULT_EIGEN_INC_PATH
}

warning(Eigen path used for AMKGl ==> $$EIGEN_INC_PATH)
