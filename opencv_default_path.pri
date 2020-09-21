unix {
    OPENCV_BASE_PATH = "/usr/"

    OPENCV_INC_PATH = "$${OPENCV_BASE_PATH}include"
    OPENCV_LIBS_PATH = "$${OPENCV_BASE_PATH}lib/x86_64-linux-gnu"
    OPENCV_VERSION = 320
}

windows {
    OPENCV_BASE_PATH = "$$PWD/../ComputreeDependencies/opencv/build/"

    OPENCV_INC_PATH = "$${OPENCV_BASE_PATH}include"
    OPENCV_LIBS_PATH = "$${OPENCV_BASE_PATH}x64/vc14/lib"
    OPENCV_VERSION = 440
}
