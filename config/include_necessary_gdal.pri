GDAL_LIB_ADD = gdal

GDAL_LIBS_FOUND =

linux {
    for(a, GDAL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$GDAL_LIBS_PATH/lib$${a}*) {
                USE_GDAL_ERROR_MSG += "Library $$GDAL_LIBS_PATH/lib$${a} was not found"
            } else {
                GDAL_LIBS_FOUND += $$GDAL_LIBS_PATH/lib$${a}.so
            }
        } else {
            !exists($$GDAL_LIBS_PATH/lib$${a}*) {
                USE_GDAL_ERROR_MSG += "Library $$GDAL_LIBS_PATH/lib$${a} was not found"
            } else {
                GDAL_LIBS_FOUND += $$GDAL_LIBS_PATH/lib$${a}.so
            }
        }
    }
}

macx {
    for(a, GDAL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$GDAL_LIBS_PATH/lib$${a}*) {
                USE_GDAL_ERROR_MSG += "Library $$GDAL_LIBS_PATH/lib$${a} was not found"
            } else {
                GDAL_LIBS_FOUND += $$GDAL_LIBS_PATH/lib$${a}.dylib
            }
        } else {
            !exists($$GDAL_LIBS_PATH/lib$${a}*) {
                USE_GDAL_ERROR_MSG += "Library $$GDAL_LIBS_PATH/lib$${a} was not found"
            } else {
                GDAL_LIBS_FOUND += $$GDAL_LIBS_PATH/lib$${a}.dylib
            }
        }
    }
}

windows {
    for(a, GDAL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$GDAL_LIBS_PATH/$${a}d*) {
                USE_GDAL_ERROR_MSG += "Library $$GDAL_LIBS_PATH/$${a}d was not found"
            } else {
                GDAL_LIBS_FOUND += -l$${a}d
            }
        } else {
            !exists($$GDAL_LIBS_PATH/$${a}*) {
                USE_GDAL_ERROR_MSG += "Library $$GDAL_LIBS_PATH/$${a} was not found"
            } else {
                GDAL_LIBS_FOUND += -l$${a}
            }
        }
    }
}

isEmpty(USE_GDAL_ERROR_MSG) {
    equals(CHECK_LIBS_ONLY, false) {
        DEFINES += USE_GDAL

        win32 {
            ABS_PATH = $$absolute_path($$GDAL_INC_PATH)
            TMP_LIB_PATH = $$LIB_PATH/

            equals(ABS_PATH, $$GDAL_INC_PATH) {
                TMP_LIB_PATH = ""
            }

            INCLUDEPATH += $$TMP_LIB_PATH$$GDAL_INC_PATH
            TR_EXCLUDE  += $$TMP_LIB_PATH$$GDAL_INC_PATH/*
            LIBS += -L$$TMP_LIB_PATH$$GDAL_LIBS_PATH

            !isEmpty(GDAL_BIN_PATH) {
                LIBS += -L$${TMP_LIB_PATH}$$GDAL_BIN_PATH
            }
        } else {
            INCLUDEPATH += $$GDAL_INC_PATH
            TR_EXCLUDE  += $$GDAL_INC_PATH/*
            LIBS += -L$$GDAL_LIBS_PATH
        }

        LIBS += $$GDAL_LIBS_FOUND
    }
} else {
    error($$USE_GDAL_ERROR_MSG)
}
