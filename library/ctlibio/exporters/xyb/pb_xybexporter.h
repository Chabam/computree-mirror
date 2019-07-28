#ifndef PB_XYBEXPORTER_H
#define PB_XYBEXPORTER_H

#include "ctlibio/ctlibio_global.h"

#include "ct_exporter/abstract/ct_abstractexporter.h"

#include "ct_tools/attributes/ct_attributestocloudworkert.h"
#include "ct_itemdrawable/ct_scanner.h"
#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include "ct_itemdrawable/abstract/ct_abstractpointsattributes.h"

class CTLIBIO_EXPORT PB_XYBExporter : public CT_AbstractExporter
{
    Q_OBJECT
    using SuperClass = CT_AbstractExporter;

public:
    PB_XYBExporter(int subMenuLevel = 0);
    PB_XYBExporter(const PB_XYBExporter& other);

    QString displayableName() const override;

    bool setPointsToExport(const QList<CT_AbstractCloudIndex*> &list) override;

    void setItemsToExport(const QList<const CT_AbstractItemDrawableWithPointCloud*>& items,
                          const CT_Scanner* scanner = nullptr,
                          const QList<const CT_AbstractPointsAttributes*> pointsAttributes = QList<const CT_AbstractPointsAttributes*>());

    bool canExportPieceByPiece() const override;
    bool createExportFileForPieceByPieceExport() override;
    bool exportOnePieceOfDataToFile() override;
    bool finalizePieceByPieceExport() override;

    CT_EXPORTER_DECL_COPY(PB_XYBExporter)

protected:
    void internalDeclareInputModels(CT_ExporterInModelStructureManager& manager) override;

    bool internalExportToFile() override;

    void clearIterators() override;

private:
    using HandleItemType = CT_HandleInSingularItem<CT_AbstractItemDrawableWithPointCloud>;

    CT_HandleInStdGroup<>                                           m_hInGroup;
    HandleItemType                                                  m_hInItem;
    CT_HandleInSingularItem<CT_Scanner, 0>                          m_hInScanner; // optionnal
    CT_HandleInSingularItem<CT_AbstractPointsAttributes, 0>         m_hInPointsAttribute; // optionnal
    HandleItemType::const_iterator                                  m_iteratorItemBegin;
    HandleItemType::const_iterator                                  m_iteratorItemEnd;

    QList<const CT_AbstractItemDrawableWithPointCloud*>             m_items;
    const CT_Scanner*                                               m_scanner;
    CT_AttributesToCloudWorkerT<CT_AbstractPointsAttributes>        m_attributsColorPointWorker;

    QString                                                         m_currentFilePath;

    CT_AbstractColorCloud* createColorCloud();

    void exportPoints(QDataStream &stream,
                      const CT_AbstractPointCloudIndex *constPCIndex,
                      const CT_AbstractColorCloud *cc,
                      const int &nExported,
                      const int &totalToExport);
};

#endif // PB_XYBEXPORTER_H
