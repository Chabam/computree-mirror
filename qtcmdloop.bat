@echo off

setlocal enabledelayedexpansion

set dirpath=%~dp0

cd %dirpath%

for /f "tokens=2 delims==" %%i in (qtpath.txt) do (
    set qtpath=%%i
)

rem First argument %1 is a Qt command
set qtcmd=%qtpath%\%1

rem .pro file list in prolist.txt
rem Skip empty lines, lines with space characters and comment lines
for /f "delims=" %%i in ('findstr /v /r /c:"^ *$" /c:"^ *#" prolist.txt') do (
    set pro=%%i

    rem Replace all / characters with \ character
    set pro=!pro:/=\!

    call %qtcmd% !pro!
)

rem .pro file list in plugins.pro
for /f "tokens=2 delims==" %%i in ('findstr /r /c:"^ *SUBDIRS[ +]*=" plugins.pro') do (
  set dir=%%i

  rem Remove all space characters
  set dir=!dir: =!

  rem Replace all / characters with \ character
  set dir=!dir:/=\!

  rem .pro file base name : directory base name + .pro suffix
  for %%j in (!dir!) do set pro=!dir!\%%~nj.pro

  call %qtcmd% !pro!
)
