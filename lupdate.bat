@echo off &setlocal

rem We use the *-pro version of tools because the old ones are deprecated
set lupdate=C:\Qt\5.15.1\msvc2019_64\bin\lupdate-pro.exe

for %%i in (
    ComputreeGui\ComputreeGui.pro
    library\ctlibcore\ctlibcore.pro
    library\ctlibexporter\ctlibexporter.pro
    library\ctlibfilters\ctlibfilters.pro
    library\ctlibio\ctlibio.pro
    library\ctliblas\ctliblas.pro
    library\ctliblaz\ctliblaz.pro
    library\ctliblog\ctliblog.pro
    library\ctlibmath\ctlibmath.pro
    library\ctlibmetrics\ctlibmetrics.pro
    library\ctlibmodelsextraviews\ctlibmodelsextraviews.pro
    library\ctlibpcl\ctlibpcl.pro
    library\ctlibplugin\ctlibplugin.pro
    library\ctlibreader\ctlibreader.pro
    library\ctlibstdactions\ctlibstdactions.pro
    library\ctlibstep\ctlibstep.pro
    library\ctlibstepaddon\ctlibstepaddon.pro
    library\ctlibstructure\ctlibstructure.pro
    library\ctlibstructureaddon\ctlibstructureaddon.pro
    library\ctlibstructurewidget\ctlibstructurewidget.pro
    library\ctlibwidget\ctlibwidget.pro
    plugins\pluginbase\pluginbase.pro
    ..\plugingenerate\plugingenerate\plugingenerate.pro
    ..\pluginignlif\pluginignlif\pluginignlif.pro
    ..\pluginmk\pluginmk\pluginmk.pro
    ..\pluginlvox\pluginlvox\pluginlvox.pro
    ..\pluginonf\pluginonf\pluginonf.pro
    ..\pluginonfdev\pluginonfdev\pluginonfdev.pro
    ..\pluginsegma\pluginsegma\pluginsegma.pro
    ..\plugintoolkit\plugintoolkit\plugintoolkit.pro
        ) do (
    %lupdate% %%i -no-obsolete
)

rem The following project can't be processed by lupdate du to an unexpected endless loop
rem     library\ctlibaction\ctlibaction.pro
rem     library\ctlibclouds\ctlibclouds.pro
rem     library\ctlibmodels\ctlibmodels.pro

pause
