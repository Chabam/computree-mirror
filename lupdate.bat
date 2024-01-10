@echo off

setlocal

set dirpath=%~dp0

call %dirpath%\qtcmdloop.bat lupdate-pro.exe
