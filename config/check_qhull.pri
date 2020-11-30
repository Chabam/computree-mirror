!exists($${QHULL_INC_PATH}) {
    CHECK_ERROR_MSG += "QHULL INCLUDE directory not found ! => $${QHULL_INC_PATH}"
}

!exists($${QHULL_LIBS_PATH}) {
    CHECK_ERROR_MSG += "QHULL LIBS directory not found ! => $${QHULL_LIBS_PATH}"
}
