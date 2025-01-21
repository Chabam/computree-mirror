linux {
    OPENCV_LIBS_PATH = "/lib64/"
    OPENCV_INC_PATH = "/usr/include/opencv4"
    OPENCV_BIN_PATH = "/usr/bin"
}

win32 {
    OPENCV_BASE_PATH = "$${VCPKG_PATH}/"

    OPENCV_INC_PATH = "$${OPENCV_BASE_PATH}include/opencv4" # it indicates '2', but it's OpenCV4

    CONFIG(debug, debug|release) {
        OPENCV_LIBS_PATH = "$${OPENCV_BASE_PATH}debug/lib"
        OPENCV_BIN_PATH = "$${OPENCV_BASE_PATH}debug/bin"
    } else {
        OPENCV_LIBS_PATH = "$${OPENCV_BASE_PATH}lib"
        OPENCV_BIN_PATH = "$${OPENCV_BASE_PATH}bin"
    }
}

macx {
    OPENCV_BASE_PATH = "/usr/local/Cellar/opencv/4.7.0_1/"

    OPENCV_LIBS_PATH = "$${OPENCV_BASE_PATH}lib"
    OPENCV_INC_PATH = "$${OPENCV_BASE_PATH}include/opencv4"
    OPENCV_BIN_PATH = "$${OPENCV_BASE_PATH}bin"
}
