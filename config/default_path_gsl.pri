linux {
    GSL_BASE_PATH = "/usr/"

    GSL_INC_PATH = "$${GSL_BASE_PATH}include/gsl"
    GSL_LIBS_PATH = "$${GSL_BASE_PATH}lib/x86_64-linux-gnu"
}

win32 {
    GSL_BASE_PATH = "../3rdparty/gsl/"

    GSL_INC_PATH = "$${GSL_BASE_PATH}include"
    GSL_LIBS_PATH = "$${GSL_BASE_PATH}lib"
}

macx {
    GSL_BASE_PATH = "/usr/local/Cellar/gsl/2.6/"

    GSL_INC_PATH = "$${GSL_BASE_PATH}include/gsl"
    GSL_LIBS_PATH = "$${GSL_BASE_PATH}lib"
}
