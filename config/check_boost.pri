!exists($${BOOST_INC_PATH}) {
    CHECK_ERROR_MSG += "BOOST INCLUDE directory not found ! => $${BOOST_INC_PATH}"
}

!exists($${BOOST_LIBS_PATH}) {
    CHECK_ERROR_MSG += "BOOST LIBS directory not found ! => $${BOOST_LIBS_PATH}"
}
