linux {
    QHULL_BASE_PATH = "/usr/"

    QHULL_INC_PATH = "$${QHULL_BASE_PATH}include"
    QHULL_LIBS_PATH = "$${QHULL_BASE_PATH}lib/x86_64-linux-gnu"
}

win32 {
    QHULL_BASE_PATH = "../3rdparty/qhull/"

    QHULL_INC_PATH = "$${QHULL_BASE_PATH}include"
    QHULL_LIBS_PATH = "$${QHULL_BASE_PATH}lib"
    QHULL_BIN_PATH = "$${QHULL_BASE_PATH}bin"
}

macx {
    QHULL_BASE_PATH = "/usr/local/Cellar/qhull/2020.1/"

    QHULL_INC_PATH = "$${QHULL_BASE_PATH}include"
    QHULL_LIBS_PATH = "$${QHULL_BASE_PATH}lib"
}
