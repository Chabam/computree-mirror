#!/bin/bash

dirpath=$(dirname $0)
cd $dirpath

qtpath=$(cut -d'=' -f2 qtpath.txt)

# First argument $1 is a Qt command
qtcmd=$qtpath/$1

# .pro file list in prolist.txt
# Delete \r character (Windows file)
# Skip empty lines, lines with space characters and comment lines
for pro in $(cat prolist.txt | tr -d '\r' | grep -v '^ *$' | grep -v '^ *#')
do
    $qtcmd $pro
done

# .pro file list in plugins.pro
# Delete \r character (Windows file)
for dir in $(cat plugins.pro | \
             tr -d '\r' | grep '^ *SUBDIRS[ +]*=' | cut -d '=' -f 2)
do
    # .pro file base name : directory base name + .pro suffix
    pro=$dir/$(basename $dir).pro

    $qtcmd $pro
done
