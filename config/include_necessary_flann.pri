
isEmpty(USE_FLANN_ERROR_MSG) {
    equals(CHECK_LIBS_ONLY, false) {
        DEFINES += USE_FLANN

        win32 {
            ABS_PATH = $$absolute_path($$FLANN_INC_PATH)
            TMP_LIB_PATH = $$LIB_PATH/

            equals(ABS_PATH, $$FLANN_INC_PATH) {
                TMP_LIB_PATH = ""
            }

            INCLUDEPATH += $$TMP_LIB_PATH$$FLANN_INC_PATH
            TR_EXCLUDE  += $$TMP_LIB_PATH$$FLANN_INC_PATH/*
            LIBS += -L$$TMP_LIB_PATH$$FLANN_LIBS_PATH

            !isEmpty(FLANN_BIN_PATH) {
                LIBS += -L$${TMP_LIB_PATH}$$FLANN_BIN_PATH
            }

        } else {
            INCLUDEPATH += $$FLANN_INC_PATH
            TR_EXCLUDE  += $$FLANN_INC_PATH/*
            LIBS += -$$FLANN_LIBS_PATH
        }
    }
} else {
    error($$USE_FLANN_ERROR_MSG)
}
