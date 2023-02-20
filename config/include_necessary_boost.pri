
equals(CHECK_LIBS_ONLY, false) {
    greaterThan(QT_MAJOR_VERSION, 4) {
        load(moc)
        QMAKE_MOC += -DBOOST_INCLUDE_GUARD_GOES_HERE
    }
}

isEmpty(USE_BOOST_ERROR_MSG) {
    equals(CHECK_LIBS_ONLY, false) {
        DEFINES += USE_BOOST

        win32 {
            ABS_PATH = $$absolute_path($$BOOST_INC_PATH)
            TMP_LIB_PATH = $$LIB_PATH/

            equals(ABS_PATH, $$BOOST_INC_PATH) {
                TMP_LIB_PATH = ""
            }

            INCLUDEPATH *= $$TMP_LIB_PATH$$BOOST_INC_PATH
            TR_EXCLUDE  *= $$TMP_LIB_PATH$$BOOST_INC_PATH/*
            LIBS *= -L$$TMP_LIB_PATH$$BOOST_LIBS_PATH
        } else {
            INCLUDEPATH *= $$BOOST_INC_PATH
            TR_EXCLUDE  *= $$BOOST_INC_PATH/*
            LIBS *= -L$$BOOST_LIBS_PATH
        }
    }
} else {
    error($$USE_BOOST_ERROR_MSG)
}
