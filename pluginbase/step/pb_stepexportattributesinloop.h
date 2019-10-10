#ifndef PB_STEPEXPORTATTRIBUTESINLOOP_H
#define PB_STEPEXPORTATTRIBUTESINLOOP_H

#include "ct_step/abstract/ct_abstractstep.h"

#include "ct_itemdrawable/ct_plotlistingrid.h"
#include "ct_itemdrawable/ct_loopcounter.h"

#ifdef USE_GDAL
#include "exporters/gdal/pb_gdalexporter.h"
#include "gdal_priv.h"
#include "ogr_core.h"
#include "ogrsf_frmts.h"
#include "cpl_conv.h"

#include "readers/tools/gdal/ct_gdaltools.h"
#endif

class PB_StepExportAttributesInLoop: public CT_AbstractStep
{
    Q_OBJECT
    using SuperClass = CT_AbstractStep;

public:

    PB_StepExportAttributesInLoop();

    QString description() const final;

    CT_VirtualAbstractStep* createNewInstance() const final;

protected:

    void declareInputModels(CT_StepInModelStructureManager& manager) final;

    void fillPreInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog) final;
    void fillPostInputConfigurationDialog(CT_StepConfigurableDialog* postInputConfigDialog) final;

    void declareOutputModels(CT_StepOutModelStructureManager& manager) final;

    void compute() final;

private:

#ifdef USE_GDAL
    QMap<QString, GDALDriver*> _gdalRasterDrivers;
    QMap<QString, GDALDriver*> _gdalVectorDrivers;
    QMap<QString, OGRFieldType>  _ogrTypes;
#endif

    QList<QString>          _modelsKeys;
    QMap<QString, QString>  _names;
    QMap<QString, QString> _shortNames;

    // Step parameters
    bool        _asciiExport;
    bool        _vectorExport;
    bool        _rasterExport;
    bool        _exportInLoop;

    QString      _rasterDriverName;
    QString      _vectorDriverName;
    QString     _vectorPrefix;
    QString     _rasterPrefix;

    QStringList _outASCIIFileName;
    QStringList _outVectorFolder;
    QStringList _outRasterFolder;
    bool        _subFolders;

    void replaceBadCharacters(QMap<QString, QString> &names) const;
    QString replaceBadCharacters(const QString &name) const;
    QString replaceAccentCharacters(const QString &name) const;
    QMap<QString, QString> computeShortNames(const QMap<QString, QString> &names) const;

};

#endif // PB_STEPEXPORTATTRIBUTESINLOOP_H
