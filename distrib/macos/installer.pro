TEMPLATE = aux

INSTALLER = Computree_Installer_MACOS_6.0.390
# version : Major.Minor.commit

# Usefull definitions of paths
APP = CompuTreeGui.app/Contents
PATH_SRC = $$PWD/../../../ComputreeInstallRelease/$$APP/
PATH_DST = $$PWD/packages
PATH_RCS = $$PWD/../resources

CLEAN_DATA  = cd $$PATH_DST ;
CLEAN_DATA += rm -rf computree.A/data/ ;
CLEAN_DATA += rm -rf computree.B/data/ ;
CLEAN_DATA += rm -rf library.A/data/   ;
CLEAN_DATA += rm -rf library.B/data/   ;
CLEAN_DATA += rm -rf library.C/data/   ;
CLEAN_DATA += rm -rf library.D/data/   ;
CLEAN_DATA += rm -rf library.E/data/   ;
CLEAN_DATA += rm -rf plugin.A/data/    ;
CLEAN_DATA += rm -rf plugin.C/data/    ;
CLEAN_DATA += rm -rf plugin.D/data/    ;
CLEAN_DATA += rm -rf plugin.E/data/    ;
CLEAN_DATA += rm -rf plugin.G/data/    ;
CLEAN_DATA += rm -rf plugin.L/data/    ;
CLEAN_DATA += rm -rf plugin.M/data/    ;
CLEAN_DATA += rm -rf plugin.N/data/    ;
CLEAN_DATA += rm -rf plugin.O/data/

PREPARE_DATA  = ; mkdir computree.A/data/ computree.A/data/CompuTreeGui.app/ computree.A/data/$$APP/ computree.A/data/$$APP/Frameworks/ computree.A/data/$$APP/PlugIns/ ;
PREPARE_DATA += mkdir computree.B/data/ computree.B/data/CompuTreeGui.app/ computree.B/data/$$APP/ computree.B/data/$$APP/Resources/  ;
PREPARE_DATA += mkdir library.A/data/   library.A/data/CompuTreeGui.app/   library.A/data/$$APP/   library.A/data/$$APP/Frameworks/   ;
PREPARE_DATA += mkdir library.B/data/   library.B/data/CompuTreeGui.app/   library.B/data/$$APP/   library.B/data/$$APP/Frameworks/   library.B/data/$$APP/PlugIns/ ;
PREPARE_DATA += mkdir library.C/data/   library.C/data/CompuTreeGui.app/   library.C/data/$$APP/   library.C/data/$$APP/Frameworks/   ;
PREPARE_DATA += mkdir library.D/data/   library.D/data/CompuTreeGui.app/   library.D/data/$$APP/   library.D/data/$$APP/Frameworks/   ;
PREPARE_DATA += mkdir library.E/data/   library.E/data/CompuTreeGui.app/   library.E/data/$$APP/   library.E/data/$$APP/Frameworks/   library.E/data/$$APP/PlugIns/ ;
PREPARE_DATA += mkdir plugin.A/data/    plugin.A/data/CompuTreeGui.app/    plugin.A/data/$$APP/    plugin.A/data/$$APP/PlugIns/       ;
PREPARE_DATA += mkdir plugin.C/data/    plugin.C/data/CompuTreeGui.app/    plugin.C/data/$$APP/    plugin.C/data/$$APP/PlugIns/       ;
PREPARE_DATA += mkdir plugin.D/data/    plugin.D/data/CompuTreeGui.app/    plugin.D/data/$$APP/    plugin.D/data/$$APP/PlugIns/       ;
PREPARE_DATA += mkdir plugin.E/data/    plugin.E/data/CompuTreeGui.app/    plugin.E/data/$$APP/    plugin.E/data/$$APP/PlugIns/       ;
PREPARE_DATA += mkdir plugin.G/data/    plugin.G/data/CompuTreeGui.app/    plugin.G/data/$$APP/    plugin.G/data/$$APP/PlugIns/       ;
PREPARE_DATA += mkdir plugin.L/data/    plugin.L/data/CompuTreeGui.app/    plugin.L/data/$$APP/    plugin.L/data/$$APP/PlugIns/       ;
PREPARE_DATA += mkdir plugin.M/data/    plugin.M/data/CompuTreeGui.app/    plugin.M/data/$$APP/    plugin.M/data/$$APP/PlugIns/       ;
PREPARE_DATA += mkdir plugin.N/data/    plugin.N/data/CompuTreeGui.app/    plugin.N/data/$$APP/    plugin.N/data/$$APP/PlugIns/       ;
PREPARE_DATA += mkdir plugin.O/data/    plugin.O/data/CompuTreeGui.app/    plugin.O/data/$$APP/    plugin.O/data/$$APP/PlugIns/       ;
PREPARE_DATA += cd $$PATH_SRC ;
PREPARE_DATA += cp -r MacOS Resources        $$PATH_DST/computree.A/data/$$APP/           ;
PREPARE_DATA += cp Info.plist PkgInfo        $$PATH_DST/computree.A/data/$$APP/           ;
PREPARE_DATA += cp -r languages              $$PATH_DST/computree.A/data/$$APP/           ;
PREPARE_DATA += cp $$PATH_RCS/charter/*.pdf  $$PATH_DST/computree.A/data/                 ;
PREPARE_DATA += cp -r $$PATH_RCS/examples    $$PATH_DST/computree.B/data/                 ;
PREPARE_DATA += cp Frameworks/libctlib*      $$PATH_DST/library.A/data/$$APP/Frameworks/  ;
PREPARE_DATA += cp -r Frameworks/Qt*         $$PATH_DST/library.B/data/$$APP/Frameworks/  ;
PREPARE_DATA += find ./PlugIns    -type d -maxdepth 1 -mindepth 1                                                            -exec cp -r '{}' $$PATH_DST/library.B/data/$$APP/PlugIns/ \; ;
PREPARE_DATA += find ./Frameworks -type f -maxdepth 1 ! \\( -name 'libpcl*' -or -name 'libopencv*' -or -name 'libctlib*' \\) -exec cp '{}' $$PATH_DST/library.C/data/$$APP/Frameworks/ \; ;
PREPARE_DATA += cp Frameworks/libopencv*     $$PATH_DST/library.D/data/$$APP/Frameworks/  ;
PREPARE_DATA += cp Frameworks/libpcl*        $$PATH_DST/library.E/data/$$APP/Frameworks/  ;
PREPARE_DATA += mv $$PATH_DST/library.A/data/$$APP/Frameworks/libctlibpcl* $$PATH_DST/library.E/data/$$APP/Frameworks/ ;
PREPARE_DATA += cp PlugIns/libplug_base.*    $$PATH_DST/plugin.A/data/$$APP/PlugIns/      ;
PREPARE_DATA += cp PlugIns/libplug_toolkit.* $$PATH_DST/plugin.C/data/$$APP/PlugIns/      ;
PREPARE_DATA += cp PlugIns/libplug_onf.*     $$PATH_DST/plugin.D/data/$$APP/PlugIns/      ;
PREPARE_DATA += cp PlugIns/libplug_segma.*   $$PATH_DST/plugin.E/data/$$APP/PlugIns/      ;
PREPARE_DATA += cp PlugIns/libplug_mk.*      $$PATH_DST/plugin.G/data/$$APP/PlugIns/      ;
PREPARE_DATA += cp PlugIns/libplug_onfdev.*  $$PATH_DST/plugin.L/data/$$APP/PlugIns/      ;
PREPARE_DATA += cp PlugIns/libplug_ignlif.*  $$PATH_DST/plugin.M/data/$$APP/PlugIns/      ;
PREPARE_DATA += cp PlugIns/libplug_lvoxv3.*  $$PATH_DST/plugin.N/data/$$APP/PlugIns/      ;
PREPARE_DATA += cp PlugIns/libplug_ams3d.*   $$PATH_DST/plugin.O/data/$$APP/PlugIns/      ;

GENERATE_BIN  = $$[QT_INSTALL_BINS]/../../../Tools/QtInstallerFramework/4.5/bin/binarycreator -c $$PWD/config/config.xml -r $$PWD/../resources/additional.qrc -p $$PWD/packages $$PATH_SRC/../../$$INSTALLER ;

INPUT = $$PWD/config/config.xml $$PWD/packages
generation.input = INPUT
generation.output = $$INSTALLER
generation.commands = $$CLEAN_DATA $$PREPARE_DATA $$GENERATE_BIN $$CLEAN_DATA
generation.CONFIG += target_predeps no_link combine

QMAKE_EXTRA_COMPILERS += generation
