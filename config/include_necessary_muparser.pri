isEmpty(USE_MUPARSER_ERROR_MSG) {
    equals(CHECK_LIBS_ONLY, false) {
        DEFINES += USE_MUPARSER

        win32 {
            ABS_PATH = $$absolute_path($$MUPARSER_INC_PATH)
            TMP_LIB_PATH = $$LIB_PATH/

            equals(ABS_PATH, $$MUPARSER_INC_PATH) {
                TMP_LIB_PATH = ""
            }

            INCLUDEPATH += $$TMP_LIB_PATH$$MUPARSER_INC_PATH
            TR_EXCLUDE  += $$TMP_LIB_PATH$$MUPARSER_INC_PATH/*
            LIBS += -lmuparser -L$$TMP_LIB_PATH$$MUPARSER_LIBS_PATH
        } else {
            INCLUDEPATH += $$MUPARSER_INC_PATH
            TR_EXCLUDE  += $$MUPARSER_INC_PATH/*
            LIBS += -lmuparser -L$$MUPARSER_LIBS_PATH
        }
    }
} else {
    error($$USE_MUPARSER_ERROR_MSG)
}
