!exists($${DGTAL_INC_PATH}) {
    USE_DGTAL_ERROR_MSG += "DGTAL INCLUDE directory not found ! => $${DGTAL_INC_PATH}"
}

!exists($${DGTAL_LIBS_PATH}) {
    USE_DGTAL_ERROR_MSG += "DGTAL LIBS directory not found ! => $${DGTAL_LIBS_PATH}"
}
