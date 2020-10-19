OPENCV_LIBS_FOUNDED =

unix {

    OPENCV_LIB_ADD =

    exists($$OPENCV_LIBS_PATH/libopencv_core*) {
        OPENCV_LIB_ADD += opencv_calib3d
        OPENCV_LIB_ADD += opencv_core
        OPENCV_LIB_ADD += opencv_features2d
        OPENCV_LIB_ADD += opencv_flann
        OPENCV_LIB_ADD += opencv_highgui
        OPENCV_LIB_ADD += opencv_imgcodecs
        OPENCV_LIB_ADD += opencv_imgproc
        OPENCV_LIB_ADD += opencv_ml
        OPENCV_LIB_ADD += opencv_objdetect
        OPENCV_LIB_ADD += opencv_photo
        OPENCV_LIB_ADD += opencv_shape
        OPENCV_LIB_ADD += opencv_stitching
        OPENCV_LIB_ADD += opencv_superres
        OPENCV_LIB_ADD += opencv_video
        OPENCV_LIB_ADD += opencv_videoio
        OPENCV_LIB_ADD += opencv_videostab
    } else {
        #OPENCV_LIB_ADD += opencv_ts
        OPENCV_LIB_ADD += opencv_world
    }

    linux {
        for(a, OPENCV_LIB_ADD) {
            CONFIG(debug, debug|release) {
                !exists($$OPENCV_LIBS_PATH/lib$${a}*) {
                    USE_OPENCV_ERROR_MSG += "Library $$OPENCV_LIBS_PATH/lib$${a} was not found"
                } else {
                    OPENCV_LIBS_FOUNDED += $$OPENCV_LIBS_PATH/lib$${a}.so
                }
            } else {
                !exists($$OPENCV_LIBS_PATH/lib$${a}*) {
                    USE_OPENCV_ERROR_MSG += "Library $$OPENCV_LIBS_PATH/lib$${a} was not found"
                } else {
                    OPENCV_LIBS_FOUNDED += $$OPENCV_LIBS_PATH/lib$${a}.so
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
                    OPENCV_LIBS_FOUNDED += $$OPENCV_LIBS_PATH/lib$${a}.dylib
                }
            } else {
                !exists($$OPENCV_LIBS_PATH/lib$${a}*) {
                    USE_OPENCV_ERROR_MSG += "Library $$OPENCV_LIBS_PATH/lib$${a} was not found"
                } else {
                    OPENCV_LIBS_FOUNDED += $$OPENCV_LIBS_PATH/lib$${a}.dylib
                }
            }
        }
    }
}

win32-g++ {
    OPENCV_LIB_ADD =

    exists($$OPENCV_LIBS_PATH/libopencv_core*) {
        OPENCV_LIB_ADD += opencv_calib3d
        OPENCV_LIB_ADD += opencv_core
        OPENCV_LIB_ADD += opencv_features2d
        OPENCV_LIB_ADD += opencv_flann
        OPENCV_LIB_ADD += opencv_highgui
        OPENCV_LIB_ADD += opencv_imgcodecs
        OPENCV_LIB_ADD += opencv_imgproc
        OPENCV_LIB_ADD += opencv_ml
        OPENCV_LIB_ADD += opencv_objdetect
        OPENCV_LIB_ADD += opencv_photo
        OPENCV_LIB_ADD += opencv_stitching
    } else {
        #OPENCV_LIB_ADD += opencv_ts
        OPENCV_LIB_ADD += opencv_world
    }

    for(a, OPENCV_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$OPENCV_LIBS_PATH/lib$${a}*) {
                USE_OPENCV_ERROR_MSG += "Library $$OPENCV_LIBS_PATH/lib$${a} was not found"
            } else {
                OPENCV_LIBS_FOUNDED += $$OPENCV_LIBS_PATH/lib$${a}.dll.a
            }
        } else {
            !exists($$OPENCV_LIBS_PATH/lib$${a}*) {
                USE_OPENCV_ERROR_MSG += "Library $$OPENCV_LIBS_PATH/lib$${a} was not found"
            } else {
                OPENCV_LIBS_FOUNDED += $$OPENCV_LIBS_PATH/lib$${a}.dll.a
            }
        }
    }
} else {

    windows {

        OPENCV_LIB_ADD =
        #OPENCV_LIB_ADD += opencv_ts
        OPENCV_LIB_ADD += opencv_world

        for(a, OPENCV_LIB_ADD) {
            CONFIG(debug, debug|release) {
                !exists($$OPENCV_LIBS_PATH/$${a}$${OPENCV_VERSION}d*) {
                    USE_OPENCV_ERROR_MSG += "Library $$OPENCV_LIBS_PATH/$${a}$${OPENCV_VERSION}d was not found"
                } else {
                    OPENCV_LIBS_FOUNDED += -l$${a}$${OPENCV_VERSION}d
                }
            } else {
                !exists($$OPENCV_LIBS_PATH/$${a}$${OPENCV_VERSION}*) {
                    USE_OPENCV_ERROR_MSG += "Library $$OPENCV_LIBS_PATH/$${a}$${OPENCV_VERSION} was not found"
                } else {
                    OPENCV_LIBS_FOUNDED += -l$${a}$${OPENCV_VERSION}
                }
            }
        }
    }

}

isEmpty(USE_OPENCV_ERROR_MSG) {
    equals(CHECK_LIBS_ONLY, false) {
        DEFINES += USE_OPENCV

        win32 {
            INCLUDEPATH += $$LIB_PATH/$$OPENCV_INC_PATH
        
            TR_EXCLUDE  += $$LIB_PATH/$$OPENCV_INC_PATH/*

            LIBS += -L$$LIB_PATH/$$OPENCV_LIBS_PATH
        } else {
            INCLUDEPATH += $$OPENCV_INC_PATH

            TR_EXCLUDE  += $$OPENCV_INC_PATH/*

            LIBS += -L$$OPENCV_LIBS_PATH
        }

        LIBS += $$OPENCV_LIBS_FOUNDED
    }
}

