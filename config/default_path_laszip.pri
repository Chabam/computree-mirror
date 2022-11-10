# linux : LASZIP_INC_PATH = "/usr/include/"
linux : LASZIP_INC_PATH = "../3rdparty/laszip/include/" # already included in package, but could be removed later
win32 : LASZIP_INC_PATH = "../3rdparty/laszip/include/" # already included in package, but could be removed later
macx  : LASZIP_INC_PATH = "../3rdparty/laszip/include/" # already included in package, but could be removed later
# macx  : LASZIP_INC_PATH = "/usr/local/Cellar/laszip/3.4.3/include/"

# linux : LASZIP_LIBS_PATH = "/usr/lib/x86_64-linux-gnu/"
linux : LASZIP_LIBS_PATH = "../3rdparty/laszip/lib/" # already included in package, but could be removed later
win32 : LASZIP_LIBS_PATH = "../3rdparty/laszip/lib/" # already included in package, but could be removed later
macx  : LASZIP_LIBS_PATH = "../3rdparty/laszip/lib/" # already included in package, but could be removed later
# macx  : LASZIP_LIBS_PATH = "/usr/local/Cellar/laszip/3.4.3/lib/"
