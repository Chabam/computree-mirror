!exists($${MUPARSER_INC_PATH}) {
    CHECK_ERROR_MSG += "MUPARSER INCLUDE directory not found ! => $${MUPARSER_INC_PATH}"
}

!exists($${MUPARSER_LIBS_PATH}) {
    CHECK_ERROR_MSG += "MUPARSER LIBS directory not found ! => $${MUPARSER_LIBS_PATH}"
}
