@echo off
cd ..
cd ..

for /f %%a in ('git rev-parse HEAD') do set SVN_REVISION=%%a

set svnRevisionFile="ComputreeGui\svnrevision.h"

echo #ifndef SVNREVISION_H > %svnRevisionFile%
echo #define SVNREVISION_H >> %svnRevisionFile%
echo. >> %svnRevisionFile%
echo #define SVN_REVISION "%SVN_REVISION%" >> %svnRevisionFile%
echo. >> %svnRevisionFile%
echo #endif // SVNREVISION_H >> %svnRevisionFile%
