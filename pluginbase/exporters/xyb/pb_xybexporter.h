#ifndef PB_XYBEXPORTER_H
#define PB_XYBEXPORTER_H

#include "ct_exporter/abstract/ct_abstractexporterpointattributesselection.h"
#include "ct_itemdrawable/ct_scanner.h"
#include "ct_view/itemdrawable/ct_itemdrawablehierarchycollectionwidget.h"
#include "ct_tools/attributes/ct_attributestocloudworkert.h"
#include "ct_tools/itemdrawable/ct_itemdrawablecollectionbuildert.h"
#include "ct_itemdrawable/abstract/ct_abstractpointsattributes.h"

#include <QFile>

class PB_XYBExporter : public CT_AbstractExporterPointAttributesSelection
{
    Q_OBJECT
    typedef CT_AbstractExporterPointAttributesSelection SuperClass;

public:
    PB_XYBExporter();
    PB_XYBExporter(const PB_XYBExporter& other) = default;

    void init();

    QString getExporterCustomName() const override;

    CT_StepsMenu::LevelPredefined getExporterSubMenuName() const override;

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list) override;
    bool setPointsToExport(const QList<CT_AbstractCloudIndex*> &list) override;

    void saveSettings(SettingsWriterInterface& writer) const override;
    bool restoreSettings(SettingsReaderInterface& reader) override;

    QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > createBuilders() const override;
    bool useSelection(const CT_ItemDrawableHierarchyCollectionWidget *selectorWidget) override;

    bool canExportPieceByPiece() const override;
    bool createExportFileForPieceByPieceExport() override;
    bool exportOnePieceOfDataToFile() override;
    bool finalizePieceByPieceExport() override;

    CT_EXPORTER_DECL_COPY(PB_XYBExporter)

protected:
    bool protectedExportToFile() override;
    void clearWorker() override;

private:
    typedef CT_ItemDrawableCollectionBuilderT<CT_Scanner> ScannerBuilder;

    QString                             m_currentFilePath;
    CT_Scanner*                         m_scanner;
    CT_OutAbstractSingularItemModel*    m_scannerModel;

    void exportPoints(QDataStream &stream,
                      const CT_AbstractPointCloudIndex *constPCIndex,
                      const CT_AbstractColorCloud *cc,
                      const int &nExported,
                      const int &totalToExport);
};

#endif // PB_XYBEXPORTER_H
