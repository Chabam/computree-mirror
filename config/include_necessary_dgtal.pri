DGTAL_LIB_ADD = libdgtal
DGTAL_LIBS_FOUNDED =

unix {
    for(a, DGTAL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$DGTAL_LIBS_PATH/$${a}*) {
                USE_DGTAL_ERROR_MSG += "Library $$DGTAL_LIBS_PATH/$${a} was not found"
            } else {
                DGTAL_LIBS_FOUNDED += $$DGTAL_LIBS_PATH/$${a}.so
            }
        } else {
            !exists($$DGTAL_LIBS_PATH/$${a}*) {
                USE_DGTAL_ERROR_MSG += "Library $$DGTAL_LIBS_PATH/$${a} was not found"
            } else {
                DGTAL_LIBS_FOUNDED += $$DGTAL_LIBS_PATH/$${a}.so
            }
        }
    }
}


windows {
DGTAL_LIB_ADD = dgtal

    for(a, DGTAL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$DGTAL_LIBS_PATH/$${a}*) {
                USE_DGTAL_ERROR_MSG += "Library $$DGTAL_LIBS_PATH/$${a} was not found"
            } else {
                DGTAL_LIBS_FOUNDED += -l$${a}
            }
        } else {

            !exists($$DGTAL_LIBS_PATH/$${a}*) {
                USE_DGTAL_ERROR_MSG += "Library $$DGTAL_LIBS_PATH/$${a} was not found"
            } else {
                DGTAL_LIBS_FOUNDED += -l$${a}
            }
        }
    }
}

isEmpty(USE_DGTAL_ERROR_MSG) {
    equals(CHECK_LIBS_ONLY, false) {
        DEFINES += USE_DGTAL
        
        INCLUDEPATH += $$DGTAL_INC_PATH
        
        TR_EXCLUDE  += $$DGTAL_INC_PATH/*

        LIBS += -L$$DGTAL_LIBS_PATH
        LIBS += $$DGTAL_LIBS_FOUNDED
    }
}

