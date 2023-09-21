
win32 {
    system(cd $$PWD/revision && rev.bat)
}

linux {
    system(cd $$PWD/revision && $$PWD/revision/rev.sh)
}

macx {
    system(cd $$PWD/revision && $$PWD/revision/rev.sh)
}
