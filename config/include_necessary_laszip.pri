win32 {
    INCLUDEPATH += $$LIB_PATH$$LASZIP_INC_PATH
    LIBS += -llaszip -L$$LIB_PATH$$LASZIP_LIBS_PATH
}

linux | macx {
    INCLUDEPATH += $$LASZIP_INC_PATH
    LIBS += -llaszip -L$$LASZIP_LIBS_PATH
}
