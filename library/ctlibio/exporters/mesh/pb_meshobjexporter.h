#ifndef PB_MESHOBJEXPORTER_H
#define PB_MESHOBJEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporter.h"

class PB_MeshObjExporter : public CT_AbstractExporter
{
    Q_OBJECT
    typedef CT_AbstractExporter SuperClass;

public:
    PB_MeshObjExporter();
    PB_MeshObjExporter(const PB_MeshObjExporter& other) = default;

    QString getExporterCustomName() const override;

    CT_StepsMenu::LevelPredefined getExporterSubMenuName() const override;

    void init() override;

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list) override;

    bool configureExport() override;

    CT_EXPORTER_DECL_COPY(PB_MeshObjExporter)

protected:
    bool protectedExportToFile() override;
};

#endif // PB_MESHOBJEXPORTER_H
