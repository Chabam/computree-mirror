OPENCV_LIB_ADD =
OPENCV_LIB_ADD += opencv_calib3d
OPENCV_LIB_ADD += opencv_core
OPENCV_LIB_ADD += opencv_dnn
OPENCV_LIB_ADD += opencv_features2d
OPENCV_LIB_ADD += opencv_flann
OPENCV_LIB_ADD += opencv_highgui
OPENCV_LIB_ADD += opencv_imgcodecs
OPENCV_LIB_ADD += opencv_imgproc
OPENCV_LIB_ADD += opencv_ml
OPENCV_LIB_ADD += opencv_objdetect
OPENCV_LIB_ADD += opencv_photo
OPENCV_LIB_ADD += opencv_stitching
OPENCV_LIB_ADD += opencv_video
OPENCV_LIB_ADD += opencv_videoio
# OPENCV_LIB_ADD += opencv_gapi # for Linux, not used in general

OPENCV_LIBS_FOUND =

linux {
    for(a, OPENCV_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$OPENCV_LIBS_PATH/lib$${a}*) {
                USE_OPENCV_ERROR_MSG += "Library $$OPENCV_LIBS_PATH/lib$${a} was not found"
            } else {
                OPENCV_LIBS_FOUND += $$OPENCV_LIBS_PATH/lib$${a}.so
            }
        } else {
            !exists($$OPENCV_LIBS_PATH/lib$${a}*) {
                USE_OPENCV_ERROR_MSG += "Library $$OPENCV_LIBS_PATH/lib$${a} was not found"
            } else {
                OPENCV_LIBS_FOUND += $$OPENCV_LIBS_PATH/lib$${a}.so
            }
        }
    }
}

macx {
    for(a, OPENCV_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$OPENCV_LIBS_PATH/lib$${a}*) {
                USE_OPENCV_ERROR_MSG += "Library $$OPENCV_LIBS_PATH/lib$${a} was not found"
            } else {
                OPENCV_LIBS_FOUND += $$OPENCV_LIBS_PATH/lib$${a}.dylib
            }
        } else {
            !exists($$OPENCV_LIBS_PATH/lib$${a}*) {
                USE_OPENCV_ERROR_MSG += "Library $$OPENCV_LIBS_PATH/lib$${a} was not found"
            } else {
                OPENCV_LIBS_FOUND += $$OPENCV_LIBS_PATH/lib$${a}.dylib
            }
        }
    }
}

windows {
    for(a, OPENCV_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$OPENCV_LIBS_PATH/$${a}4d*) {
                USE_OPENCV_ERROR_MSG += "Library $$OPENCV_LIBS_PATH/$${a}4d was not found"
            } else {
                OPENCV_LIBS_FOUND += -l$${a}4d
            }
        } else {
            !exists($$OPENCV_LIBS_PATH/$${a}4*) {
                USE_OPENCV_ERROR_MSG += "Library $$OPENCV_LIBS_PATH/$${a}4 was not found"
            } else {
                OPENCV_LIBS_FOUND += -l$${a}4
            }
        }
    }
}

isEmpty(USE_OPENCV_ERROR_MSG) {
    equals(CHECK_LIBS_ONLY, false) {
        DEFINES += USE_OPENCV

        win32 {
            ABS_PATH = $$absolute_path($$OPENCV_INC_PATH)
            TMP_LIB_PATH = $$LIB_PATH

            equals(ABS_PATH, $$OPENCV_INC_PATH) {
                TMP_LIB_PATH = ""
            }

            INCLUDEPATH *= $$TMP_LIB_PATH$$OPENCV_INC_PATH
            TR_EXCLUDE  *= $$TMP_LIB_PATH$$OPENCV_INC_PATH/*
            LIBS *= -L$$TMP_LIB_PATH$$OPENCV_LIBS_PATH

            !isEmpty(OPENCV_BIN_PATH) {
                LIBS *= -L$${TMP_LIB_PATH}$$OPENCV_BIN_PATH
            }
        } else {
            INCLUDEPATH *= $$OPENCV_INC_PATH
            TR_EXCLUDE  *= $$OPENCV_INC_PATH/*
            LIBS        *= -L$$OPENCV_LIBS_PATH
        }

        LIBS *= $$OPENCV_LIBS_FOUND
    }
} else {
    error($$USE_OPENCV_ERROR_MSG)
}
