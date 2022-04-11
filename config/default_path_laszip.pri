linux {
    LASZIP_BASE_PATH = "/usr/"

    LASZIP_INC_PATH = "$${LASZIP_BASE_PATH}include"
    LASZIP_LIBS_PATH = "$${LASZIP_BASE_PATH}lib/x86_64-linux-gnu"
}

win32 {
    LASZIP_BASE_PATH = "../3rdparty/laszip/"

    LASZIP_INC_PATH = $${LASZIP_BASE_PATH}include
    LASZIP_LIBS_PATH = $${LASZIP_BASE_PATH}lib
}

macx {
    LASZIP_BASE_PATH = "/usr/local/Cellar/laszip/3.4.3_2/"

    LASZIP_INC_PATH = "$${LASZIP_BASE_PATH}include"
    LASZIP_LIBS_PATH = "$${LASZIP_BASE_PATH}lib"
}
