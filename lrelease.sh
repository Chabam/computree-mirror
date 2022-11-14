#!/bin/bash

export PATH=$PATH:~/Qt/5.15.2/gcc_64/bin/

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd $SCRIPT_DIR

if [[ "$OSTYPE" == "darwin"* ]]; then
	function lr() { lrelease $1; }
else
	function lr() { lrelease-pro $1; }
fi

lr "ComputreeGui/ComputreeGui.pro"
lr "library/ctlibcore/ctlibcore.pro"
lr "library/ctlibexporter/ctlibexporter.pro"
lr "library/ctlibfilters/ctlibfilters.pro"
lr "library/ctlibio/ctlibio.pro"
lr "library/ctliblas/ctliblas.pro"
lr "library/ctliblaz/ctliblaz.pro"
lr "library/ctliblog/ctliblog.pro"
lr "library/ctlibmath/ctlibmath.pro"
lr "library/ctlibmetrics/ctlibmetrics.pro"
lr "library/ctlibmodelsextraviews/ctlibmodelsextraviews.pro"
lr "library/ctlibpcl/ctlibpcl.pro"
lr "library/ctlibplugin/ctlibplugin.pro"
lr "library/ctlibreader/ctlibreader.pro"
lr "library/ctlibstdactions/ctlibstdactions.pro"
lr "library/ctlibstep/ctlibstep.pro"
lr "library/ctlibstepaddon/ctlibstepaddon.pro"
lr "library/ctlibstructure/ctlibstructure.pro"
lr "library/ctlibstructureaddon/ctlibstructureaddon.pro"
lr "library/ctlibstructurewidget/ctlibstructurewidget.pro"
lr "library/ctlibwidget/ctlibwidget.pro"
lr "plugins/pluginbase/pluginbase.pro"
lr "../plugingenerate/plugingenerate/plugingenerate.pro"
lr "../pluginignlif/pluginignlif/pluginignlif.pro"
lr "../pluginmk/pluginmk/pluginmk.pro"
lr "../pluginlvox/pluginlvox/pluginlvox.pro"
lr "../pluginonf/pluginonf/pluginonf.pro"
lr "../pluginonfdev/pluginonfdev/pluginonfdev.pro"
lr "../pluginsegma/pluginsegma/pluginsegma.pro"
lr "../plugintoolkit/plugintoolkit/plugintoolkit.pro"

# lr "library/ctlibaction/ctlibaction.pro"
# lr "library/ctlibclouds/ctlibclouds.pro"
# lr "library/ctlibmodels/ctlibmodels.pro"
