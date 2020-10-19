linux {
    PCL_BASE_PATH = "/usr/"

    PCL_LIBS_PATH = "$${PCL_BASE_PATH}lib/x86_64-linux-gnu"
    PCL_INC_PATH = "$${PCL_BASE_PATH}include/pcl-1.10"
    QHULL_INC_PATH = "$${PCL_BASE_PATH}include"
    QHULL_LIBS_PATH = "$${PCL_BASE_PATH}lib/x86_64-linux-gnu"
    FLANN_INC_PATH = "$${PCL_BASE_PATH}include"
    BOOST_LIBS_PATH = "$${PCL_BASE_PATH}lib/x86_64-linux-gnu"
    BOOST_INC_PATH = "$${PCL_BASE_PATH}include"
}

win32 {
    PCL_BASE_PATH = "../3rdparty/pcl/"
    PCL_3RDPARTY_PATH = "$${PCL_BASE_PATH}3rdParty/"

    PCL_LIBS_PATH = "$${PCL_BASE_PATH}lib"
    PCL_INC_PATH = "$${PCL_BASE_PATH}include/pcl-1.11"
    QHULL_INC_PATH = "$${PCL_3RDPARTY_PATH}Qhull/include"
    QHULL_LIBS_PATH = "$${PCL_3RDPARTY_PATH}Qhull/lib"
    FLANN_INC_PATH = "$${PCL_3RDPARTY_PATH}FLANN/include"
    BOOST_LIBS_PATH = "$${PCL_3RDPARTY_PATH}Boost/lib"
    BOOST_INC_PATH = "$${PCL_3RDPARTY_PATH}Boost/include/boost-1_74"
}

macx {
    PCL_BASE_PATH = "/usr/local/Cellar/pcl/1.11.1_1.reinstall/"
    QHULL_BASE_PATH = "/usr/local/Cellar/qhull/2020.1/"
    FLANN_BASE_PATH = "/usr/local/Cellar/flann/1.9.1_9/"
    BOOST_BASE_PATH = "/usr/local/Cellar/boost/1.73.0/"

    PCL_LIBS_PATH = "$${PCL_BASE_PATH}lib"
    PCL_INC_PATH = "$${PCL_BASE_PATH}include/pcl-1.11"
    QHULL_INC_PATH = "$${QHULL_BASE_PATH}include"
    QHULL_LIBS_PATH = "$${QHULL_BASE_PATH}lib"
    FLANN_INC_PATH = "$${FLANN_BASE_PATH}include"
    BOOST_LIBS_PATH = "$${BOOST_BASE_PATH}lib"
    BOOST_INC_PATH = "$${BOOST_BASE_PATH}include"
}
