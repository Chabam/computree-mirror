QHULL_LIB_ADD =

QHULL_LIBS_FOUNDED =

linux {
    QHULL_LIB_ADD += qhull

    for(a, QHULL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$QHULL_LIBS_PATH/lib$${a}*) {
                USE_QHULL_ERROR_MSG += "Library $$QHULL_LIBS_PATH/lib$${a} was not found"
            } else {
                QHULL_LIBS_FOUNDED += $$QHULL_LIBS_PATH/lib$${a}.so
            }
        } else {
            !exists($$QHULL_LIBS_PATH/lib$${a}*) {
                USE_QHULL_ERROR_MSG += "Library $$QHULL_LIBS_PATH/lib$${a} was not found"
            } else {
                QHULL_LIBS_FOUNDED += $$QHULL_LIBS_PATH/lib$${a}.so
            }
        }
    }
}

macx {
    QHULL_LIB_ADD += qhull

    for(a, QHULL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$QHULL_LIBS_PATH/lib$${a}*) {
                USE_QHULL_ERROR_MSG += "Library $$QHULL_LIBS_PATH/lib$${a} was not found"
            } else {
                QHULL_LIBS_FOUNDED += $$QHULL_LIBS_PATH/lib$${a}.dylib
            }
        } else {
            !exists($$QHULL_LIBS_PATH/lib$${a}*) {
                USE_QHULL_ERROR_MSG += "Library $$QHULL_LIBS_PATH/lib$${a} was not found"
            } else {
                QHULL_LIBS_FOUNDED += $$QHULL_LIBS_PATH/lib$${a}.dylib
            }
        }
    }
}

windows {
    QHULL_LIB_ADD += qhullstatic
    #QHULL_LIB_ADD += qhullstatic_d

    for(a, QHULL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$QHULL_LIBS_PATH/$${a}_d*) {
                USE_QHULL_ERROR_MSG += "Library $$QHULL_LIBS_PATH/$${a}_d was not found"
            } else {
                QHULL_LIBS_FOUNDED += -l$${a}_d
            }
        } else {
            !exists($$QHULL_LIBS_PATH/$${a}*) {
                USE_QHULL_ERROR_MSG += "Library $$QHULL_LIBS_PATH/$${a} was not found"
            } else {
                QHULL_LIBS_FOUNDED += -l$${a}
            }
        }
    }
}

isEmpty(USE_QHULL_ERROR_MSG) {
    equals(CHECK_LIBS_ONLY, false) {
        DEFINES += USE_QHULL

        win32 {
            ABS_PATH = $$absolute_path($$QHULL_INC_PATH)
            TMP_LIB_PATH = $$LIB_PATH/

            equals(ABS_PATH, $$QHULL_INC_PATH) {
                TMP_LIB_PATH = ""
            }

            INCLUDEPATH += $$TMP_LIB_PATH$$QHULL_INC_PATH
            TR_EXCLUDE  += $$TMP_LIB_PATH$$QHULL_INC_PATH/*
            DEPENDPATH  += $$TMP_LIB_PATH$$QHULL_INC_PATH
            LIBS += -L$$TMP_LIB_PATH$$QHULL_LIBS_PATH

            !isEmpty(QHULL_BIN_PATH) {
                LIBS += -L$${TMP_LIB_PATH}$$QHULL_BIN_PATH
            }
        } else {
            INCLUDEPATH += $$QHULL_INC_PATH
            TR_EXCLUDE  += $$QHULL_INC_PATH/*
            DEPENDPATH  += $$QHULL_INC_PATH
            LIBS += -L$$QHULL_LIBS_PATH
        }

        LIBS += $$QHULL_LIBS_FOUNDED
    }
}
