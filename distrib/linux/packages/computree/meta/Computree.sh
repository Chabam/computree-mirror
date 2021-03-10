#!/bin/bash

export LD_LIBRARY_PATH=./libraries/core:./libraries/Qt:./libraries/gdal:./libraries/opencv:./libraries/pcl:./plugins:$LD_LIBRARY_PATH
export PATH=./libraries/core:./libraries/Qt:./libraries/gdal:./libraries/opencv:./libraries/pcl:./plugins:$PATH

./CompuTreeGui
