QHULL_LIB_ADD += qhull

QHULL_LIBS_FOUNDED =

linux {
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
    for(a, QHULL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$QHULL_LIBS_PATH/$${a}_rd*) {
                USE_QHULL_ERROR_MSG += "Library $$QHULL_LIBS_PATH/$${a}_rd was not found"
            } else {
                QHULL_LIBS_FOUNDED += -l$${a}_rd
            }
        } else {
            !exists($$QHULL_LIBS_PATH/$${a}_r*) {
                USE_QHULL_ERROR_MSG += "Library $$QHULL_LIBS_PATH/$${a}_r was not found"
            } else {
                QHULL_LIBS_FOUNDED += -l$${a}_r
            }
        }
    }
}

isEmpty(USE_QHULL_ERROR_MSG) {
    equals(CHECK_LIBS_ONLY, false) {
        DEFINES += USE_QHULL

        INCLUDEPATH *= $$QHULL_INC_PATH
        TR_EXCLUDE  *= $$QHULL_INC_PATH/*
        DEPENDPATH  *= $$QHULL_INC_PATH
        LIBS *= -L$$QHULL_LIBS_PATH

        LIBS *= $$QHULL_LIBS_FOUNDED
    }
} else {
    error($$USE_QHULL_ERROR_MSG)
}
