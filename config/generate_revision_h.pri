
linux|mingw {
    system(cd $$PWD/revision && $$PWD/revision/rev.sh)
}

win32&!mingw{
    system(cd $$PWD/revision && rev.bat)
}

macx {
    system(cd $$PWD/revision && $$PWD/revision/rev.sh)
}
