#ifndef PB_GROUPDATAEXPORTER_H
#define PB_GROUPDATAEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

class PB_GroupDataExporter : public CT_AbstractExporter
{
    Q_OBJECT
    typedef CT_AbstractExporter SuperClass;

public:
    PB_GroupDataExporter();
    PB_GroupDataExporter(const PB_GroupDataExporter& other) = default;

    QString getExporterCustomName() const override;

    CT_StepsMenu::LevelPredefined getExporterSubMenuName() const override;

    void init() override;

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list) override;

    bool configureExport() override;

    CT_EXPORTER_DECL_COPY(PB_GroupDataExporter)

protected:
    bool protectedExportToFile() override;
};

#endif // PB_GROUPDATAEXPORTER_H
