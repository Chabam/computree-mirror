!exists($${GEOS_INC_PATH}) {
    CHECK_ERROR_MSG += "GEOS INCLUDE directory not found ! => $${GEOS_INC_PATH}"
}

!exists($${GEOS_LIBS_PATH}) {
    CHECK_ERROR_MSG += "GEOS LIBS directory not found ! => $${GEOS_LIBS_PATH}"
}
