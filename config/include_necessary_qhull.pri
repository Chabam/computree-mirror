QHULL_LIB_ADD += qhull
QHULL_LIB_ADD += qhullcpp

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


#windows {
#    for(a, QHULL_LIB_ADD) {
#        CONFIG(debug, debug|release) {
#            !exists($$QHULL_LIBS_PATH/$${a}_rd*) {
#                USE_QHULL_ERROR_MSG += "Library $$QHULL_LIBS_PATH/$${a}_rd was not found"
#            } else {
#                QHULL_LIBS_FOUNDED += -l$${a}_rd
#            }
#        } else {
#            !exists($$QHULL_LIBS_PATH/$${a}_r*) {
#                USE_QHULL_ERROR_MSG += "Library $$QHULL_LIBS_PATH/$${a}_r was not found"
#            } else {
#                QHULL_LIBS_FOUNDED += -l$${a}_r
#            }
#        }
#    }
#}

windows {
    CONFIG(debug, debug|release) {
        !exists($$QHULL_LIBS_PATH/qhull_rd*) {
            USE_QHULL_ERROR_MSG += "Library $$QHULL_LIBS_PATH/qhull_rd was not found"
        } else {
            QHULL_LIBS_FOUNDED += -lqhull_rd
        }
    } else {
        !exists($$QHULL_LIBS_PATH/qhull_r*) {
            USE_QHULL_ERROR_MSG += "Library $$QHULL_LIBS_PATH/qhull_r was not found"
        } else {
            QHULL_LIBS_FOUNDED += -lqhull_r
        }
    }

    CONFIG(debug, debug|release) {
        !exists($$QHULL_LIBS_PATH/qhullcpp_d*) {
            USE_QHULL_ERROR_MSG += "Library $$QHULL_LIBS_PATH/qhullcpp_d was not found"
        } else {
            QHULL_LIBS_FOUNDED += -lqhullcpp_d
        }
    } else {
        !exists($$QHULL_LIBS_PATH/qhullcpp*) {
            USE_QHULL_ERROR_MSG += "Library $$QHULL_LIBS_PATH/qhullcpp was not found"
        } else {
            QHULL_LIBS_FOUNDED += -lqhullcpp
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
