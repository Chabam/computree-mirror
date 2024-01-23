
win32 {
    system((set qtpath=$$[QT_INSTALL_BINS]) & (call echo qtpath=%qtpath:/=\%) > ../qtpath.txt)
}


linux | macx {
    system(echo "qtpath=$$[QT_INSTALL_BINS]" > ../qtpath.txt)
}
