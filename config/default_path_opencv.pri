linux {
    OPENCV_BASE_PATH = "/usr/"

    OPENCV_INC_PATH = "$${OPENCV_BASE_PATH}include/opencv4"
    OPENCV_LIBS_PATH = "$${OPENCV_BASE_PATH}lib/x86_64-linux-gnu"
    OPENCV_VERSION = 420
}

win32-msvc {
    OPENCV_BASE_PATH = "../3rdparty/opencv/build/"

    OPENCV_INC_PATH = "$${OPENCV_BASE_PATH}include"
    OPENCV_LIBS_PATH = "$${OPENCV_BASE_PATH}x64/vc15/lib"
    OPENCV_BIN_PATH = "$${OPENCV_BASE_PATH}x64/vc15/bin"
    OPENCV_VERSION = 450
}

win32-mingw {
    OPENCV_BASE_PATH = "../3rdparty/opencv/"

    OPENCV_INC_PATH = "$${OPENCV_BASE_PATH}include/opencv4"
    OPENCV_LIBS_PATH = "$${OPENCV_BASE_PATH}lib"
    OPENCV_BIN_PATH = "$${OPENCV_BASE_PATH}bin"
    OPENCV_VERSION = 440
}

macx {
    OPENCV_BASE_PATH = "/usr/local/Cellar/opencv/4.5.2_4/"

    OPENCV_INC_PATH = "$${OPENCV_BASE_PATH}include/opencv4"
    OPENCV_LIBS_PATH = "$${OPENCV_BASE_PATH}lib"
    OPENCV_VERSION = 440
}
