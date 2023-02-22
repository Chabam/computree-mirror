isEmpty(USE_LASZIP_ERROR_MSG) {
    equals(CHECK_LIBS_ONLY, false) {
        DEFINES += USE_LASZIP

        INCLUDEPATH *= $$LASZIP_INC_PATH
        TR_EXCLUDE  *= $$LASZIP_INC_PATH/*
        LIBS *= -llaszip3 -L$$LASZIP_LIBS_PATH
    }
} else {
    error($$USE_LASZIP_ERROR_MSG)
}
