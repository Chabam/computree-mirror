linux {
    LASZIP_BASE_PATH = "/usr/"

    LASZIP_LIBS_PATH = "$${LASZIP_BASE_PATH}lib/x86_64-linux-gnu"
    LASZIP_INC_PATH = "$${LASZIP_BASE_PATH}include"
    LASZIP_BIN_PATH = "$${LASZIP_BASE_PATH}bin"
}

win32 {
    LASZIP_BASE_PATH = "/usr/lib/mxe/usr/x86_64-w64-mingw32.shared"

    LASZIP_INC_PATH = "$${LASZIP_BASE_PATH}/include"

    LASZIP_LIBS_PATH = "$${LASZIP_BASE_PATH}/lib"
    LASZIP_BIN_PATH = "$${LASZIP_BASE_PATH}/bin"
}
