isEmpty(USE_LASZIP_ERROR_MSG) {
    equals(CHECK_LIBS_ONLY, false) {
        DEFINES += USE_LASZIP

        win32 {
            ABS_PATH = $$absolute_path($$LASZIP_INC_PATH)
            TMP_LIB_PATH = $$LIB_PATH

            equals(ABS_PATH, $$LASZIP_INC_PATH) {
                TMP_LIB_PATH = ""
            }

            INCLUDEPATH *= $$LIB_PATH$$LASZIP_INC_PATH
            TR_EXCLUDE  *= $$TMP_LIB_PATH$$LASZIP_INC_PATH/*
            LIBS *= -llaszip3 -L$$LIB_PATH$$LASZIP_LIBS_PATH
        } else {
            INCLUDEPATH *= $$LASZIP_INC_PATH
            TR_EXCLUDE  *= $$LASZIP_INC_PATH/*
            LIBS *= -llaszip -L$$LASZIP_LIBS_PATH
        }
    }
} else {
    error($$USE_LASZIP_ERROR_MSG)
}
