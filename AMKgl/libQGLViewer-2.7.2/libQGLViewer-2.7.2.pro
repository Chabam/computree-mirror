CONFIG       += ordered
TEMPLATE      = subdirs
SUBDIRS       = QGLViewer

QT_VERSION=$$[QT_VERSION]

equals (QT_MAJOR_VERSION, 5) {
	cache()
}
