isEmpty(USE_MUPARSER_ERROR_MSG) {
    equals(CHECK_LIBS_ONLY, false) {
        DEFINES += USE_MUPARSER

        INCLUDEPATH *= $$MUPARSER_INC_PATH
        TR_EXCLUDE  *= $$MUPARSER_INC_PATH/*
        LIBS *= -lmuparser -L$$MUPARSER_LIBS_PATH
    }
} else {
    error($$USE_MUPARSER_ERROR_MSG)
}
