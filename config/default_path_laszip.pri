linux {
    LASZIP_BASE_PATH = "/usr/"

    LASZIP_LIBS_PATH = "$${LASZIP_BASE_PATH}lib/x86_64-linux-gnu"
    LASZIP_INC_PATH = "$${LASZIP_BASE_PATH}include"
    LASZIP_BIN_PATH = "$${LASZIP_BASE_PATH}bin"
}

win32 {
    LASZIP_BASE_PATH = "$${VCPKG_PATH}/"

    LASZIP_INC_PATH = "$${LASZIP_BASE_PATH}include"

    CONFIG(debug, debug|release) {
        LASZIP_LIBS_PATH = "$${LASZIP_BASE_PATH}debug/lib"
        LASZIP_BIN_PATH = "$${LASZIP_BASE_PATH}debug/bin"
    } else {
        LASZIP_LIBS_PATH = "$${LASZIP_BASE_PATH}lib"
        LASZIP_BIN_PATH = "$${LASZIP_BASE_PATH}bin"
    }
}

macx {
    LASZIP_BASE_PATH = "/usr/local/Cellar/laszip/3.4.3/"

    LASZIP_LIBS_PATH = "$${LASZIP_BASE_PATH}lib"
    LASZIP_INC_PATH = "$${LASZIP_BASE_PATH}include"
    LASZIP_BIN_PATH = "$${LASZIP_BASE_PATH}bin"
}
