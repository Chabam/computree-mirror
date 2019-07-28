#ifndef PB_PGMEXPORTER_H
#define PB_PGMEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

class PB_PgmExporter : public CT_AbstractExporter
{
    Q_OBJECT
    typedef CT_AbstractExporter SuperClass;

public:
    PB_PgmExporter();
    PB_PgmExporter(const PB_PgmExporter& other) = default;

    QString getExporterCustomName() const override;

    CT_StepsMenu::LevelPredefined getExporterSubMenuName() const override;

    void init() override;

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list) override;

    bool configureExport() override;

    CT_EXPORTER_DECL_COPY(PB_PgmExporter)

protected:
    bool protectedExportToFile() override;
};

#endif // PB_PGMEXPORTER_H
