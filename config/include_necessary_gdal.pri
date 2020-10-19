GDAL_LIB_ADD = gdal
GDAL_LIBS_FOUNDED =

linux {
    for(a, GDAL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$GDAL_LIBS_PATH/lib$${a}*) {
                USE_GDAL_ERROR_MSG += "Library $$GDAL_LIBS_PATH/lib$${a} was not found"
            } else {
                GDAL_LIBS_FOUNDED += $$GDAL_LIBS_PATH/lib$${a}.so
            }
        } else {
            !exists($$GDAL_LIBS_PATH/lib$${a}*) {
                USE_GDAL_ERROR_MSG += "Library $$GDAL_LIBS_PATH/lib$${a} was not found"
            } else {
                GDAL_LIBS_FOUNDED += $$GDAL_LIBS_PATH/lib$${a}.so
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
                GDAL_LIBS_FOUNDED += $$GDAL_LIBS_PATH/lib$${a}.dylib
            }
        } else {
            !exists($$GDAL_LIBS_PATH/lib$${a}*) {
                USE_GDAL_ERROR_MSG += "Library $$GDAL_LIBS_PATH/lib$${a} was not found"
            } else {
                GDAL_LIBS_FOUNDED += $$GDAL_LIBS_PATH/lib$${a}.dylib
            }
        }
    }
}

windows {
    for(a, GDAL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$GDAL_LIBS_PATH/$${a}_i*) {
                USE_GDAL_ERROR_MSG += "Library $$GDAL_LIBS_PATH/$${a}_i was not found"
            } else {
                GDAL_LIBS_FOUNDED += -l$${a}_i
            }
        } else {

            !exists($$GDAL_LIBS_PATH/$${a}_i*) {
                USE_GDAL_ERROR_MSG += "Library $$GDAL_LIBS_PATH/$${a}_i was not found"
            } else {
                GDAL_LIBS_FOUNDED += -l$${a}_i
            }
        }
    }
}

isEmpty(USE_GDAL_ERROR_MSG) {
    equals(CHECK_LIBS_ONLY, false) {
        DEFINES += USE_GDAL
        
        INCLUDEPATH += $$LIB_PATH/$$GDAL_INC_PATH
        
        TR_EXCLUDE  += $$LIB_PATH/$$GDAL_INC_PATH/*
        
        LIBS += -L$$LIB_PATH/$$GDAL_LIBS_PATH
        LIBS += $$GDAL_LIBS_FOUNDED
    }
}

