#ifndef PB_ASCIDEXPORTER_H
#define PB_ASCIDEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporterpointattributesselection.h"
#include "ct_view/itemdrawable/ct_itemdrawablehierarchycollectionwidget.h"
#include "ct_tools/attributes/ct_attributestocloudworkert.h"
#include "ct_itemdrawable/abstract/ct_abstractpointsattributes.h"

class PB_ASCIDExporter : public CT_AbstractExporterPointAttributesSelection
{
    Q_OBJECT
    typedef CT_AbstractExporterPointAttributesSelection SuperClass;

public:
    PB_ASCIDExporter();
    PB_ASCIDExporter(const PB_ASCIDExporter& other) = default;

    void init() override;

    QString getExporterCustomName() const override;

    CT_StepsMenu::LevelPredefined getExporterSubMenuName() const override;

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list) override;

    CT_EXPORTER_DECL_COPY(PB_ASCIDExporter)

protected:
    bool protectedExportToFile() override;
};

#endif // PB_ASCIDEXPORTER_H
