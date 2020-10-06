linux {
    GDAL_BASE_PATH = "/usr/"

    GDAL_INC_PATH = "$${GDAL_BASE_PATH}include/gdal"
    GDAL_LIBS_PATH = "$${GDAL_BASE_PATH}lib"
}

win32 {
    GDAL_BASE_PATH = "$$PWD/../ComputreeDependencies/gdal/"

    GDAL_INC_PATH = $${GDAL_BASE_PATH}include
    GDAL_LIBS_PATH = $${GDAL_BASE_PATH}lib
}

macx {
    GDAL_BASE_PATH = "/usr/local/Cellar/gdal/3.1.2_1/"

    GDAL_INC_PATH = "$${GDAL_BASE_PATH}include"
    GDAL_LIBS_PATH = "$${GDAL_BASE_PATH}lib"
}
