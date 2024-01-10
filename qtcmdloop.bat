@echo off

setlocal enabledelayedexpansion

set dirpath=%~dp0

cd %dirpath%

for /f "tokens=2 delims==" %%i in (qtpath.txt) do (
    set qtpath=%%i
)

rem First argument %1 is a Qt command
set qtcmd=%qtpath%\%1


rem Skip empty lines, lines with space characters and comment lines
for /f "delims=" %%i in ('findstr /v /r /c:"^ *$" /c:"^ *#" prolist.txt') do (
    set pro=%%i

    rem Replace / character with \ character
    set pro=!pro:/=\!

    call %qtcmd% !pro!
)
