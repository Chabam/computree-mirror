linux | macx | mingw {
    system(echo "qtpath=$$[QT_INSTALL_BINS]" > ../qtpath.txt)
}

win32 & !mingw {
    system((set qtpath=$$[QT_INSTALL_BINS]) & (call echo qtpath=%qtpath:/=\%) > ../qtpath.txt)
}


