!exists($${LASZIP_INC_PATH}) {
    CHECK_ERROR_MSG += "LASZIP INCLUDE directory not found ! => $${LASZIP_INC_PATH}"
}

!exists($${LASZIP_LIBS_PATH}) {
    CHECK_ERROR_MSG += "LASZIP LIBS directory not found ! => $${LASZIP_LIBS_PATH}"
}
