linux {
    FLANN_BASE_PATH = "/usr/"

    FLANN_LIBS_PATH = "$${FLANN_BASE_PATH}lib/x86_64-linux-gnu"
    FLANN_INC_PATH = "$${FLANN_BASE_PATH}include"
    FLANN_BIN_PATH = "$${FLANN_BASE_PATH}bin"
}

win32 {
    FLANN_BASE_PATH = "/usr/x86_64-w64-mingw32/sys-root/mingw/"

    FLANN_INC_PATH = "$${FLANN_BASE_PATH}/include"

    FLANN_LIBS_PATH = "$${FLANN_BASE_PATH}/lib"
    FLANN_BIN_PATH = "$${FLANN_BASE_PATH}/bin"
}
