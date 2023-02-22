TEMPLATE = aux

INSTALLER = Computree_Installer_WINDOWS_6.0.334
# version : Major.Minor.commit

# Usefull definitions of paths
PATH_SRC = $$PWD/../../../ComputreeInstallRelease
PATH_SRC ~= s,/,\\,g
PATH_DST = $$PWD/packages
PATH_DST ~= s,/,\\,g
PATH_RCS = $$PWD/../resources
PATH_RCS ~= s,/,\\,g

CLEAN_DATA  = cd $$PATH_DST &&
CLEAN_DATA += (if exist computree.A\data\ rd /s /q computree.A\data\) &&
CLEAN_DATA += (if exist computree.B\data\ rd /s /q computree.B\data\) &&
CLEAN_DATA += (if exist computree.C\data\ rd /s /q computree.C\data\) &&
CLEAN_DATA += (if exist library.A\data\   rd /s /q library.A\data\  ) &&
CLEAN_DATA += (if exist library.B\data\   rd /s /q library.B\data\  ) &&
CLEAN_DATA += (if exist library.B.1\data\ rd /s /q library.B.1\data\) &&
CLEAN_DATA += (if exist library.C\data\   rd /s /q library.C\data\  ) &&
CLEAN_DATA += (if exist library.D\data\   rd /s /q library.D\data\  ) &&
CLEAN_DATA += (if exist library.E\data\   rd /s /q library.E\data\  ) &&
CLEAN_DATA += (if exist plugin.A\data\    rd /s /q plugin.A\data\   ) &&
CLEAN_DATA += (if exist plugin.C\data\    rd /s /q plugin.C\data\   ) &&
CLEAN_DATA += (if exist plugin.D\data\    rd /s /q plugin.D\data\   ) &&
CLEAN_DATA += (if exist plugin.E\data\    rd /s /q plugin.E\data\   ) &&
CLEAN_DATA += (if exist plugin.G\data\    rd /s /q plugin.G\data\   ) &&
CLEAN_DATA += (if exist plugin.L\data\    rd /s /q plugin.L\data\   ) &&
CLEAN_DATA += (if exist plugin.M\data\    rd /s /q plugin.M\data\   )

PREPARE_DATA  = && cd $$PATH_SRC &&
PREPARE_DATA += xcopy /s /i /y CompuTreeGui.*              $$PATH_DST\computree.A\data\                        &&
PREPARE_DATA += xcopy /s /i /y qt.conf                     $$PATH_DST\computree.A\data\                        &&
PREPARE_DATA += xcopy /s /i /y $$PATH_RCS\charter\*eng.pdf $$PATH_DST\computree.A\data\                        &&
PREPARE_DATA += xcopy /s /i /y $$PATH_RCS\charter\*fr.pdf  $$PATH_DST\computree.A\data\                        &&
PREPARE_DATA += xcopy /s /i /y languages\*                 $$PATH_DST\computree.A\data\languages\              &&
PREPARE_DATA += xcopy /s /i /y $$PATH_RCS\examples\*       $$PATH_DST\computree.B\data\examples\               &&
PREPARE_DATA += xcopy /s /i /y vc_redist.x64.exe           $$PATH_DST\computree.C\data\                        &&
PREPARE_DATA += xcopy /s /i /y libraries\core\ctlib*.dll   $$PATH_DST\library.A\data\libraries\core\           &&
PREPARE_DATA += xcopy /s /i /y libraries\bin\*.dll         $$PATH_DST\library.A\data\libraries\bin\            &&
PREPARE_DATA += xcopy /s /i /y libraries\Qt\Qt5*.dll       $$PATH_DST\library.B\data\libraries\Qt\             &&
PREPARE_DATA += xcopy /s /i /y libraries\Qt\iconengines\*  $$PATH_DST\library.B\data\libraries\Qt\iconengines  &&
PREPARE_DATA += xcopy /s /i /y libraries\Qt\imageformats\* $$PATH_DST\library.B\data\libraries\Qt\imageformats &&
PREPARE_DATA += xcopy /s /i /y libraries\Qt\platforms\*    $$PATH_DST\library.B\data\libraries\Qt\platforms    &&
PREPARE_DATA += xcopy /s /i /y libraries\Qt\styles\*       $$PATH_DST\library.B\data\libraries\Qt\styles       &&
PREPARE_DATA += xcopy /s /i /y libraries\Qt\lib*           $$PATH_DST\library.B.1\data\libraries\Qt\           &&
PREPARE_DATA += xcopy /s /i /y libraries\Qt\d3d*           $$PATH_DST\library.B.1\data\libraries\Qt\           &&
PREPARE_DATA += xcopy /s /i /y opengl32.dll                $$PATH_DST\library.B.1\data\                        &&
PREPARE_DATA += xcopy /s /i /y libraries\gdal\*.dll        $$PATH_DST\library.C\data\libraries\gdal\           &&
PREPARE_DATA += xcopy /s /i /y libraries\opencv\opencv_*   $$PATH_DST\library.D\data\libraries\opencv\         &&
PREPARE_DATA += xcopy /s /i /y libraries\pcl\pcl_*         $$PATH_DST\library.E\data\libraries\pcl\            &&
PREPARE_DATA += mkdir $$PATH_DST\library.E\data\libraries\core\ &&
PREPARE_DATA += move $$PATH_DST\library.A\data\libraries\core\ctlibpcl.dll $$PATH_DST\library.E\data\libraries\core\ &&
PREPARE_DATA += xcopy /s /i /y plugins\plug_base.dll       $$PATH_DST\plugin.A\data\plugins\                   &&
PREPARE_DATA += xcopy /s /i /y plugins\plug_generate.dll   $$PATH_DST\plugin.B\data\plugins\                   &&
PREPARE_DATA += xcopy /s /i /y plugins\plug_toolkit.dll    $$PATH_DST\plugin.C\data\plugins\                   &&
PREPARE_DATA += xcopy /s /i /y plugins\plug_onf.dll        $$PATH_DST\plugin.D\data\plugins\                   &&
PREPARE_DATA += xcopy /s /i /y plugins\plug_segma.dll      $$PATH_DST\plugin.E\data\plugins\                   &&
PREPARE_DATA += xcopy /s /i /y plugins\plug_mk.dll         $$PATH_DST\plugin.G\data\plugins\                   &&
PREPARE_DATA += xcopy /s /i /y plugins\plug_onfdev.dll     $$PATH_DST\plugin.L\data\plugins\                   &&
PREPARE_DATA += xcopy /s /i /y plugins\plug_ignlif.dll     $$PATH_DST\plugin.M\data\plugins\                   &&

GENERATE_BIN  = $$[QT_INSTALL_BINS]/../../../Tools/QtInstallerFramework/4.1/bin/binarycreator.exe -c $$PWD/config/config.xml -r $$PWD/../resources/additional.qrc -p $$PWD/packages $$PATH_SRC/../$$INSTALLER &&
GENERATE_BIN ~= s,/,\\,g

INPUT = $$PWD/config/config.xml $$PWD/packages
generation.input = INPUT
generation.output = $$INSTALLER
generation.commands = $$CLEAN_DATA $$PREPARE_DATA $$GENERATE_BIN $$CLEAN_DATA
generation.CONFIG += target_predeps no_link combine

QMAKE_EXTRA_COMPILERS += generation
