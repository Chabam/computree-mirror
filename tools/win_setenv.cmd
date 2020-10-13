SET local=%~dp0
SET np=^
%local%libraries;^
%local%libraries\core;^
%local%libraries\gdal;^
%local%libraries\pcl;^
%local%libraries\Qt;^
%local%libraries\opencv;

echo %path%|find /i "%local%">nul || SETX /s %COMPUTERNAME% /u %USERNAME% PATH "%path%%np%"
