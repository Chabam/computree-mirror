win32 {
    ABS_PATH = $$absolute_path($$EIGEN_INC_PATH)
    TMP_LIB_PATH = $$LIB_PATH

    equals(ABS_PATH, $$EIGEN_INC_PATH) {
        TMP_LIB_PATH = ""
    }

    INCLUDEPATH += $$TMP_LIB_PATH$$EIGEN_INC_PATH
    TR_EXCLUDE  += $$TMP_LIB_PATH$$EIGEN_INC_PATH/*
} else {
    INCLUDEPATH += $$EIGEN_INC_PATH
    TR_EXCLUDE  += $$EIGEN_INC_PATH/*
}
