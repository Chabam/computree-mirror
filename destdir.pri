isEmpty(CT_PREFIX) {
    CT_PREFIX = ..
}

isEmpty(CT_PREFIX_INSTALL) {
    CT_PREFIX_INSTALL = ../..
}

CONFIG(debug, debug|release) {
    CT_DESTDIR_NAME = ComputreeInstallDebug
} else {
    CT_DESTDIR_NAME = ComputreeInstallRelease
}

EXECUTABLE_DESTDIR = $${CT_PREFIX_INSTALL}/$${CT_DESTDIR_NAME}
PLUGIN_DESTDIR = $${EXECUTABLE_DESTDIR}/plugins

message(Element(s) will be installed to $${EXECUTABLE_DESTDIR})

contains(QMAKE_TARGET.arch, x86_64) {
    DEFINES += ENVIRONMENT64
} else {
    isEmpty(QMAKE_TARGET.arch) {
        *-64 {
            DEFINES += ENVIRONMENT64
        } else {
            win32-g++:contains(QMAKE_HOST.arch, x86_64):{
                DEFINES += ENVIRONMENT64
            } else {
                DEFINES += ENVIRONMENT32
            }
        }
    } else {
        win32-g++:contains(QMAKE_HOST.arch, x86_64):{
            DEFINES += ENVIRONMENT64
        } else {
            DEFINES += ENVIRONMENT32
        }
    }
}

contains(DEFINES, ENVIRONMENT64) {
    message(64 bit compilation type detected)
} else {
    message(32 bit compilation type detected)
}
