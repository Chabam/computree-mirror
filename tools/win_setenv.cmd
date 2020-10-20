SET local=.\libraries\
SET np=^
%local%core;^
%local%gdal;^
%local%pcl;^
%local%Qt;^
%local%opencv;

echo %path%|find /i "%local%">nul || SETX /s %COMPUTERNAME% /u %USERNAME% PATH "%path%%np%"
