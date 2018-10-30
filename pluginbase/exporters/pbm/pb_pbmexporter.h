#ifndef PB_PNMEXPORTER_H
#define PB_PNMEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

class PB_PbmExporter : public CT_AbstractExporter
{
    Q_OBJECT
    typedef CT_AbstractExporter SuperClass;

public:
    PB_PbmExporter();
    PB_PbmExporter(const PB_PbmExporter& other) = default;

    QString getExporterCustomName() const override;

    CT_StepsMenu::LevelPredefined getExporterSubMenuName() const override;

    void init() override;

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list) override;

    bool configureExport() override;

    CT_EXPORTER_DECL_COPY(PB_PbmExporter)

protected:
    bool protectedExportToFile() override;
};


#endif // PB_PNMEXPORTER_H
