# Starting from computreev6 directory
CT_PREFIX_INSTALL = ..
include(destdir.pri)

# Usefull definitions of paths
DESTDIR = $${EXECUTABLE_DESTDIR}
DESTDIRFULL = $$PWD/../$$DESTDIR
LIB = $$DESTDIR/libraries

# For final deployment of binaries to make a standalone package (this avoid to do the usual 'make install')
win32 {
    # Common part for deployment
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

    # Specific part for deployment
    qt_deploy_options = --force --no-translations --angle --compiler-runtime --plugindir $$LIB/Qt --libdir $$LIB/Qt
    CONFIG(release, debug|release) : qt_deploy_options += --release
    CONFIG(debug,   debug|release) : qt_deploy_options += --debug

    qt_deploy_cmd1 = $$[QT_INSTALL_BINS]/windeployqt.exe $$DESTDIR $$qt_deploy_options &&
    qt_deploy_cmd2 = copy "tools\qt.conf" $$WIN_PATH && copy "tools\CompuTreeGui.cmd" $$WIN_PATH && #cmd /C $$DESTDIRFULL/CompuTreeGui.cmd  && del $$WIN_PATH\win_setenv.cmd &&
    qt_deploy_cmd3 = move $$WIN_PATH\libraries\Qt\opengl32sw.dll $$WIN_PATH\opengl32.dll &&
    qt_deploy_cmd4 = move $$WIN_PATH\libraries\Qt\vc_redist.x64.exe $$WIN_PATH

    qt_deploy.path = $$DESTDIR
    qt_deploy.extra = $$qt_deploy_cmd1 $$qt_deploy_cmd2 $$qt_deploy_cmd3 $$qt_deploy_cmd4

    INSTALLS += qt_deploy
}

linux {
    # Common part for deployment
    include(default_path_opencv.pri)
    lib_opencv.files += $$OPENCV_LIBS_PATH/libopencv_*.so*
    lib_opencv.path = $$LIB/opencv

    include(default_path_gdal.pri)
    lib_gdal.files += $$GDAL_LIBS_PATH/libgdal.so*
    lib_gdal.path = $$LIB/gdal

    INSTALLS += lib_opencv lib_gdal

    include(default_path_pcl.pri)
    exists($$PCL_LIBS_PATH/libpcl_common.so) {
        lib_pcl.files += $$PCL_LIBS_PATH/libpcl_*.so*
        lib_pcl.path = $$LIB/pcl

        INSTALLS += lib_pcl
    }

    # Usefull definitions of paths
    QT_PATH = $$[QT_INSTALL_BINS]/..
    DEPLOYTOOL_PATH = $$[QT_INSTALL_BINS]

    # Specific part for deployment
    qt_deploy_cmd1 = cp tools/Computree.desktop tools/qt.conf tools/linux_setenv.sh $$DESTDIR ; cp ComputreeGui/resource/Icones/Icone_256x256.png $$DESTDIR/CompuTreeGui.png & wait ;
    qt_deploy_cmd2 = export LD_LIBRARY_PATH=$$[QT_INSTALL_LIBS]:$$QT_PATH${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH} ;
    qt_deploy_cmd3 = $$DEPLOYTOOL_PATH/linuxdeployqt-7-x86_64.AppImage $$DESTDIRFULL/CompuTreeGui -unsupported-allow-new-glibc & wait ;
    # We don't use AppImage format separately, but it can be done by uncommenting this line and adding the step to the INSTALL command below
    #qt_deploy_cmd4 = cd $$DESTDIR ; $$DEPLOYTOOL_PATH/linuxdeployqt-7-x86_64.AppImage $$PWDUP/$$DESTDIR/CompuTreeGui -unsupported-allow-new-glibc -appimage & wait ; cd $$PWD/.. ;
    qt_deploy_cmd5 = mkdir -p $$LIB/Qt ; rm -rf $$LIB/Qt/* & wait ;
    qt_deploy_cmd6 = mv $$DESTDIR/lib/* $$LIB/Qt ; mv $$DESTDIR/plugins/*/ $$LIB/Qt & wait ;
    qt_deploy_cmd7 = rm -rf $$DESTDIR/doc $$DESTDIR/lib $$DESTDIR/translations ; rm -f $$LIB/Qt/libct*.so

    qt_deploy.path = $$DESTDIR
    qt_deploy.extra = $$qt_deploy_cmd1 $$qt_deploy_cmd2 $$qt_deploy_cmd3 $$qt_deploy_cmd5 $$qt_deploy_cmd6 $$qt_deploy_cmd7

    INSTALLS += qt_deploy
}

macx {
    # Usefull definitions of paths
    DIR_FRAMEWORKS = $$DESTDIRFULL/CompuTreeGui.app/Contents/Frameworks
    DIR_PLUGINS    = $$DESTDIRFULL/CompuTreeGui.app/Contents/PlugIns

    # Specific part for deployment
    qt_deploy_cmd1 = mkdir -p $$DIR_FRAMEWORKS $$DIR_PLUGINS  & wait ; cp $$DESTDIRFULL/libraries/core/*.dylib $$DIR_FRAMEWORKS ; cp $$DESTDIRFULL/plugins/*.dylib $$DIR_PLUGINS & wait ;
    qt_deploy_cmd2 = cd $$DIR_FRAMEWORKS ; for target in $$DIR_FRAMEWORKS/libctli*; do for file in libctli*; do install_name_tool -change "\$\$file" @executable_path/../Frameworks/"\$\$file" "\$\$target"; done; done & wait ;
    qt_deploy_cmd3 = cd $$DIR_FRAMEWORKS ; for target in $$DIR_PLUGINS/libplug_*  ; do for file in libctli*; do install_name_tool -change "\$\$file" @executable_path/../Frameworks/"\$\$file" "\$\$target"; done; done & wait ;
    qt_deploy_cmd4 = cd $$DIR_PLUGINS    ; for file   in libplug_*  ; do install_name_tool -add_rpath @executable_path/../Frameworks/"\$\$file" $$DIR_FRAMEWORKS/../MacOS/CompuTreeGui; done & wait ;
    qt_deploy_cmd5 = $$[QT_INSTALL_BINS]/macdeployqt $$DESTDIRFULL/CompuTreeGui.app -libpath=$$DIR_FRAMEWORKS & wait ;
    # We don't use DMG format separately, but it can be done by uncommenting this line and adding the step to the INSTALL command below
    #qt_deploy_cmd5 = $$[QT_INSTALL_BINS]/macdeployqt $$DESTDIRFULL/CompuTreeGui.app -libpath=$$DIR_FRAMEWORKS -dmg & wait ;

    qt_deploy.path = $$DESTDIR
    qt_deploy.extra = $$qt_deploy_cmd1 $$qt_deploy_cmd2 $$qt_deploy_cmd3 $$qt_deploy_cmd4 $$qt_deploy_cmd5

    INSTALLS += qt_deploy
}