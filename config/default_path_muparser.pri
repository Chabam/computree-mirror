linux {
    MUPARSER_BASE_PATH = "/usr/"

    MUPARSER_INC_PATH = "$${MUPARSER_BASE_PATH}include"
    MUPARSER_LIBS_PATH = "$${MUPARSER_BASE_PATH}lib/x86_64-linux-gnu"
}

win32 {
    MUPARSER_BASE_PATH = "../3rdparty/muparser/"

    MUPARSER_INC_PATH = $${MUPARSER_BASE_PATH}include
    MUPARSER_LIBS_PATH = $${MUPARSER_BASE_PATH}
}

macx {
    MUPARSER_BASE_PATH = "/usr/local/Cellar/muparser/2.3.2_2/"

    MUPARSER_INC_PATH = "$${MUPARSER_BASE_PATH}include"
    MUPARSER_LIBS_PATH = "$${MUPARSER_BASE_PATH}lib"
}
