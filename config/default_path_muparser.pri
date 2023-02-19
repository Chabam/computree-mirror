linux {
    MUPARSER_BASE_PATH = "/usr/"

    MUPARSER_LIBS_PATH = "$${MUPARSER_BASE_PATH}lib/x86_64-linux-gnu"
    MUPARSER_INC_PATH = "$${MUPARSER_BASE_PATH}include"
    MUPARSER_BIN_PATH = "$${MUPARSER_BASE_PATH}bin"
}

win32 {
    MUPARSER_BASE_PATH = "$${VCPKG_PATH}/"

    MUPARSER_INC_PATH = "$${MUPARSER_BASE_PATH}include"

    CONFIG(debug, debug|release) {
        MUPARSER_LIBS_PATH = "$${MUPARSER_BASE_PATH}debug/lib"
        MUPARSER_BIN_PATH = "$${MUPARSER_BASE_PATH}debug/bin"
    } else {
        MUPARSER_LIBS_PATH = "$${MUPARSER_BASE_PATH}lib"
        MUPARSER_BIN_PATH = "$${MUPARSER_BASE_PATH}bin"
    }
}

macx {
    MUPARSER_BASE_PATH = "/usr/local/Cellar/muparser/2.3.2_2/"

    MUPARSER_LIBS_PATH = "$${MUPARSER_BASE_PATH}lib"
    MUPARSER_INC_PATH = "$${MUPARSER_BASE_PATH}include"
    MUPARSER_BIN_PATH = "$${MUPARSER_BASE_PATH}bin"
}
