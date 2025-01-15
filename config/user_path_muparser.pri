linux {
    MUPARSER_BASE_PATH = "/usr/"

    MUPARSER_LIBS_PATH = "$${MUPARSER_BASE_PATH}lib/x86_64-linux-gnu"
    MUPARSER_INC_PATH = "$${MUPARSER_BASE_PATH}include"
    MUPARSER_BIN_PATH = "$${MUPARSER_BASE_PATH}bin"
}

win32 {
    MUPARSER_BASE_PATH = "/usr/lib/mxe/usr/x86_64-w64-mingw32.shared"

    MUPARSER_INC_PATH = "$${MUPARSER_BASE_PATH}/include"

    MUPARSER_LIBS_PATH = "$${MUPARSER_BASE_PATH}/lib"
    MUPARSER_BIN_PATH = "$${MUPARSER_BASE_PATH}/bin"
}
