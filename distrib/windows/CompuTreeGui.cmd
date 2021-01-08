@echo off

SET local=.\libraries\
SET np=^
%local%core;^
%local%gdal;^
%local%pcl;^
%local%Qt;^
%local%opencv;

SET PATH=%np%%path%

cd "%~dp0"
start CompuTreeGui.exe
exit
