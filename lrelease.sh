#!/bin/bash

export PATH=$PATH:~/Qt/5.15.2/gcc_64/bin/

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd $SCRIPT_DIR

lrelease-pro "ComputreeGui/ComputreeGui.pro"
lrelease-pro "library/ctlibcore/ctlibcore.pro"
lrelease-pro "library/ctlibexporter/ctlibexporter.pro"
lrelease-pro "library/ctlibfilters/ctlibfilters.pro"
lrelease-pro "library/ctlibio/ctlibio.pro"
lrelease-pro "library/ctliblas/ctliblas.pro"
lrelease-pro "library/ctliblaz/ctliblaz.pro"
lrelease-pro "library/ctliblog/ctliblog.pro"
lrelease-pro "library/ctlibmath/ctlibmath.pro"
lrelease-pro "library/ctlibmetrics/ctlibmetrics.pro"
lrelease-pro "library/ctlibmodelsextraviews/ctlibmodelsextraviews.pro"
lrelease-pro "library/ctlibpcl/ctlibpcl.pro"
lrelease-pro "library/ctlibplugin/ctlibplugin.pro"
lrelease-pro "library/ctlibreader/ctlibreader.pro"
lrelease-pro "library/ctlibstdactions/ctlibstdactions.pro"
lrelease-pro "library/ctlibstep/ctlibstep.pro"
lrelease-pro "library/ctlibstepaddon/ctlibstepaddon.pro"
lrelease-pro "library/ctlibstructure/ctlibstructure.pro"
lrelease-pro "library/ctlibstructureaddon/ctlibstructureaddon.pro"
lrelease-pro "library/ctlibstructurewidget/ctlibstructurewidget.pro"
lrelease-pro "library/ctlibwidget/ctlibwidget.pro"
lrelease-pro "plugins/pluginbase/pluginbase.pro"
lrelease-pro "../plugingenerate/plugingenerate/plugingenerate.pro"
lrelease-pro "../pluginignlif/pluginignlif/pluginignlif.pro"
lrelease-pro "../pluginmk/pluginmk/pluginmk.pro"
lrelease-pro "../pluginlvox/pluginlvox/pluginlvox.pro"
lrelease-pro "../pluginonf/pluginonf/pluginonf.pro"
lrelease-pro "../pluginonfdev/pluginonfdev/pluginonfdev.pro"
lrelease-pro "../pluginsegma/pluginsegma/pluginsegma.pro"
lrelease-pro "../plugintoolkit/plugintoolkit/plugintoolkit.pro"

# lrelease-pro "library/ctlibaction/ctlibaction.pro"
# lrelease-pro "library/ctlibclouds/ctlibclouds.pro"
# lrelease-pro "library/ctlibmodels/ctlibmodels.pro"
