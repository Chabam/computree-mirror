#!/bin/bash

#LIBS_ALL="$PWD/libraries/core:$PWD/libraries/gdal:$PWD/libraries/opencv:$PWD/libraries/Qt:$PWD/plugins"

#if [[ "$LD_LIBRARY_PATH" != *"$LIBS_ALL"* ]]; then
#    export LD_LIBRARY_PATH=$LIBS_ALL${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}
#fi

rm -f computree.conf

echo "$PWD/libraries/core"   >> computree.conf
echo "$PWD/libraries/gdal"   >> computree.conf
echo "$PWD/libraries/opencv" >> computree.conf
echo "$PWD/libraries/pcl"    >> computree.conf
echo "$PWD/libraries/Qt"     >> computree.conf
echo "$PWD/plugins"          >> computree.conf

sudo rm -f /etc/ld.so.conf.d/computree.conf
sudo mv computree.conf /etc/ld.so.conf.d/
