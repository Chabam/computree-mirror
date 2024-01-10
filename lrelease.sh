#!/bin/bash

if [[ "$OSTYPE" == "darwin"* ]]; then
    lr=lrelease
else
    lr=lrelease-pro
fi

dirpath=$(dirname $0)
bash $dirpath/qtcmdloop.sh $lr
