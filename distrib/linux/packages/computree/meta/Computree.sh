#!/bin/bash

export LD_LIBRARY_PATH=../libraries/core:../libraries/Qt:../libraries/gdal:../libraries/opencv:../libraries/pcl:../plugins:$LD_LIBRARY_PATH
export PATH=../libraries/core:../libraries/Qt:../libraries/gdal:../libraries/opencv:../libraries/pcl:../plugins:$PATH

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd $SCRIPT_DIR/bin
./CompuTreeGui

