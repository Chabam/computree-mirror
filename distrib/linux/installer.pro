TEMPLATE = aux

INSTALLER = Computree_Installer_LINUX_6.0.339
# version : Major.Minor.commit

# Usefull definitions of paths
PATH_SRC = $$PWD/../../../ComputreeInstallRelease/
PATH_PKG = $$PWD/../../../ComputreeInstallRelease/CompuTree.appdir/usr/
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
CLEAN_DATA += rm -rf plugin.B/data/    ;
CLEAN_DATA += rm -rf plugin.C/data/    ;
CLEAN_DATA += rm -rf plugin.D/data/    ;
CLEAN_DATA += rm -rf plugin.E/data/    ;
CLEAN_DATA += rm -rf plugin.G/data/    ;
CLEAN_DATA += rm -rf plugin.L/data/    ;
CLEAN_DATA += rm -rf plugin.M/data/

PREPARE_DATA  = ; mkdir computree.A/data/ computree.A/data/languages/ computree.A/data/bin/ ;
PREPARE_DATA += mkdir computree.B/data/ computree.B/data/examples/  ;
PREPARE_DATA += mkdir library.A/data/   library.A/data/libraries/   library.A/data/libraries/core/   ;
PREPARE_DATA += mkdir library.B/data/   library.B/data/libraries/   library.B/data/libraries/Qt/     ;
PREPARE_DATA += mkdir library.C/data/   library.C/data/libraries/   library.C/data/libraries/gdal/   ;
PREPARE_DATA += mkdir library.D/data/   library.D/data/libraries/   library.D/data/libraries/opencv/ ;
PREPARE_DATA += mkdir library.E/data/   library.E/data/libraries/   library.E/data/libraries/pcl/ library.E/data/libraries/core/ library.E/data/plugins/ ;
PREPARE_DATA += mkdir plugin.A/data/    plugin.A/data/plugins/ ;
PREPARE_DATA += mkdir plugin.B/data/    plugin.B/data/plugins/ ;
PREPARE_DATA += mkdir plugin.C/data/    plugin.C/data/plugins/ ;
PREPARE_DATA += mkdir plugin.D/data/    plugin.D/data/plugins/ ;
PREPARE_DATA += mkdir plugin.E/data/    plugin.E/data/plugins/ ;
PREPARE_DATA += mkdir plugin.G/data/    plugin.G/data/plugins/ ;
PREPARE_DATA += mkdir plugin.L/data/    plugin.L/data/plugins/ ;
PREPARE_DATA += mkdir plugin.M/data/    plugin.M/data/plugins/ ;
PREPARE_DATA += cd $$PATH_PKG ;
PREPARE_DATA += cp $$PATH_DST/computree/meta/*.sh    $$PATH_DST/computree.A/data/         ;
PREPARE_DATA += cp $$PATH_DST/computree/meta/qt.conf $$PATH_DST/computree.A/data/bin/     ;
PREPARE_DATA += cp ../../CompuTreeGui         $$PATH_DST/computree.A/data/bin/            ;
PREPARE_DATA += cp ../CompuTreeGui.png        $$PATH_DST/computree.A/data/                ;
PREPARE_DATA += cp $$PATH_RCS/charter/*.pdf   $$PATH_DST/computree.A/data/                ;
PREPARE_DATA += cp languages/*                $$PATH_DST/computree.A/data/languages/      ;
PREPARE_DATA += cp $$PATH_RCS/examples/*      $$PATH_DST/computree.B/data/examples/       ;
PREPARE_DATA += cp lib/libctlib*              $$PATH_DST/library.A/data/libraries/core/   ;
PREPARE_DATA += cp lib/libQt5*                $$PATH_DST/library.B/data/libraries/Qt/     ;
PREPARE_DATA += find ./plugins -type d -maxdepth 1 -mindepth 1                                                            -exec cp -r '{}' $$PATH_DST/library.B/data/libraries/Qt/ \; ;
PREPARE_DATA += find ./lib     -type f -maxdepth 1 ! \\( -name 'libpcl*' -or -name 'libopencv*' -or -name 'libctlib*' \\) -exec cp '{}' $$PATH_DST/library.C/data/libraries/gdal/ \; ;
PREPARE_DATA += cp lib/libopencv_*            $$PATH_DST/library.D/data/libraries/opencv/ ;
#PREPARE_DATA += cp lib/libpcl_*               $$PATH_DST/library.E/data/libraries/pcl/    ;
PREPARE_DATA += mv $$PATH_DST/library.A/data/libraries/core/libctlibpcl* $$PATH_DST/library.E/data/libraries/core/ ;
PREPARE_DATA += cd $$PATH_SRC ;
PREPARE_DATA += cp plugins/libplug_base.*     $$PATH_DST/plugin.A/data/plugins/           ;
PREPARE_DATA += cp plugins/libplug_generate.* $$PATH_DST/plugin.B/data/plugins/           ;
PREPARE_DATA += cp plugins/libplug_toolkit.*  $$PATH_DST/plugin.C/data/plugins/           ;
PREPARE_DATA += cp plugins/libplug_onf.*      $$PATH_DST/plugin.D/data/plugins/           ;
PREPARE_DATA += cp plugins/libplug_segma.*    $$PATH_DST/plugin.E/data/plugins/           ;
PREPARE_DATA += cp plugins/libplug_mk.*       $$PATH_DST/plugin.G/data/plugins/           ;
PREPARE_DATA += cp plugins/libplug_onfdev.*   $$PATH_DST/plugin.L/data/plugins/           ;
PREPARE_DATA += cp plugins/libplug_ignlif.*   $$PATH_DST/plugin.M/data/plugins/           ;

GENERATE_BIN  = $$[QT_INSTALL_BINS]/../../../Tools/QtInstallerFramework/4.5/bin/binarycreator -c $$PWD/config/config.xml -r $$PWD/../resources/additional.qrc -p $$PWD/packages $$PATH_SRC/$$INSTALLER ;

INPUT = $$PWD/config/config.xml $$PWD/packages
generation.input = INPUT
generation.output = $$INSTALLER
generation.commands = $$CLEAN_DATA $$PREPARE_DATA $$GENERATE_BIN $$CLEAN_DATA
generation.CONFIG += target_predeps no_link combine

QMAKE_EXTRA_COMPILERS += generation
