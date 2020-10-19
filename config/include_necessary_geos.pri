GEOS_LIB_ADD = geos
GEOS_LIBS_FOUNDED =

linux {
    for(a, GEOS_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$GEOS_LIBS_PATH/lib$${a}*) {
                USE_GEOS_ERROR_MSG += "Library $$GEOS_LIBS_PATH/lib$${a} was not found"
            } else {
                GEOS_LIBS_FOUNDED += $$GEOS_LIBS_PATH/lib$${a}.so
            }
        } else {
            !exists($$GEOS_LIBS_PATH/lib$${a}*) {
                USE_GEOS_ERROR_MSG += "Library $$GEOS_LIBS_PATH/lib$${a} was not found"
            } else {
                GEOS_LIBS_FOUNDED += $$GEOS_LIBS_PATH/lib$${a}.so
            }
        }
    }
}

win32 {
    for(a, GEOS_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$GEOS_LIBS_PATH/$${a}*) {
                USE_GEOS_ERROR_MSG += "Library $$GEOS_LIBS_PATH/$${a}_i was not found"
            } else {
                GEOS_LIBS_FOUNDED += -l$${a}
            }
        } else {

            !exists($$GEOS_LIBS_PATH/$${a}*) {
                USE_GEOS_ERROR_MSG += "Library $$GEOS_LIBS_PATH/$${a}_i was not found"
            } else {
                GEOS_LIBS_FOUNDED += -l$${a}
            }
        }
    }
}

isEmpty(USE_GEOS_ERROR_MSG) {
    equals(CHECK_LIBS_ONLY, false) {
        DEFINES += USE_GEOS
        
        INCLUDEPATH += $$LIB_PATH/$$GEOS_INC_PATH
        
        TR_EXCLUDE  += $$LIB_PATH/$$GEOS_INC_PATH/*

        LIBS += -L$$LIB_PATH/$$GEOS_LIBS_PATH
        LIBS += $$GEOS_LIBS_FOUNDED
    }
}

