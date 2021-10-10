linux {
    FLANN_BASE_PATH = "/usr/"

    FLANN_INC_PATH = "$${FLANN_BASE_PATH}include"
}

win32 {
    FLANN_BASE_PATH = "../3rdparty/flann/"

    FLANN_INC_PATH = "$${FLANN_BASE_PATH}include"
    FLANN_BIN_PATH = "$${FLANN_BASE_PATH}bin"
}

macx {
    FLANN_BASE_PATH = "/usr/local/Cellar/flann/1.9.1_9/"

    FLANN_INC_PATH = "$${FLANN_BASE_PATH}include"
}
