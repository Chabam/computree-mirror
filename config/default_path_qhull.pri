linux {
    QHULL_BASE_PATH = "/usr/"

    QHULL_INC_PATH = "$${QHULL_BASE_PATH}include"
    QHULL_LIBS_PATH = "$${QHULL_BASE_PATH}lib/x86_64-linux-gnu"
}

win32 {
    QHULL_BASE_PATH = "$${VCPKG_PATH}/"

    QHULL_INC_PATH = "$${QHULL_BASE_PATH}include"

    CONFIG(debug, debug|release) {
        QHULL_LIBS_PATH = "$${QHULL_BASE_PATH}debug/lib"
        QHULL_BIN_PATH = "$${QHULL_BASE_PATH}debug/bin"
    } else {
        QHULL_LIBS_PATH = "$${QHULL_BASE_PATH}lib"
        QHULL_BIN_PATH = "$${QHULL_BASE_PATH}bin"
    }
}

macx {
    QHULL_BASE_PATH = "/usr/local/Cellar/qhull/2020.1/"

    QHULL_INC_PATH = "$${QHULL_BASE_PATH}include"
    QHULL_LIBS_PATH = "$${QHULL_BASE_PATH}lib"
}
