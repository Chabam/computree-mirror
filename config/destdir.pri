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
LIBRARY_DESTDIR = $${EXECUTABLE_DESTDIR}/libraries/core

# check folder for windows final install ?

# message(Element(s) will be installed to $${EXECUTABLE_DESTDIR})

equals(QT_ARCH, "x86") {
    # message(32 bit compilation type detected)
}

equals(QT_ARCH, "x86_64") {
}    # message(64 bit compilation type detected)
