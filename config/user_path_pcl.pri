linux {
    PCL_BASE_PATH = "/usr/"

    PCL_LIBS_PATH = "$${PCL_BASE_PATH}lib/x86_64-linux-gnu"
    PCL_INC_PATH = "$${PCL_BASE_PATH}include/pcl-1.12"
    PCL_BIN_PATH = "$${PCL_BASE_PATH}bin"
}

win32 {
    PCL_BASE_PATH = "/usr/x86_64-w64-mingw32/sys-root/mingw/"

    PCL_INC_PATH = "$${PCL_BASE_PATH}/include/pcl-1.13"

    PCL_LIBS_PATH = "$${PCL_BASE_PATH}/lib"
    PCL_BIN_PATH = "$${PCL_BASE_PATH}/bin"
}
