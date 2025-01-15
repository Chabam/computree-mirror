linux {
    GDAL_BASE_PATH = "/usr/"

    GDAL_INC_PATH = "$${GDAL_BASE_PATH}include/gdal"
    GDAL_LIBS_PATH = "$${GDAL_BASE_PATH}lib"
    GDAL_BIN_PATH = $${GDAL_BASE_PATH}bin
}

win32 {
    GDAL_BASE_PATH = "/usr/lib/mxe/usr/x86_64-w64-mingw32.shared"

    GDAL_INC_PATH = $${GDAL_BASE_PATH}/include
    GDAL_LIBS_PATH = "$${GDAL_BASE_PATH}/lib"
    GDAL_BIN_PATH = "$${GDAL_BASE_PATH}/bin"
}
