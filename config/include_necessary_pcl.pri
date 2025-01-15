#PCL_LIB_ADD =
PCL_LIB_ADD += pcl_common
PCL_LIB_ADD += pcl_features
PCL_LIB_ADD += pcl_filters
PCL_LIB_ADD += pcl_io
PCL_LIB_ADD += pcl_io_ply
PCL_LIB_ADD += pcl_kdtree
PCL_LIB_ADD += pcl_keypoints
PCL_LIB_ADD += pcl_ml
PCL_LIB_ADD += pcl_octree
PCL_LIB_ADD += pcl_recognition
PCL_LIB_ADD += pcl_registration
PCL_LIB_ADD += pcl_sample_consensus
PCL_LIB_ADD += pcl_search
PCL_LIB_ADD += pcl_segmentation
PCL_LIB_ADD += pcl_stereo
PCL_LIB_ADD += pcl_surface
PCL_LIB_ADD += pcl_tracking
PCL_LIB_ADD += lz4

linux | macx {
PCL_LIB_ADD += pcl_apps
PCL_LIB_ADD += pcl_outofcore
PCL_LIB_ADD += pcl_people
PCL_LIB_ADD += pcl_visualization
}

PCL_LIBS_FOUND =

linux {
    for(a, PCL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$PCL_LIBS_PATH/lib$${a}*) {
                USE_PCL_ERROR_MSG += "Library $$PCL_LIBS_PATH/lib$${a} was not found"
            } else {
                PCL_LIBS_FOUND += $$PCL_LIBS_PATH/lib$${a}.so
            }
        } else {
            !exists($$PCL_LIBS_PATH/lib$${a}*) {
                USE_PCL_ERROR_MSG += "Library $$PCL_LIBS_PATH/lib$${a} was not found"
            } else {
                PCL_LIBS_FOUND += $$PCL_LIBS_PATH/lib$${a}.so
            }
        }
    }
}

macx {
    for(a, PCL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$PCL_LIBS_PATH/lib$${a}*) {
                USE_PCL_ERROR_MSG += "Library $$PCL_LIBS_PATH/lib$${a} was not found"
            } else {
                PCL_LIBS_FOUND += $$PCL_LIBS_PATH/lib$${a}.dylib
            }
        } else {
            !exists($$PCL_LIBS_PATH/lib$${a}*) {
                USE_PCL_ERROR_MSG += "Library $$PCL_LIBS_PATH/lib$${a} was not found"
            } else {
                PCL_LIBS_FOUND += $$PCL_LIBS_PATH/lib$${a}.dylib
            }
        }
    }
}

windows {
    for(a, PCL_LIB_ADD) {
        !exists($$PCL_LIBS_PATH/lib$${a}*) {
            USE_PCL_ERROR_MSG += "Library $$PCL_LIBS_PATH/$${a} was not found"
        } else {
            PCL_LIBS_FOUND += -l$${a}
        }
    }
}

isEmpty(USE_PCL_ERROR_MSG) {
    equals(CHECK_LIBS_ONLY, false) {
        DEFINES += USE_PCL

        INCLUDEPATH *= $$PCL_INC_PATH
        TR_EXCLUDE  *= $$PCL_INC_PATH/*
        LIBS *= -L$$PCL_LIBS_PATH

        LIBS *= $$PCL_LIBS_FOUND
    }
} else {
    error($$USE_PCL_ERROR_MSG)
}
