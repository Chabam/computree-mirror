
isEmpty(USE_FLANN_ERROR_MSG) {
    equals(CHECK_LIBS_ONLY, false) {
        DEFINES += USE_FLANN

        macx {
            INCLUDEPATH *= $$FLANN_INC_PATH $$LZ4_INC_PATH
        } else {
            INCLUDEPATH *= $$FLANN_INC_PATH
        }
        TR_EXCLUDE  *= $$FLANN_INC_PATH/*
        LIBS *= -L$$FLANN_LIBS_PATH
    }
} else {
    error($$USE_FLANN_ERROR_MSG)
}
