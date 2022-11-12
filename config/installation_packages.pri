# Starting from computreev6 directory
CT_PREFIX_INSTALL = ..
include(destdir.pri)

# Usefull definitions of paths
DESTDIR = $${EXECUTABLE_DESTDIR}
DESTDIRFULL = $$PWD/../$$DESTDIR
LIB = $$DESTDIR/libraries

# Final deployment of binaries : make a standalone package (this avoid to do the usual 'make install')
win32 {
    # Copy other libraries dependencies
    include(default_path_opencv.pri)
    CONFIG(release, debug|release) : lib_opencv.files += $$PWD/$$OPENCV_LIBS_PATH/../bin/opencv_world450.dll
    CONFIG(debug,   debug|release) : lib_opencv.files += $$PWD/$$OPENCV_LIBS_PATH/../bin/opencv_world450d.dll
    lib_opencv.path = $$LIB/opencv

    include(default_path_gdal.pri)
    lib_gdal.files += $$PWD/$$GDAL_BASE_PATH/bin/*.dll
    lib_gdal.path = $$LIB/gdal

    INSTALLS += lib_opencv lib_gdal

    include(default_path_pcl.pri)
    exists($$PCL_BASE_PATH) {
        CONFIG(release, debug|release) : lib_pcl.files += $$PWD/$$PCL_BASE_PATH/bin/*[^d].dll
        CONFIG(debug,   debug|release) : lib_pcl.files += $$PWD/$$PCL_BASE_PATH/bin/*d.dll
        lib_pcl.path = $$LIB/pcl

        INSTALLS += lib_pcl
    }

    # Usefull definitions of paths
    WIN_PATH = $$DESTDIRFULL
    WIN_PATH ~= s,/,\\,g
    PWDC = $$PWD

    # Deployment using WinDeployQt
    qt_deploy_options = --force --no-translations --angle --compiler-runtime --plugindir $$LIB/Qt --libdir $$LIB/Qt
    CONFIG(release, debug|release) {
        qt_deploy_options += --release
    }
    CONFIG(debug,   debug|release) {
        qt_deploy_options += --debug
    }

    qt_deploy_cmd1 = $$[QT_INSTALL_BINS]/windeployqt.exe $$DESTDIR $$qt_deploy_options &&
    qt_deploy_cmd2 = copy "distrib\windows\qt.conf" $$WIN_PATH && copy "distrib\windows\CompuTreeGui.cmd" $$WIN_PATH &&
    qt_deploy_cmd3 = move $$WIN_PATH\libraries\Qt\opengl32sw.dll $$WIN_PATH\opengl32.dll &&
    qt_deploy_cmd4 = if not exist $$WIN_PATH\languages\ mkdir $$WIN_PATH\languages\ && call lrelease.bat && cd .. && forfiles /s /m *_*.qm /c \"cmd /c copy @path $$WIN_PATH\languages\" && cd $$PWDC
    CONFIG(release, debug|release) {
        qt_deploy_cmd5 = && move $$WIN_PATH\libraries\Qt\vc_redist.x64.exe $$WIN_PATH
    }

    qt_deploy.path = $$DESTDIR
    CONFIG(release, debug|release) {
        qt_deploy.extra = $$qt_deploy_cmd1 $$qt_deploy_cmd2 $$qt_deploy_cmd3 $$qt_deploy_cmd4 $$qt_deploy_cmd5
    }
    CONFIG(debug,   debug|release) {
        qt_deploy.extra = $$qt_deploy_cmd1 $$qt_deploy_cmd2 $$qt_deploy_cmd3 $$qt_deploy_cmd4
    }

    INSTALLS += qt_deploy
}

linux {
    # Usefull definitions of paths
    QT_PATH = $$[QT_INSTALL_BINS]/..
    DEPLOYTOOL_PATH = $$[QT_INSTALL_BINS]

    # Deployment using LinuxDeployQt
    APPDIR      = $$DESTDIRFULL/CompuTree.appdir/usr
    DIR_LIBS    = $$APPDIR/lib
    DIR_PLUGINS = $$APPDIR/plugins

    qt_deploy_cmd0 = find ../ -type f -name "*_*.qm" -exec cp {} $$DESTDIRFULL/languages \;
    qt_deploy_cmd1 = mkdir -p $$APPDIR/bin $$APPDIR/lib $$APPDIR/plugins $$APPDIR/languages $$APPDIR/share/applications $$APPDIR/share/icons/hicolor/256x256/apps & wait ;
    qt_deploy_cmd2 = cp distrib/linux/Computree.desktop $$APPDIR/share/applications/CompuTreeGui.desktop ; cp ComputreeGui/resource/Icones/Icone_256x256.png $$APPDIR/share/icons/hicolor/256x256/apps/CompuTreeGui.png ; cp ComputreeGui/resource/Icones/Icone_256x256.png $$APPDIR/../CompuTreeGui.png & wait ;
    qt_deploy_cmd3 = cp $$LIB/core/*.so $$APPDIR/lib ; cp $$DESTDIRFULL/plugins/*.so $$APPDIR/plugins ; cp $$DESTDIRFULL/CompuTreeGui $$APPDIR/bin/CompuTreeGui & wait ;
    # Update the rpath in the various libraries/plugins we have to make sure they'll be loadable in an Appimage context (if needed : apt install patchelf)
    qt_deploy_cmd4 = cd $$DIR_LIBS    ; for target in $$DIR_LIBS/libctli*     ; do for file in libctli*; do patchelf --replace-needed "\$\$file" "\\\$\$ORIGIN/\$\$file" "\$\$target"; done; done & wait ;
    qt_deploy_cmd5 = cd $$DIR_LIBS    ; for target in $$DIR_PLUGINS/libplug_* ; do for file in libctli*; do patchelf --replace-needed "\$\$file" "\\\$\$ORIGIN/../lib/\$\$file" "\$\$target"; done; done & wait ;
    qt_deploy_cmd6 = cd $$DIR_LIBS    ; for file   in libctli*  ; do patchelf --replace-needed "\$\$file" "\\\$\$ORIGIN/../lib/\$\$file"     $$APPDIR/bin/CompuTreeGui; done & wait ;
    qt_deploy_cmd7 = cd $$DIR_PLUGINS ; for file   in libplug_* ; do patchelf --replace-needed "\$\$file" "\\\$\$ORIGIN/../plugins/\$\$file" $$APPDIR/bin/CompuTreeGui; done & wait ;
    qt_deploy_cmd8 = export LD_LIBRARY_PATH=$$[QT_INSTALL_LIBS]:$$APPDIR/lib:$$QT_PATH${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH} & wait ;
    # Build the image (if not installed, download it from https://github.com/probonopd/linuxdeployqt and paste it into Qt's binary folder)
    qt_deploy_cmd9 = $$DEPLOYTOOL_PATH/linuxdeployqt-7-x86_64.AppImage $$APPDIR/share/applications/CompuTreeGui.desktop -executable=$$APPDIR/bin/CompuTreeGui -no-translations -bundle-non-qt-libs -extra-plugins=$$APPDIR/lib,$$APPDIR/plugins -unsupported-allow-new-glibc & wait ;
    qt_deploy_cmd10 = rm -f $$APPDIR/../AppRun ; cp $$PWD/../distrib/linux/AppRun $$APPDIR/../AppRun ; chmod +x $$APPDIR/../AppRun ; cp $$DESTDIRFULL/languages/* $$APPDIR/languages & wait ;
    qt_deploy_cmd11 = cd $$DESTDIRFULL ; $$DEPLOYTOOL_PATH/linuxdeployqt-7-x86_64.AppImage $$APPDIR/bin/CompuTreeGui -no-translations -unsupported-allow-new-glibc -appimage & wait ;

    qt_deploy.path = $$DESTDIR
    qt_deploy.extra = $$qt_deploy_cmd0 $$qt_deploy_cmd1 $$qt_deploy_cmd2 $$qt_deploy_cmd3 $$qt_deploy_cmd4 $$qt_deploy_cmd5 $$qt_deploy_cmd6 $$qt_deploy_cmd7 $$qt_deploy_cmd8 $$qt_deploy_cmd9 $$qt_deploy_cmd10 $$qt_deploy_cmd11

    INSTALLS += qt_deploy
}

macx {
    # Usefull definitions of paths
    DIR_FRAMEWORKS = $$DESTDIRFULL/CompuTreeGui.app/Contents/Frameworks
    DIR_PLUGINS    = $$DESTDIRFULL/CompuTreeGui.app/Contents/PlugIns
    DIR_TRANSLATE  = $$DESTDIRFULL/CompuTreeGui.app/Contents/Languages

    # Deployment using MacDeployQt
    qt_deploy_cmd0 = find ../ -type f -name "*_*.qm" -exec cp {} $$DESTDIRFULL/languages \;
    qt_deploy_cmd1 = mkdir -p $$DIR_FRAMEWORKS $$DIR_PLUGINS  & wait ; cp $$DESTDIRFULL/libraries/core/*.dylib $$DIR_FRAMEWORKS ; cp $$DESTDIRFULL/plugins/*.dylib $$DIR_PLUGINS ; cp $$DESTDIRFULL/languages/release/*.qm $$DIR_TRANSLATE & wait ;
    qt_deploy_cmd2 = cd $$DIR_FRAMEWORKS ; for target in $$DIR_FRAMEWORKS/libctli*; do for file in libctli*; do install_name_tool -change "\$\$file" @executable_path/../Frameworks/"\$\$file" "\$\$target"; done; done & wait ;
    qt_deploy_cmd3 = cd $$DIR_FRAMEWORKS ; for target in $$DIR_PLUGINS/libplug_*  ; do for file in libctli*; do install_name_tool -change "\$\$file" @executable_path/../Frameworks/"\$\$file" "\$\$target"; done; done & wait ;
    qt_deploy_cmd4 = cd $$DIR_PLUGINS    ; for file   in libplug_*  ; do install_name_tool -add_rpath @executable_path/../Frameworks/"\$\$file" $$DIR_FRAMEWORKS/../MacOS/CompuTreeGui; done & wait ;
    qt_deploy_cmd5 = $$[QT_INSTALL_BINS]/macdeployqt $$DESTDIRFULL/CompuTreeGui.app -libpath=$$DIR_FRAMEWORKS & wait ;
    # We don't use DMG format separately, but it can be done by uncommenting this line and adding the step to the INSTALL command below
    #qt_deploy_cmd5 = $$[QT_INSTALL_BINS]/macdeployqt $$DESTDIRFULL/CompuTreeGui.app -libpath=$$DIR_FRAMEWORKS -dmg & wait ;

    qt_deploy.path = $$DESTDIR
    qt_deploy.extra = $$qt_deploy_cmd0 $$qt_deploy_cmd1 $$qt_deploy_cmd2 $$qt_deploy_cmd3 $$qt_deploy_cmd4 $$qt_deploy_cmd5

    INSTALLS += qt_deploy
}
