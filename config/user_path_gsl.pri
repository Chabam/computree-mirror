linux {
    GSL_BASE_PATH = "/usr/"

    GSL_INC_PATH = "$${GSL_BASE_PATH}include/gsl"
    GSL_LIBS_PATH = "$${GSL_BASE_PATH}lib/x86_64-linux-gnu"
}

win32 {
    GSL_BASE_PATH = "/usr/x86_64-w64-mingw32/sys-root/mingw/"

    GSL_INC_PATH = "$${GSL_BASE_PATH}/include"

	GSL_LIBS_PATH = "$${GSL_BASE_PATH}/lib"
	GSL_BIN_PATH = "$${GSL_BASE_PATH}/bin"
}
