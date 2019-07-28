#ifndef PB_PROFILEEXPORTER_H
#define PB_PROFILEEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

class PB_ProfileExporter : public CT_AbstractExporter
{
    Q_OBJECT
    typedef CT_AbstractExporter SuperClass;

public:
    PB_ProfileExporter();
    PB_ProfileExporter(const PB_ProfileExporter& other) = default;

    virtual QString getExporterCustomName() const override;

    CT_StepsMenu::LevelPredefined getExporterSubMenuName() const override;

    void init() override;

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list) override;

    bool configureExport() override;

    CT_EXPORTER_DECL_COPY(PB_ProfileExporter)

protected:
    bool protectedExportToFile() override;
};

#endif // PB_PROFILEEXPORTER_H
