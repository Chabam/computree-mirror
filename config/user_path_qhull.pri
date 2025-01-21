linux {
    QHULL_BASE_PATH = "/usr/"

    QHULL_INC_PATH = "$${QHULL_BASE_PATH}include"
    QHULL_LIBS_PATH = "$${QHULL_BASE_PATH}lib/x86_64-linux-gnu"
}

win32 {
    QHULL_BASE_PATH = "/usr/x86_64-w64-mingw32/sys-root/mingw/"

    QHULL_INC_PATH = "$${QHULL_BASE_PATH}/include"
    QHULL_LIBS_PATH = "$${QHULL_BASE_PATH}/lib"
    QHULL_BIN_PATH = "$${QHULL_BASE_PATH}/bin"
}
