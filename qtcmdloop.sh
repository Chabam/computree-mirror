#!/bin/bash

dirpath=$(dirname $0)
cd $dirpath

qtpath=$(cut -d'=' -f2 qtpath.txt)

# First argument $1 is a Qt command
qtcmd=$qtpath/$1

# Delete \r character (Windows file)
# Skip empty lines, lines with space characters and comment lines
for pro in $(cat prolist.txt | tr -d '\r' | grep -v '^ *$' | grep -v '^ *#')
do
    $qtcmd $pro
done
