
isEmpty(USE_FLANN_ERROR_MSG) {
    equals(CHECK_LIBS_ONLY, false) {
        win32 {
            INCLUDEPATH += $$LIB_PATH/$$FLANN_INC_PATH
            TR_EXCLUDE  += $$LIB_PATH/$$FLANN_INC_PATH/*
        } else {
            INCLUDEPATH += $$FLANN_INC_PATH
            TR_EXCLUDE  += $$FLANN_INC_PATH/*
        }
    }
}
