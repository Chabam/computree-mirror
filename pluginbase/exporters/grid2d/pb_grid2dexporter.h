#ifndef PB_GRID2DEXPORTER_H
#define PB_GRID2DEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

class PB_Grid2DExporter : public CT_AbstractExporter
{
    Q_OBJECT
    typedef CT_AbstractExporter SuperClass;

public:
    PB_Grid2DExporter();
    PB_Grid2DExporter(const PB_Grid2DExporter& other) = default;

    QString getExporterCustomName() const override;

    CT_StepsMenu::LevelPredefined getExporterSubMenuName() const override;

    void init() override;

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list) override;

    bool configureExport() override;

    CT_EXPORTER_DECL_COPY(PB_Grid2DExporter)

protected:
    bool protectedExportToFile() override;
};

#endif // PB_GRID2DEXPORTER_H
