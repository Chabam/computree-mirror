isEmpty(DEFAULT_EIGEN_INC_PATH) {
    DEFAULT_EIGEN_INC_PATH = "../3rdparty/eigen"
}

##### PCL ####
include(../config/default_path_pcl.pri)

exists(../config/user_path_pcl.pri) {
    include(../config/user_path_pcl.pri)
    PCL_USER_MODE = "../config/user_path_pcl.pri"
} else {
    PCL_USER_MODE = "../config/default_path_pcl.pri"
}

include(../config/check_pcl.pri)

isEmpty(USE_PCL_ERROR_MSG) {
    CHECK_LIBS_ONLY = true
    include(../config/include_necessary_pcl.pri)
    CHECK_LIBS_ONLY = false
}

!isEmpty(EIGEN_INC_PATH) {
    !exists($$EIGEN_INC_PATH) {
        EIGEN_INC_PATH = $$DEFAULT_EIGEN_INC_PATH
        message(Eigen path is ==> $$EIGEN_INC_PATH)
    }
} else {
    EIGEN_INC_PATH = $$DEFAULT_EIGEN_INC_PATH
}


INCLUDEPATH *= $$EIGEN_INC_PATH
TR_EXCLUDE  *= $$EIGEN_INC_PATH/*
