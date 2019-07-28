#ifndef PB_POLYGON2DEXPORTER_H
#define PB_POLYGON2DEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

class PB_Polygon2DExporter : public CT_AbstractExporter
{
    Q_OBJECT
    typedef CT_AbstractExporter SuperClass;

public:
    PB_Polygon2DExporter();
    PB_Polygon2DExporter(const PB_Polygon2DExporter& other) = default;

    QString getExporterCustomName() const override;

    CT_StepsMenu::LevelPredefined getExporterSubMenuName() const override;

    void init() override;

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list) override;

    bool configureExport() override;

    CT_EXPORTER_DECL_COPY(PB_Polygon2DExporter)

protected:
    bool protectedExportToFile() override;
};

#endif // PB_POLYGON2DEXPORTER_H
