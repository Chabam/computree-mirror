!exists($${PCL_INC_PATH}) {
    CHECK_ERROR_MSG += "PCL INCLUDE directory not found ! => $${PCL_INC_PATH}"
}

!exists($${PCL_LIBS_PATH}) {
    CHECK_ERROR_MSG += "PCL LIBS directory not found ! => $${PCL_LIBS_PATH}"
}
