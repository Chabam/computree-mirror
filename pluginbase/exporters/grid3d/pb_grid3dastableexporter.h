#ifndef PB_GRID3DASTABLEEXPORTER_H
#define PB_GRID3DASTABLEEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

class PB_Grid3DAsTableExporter : public CT_AbstractExporter
{
    Q_OBJECT
    typedef CT_AbstractExporter SuperClass;

public:
    PB_Grid3DAsTableExporter();
    PB_Grid3DAsTableExporter(const PB_Grid3DAsTableExporter& other) = default;

    QString getExporterCustomName() const override;

    CT_StepsMenu::LevelPredefined getExporterSubMenuName() const override;

    void init() override;

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list) override;

    bool configureExport() override;

    CT_EXPORTER_DECL_COPY(PB_Grid3DAsTableExporter)

protected:
    bool protectedExportToFile() override;
};

#endif // PB_GRID3DASTABLEEXPORTER_H
