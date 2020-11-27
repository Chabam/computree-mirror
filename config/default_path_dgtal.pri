linux {
    DGTAL_BASE_PATH = "usr/local/"

    DGTAL_INC_PATH = "$${DGTAL_BASE_PATH}include/DGtal"
    DGTAL_LIBS_PATH = "$${DGTAL_BASE_PATH}lib"
}

win32 {
    DGTAL_BASE_PATH = "$$PWD/../3rdparty/dgtal/"

    DGTAL_INC_PATH = "$${DGTAL_BASE_PATH}include/DGtal"
    DGTAL_LIBS_PATH = "$${DGTAL_BASE_PATH}lib"
}

macx {
    DGTAL_BASE_PATH = "/usr/local/Cellar/dgtal/1.1/"

    DGTAL_INC_PATH = "$${DGTAL_BASE_PATH}include/dgtal"
    DGTAL_LIBS_PATH = "$${DGTAL_BASE_PATH}lib"
}
