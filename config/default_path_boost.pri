linux {
    BOOST_BASE_PATH = "/usr/"

    BOOST_LIBS_PATH = "$${BOOST_BASE_PATH}lib/x86_64-linux-gnu"
    BOOST_INC_PATH = "$${BOOST_BASE_PATH}include"
}

win32 {
    BOOST_BASE_PATH = "../3rdparty/boost/"

    BOOST_LIBS_PATH = "$${BOOST_BASE_PATH}lib"
    BOOST_INC_PATH = "$${BOOST_BASE_PATH}include/boost-1_74"
}

macx {
    BOOST_BASE_PATH = "/usr/local/Cellar/boost/1.74.0/"

    BOOST_LIBS_PATH = "$${BOOST_BASE_PATH}lib"
    BOOST_INC_PATH = "$${BOOST_BASE_PATH}include"
}
