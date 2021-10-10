linux {
    PCL_BASE_PATH = "/usr/"

    PCL_LIBS_PATH = "$${PCL_BASE_PATH}lib/x86_64-linux-gnu"
    PCL_INC_PATH = "$${PCL_BASE_PATH}include/pcl-1.10"
}

win32 {
    PCL_BASE_PATH = "../3rdparty/pcl/"

    PCL_LIBS_PATH = "$${PCL_BASE_PATH}lib"
    PCL_BIN_PATH = "$${PCL_BASE_PATH}bin"
    PCL_INC_PATH = "$${PCL_BASE_PATH}include/pcl-1.11"
}

macx {
    PCL_BASE_PATH = "/usr/local/Cellar/pcl/1.11.1_6/"

    PCL_LIBS_PATH = "$${PCL_BASE_PATH}lib"
    PCL_INC_PATH = "$${PCL_BASE_PATH}include/pcl-1.11"
}
