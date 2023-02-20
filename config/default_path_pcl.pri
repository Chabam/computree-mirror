linux {
    PCL_BASE_PATH = "/usr/"

    PCL_LIBS_PATH = "$${PCL_BASE_PATH}lib/x86_64-linux-gnu"
    PCL_INC_PATH = "$${PCL_BASE_PATH}include/pcl-1.12"
    PCL_BIN_PATH = "$${PCL_BASE_PATH}bin"
}

win32 {
    PCL_BASE_PATH = "$${VCPKG_PATH}/"

    PCL_INC_PATH = "$${PCL_BASE_PATH}include/pcl"

    CONFIG(debug, debug|release) {
        PCL_LIBS_PATH = "$${PCL_BASE_PATH}debug/lib"
        PCL_BIN_PATH = "$${PCL_BASE_PATH}debug/bin"
    } else {
        PCL_LIBS_PATH = "$${PCL_BASE_PATH}lib"
        PCL_BIN_PATH = "$${PCL_BASE_PATH}bin"
    }
}

macx {
    PCL_BASE_PATH = "/usr/local/Cellar/pcl/1.11.1_8/"

    PCL_LIBS_PATH = "$${PCL_BASE_PATH}lib"
    PCL_INC_PATH = "$${PCL_BASE_PATH}include/pcl-1.11"
    PCL_BIN_PATH = "$${PCL_BASE_PATH}bin"
}
