linux {
    OPENCV_BASE_PATH = "/usr/"

    OPENCV_LIBS_PATH = "$${OPENCV_BASE_PATH}lib/x86_64-linux-gnu"
    OPENCV_INC_PATH = "$${OPENCV_BASE_PATH}include/opencv4"
    OPENCV_BIN_PATH = "$${OPENCV_BASE_PATH}bin"
}

win32 {
    OPENCV_BASE_PATH = "/usr/x86_64-w64-mingw32/sys-root/mingw"

    OPENCV_INC_PATH = "$${OPENCV_BASE_PATH}/include/opencv4"

    OPENCV_LIBS_PATH = "$${OPENCV_BASE_PATH}/lib"
    OPENCV_BIN_PATH = "$${OPENCV_BASE_PATH}/bin"
}

macx {
    OPENCV_BASE_PATH = "/usr/local/Cellar/opencv/4.7.0_1/"

    OPENCV_LIBS_PATH = "$${OPENCV_BASE_PATH}lib"
    OPENCV_INC_PATH = "$${OPENCV_BASE_PATH}include/opencv4"
    OPENCV_BIN_PATH = "$${OPENCV_BASE_PATH}bin"
}
