@echo off

SET local=.\libraries\
SET np=^
%local%core;^
%local%Qt;^
%local%bin;^
%local%gdal;^
%local%opencv;^
%local%pcl;

SET PATH=%np%%path%

cd "%~dp0"
start CompuTreeGui.exe
exit
