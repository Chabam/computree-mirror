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
    VCPKG_PATH = $$(VCPKG)
    VCPKG_PATH ~= s,/,\\,g

    # TODO : this 'dirty' list of binary dependencies will be simplified when CMake will be used

    CONFIG(release, debug|release) {
        lib_bin.files += $$VCPKG_PATH/bin/boost_*.dll
        lib_bin.files += $$VCPKG_PATH/bin/bz2.dll
        lib_bin.files += $$VCPKG_PATH/bin/gsl*.dll
        lib_bin.files += $$VCPKG_PATH/bin/jpeg*.dll
        lib_bin.files += $$VCPKG_PATH/bin/laszip3.dll
        lib_bin.files += $$VCPKG_PATH/bin/liblzma.dll
        lib_bin.files += $$VCPKG_PATH/bin/libpng16.dll
        lib_bin.files += $$VCPKG_PATH/bin/libsharpyuv.dll
        lib_bin.files += $$VCPKG_PATH/bin/libwebp*.dll
        lib_bin.files += $$VCPKG_PATH/bin/lz4.dll
        lib_bin.files += $$VCPKG_PATH/bin/muparser.dll
        lib_bin.files += $$VCPKG_PATH/bin/qhull_r.dll
        lib_bin.files += $$VCPKG_PATH/bin/tiff.dll
        lib_bin.files += $$VCPKG_PATH/bin/turbojpeg.dll
        lib_bin.files += $$VCPKG_PATH/bin/zlib1.dll
        lib_bin.files += $$VCPKG_PATH/bin/zstd.dll
        lib_bin.path = $$LIB/bin

        lib_gdal.files += $$VCPKG_PATH/bin/charset*.dll
        lib_gdal.files += $$VCPKG_PATH/bin/freexl*.dll
        lib_gdal.files += $$VCPKG_PATH/bin/gdal.dll
        lib_gdal.files += $$VCPKG_PATH/bin/geos*.dll
        lib_gdal.files += $$VCPKG_PATH/bin/geotiff.dll
        lib_gdal.files += $$VCPKG_PATH/bin/gif.dll
        lib_gdal.files += $$VCPKG_PATH/bin/hdf5*.dll
        lib_gdal.files += $$VCPKG_PATH/bin/iconv*.dll
        lib_gdal.files += $$VCPKG_PATH/bin/json-c.dll
        lib_gdal.files += $$VCPKG_PATH/bin/legacy.dll
        lib_gdal.files += $$VCPKG_PATH/bin/Lerc.dll
        lib_gdal.files += $$VCPKG_PATH/bin/libcrypto*.dll
        lib_gdal.files += $$VCPKG_PATH/bin/libcurl.dll
        lib_gdal.files += $$VCPKG_PATH/bin/libecpg*.dll
        lib_gdal.files += $$VCPKG_PATH/bin/libexpat.dll
        lib_gdal.files += $$VCPKG_PATH/bin/libffi.dll
        lib_gdal.files += $$VCPKG_PATH/bin/libpgtypes.dll
        lib_gdal.files += $$VCPKG_PATH/bin/libpq.dll
        lib_gdal.files += $$VCPKG_PATH/bin/libssl*.dll
        lib_gdal.files += $$VCPKG_PATH/bin/libxml2.dll
        lib_gdal.files += $$VCPKG_PATH/bin/netcdf.dll
        lib_gdal.files += $$VCPKG_PATH/bin/openjp2.dll
        lib_gdal.files += $$VCPKG_PATH/bin/pcre2*.dll
        lib_gdal.files += $$VCPKG_PATH/bin/pkgconf*.dll
        lib_gdal.files += $$VCPKG_PATH/bin/proj.dll
        lib_gdal.files += $$VCPKG_PATH/bin/python*.dll
        lib_gdal.files += $$VCPKG_PATH/bin/spatialite.dll
        lib_gdal.files += $$VCPKG_PATH/bin/sqlite3.dll
        lib_gdal.files += $$VCPKG_PATH/bin/szip.dll
        lib_gdal.files += $$VCPKG_PATH/bin/uriparser.dll
        lib_gdal.path = $$LIB/gdal

        lib_opencv.files += $$VCPKG_PATH/bin/libproto*.dll
        lib_opencv.files += $$VCPKG_PATH/bin/opencv_*.dll
        lib_opencv.path = $$LIB/opencv

        lib_pcl.files += $$VCPKG_PATH/bin/flann*.dll
        lib_pcl.files += $$VCPKG_PATH/bin/pcl_*.dll
        lib_pcl.path = $$LIB/pcl

#        lib_pdal.files += $$VCPKG_PATH/bin/libpdal*.dll
#        lib_pdal.files += $$VCPKG_PATH/bin/pdal*.dll
#        lib_pdal.files += $$VCPKG_PATH/bin/xerces*.dll
#        lib_pdal.path = $$LIB/pdal
    } else {
        lib_bin.files += $$VCPKG_PATH/debug/bin/boost_*.dll
        lib_bin.files += $$VCPKG_PATH/debug/bin/bz2d.dll
        lib_bin.files += $$VCPKG_PATH/debug/bin/gsl*.dll
        lib_bin.files += $$VCPKG_PATH/debug/bin/jpeg62.*
        lib_bin.files += $$VCPKG_PATH/debug/bin/laszip3.dll
        lib_bin.files += $$VCPKG_PATH/debug/bin/liblzma.dll
        lib_bin.files += $$VCPKG_PATH/debug/bin/libpng16d.dll
        lib_bin.files += $$VCPKG_PATH/debug/bin/libsharpyuv.dll
        lib_bin.files += $$VCPKG_PATH/debug/bin/libwebp*.dll
        lib_bin.files += $$VCPKG_PATH/debug/bin/lz4d.dll
        lib_bin.files += $$VCPKG_PATH/debug/bin/muparser.dll
        lib_bin.files += $$VCPKG_PATH/debug/bin/qhull_rd.dll
        lib_bin.files += $$VCPKG_PATH/debug/bin/tiffd.dll
        lib_bin.files += $$VCPKG_PATH/debug/bin/turbojpeg.*
        lib_bin.files += $$VCPKG_PATH/debug/bin/zlibd1.dll
        lib_bin.files += $$VCPKG_PATH/debug/bin/zstd.dll
        lib_bin.path = $$LIB/bin

        lib_gdal.files += $$VCPKG_PATH/debug/bin/charset*.dll
        lib_gdal.files += $$VCPKG_PATH/debug/bin/freexl*.dll
        lib_gdal.files += $$VCPKG_PATH/debug/bin/gdald.dll
        lib_gdal.files += $$VCPKG_PATH/debug/bin/geos*.dll
        lib_gdal.files += $$VCPKG_PATH/debug/bin/geotiff_d.dll
        lib_gdal.files += $$VCPKG_PATH/debug/bin/gif.dll
        lib_gdal.files += $$VCPKG_PATH/debug/bin/hdf5*.dll
        lib_gdal.files += $$VCPKG_PATH/debug/bin/iconv*.dll
        lib_gdal.files += $$VCPKG_PATH/debug/bin/json-c.dll
        lib_gdal.files += $$VCPKG_PATH/debug/bin/legacy.*
        lib_gdal.files += $$VCPKG_PATH/debug/bin/Lerc.dll
        lib_gdal.files += $$VCPKG_PATH/debug/bin/libcrypto*
        lib_gdal.files += $$VCPKG_PATH/debug/bin/libcurl-d.dll
        lib_gdal.files += $$VCPKG_PATH/debug/bin/libecpg*
        lib_gdal.files += $$VCPKG_PATH/debug/bin/libexpatd.dll
        lib_gdal.files += $$VCPKG_PATH/debug/bin/libffi.dll
        lib_gdal.files += $$VCPKG_PATH/debug/bin/libpgtypes.*
        lib_gdal.files += $$VCPKG_PATH/debug/bin/libpq.*
        lib_gdal.files += $$VCPKG_PATH/debug/bin/libssl*
        lib_gdal.files += $$VCPKG_PATH/debug/bin/libxml2.*
        lib_gdal.files += $$VCPKG_PATH/debug/bin/netcdf.dll
        lib_gdal.files += $$VCPKG_PATH/debug/bin/openjp2.dll
        lib_gdal.files += $$VCPKG_PATH/debug/bin/pcre2*
        lib_gdal.files += $$VCPKG_PATH/debug/bin/pkgconf*
        lib_gdal.files += $$VCPKG_PATH/debug/bin/proj_d.dll
        lib_gdal.files += $$VCPKG_PATH/debug/bin/python*.dll
        lib_gdal.files += $$VCPKG_PATH/debug/bin/spatialite.dll
        lib_gdal.files += $$VCPKG_PATH/debug/bin/sqlite3.dll
        lib_gdal.files += $$VCPKG_PATH/debug/bin/szip_D.dll
        lib_gdal.files += $$VCPKG_PATH/debug/bin/uriparser.dll
        lib_gdal.path = $$LIB/gdal

        lib_opencv.files += $$VCPKG_PATH/debug/bin/libproto*.dll
        lib_opencv.files += $$VCPKG_PATH/debug/bin/opencv_*.dll
        lib_opencv.path = $$LIB/opencv

        lib_pcl.files += $$VCPKG_PATH/debug/bin/flann*.dll
        lib_pcl.files += $$VCPKG_PATH/debug/bin/pcl_*.dll
        lib_pcl.path = $$LIB/pcl

#        lib_pdal.files += $$VCPKG_PATH/debug/bin/libpdal*.dll
#        lib_pdal.files += $$VCPKG_PATH/debug/bin/pdal*.dll
#        lib_pdal.files += $$VCPKG_PATH/debug/bin/xerces*.dll
#        lib_pdal.path = $$LIB/pdal
    }

    INSTALLS += lib_bin lib_gdal lib_opencv lib_pcl #lib_pdal

    # Usefull definitions of paths
    WIN_PATH = $$DESTDIRFULL
    WIN_PATH ~= s,/,\\,g
    PWDC = $$PWD

    # Deployment using WinDeployQt
    qt_deploy_options = --force --no-translations --angle --compiler-runtime --plugindir $$LIB/Qt --libdir $$LIB/Qt
    CONFIG(release, debug|release) : qt_deploy_options += --release
    CONFIG(debug,   debug|release) : qt_deploy_options += --debug

    qt_deploy_cmd1 = $$[QT_INSTALL_BINS]/windeployqt.exe $$DESTDIR $$qt_deploy_options &&
    qt_deploy_cmd2 = copy "distrib\windows\qt.conf" $$WIN_PATH && copy "distrib\windows\CompuTreeGui.cmd" $$WIN_PATH && copy "distrib\windows\CompuTreeBatch.cmd" $$WIN_PATH &&
    qt_deploy_cmd3 = move $$WIN_PATH\libraries\Qt\opengl32sw.dll $$WIN_PATH\opengl32.dll &&
    qt_deploy_cmd4 = rmdir /s /q $$WIN_PATH\languages\ && mkdir $$WIN_PATH\languages\ && call lrelease.bat && cd .. &&
    qt_deploy_cmd5 = forfiles /s /m *_*.qm /c \"cmd /c copy @path $$WIN_PATH\languages\" && cd $$PWDC &&
    qt_deploy_cmd6 = move $$WIN_PATH\libraries\Qt\vc_redist.x64.exe $$WIN_PATH

    qt_deploy.path = $$DESTDIR
    qt_deploy.extra = $$qt_deploy_cmd1 $$qt_deploy_cmd2 $$qt_deploy_cmd3 $$qt_deploy_cmd4 $$qt_deploy_cmd5 $$qt_deploy_cmd6

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

    qt_deploy_cmd0 = mkdir -p $$DESTDIRFULL/languages ; find ../ -type f -name "*_*.qm" -exec cp -n {} $$DESTDIRFULL/languages \; & wait ;
    qt_deploy_cmd1 = mkdir -p $$APPDIR/bin $$APPDIR/lib $$APPDIR/plugins $$APPDIR/languages $$APPDIR/share/applications $$APPDIR/share/icons/hicolor/256x256/apps & wait ;
    qt_deploy_cmd2 = cp distrib/linux/CompuTreeGui.desktop $$APPDIR/share/applications/CompuTreeGui.desktop ; cp ComputreeGui/resource/Icones/Icone_256x256.png $$APPDIR/share/icons/hicolor/256x256/apps/CompuTreeGui.png ; cp ComputreeGui/resource/Icones/Icone_256x256.png $$APPDIR/../CompuTreeGui.png & wait ;
    qt_deploy_cmd3 = cp $$LIB/core/*.so $$APPDIR/lib ; cp $$DESTDIRFULL/plugins/*.so $$APPDIR/plugins ; cp $$DESTDIRFULL/CompuTreeGui $$APPDIR/bin/CompuTreeGui & wait ;
    # Update the rpath in the various libraries/plugins we have to make sure they'll be loadable in an Appimage context (if needed : apt install patchelf)
    qt_deploy_cmd4 = cd $$DIR_LIBS    ; for target in $$DIR_LIBS/libctli*     ; do for file in libctli*; do patchelf --replace-needed "\$\$file" "\\\$\$ORIGIN/\$\$file" "\$\$target"; done; done & wait ;
    qt_deploy_cmd5 = cd $$DIR_LIBS    ; for target in $$DIR_PLUGINS/libplug_* ; do for file in libctli*; do patchelf --replace-needed "\$\$file" "\\\$\$ORIGIN/../lib/\$\$file" "\$\$target"; done; done & wait ;
    qt_deploy_cmd6 = cd $$DIR_LIBS    ; for file   in libctli*  ; do patchelf --replace-needed "\$\$file" "\\\$\$ORIGIN/../lib/\$\$file"     $$APPDIR/bin/CompuTreeGui; done & wait ;
    qt_deploy_cmd7 = cd $$DIR_PLUGINS ; for file   in libplug_* ; do patchelf --replace-needed "\$\$file" "\\\$\$ORIGIN/../plugins/\$\$file" $$APPDIR/bin/CompuTreeGui; done & wait ;
    qt_deploy_cmd8 = export LD_LIBRARY_PATH=$$[QT_INSTALL_LIBS]:$$APPDIR/lib:$$QT_PATH${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH} & wait ;
    # Build the image (if not installed, download it from https://github.com/probonopd/linuxdeployqt and paste it into Qt's binary folder)
    qt_deploy_cmd9 = $$DEPLOYTOOL_PATH/linuxdeployqt-continuous-x86_64.AppImage $$APPDIR/share/applications/CompuTreeGui.desktop -executable=$$APPDIR/bin/CompuTreeGui -no-translations -bundle-non-qt-libs -extra-plugins=$$APPDIR/lib,$$APPDIR/plugins -unsupported-allow-new-glibc & wait ;
    qt_deploy_cmd10 = rm -f $$APPDIR/../AppRun ; cp $$PWD/../distrib/linux/AppRun $$APPDIR/../AppRun ; chmod +x $$APPDIR/../AppRun ; cp $$DESTDIRFULL/languages/* $$APPDIR/languages & wait ;
    qt_deploy_cmd11 = cd $$DESTDIRFULL ; $$DEPLOYTOOL_PATH/linuxdeployqt-continuous-x86_64.AppImage $$APPDIR/bin/CompuTreeGui -no-translations -unsupported-allow-new-glibc -appimage & wait ;

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
    qt_deploy_cmd0 = mkdir -p $$DESTDIRFULL/languages ; find ../ -type f -name "*_*.qm" -exec cp {} $$DESTDIRFULL/languages \; & wait ;
    qt_deploy_cmd1 = mkdir -p $$DIR_FRAMEWORKS $$DIR_PLUGINS $$DIR_TRANSLATE & wait ; cp $$DESTDIRFULL/libraries/core/*.dylib $$DIR_FRAMEWORKS ; cp $$DESTDIRFULL/plugins/*.dylib $$DIR_PLUGINS ; cp $$DESTDIRFULL/languages/*.qm $$DIR_TRANSLATE & wait ;
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
