GSL_LIB_ADD += gsl
GSL_LIB_ADD += gslcblas

GSL_LIBS_FOUNDED =

unix {
    for(a, GSL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$GSL_LIBS_PATH/lib$${a}*) {
                USE_GSL_ERROR_MSG += "Library $$GSL_LIBS_PATH/lib$${a} was not found"
            } else {
                GSL_LIBS_FOUNDED += $$GSL_LIBS_PATH/lib$${a}.so
            }
        } else {
            !exists($$GSL_LIBS_PATH/lib$${a}*) {
                USE_GSL_ERROR_MSG += "Library $$GSL_LIBS_PATH/lib$${a} was not found"
            } else {
                GSL_LIBS_FOUNDED += $$GSL_LIBS_PATH/lib$${a}.so
            }
        }
    }
}

windows {
    for(a, GSL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$GSL_LIBS_PATH/$${a}*) {
                USE_GSL_ERROR_MSG += "Library $$GSL_LIBS_PATH/$${a} was not found"
            } else {
                GSL_LIBS_FOUNDED += -l$${a}
            }
        } else {

            !exists($$GSL_LIBS_PATH/$${a}*) {
                USE_GSL_ERROR_MSG += "Library $$GSL_LIBS_PATH/$${a} was not found"
            } else {
                GSL_LIBS_FOUNDED += -l$${a}
            }
        }
    }
}

isEmpty(USE_GSL_ERROR_MSG) {
    equals(CHECK_LIBS_ONLY, false) {
        DEFINES += USE_GSL
        
        INCLUDEPATH *= $$GSL_INC_PATH
        TR_EXCLUDE  *= $$GSL_INC_PATH/*
        LIBS *= -L$$GSL_LIBS_PATH

        LIBS *= $$GSL_LIBS_FOUNDED
    }
} else {
    error($$USE_GSL_ERROR_MSG)
}
