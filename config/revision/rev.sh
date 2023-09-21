#!/bin/bash

cd ..
cd ..

SVN_REVISION=$(svnversion -n .)

svnRevisionFile="ComputreeGui/svnrevision.h"

echo "#ifndef SVNREVISION_H" > $svnRevisionFile
echo "#define SVNREVISION_H" >> $svnRevisionFile
echo "" >> $svnRevisionFile
echo "#define SVN_REVISION \"$SVN_REVISION\"" >> $svnRevisionFile
echo "" >> $svnRevisionFile
echo "#endif // SVNREVISION_H" >> $svnRevisionFile