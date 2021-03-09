TEMPLATE = aux

INSTALLER = computree_6_0_installer_x86_64_MACOS

# Usefull definitions of paths
PATH_SRC = $$PWD/../../../ComputreeInstallRelease/CompuTreeGui.app/Contents/
PATH_DST = $$PWD/packages
PATH_RCS = $$PWD/../resources
APP = CompuTree.app/Contents

CLEAN_DATA  = cd $$PATH_DST ;
CLEAN_DATA += rm -rf computree.A/data/ ; mkdir computree.A/data/ computree.A/data/CompuTree.app/ computree.A/data/$$APP/ computree.A/data/$$APP/Frameworks/ computree.A/data/$$APP/PlugIns/ ;
CLEAN_DATA += rm -rf computree.B/data/ ; mkdir computree.B/data/ computree.B/data/CompuTree.app/ computree.B/data/$$APP/ computree.B/data/$$APP/Resources/ ;
CLEAN_DATA += rm -rf library.A/data/   ; mkdir library.A/data/   library.A/data/CompuTree.app/   library.A/data/$$APP/   library.A/data/$$APP/Frameworks/   ;
CLEAN_DATA += rm -rf library.B/data/   ; mkdir library.B/data/   library.B/data/CompuTree.app/   library.B/data/$$APP/   library.B/data/$$APP/Frameworks/   library.B/data/$$APP/PlugIns/ ;
CLEAN_DATA += rm -rf library.C/data/   ; mkdir library.C/data/   library.C/data/CompuTree.app/   library.C/data/$$APP/   library.C/data/$$APP/Frameworks/   ;
CLEAN_DATA += rm -rf library.D/data/   ; mkdir library.D/data/   library.D/data/CompuTree.app/   library.D/data/$$APP/   library.D/data/$$APP/Frameworks/   ;
CLEAN_DATA += rm -rf library.E/data/   ; mkdir library.E/data/   library.E/data/CompuTree.app/   library.E/data/$$APP/   library.E/data/$$APP/Frameworks/   library.E/data/$$APP/PlugIns/ ;
CLEAN_DATA += rm -rf plugin.A/data/    ; mkdir plugin.A/data/    plugin.A/data/CompuTree.app/    plugin.A/data/$$APP/    plugin.A/data/$$APP/PlugIns/       ;
CLEAN_DATA += rm -rf plugin.C/data/    ; mkdir plugin.C/data/    plugin.C/data/CompuTree.app/    plugin.C/data/$$APP/    plugin.C/data/$$APP/PlugIns/       ;
CLEAN_DATA += rm -rf plugin.D/data/    ; mkdir plugin.D/data/    plugin.D/data/CompuTree.app/    plugin.D/data/$$APP/    plugin.D/data/$$APP/PlugIns/       ;
CLEAN_DATA += rm -rf plugin.E/data/    ; mkdir plugin.E/data/    plugin.E/data/CompuTree.app/    plugin.E/data/$$APP/    plugin.E/data/$$APP/PlugIns/       ;
CLEAN_DATA += rm -rf plugin.G/data/    ; mkdir plugin.G/data/    plugin.G/data/CompuTree.app/    plugin.G/data/$$APP/    plugin.G/data/$$APP/PlugIns/

PREPARE_DATA  = ; cd $$PATH_SRC ;
PREPARE_DATA += cp -r MacOS Resources        $$PATH_DST/computree.A/data/$$APP/           ;
PREPARE_DATA += cp Info.plist PkgInfo        $$PATH_DST/computree.A/data/$$APP/           ;
PREPARE_DATA += cp $$PATH_RCS/charter/*.pdf  $$PATH_DST/computree.A/data/ ;
PREPARE_DATA += cp -r $$PATH_RCS/examples    $$PATH_DST/computree.B/data/ ;
PREPARE_DATA += cp Frameworks/libctlib*      $$PATH_DST/library.A/data/$$APP/Frameworks/  ;
PREPARE_DATA += cp -r Frameworks/Qt*         $$PATH_DST/library.B/data/$$APP/Frameworks/  ;
PREPARE_DATA += find ./PlugIns    -type d -maxdepth 1 -mindepth 1                                                            -exec cp -r '{}' $$PATH_DST/library.B/data/$$APP/PlugIns/ \; ;
PREPARE_DATA += find ./Frameworks -type f -maxdepth 1 ! \\( -name 'libpcl*' -or -name 'libopencv*' -or -name 'libctlib*' \\) -exec cp '{}' $$PATH_DST/library.C/data/$$APP/Frameworks/ \; ;
PREPARE_DATA += cp Frameworks/libopencv*     $$PATH_DST/library.D/data/$$APP/Frameworks/  ;
PREPARE_DATA += cp Frameworks/libpcl*        $$PATH_DST/library.E/data/$$APP/Frameworks/  ;
PREPARE_DATA += mv $$PATH_DST/library.A/data/$$APP/Frameworks/libctlibpcl* $$PATH_DST/library.E/data/$$APP/Frameworks/ ;
PREPARE_DATA += cp PlugIns/libplug_base.*    $$PATH_DST/plugin.A/data/$$APP/PlugIns/       ;
PREPARE_DATA += cp PlugIns/libplug_toolkit.* $$PATH_DST/plugin.C/data/$$APP/PlugIns/       ;
PREPARE_DATA += cp PlugIns/libplug_onf.*     $$PATH_DST/plugin.D/data/$$APP/PlugIns/       ;
PREPARE_DATA += cp PlugIns/libplug_segma.*   $$PATH_DST/plugin.E/data/$$APP/PlugIns/       ;
PREPARE_DATA += cp PlugIns/libplug_mk.*      $$PATH_DST/plugin.G/data/$$APP/PlugIns/       ;

GENERATE_BIN  = $$[QT_INSTALL_BINS]/../../../Tools/QtInstallerFramework/4.0/bin/binarycreator -c $$PWD/config/config.xml -r $$PWD/../resources/additional.qrc -p $$PWD/packages $$PATH_SRC/../../$$INSTALLER ;

INPUT = $$PWD/config/config.xml $$PWD/packages
generation.input = INPUT
generation.output = $$INSTALLER
generation.commands = $$CLEAN_DATA $$PREPARE_DATA $$GENERATE_BIN $$CLEAN_DATA
generation.CONFIG += target_predeps no_link combine

QMAKE_EXTRA_COMPILERS += generation
