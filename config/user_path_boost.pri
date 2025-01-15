linux {
    BOOST_BASE_PATH = "/usr/"

    BOOST_LIBS_PATH = "$${BOOST_BASE_PATH}lib/x86_64-linux-gnu"
    BOOST_INC_PATH = "$${BOOST_BASE_PATH}include"
    BOOST_BIN_PATH = "$${BOOST_BASE_PATH}bin"
}

win32 {
    BOOST_BASE_PATH = "/usr/lib/mxe/usr/x86_64-w64-mingw32.shared"

    BOOST_INC_PATH = "$${BOOST_BASE_PATH}/include"

    BOOST_LIBS_PATH = "$${BOOST_BASE_PATH}/lib"
    BOOST_BIN_PATH = "$${BOOST_BASE_PATH}/bin"
}
