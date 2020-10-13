TEMPLATE = subdirs
SUBDIRS = base plugins

base.file = base.pro
plugins.file = plugins.pro
plugins.depends = base
