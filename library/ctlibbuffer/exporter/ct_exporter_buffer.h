#ifndef CT_EXPORTER_BUFFER_H
#define CT_EXPORTER_BUFFER_H

#include "ctlibbuffer/ctlibbuffer_global.h"

#include "ct_exporter/abstract/ct_abstractexporter.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include "ct_itemdrawable/abstract/ct_abstractpointsattributes.h"

#include <QMutex>

class CTLIBBUFFER_EXPORT CT_Exporter_BUFFER : public CT_AbstractExporter
{
    Q_OBJECT
    using SuperClass = CT_AbstractExporter;

public:
    CT_Exporter_BUFFER(int subMenuLevel = 0);
    CT_Exporter_BUFFER(const CT_Exporter_BUFFER& other);

    QString displayableName() const override;

    bool setPointsToExport(const QList<CT_AbstractCloudIndex*> &list) override;

    void setItemsToExport(const QList<const CT_AbstractItemDrawableWithPointCloud*>& items);

    CT_EXPORTER_DECL_COPY(CT_Exporter_BUFFER)

protected:
    void internalDeclareInputModels(CT_ExporterInModelStructureManager& manager) override;

    ExportReturn internalExportToFile() override;

    void clearIterators() override;

    void clearAttributesClouds() override;

private:
    using HandleItemType = CT_HandleInSingularItem<CT_AbstractItemDrawableWithPointCloud>;

    CT_HandleInStdGroup<>                                           m_hInGroup;
    HandleItemType                                                  m_hInItem;
    CT_HandleInAbstractPointAttribute<0>                            m_hInPointsAttribute; // optionnal

    HandleItemType::const_iterator                                  mIteratorItemBegin;
    HandleItemType::const_iterator                                  mIteratorItemEnd;

    QList<const CT_AbstractItemDrawableWithPointCloud*>             m_items;

    void exportItem(const CT_AbstractItemDrawableWithPointCloud* item, QDataStream& stream, const int& nExported, const int& totalToExport);
};

#endif // CT_EXPORTER_BUFFER_H
