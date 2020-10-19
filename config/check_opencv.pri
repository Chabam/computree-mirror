!exists($${OPENCV_INC_PATH}) {
    CHECK_ERROR_MSG += "OPENCV INCLUDE directory not found ! => $${OPENCV_INC_PATH}"
}

!exists($${OPENCV_LIBS_PATH}) {
    CHECK_ERROR_MSG += "OPENCV LIBS directory not found ! => $${OPENCV_LIBS_PATH}"
}
