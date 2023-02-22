
equals(CHECK_LIBS_ONLY, false) {
    greaterThan(QT_MAJOR_VERSION, 4) {
        load(moc)
        QMAKE_MOC += -DBOOST_INCLUDE_GUARD_GOES_HERE
    }
}

isEmpty(USE_BOOST_ERROR_MSG) {
    equals(CHECK_LIBS_ONLY, false) {
        DEFINES += USE_BOOST

        INCLUDEPATH *= $$BOOST_INC_PATH
        TR_EXCLUDE  *= $$BOOST_INC_PATH/*
        LIBS *= -L$$BOOST_LIBS_PATH
    }
} else {
    error($$USE_BOOST_ERROR_MSG)
}
