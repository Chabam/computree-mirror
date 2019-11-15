#ifndef CT_EXPORTER_LAS_H
#define CT_EXPORTER_LAS_H

#include "ctliblas/ctliblas_global.h"
#include "ctliblas/itemdrawable/las/ct_stdlaspointsattributescontainer.h"
#include "ctliblas/tools/las/abstract/ct_abstractlaspointformat.h"

#include "ct_exporter/abstract/ct_abstractexporter.h"
#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include "ct_itemdrawable/ct_pointsattributescolor.h"

class CTLIBLAS_EXPORT CT_Exporter_LAS : public CT_AbstractExporter
{
    Q_OBJECT
    using SuperClass = CT_AbstractExporter;

public:
    CT_Exporter_LAS(int subMenuLevel = 0);
    CT_Exporter_LAS(const CT_Exporter_LAS& other);

    QString displayableName() const final;

    bool setPointsToExport(const QList<CT_AbstractCloudIndex*> &list) override;

    void setItemsToExport(const QList<const CT_AbstractItemDrawableWithPointCloud*>& items,
                          const CT_StdLASPointsAttributesContainer* lasAttributesContainer = nullptr);

    bool canExportPieceByPiece() const final;
    CT_AbstractPieceByPieceExporter* createPieceByPieceExporter(const QString& outputFilepath) override;

    CT_EXPORTER_DECL_COPY(CT_Exporter_LAS)

protected:
    void internalDeclareInputModels(CT_ExporterInModelStructureManager& manager) override;

    bool internalExportToFile() override;

    bool internalExportOnePiece(const QList<CT_AbstractPieceByPieceExporter*>& pieceByPieceExporters) override;

    void clearIterators() override;

private:
    friend class CT_LASPieceByPiecePrivateExporter;

    using HandleItemType = CT_HandleInSingularItem<CT_AbstractItemDrawableWithPointCloud>;

    CT_HandleInStdGroup<>                                           m_hInGroup;
    HandleItemType                                                  m_hInItem;
    CT_HandleInSingularItem<CT_StdLASPointsAttributesContainer, 0>  m_hInLASAttributesContainer; // optionnal

    // TODO :
    CT_HandleInSingularItem<CT_AbstractPointAttributesScalar, 0>    m_hInReturnNumber; // optionnal
    CT_HandleInSingularItem<CT_AbstractPointAttributesScalar, 0>    m_hInNumberOfReturn; // optionnal
    CT_HandleInSingularItem<CT_AbstractPointAttributesScalar, 0>    m_hInClassificationFlag; // optionnal
    CT_HandleInSingularItem<CT_AbstractPointAttributesScalar, 0>    m_hInScannerChannel; // optionnal
    CT_HandleInSingularItem<CT_AbstractPointAttributesScalar, 0>    m_hInScanDirectionFlag; // optionnal
    CT_HandleInSingularItem<CT_AbstractPointAttributesScalar, 0>    m_hInEdgeOfFlightLine; // optionnal

    CT_HandleInSingularItem<CT_AbstractPointAttributesScalar, 0>    m_hInIntensity; // optionnal
    CT_HandleInSingularItem<CT_AbstractPointAttributesScalar, 0>    m_hInClassification; // optionnal
    CT_HandleInSingularItem<CT_AbstractPointAttributesScalar, 0>    m_hInUserData; // optionnal
    CT_HandleInSingularItem<CT_AbstractPointAttributesScalar, 0>    m_hInPointSourceID; // optionnal

    CT_HandleInSingularItem<CT_AbstractPointAttributesScalar, 0>    m_hInScanAngle; // optionnal

    CT_HandleInSingularItem<CT_AbstractPointAttributesScalar, 0>    m_hInGPSTime; // optionnal

    CT_HandleInSingularItem<CT_PointsAttributesColor, 0>            m_hInColor; // optionnal
    CT_HandleInSingularItem<CT_AbstractPointAttributesScalar, 0>    m_hInRed; // optionnal
    CT_HandleInSingularItem<CT_AbstractPointAttributesScalar, 0>    m_hInGreen; // optionnal
    CT_HandleInSingularItem<CT_AbstractPointAttributesScalar, 0>    m_hInBlue; // optionnal

    CT_HandleInSingularItem<CT_AbstractPointAttributesScalar, 0>    m_hInWavePacketDescriptorIndex; // optionnal
    CT_HandleInSingularItem<CT_AbstractPointAttributesScalar, 0>    m_hInByteOffsetToWaveformData; // optionnal
    CT_HandleInSingularItem<CT_AbstractPointAttributesScalar, 0>    m_hInWaveformPacketSizeInBytes; // optionnal
    CT_HandleInSingularItem<CT_AbstractPointAttributesScalar, 0>    m_hInReturnPointWaveformLocation; // optionnal

    CT_HandleInSingularItem<CT_AbstractPointAttributesScalar, 0>    m_hInNIR; // optionnal

    HandleItemType::const_iterator                                  mIteratorItemBegin;
    HandleItemType::const_iterator                                  mIteratorItemEnd;

    QList<const CT_AbstractItemDrawableWithPointCloud*>             mItems;
    const CT_StdLASPointsAttributesContainer*                       mLasContainer;
    CT_StdLASPointsAttributesContainer                              mLasContainerIfModelNotFound;

    double                                                          mCurrentPieceByPieceProgress;
    double                                                          mPieceByPieceProgressMultiplicator;

    template<typename HandleType>
    void findAttributeAndAddToContainer(CT_LasDefine::LASPointAttributesType key, CT_StdLASPointsAttributesContainer* container, HandleType& t) const
    {
        for(const CT_AbstractPointAttributesScalar* attribute : t.iterateInputs(m_handleResultExport)) {
            container->insertPointsAttributesAt(key, attribute);
            break;
        }
    }

    /**
     * @brief Return the point data format to use (check mLasContainer to know where attributes is in the container and select the appropriate format)
     */
    quint8 getPointDataFormat() const;

    /**
     * @brief Return the length of a point in bytes (use the method "getPointDataFormat" if optFormat == -1, otherwise pass the format to this method)
     */
    quint16 getPointDataLength(const int &optFormat = -1) const;

    /**
     * @brief Create a point data format (tools to export)
     */
    CT_AbstractLASPointFormat* createPointDataFormat(const int &optFormat = -1) const;

private slots:
    void setPieceByPieceProgress(int progress);
};

#endif // CT_EXPORTER_LAS_H
