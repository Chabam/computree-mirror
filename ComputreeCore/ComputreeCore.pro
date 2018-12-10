CT_PREFIX = ..
CT_PREFIX_INSTALL = ../..
CT_LIB_PREFIX = ../library

COMPUTREE = ctlibplugin

include($${CT_PREFIX}/destdir.pri)
include($${CT_PREFIX}/include_ct_library.pri)
include($${CT_LIB_PREFIX}/library_include_eigen.pri)

CONFIG -= plugin
TARGET = CompuTreeCore
TEMPLATE = lib

QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR = $${EXECUTABLE_DESTDIR}
DEFINES += COMPUTREECORE_LIBRARY

INCLUDEPATH += ./src
INCLUDEPATH += $$CT_LIB_PREFIX

#$${PLUGIN_SHARED_INTERFACE_DIR}/interfaces.h \
HEADERS += src/cdm_configfile.h \
    computreeCore_global.h \
    src/cdm_stepmanageroptions.h \
    src/cdm_stepmanager.h \
    src/cdm_pluginmanager.h \
    src/cdm_scriptmanagerabstract.h \
    src/cdm_stdiolistener.h \
    src/cdm_stdioparser.h \
    src/parserAction/cdm_parseractionmanager.h \
    src/parserAction/cdm_iparseraction.h \
    src/parserAction/cdm_abstractparseraction.h \
    src/parserAction/cdm_parseractiongetplugindir.h \
    src/parserAction/cdm_parseractionsetplugindir.h \
    src/computreeCoreDefaultIOCmd.h \
    src/parserAction/cdm_parseractionlaunchstepmanager.h \
    src/cdm_actionsmanager.h \
    src/cdm_log.h \
    src/cdm_tools.h \
    src/cdm_internationalization.h \
    src/cdm_scriptproblem.h \
    src/cdm_stepsmenumanager.h \
    src/cdm_citationinfo.h \
    src/cdm_hrscriptmanagerxml.h \
    src/cdm_hrscriptxmlwriter.h \
    src/cdm_hrscriptxmlreader.h \
    src/cdm_xmltools.h \
    src/cdm_scriptmanagerxmlallversions.h

SOURCES += \
    src/cdm_configfile.cpp \
    src/cdm_stepmanageroptions.cpp \
    src/cdm_stepmanager.cpp \
    src/cdm_pluginmanager.cpp \
    src/cdm_scriptmanagerabstract.cpp \
    src/cdm_stdiolistener.cpp \
    src/cdm_stdioparser.cpp \
    src/parserAction/cdm_parseractionmanager.cpp \
    src/parserAction/cdm_abstractparseraction.cpp \
    src/parserAction/cdm_parseractiongetplugindir.cpp \
    src/parserAction/cdm_parseractionsetplugindir.cpp \
    src/parserAction/cdm_parseractionlaunchstepmanager.cpp \
    src/cdm_actionsmanager.cpp \
    src/cdm_log.cpp \
    src/cdm_tools.cpp \
    src/cdm_internationalization.cpp \
    src/cdm_scriptproblem.cpp \
    src/cdm_stepsmenumanager.cpp \
    src/cdm_citationinfo.cpp \
    src/cdm_hrscriptmanagerxml.cpp \
    src/cdm_hrscriptxmlwriter.cpp \
    src/cdm_hrscriptxmlreader.cpp \
    src/cdm_xmltools.cpp \
    src/cdm_scriptmanagerxmlallversions.cpp

!equals(PWD, $${OUT_PWD}) {
    error("Shadow build seems to be activated, please desactivated it !")
}
