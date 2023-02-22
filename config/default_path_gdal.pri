linux {
    GDAL_BASE_PATH = "/usr/"

    GDAL_INC_PATH = "$${GDAL_BASE_PATH}include/gdal"
    GDAL_LIBS_PATH = "$${GDAL_BASE_PATH}lib"
    GDAL_BIN_PATH = $${GDAL_BASE_PATH}bin
}

win32 {
    GDAL_BASE_PATH = "$${VCPKG_PATH}/"

    GDAL_INC_PATH = $${GDAL_BASE_PATH}include

    CONFIG(debug, debug|release) {
        GDAL_LIBS_PATH = "$${GDAL_BASE_PATH}debug/lib"
        GDAL_BIN_PATH = "$${GDAL_BASE_PATH}debug/bin"
    } else {
        GDAL_LIBS_PATH = "$${GDAL_BASE_PATH}lib"
        GDAL_BIN_PATH = "$${GDAL_BASE_PATH}bin"
    }
}

macx {
    GDAL_BASE_PATH = "/usr/local/Cellar/gdal/3.6.2_2/"

    GDAL_INC_PATH = "$${GDAL_BASE_PATH}include"
    GDAL_LIBS_PATH = "$${GDAL_BASE_PATH}lib"
    GDAL_BIN_PATH = $${GDAL_BASE_PATH}bin
}
