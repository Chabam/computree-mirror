linux {
    GSL_BASE_PATH = "/usr/"

    GSL_INC_PATH = "$${GSL_BASE_PATH}include/gsl"
    GSL_LIBS_PATH = "$${GSL_BASE_PATH}lib/x86_64-linux-gnu"
}

win32 {
    GSL_BASE_PATH = "/usr/lib/mxe/usr/x86_64-w64-mingw32.shared"

    GSL_INC_PATH = "$${GSL_BASE_PATH}/include"

	GSL_LIBS_PATH = "$${GSL_BASE_PATH}/lib"
	GSL_BIN_PATH = "$${GSL_BASE_PATH}/bin"
}
