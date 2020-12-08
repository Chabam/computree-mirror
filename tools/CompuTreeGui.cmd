@echo off

SET local=.\libraries\
SET np=^
%local%core;^
%local%gdal;^
%local%pcl;^
%local%Qt;^
%local%opencv;

SET PATH "%path%%np%"

start CompuTreeGui.exe
exit
