PCL_LIB_ADD =
PCL_LIB_ADD += pcl_common
PCL_LIB_ADD += pcl_features
PCL_LIB_ADD += pcl_filters
PCL_LIB_ADD += pcl_io_ply
PCL_LIB_ADD += pcl_io
PCL_LIB_ADD += pcl_kdtree
PCL_LIB_ADD += pcl_keypoints
PCL_LIB_ADD += pcl_octree
PCL_LIB_ADD += pcl_outofcore
PCL_LIB_ADD += pcl_people
PCL_LIB_ADD += pcl_recognition
PCL_LIB_ADD += pcl_registration
PCL_LIB_ADD += pcl_sample_consensus
PCL_LIB_ADD += pcl_search
PCL_LIB_ADD += pcl_segmentation
PCL_LIB_ADD += pcl_surface
PCL_LIB_ADD += pcl_tracking
PCL_LIB_ADD += pcl_visualization

PCL_LIBS_FOUNDED =

linux {
    QHULL_LIB_ADD =
    QHULL_LIB_ADD += qhull

    for(a, PCL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$PCL_LIBS_PATH/lib$${a}*) {
                USE_PCL_ERROR_MSG += "Library $$PCL_LIBS_PATH/lib$${a} was not found"
            } else {
                PCL_LIBS_FOUNDED += $$PCL_LIBS_PATH/lib$${a}.so
            }
        } else {
            !exists($$PCL_LIBS_PATH/lib$${a}*) {
                USE_PCL_ERROR_MSG += "Library $$PCL_LIBS_PATH/lib$${a} was not found"
            } else {
                PCL_LIBS_FOUNDED += $$PCL_LIBS_PATH/lib$${a}.so
            }
        }
    }

    for(a, QHULL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$QHULL_LIBS_PATH/lib$${a}*) {
                USE_PCL_ERROR_MSG += "Library $$QHULL_LIBS_PATH/lib$${a} was not found"
            } else {
                PCL_LIBS_FOUNDED += $$QHULL_LIBS_PATH/lib$${a}.so
            }
        } else {
            !exists($$QHULL_LIBS_PATH/lib$${a}*) {
                USE_PCL_ERROR_MSG += "Library $$QHULL_LIBS_PATH/lib$${a} was not found"
            } else {
                PCL_LIBS_FOUNDED += $$QHULL_LIBS_PATH/lib$${a}.so
            }
        }
    }
}

macx {
    QHULL_LIB_ADD =
    QHULL_LIB_ADD += qhull

    for(a, PCL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$PCL_LIBS_PATH/lib$${a}*) {
                USE_PCL_ERROR_MSG += "Library $$PCL_LIBS_PATH/lib$${a} was not found"
            } else {
                PCL_LIBS_FOUNDED += $$PCL_LIBS_PATH/lib$${a}.dylib
            }
        } else {
            !exists($$PCL_LIBS_PATH/lib$${a}*) {
                USE_PCL_ERROR_MSG += "Library $$PCL_LIBS_PATH/lib$${a} was not found"
            } else {
                PCL_LIBS_FOUNDED += $$PCL_LIBS_PATH/lib$${a}.dylib
            }
        }
    }

    for(a, QHULL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$QHULL_LIBS_PATH/lib$${a}*) {
                USE_PCL_ERROR_MSG += "Library $$QHULL_LIBS_PATH/lib$${a} was not found"
            } else {
                PCL_LIBS_FOUNDED += $$QHULL_LIBS_PATH/lib$${a}.dylib
            }
        } else {
            !exists($$QHULL_LIBS_PATH/lib$${a}*) {
                USE_PCL_ERROR_MSG += "Library $$QHULL_LIBS_PATH/lib$${a} was not found"
            } else {
                PCL_LIBS_FOUNDED += $$QHULL_LIBS_PATH/lib$${a}.dylib
            }
        }
    }
}

windows {
    QHULL_LIB_ADD =
    QHULL_LIB_ADD += qhullstatic
    #QHULL_LIB_ADD += qhullstatic_d

    for(a, PCL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$PCL_LIBS_PATH/$${a}*) {
                USE_PCL_ERROR_MSG += "Library $$PCL_LIBS_PATH/$${a} was not found"
            } else {
                PCL_LIBS_FOUNDED += -l$${a}
            }
        } else {

            !exists($$PCL_LIBS_PATH/$${a}*) {
                USE_PCL_ERROR_MSG += "Library $$PCL_LIBS_PATH/$${a} was not found"
            } else {
                PCL_LIBS_FOUNDED += -l$${a}
            }
        }
    }


    for(a, QHULL_LIB_ADD) {
        CONFIG(debug, debug|release) {
            !exists($$QHULL_LIBS_PATH/$${a}_d*) {
                USE_PCL_ERROR_MSG += "Library $$QHULL_LIBS_PATH/$${a}_d was not found"
            } else {
                PCL_LIBS_FOUNDED += -l$${a}_d
            }
        } else {
            !exists($$QHULL_LIBS_PATH/$${a}*) {
                USE_PCL_ERROR_MSG += "Library $$QHULL_LIBS_PATH/$${a} was not found"
            } else {
                PCL_LIBS_FOUNDED += -l$${a}
            }
        }
    }
}

equals(CHECK_LIBS_ONLY, false) {
    greaterThan(QT_MAJOR_VERSION, 4) {
        load(moc)
        QMAKE_MOC += -DBOOST_INCLUDE_GUARD_GOES_HERE
    }
}

isEmpty(USE_PCL_ERROR_MSG) {
    equals(CHECK_LIBS_ONLY, false) {
        DEFINES += USE_PCL
        DEFINES += USE_BOOST
        
        INCLUDEPATH += $$PCL_INC_PATH
        INCLUDEPATH += $$EIGEN_INC_PATH
        INCLUDEPATH += $$QHULL_INC_PATH
        INCLUDEPATH += $$FLANN_INC_PATH
        INCLUDEPATH += $$BOOST_INC_PATH
        
        TR_EXCLUDE  += $$PCL_INC_PATH/*
        TR_EXCLUDE  += $$EIGEN_INC_PATH/*
        TR_EXCLUDE  += $$QHULL_INC_PATH/*
        TR_EXCLUDE  += $$FLANN_INC_PATH/*
        TR_EXCLUDE  += $$BOOST_INC_PATH/*

        DEPENDPATH  += $$QHULL_INC_PATH

        LIBS += -L$$PCL_LIBS_PATH
        LIBS += -L$$BOOST_LIBS_PATH
        LIBS += -L$$QHULL_LIBS_PATH

        LIBS += $$PCL_LIBS_FOUNDED
    }
}

