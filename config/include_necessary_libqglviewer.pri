INCLUDEPATH *= $$LIB_PATH$$LIBQGLVIEWER_PATH
TR_EXCLUDE  *= $$LIB_PATH$$LIBQGLVIEWER_PATH/*

DEFINES += QGLVIEWER_STATIC

win32 {
    CONFIG(debug, debug|release) {
        LIBS += -lQGLViewerd
    } else {
        LIBS += -lQGLViewer
    }
    LIBS += -L$$LIB_PATH$$LIBQGLVIEWER_PATH
}

linux {
    LIBS += $$LIB_PATH$$LIBQGLVIEWER_PATH/libQGLViewer-qt5.a
}

macx {
    LIBS += $$LIB_PATH$$LIBQGLVIEWER_PATH/libQGLViewer.a
}
