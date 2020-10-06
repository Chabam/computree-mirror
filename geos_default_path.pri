linux {
    GEOS_BASE_PATH = "/usr/"

    GEOS_INC_PATH = "$${GEOS_BASE_PATH}include"
    GEOS_LIBS_PATH = "$${GEOS_BASE_PATH}lib/x86_64-linux-gnu"
}

win32 {
    GEOS_BASE_PATH = "$$PWD/../ComputreeDependencies/geos/"

    GEOS_INC_PATH = $${GEOS_BASE_PATH}include
    GEOS_LIBS_PATH = $${GEOS_BASE_PATH}lib
}

macx {
    GEOS_BASE_PATH = "/usr/local/Cellar/geos/3.8.1_1/"

    GEOS_INC_PATH = "$${GEOS_BASE_PATH}include"
    GEOS_LIBS_PATH = "$${GEOS_BASE_PATH}lib"
}
