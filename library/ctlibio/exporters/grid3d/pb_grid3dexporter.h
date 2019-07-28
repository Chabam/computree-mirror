#ifndef PB_GRID3DEXPORTER_H
#define PB_GRID3DEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

class PB_Grid3DExporter : public CT_AbstractExporter
{
    Q_OBJECT
    typedef CT_AbstractExporter SuperClass;

public:
    PB_Grid3DExporter();
    PB_Grid3DExporter(const PB_Grid3DExporter& other) = default;

    QString getExporterCustomName() const override;

    CT_StepsMenu::LevelPredefined getExporterSubMenuName() const override;

    void init() override;

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list) override;

    bool configureExport() override;

    CT_EXPORTER_DECL_COPY(PB_Grid3DExporter)

protected:
    bool protectedExportToFile();
};

#endif // PB_GRID3DEXPORTER_H
