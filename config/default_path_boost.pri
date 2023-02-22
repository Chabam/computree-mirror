linux {
    BOOST_BASE_PATH = "/usr/"

    BOOST_LIBS_PATH = "$${BOOST_BASE_PATH}lib/x86_64-linux-gnu"
    BOOST_INC_PATH = "$${BOOST_BASE_PATH}include"
    BOOST_BIN_PATH = "$${BOOST_BASE_PATH}bin"
}

win32 {
    BOOST_BASE_PATH = "$${VCPKG_PATH}/"

    BOOST_INC_PATH = "$${BOOST_BASE_PATH}include"

    CONFIG(debug, debug|release) {
        BOOST_LIBS_PATH = "$${BOOST_BASE_PATH}debug/lib"
        BOOST_BIN_PATH = "$${BOOST_BASE_PATH}debug/bin"
    } else {
        BOOST_LIBS_PATH = "$${BOOST_BASE_PATH}lib"
        BOOST_BIN_PATH = "$${BOOST_BASE_PATH}bin"
    }
}

macx {
    BOOST_BASE_PATH = "/usr/local/Cellar/boost/1.81.0_1/"

    BOOST_LIBS_PATH = "$${BOOST_BASE_PATH}lib"
    BOOST_INC_PATH = "$${BOOST_BASE_PATH}include"
    BOOST_BIN_PATH = "$${BOOST_BASE_PATH}bin"
}
