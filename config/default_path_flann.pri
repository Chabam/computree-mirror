linux {
    FLANN_BASE_PATH = "/usr/"

    FLANN_LIBS_PATH = "$${FLANN_BASE_PATH}lib/x86_64-linux-gnu"
    FLANN_INC_PATH = "$${FLANN_BASE_PATH}include"
    FLANN_BIN_PATH = "$${FLANN_BASE_PATH}bin"
}

win32 {
    FLANN_BASE_PATH = "$${VCPKG_PATH}/"

    FLANN_INC_PATH = "$${FLANN_BASE_PATH}include"

    CONFIG(debug, debug|release) {
        FLANN_LIBS_PATH = "$${FLANN_BASE_PATH}debug/lib"
        FLANN_BIN_PATH = "$${FLANN_BASE_PATH}debug/bin"
    } else {
        FLANN_LIBS_PATH = "$${FLANN_BASE_PATH}lib"
        FLANN_BIN_PATH = "$${FLANN_BASE_PATH}bin"
    }
}

macx {
    FLANN_BASE_PATH = "/usr/local/Cellar/flann/1.9.2/"

    FLANN_LIBS_PATH = "$${FLANN_BASE_PATH}lib"
    FLANN_INC_PATH = "$${FLANN_BASE_PATH}include"
    FLANN_BIN_PATH = "$${FLANN_BASE_PATH}bin"
}
