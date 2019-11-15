#include "ct_exporter_las.h"

#include "private/ct_laspiecebypieceprivateexporter.h"

#include "ctliblas/tools/las/ct_laspointformat0.h"
#include "ctliblas/tools/las/ct_laspointformat1.h"
#include "ctliblas/tools/las/ct_laspointformat2.h"
#include "ctliblas/tools/las/ct_laspointformat3.h"
#include "ctliblas/tools/las/ct_laspointformat4.h"
#include "ctliblas/tools/las/ct_laspointformat5.h"
#include "ctliblas/tools/las/ct_laspointformat6.h"
#include "ctliblas/tools/las/ct_laspointformat7.h"
#include "ctliblas/tools/las/ct_laspointformat8.h"
#include "ctliblas/tools/las/ct_laspointformat9.h"
#include "ctliblas/tools/las/ct_laspointformat10.h"

CT_Exporter_LAS::CT_Exporter_LAS(int subMenuLevel) : SuperClass(subMenuLevel),
    mLasContainer(nullptr)
{
    setCanExportPoints(true);
    addNewExportFormat(FileFormat("las", tr("Fichiers LAS .las")));
    setToolTip(tr("Exporte tous les points au format LAS, ASPRS<br>http://www.asprs.org/Committee-General/LASer-LAS-File-Format-Exchange-Activities.html"));
}

CT_Exporter_LAS::CT_Exporter_LAS(const CT_Exporter_LAS &other) : SuperClass(other)
{
    mLasContainer = nullptr;
}

QString CT_Exporter_LAS::displayableName() const
{
    return tr("Points, LAS");
}

bool CT_Exporter_LAS::setPointsToExport(const QList<CT_AbstractCloudIndex*> &list)
{
    QList<CT_AbstractCloudIndex*> myList;
    QListIterator<CT_AbstractCloudIndex*> it(list);

    while(it.hasNext())
    {
        CT_AbstractCloudIndex *item = it.next();

        if(dynamic_cast<CT_AbstractPointCloudIndex*>(item) != nullptr)
            myList.append(item);
    }

    if(myList.isEmpty())
    {
        setErrorMessage(errorMessage() + "\r\n" + tr("- Aucun points à exporter"));
        return false;
    }

    return SuperClass::setPointsToExport(myList);
}

void CT_Exporter_LAS::setItemsToExport(const QList<const CT_AbstractItemDrawableWithPointCloud*>& items,
                                       const CT_StdLASPointsAttributesContainer* lasAttributesContainer)
{
    setMustUseModels(false);
    mItems = items;
    mLasContainer = lasAttributesContainer;
}

bool CT_Exporter_LAS::canExportPieceByPiece() const
{
    return true;
}

CT_AbstractPieceByPieceExporter* CT_Exporter_LAS::createPieceByPieceExporter(const QString& outputFilepath)
{
    return new CT_LASPieceByPiecePrivateExporter(*this, outputFilepath);
}

void CT_Exporter_LAS::internalDeclareInputModels(CT_ExporterInModelStructureManager& manager)
{
    manager.addGroupToRootGroup(m_hInGroup);
    manager.addItemToGroup(m_hInGroup, m_hInItem, tr("Item à exporter"));
    manager.addItemToGroup(m_hInGroup, m_hInLASAttributesContainer, tr("Conteneur d'attributs LAS"));

    manager.addItemToGroup(m_hInGroup, m_hInReturnNumber, tr("Return Number"));
    manager.addItemToGroup(m_hInGroup, m_hInNumberOfReturn, tr("Number of Returns"));
    manager.addItemToGroup(m_hInGroup, m_hInClassificationFlag, tr("Classification Flags"));
    manager.addItemToGroup(m_hInGroup, m_hInScannerChannel, tr("Scanner Channel"));
    manager.addItemToGroup(m_hInGroup, m_hInScanDirectionFlag, tr("Scan Direction Flag"));
    manager.addItemToGroup(m_hInGroup, m_hInEdgeOfFlightLine, tr("Edge of Flight Line"));

    manager.addItemToGroup(m_hInGroup, m_hInIntensity, tr("Intensité"));
    manager.addItemToGroup(m_hInGroup, m_hInClassification, tr("Classification"));
    manager.addItemToGroup(m_hInGroup, m_hInUserData, tr("User Data"));
    manager.addItemToGroup(m_hInGroup, m_hInPointSourceID, tr("Point Source "));

    manager.addItemToGroup(m_hInGroup, m_hInScanAngle, tr("Scan Angle"));

    manager.addItemToGroup(m_hInGroup, m_hInGPSTime, tr("GPS Time"));

    manager.addItemToGroup(m_hInGroup, m_hInRed, tr("Red"));
    manager.addItemToGroup(m_hInGroup, m_hInGreen, tr("Green"));
    manager.addItemToGroup(m_hInGroup, m_hInBlue, tr("Blue"));

    manager.addItemToGroup(m_hInGroup, m_hInWavePacketDescriptorIndex, tr("Wave Packet Descriptor Index"));
    manager.addItemToGroup(m_hInGroup, m_hInByteOffsetToWaveformData, tr("Byte Offset To Waveform Data"));
    manager.addItemToGroup(m_hInGroup, m_hInWaveformPacketSizeInBytes, tr("Waveform Packet Size In Bytes"));
    manager.addItemToGroup(m_hInGroup, m_hInReturnPointWaveformLocation, tr("Return Point Waveform Location"));

    manager.addItemToGroup(m_hInGroup, m_hInNIR, tr("NIR"));

    manager.addItemToGroup(m_hInGroup, m_hInColor, tr("Color"));
}

bool CT_Exporter_LAS::internalExportToFile()
{
    QList<CT_AbstractPieceByPieceExporter*> exporters;
    exporters << createPieceByPieceExporter(filePath());

    if(exportOnePieceOfDataToFiles(exporters))
        return finalizePieceByPieceExport(exporters);

    return false;
}

bool CT_Exporter_LAS::internalExportOnePiece(const QList<CT_AbstractPieceByPieceExporter*>& pieceByPieceExporters)
{
    if(mustUseModels())
    {
        mLasContainer = nullptr;

        for(const CT_StdLASPointsAttributesContainer* container : m_hInLASAttributesContainer.iterateInputs(m_handleResultExport)) {
            mLasContainer = container;
            break;
        }

        if(mLasContainer == nullptr)
        {
            mLasContainerIfModelNotFound.clearPointsAttributesCollection();
            CT_StdLASPointsAttributesContainer* container = &mLasContainerIfModelNotFound;

            findAttributeAndAddToContainer(CT_LasDefine::Return_Number, container, m_hInReturnNumber);
            findAttributeAndAddToContainer(CT_LasDefine::Number_of_Returns, container, m_hInNumberOfReturn);
            findAttributeAndAddToContainer(CT_LasDefine::Classification_Flags, container, m_hInClassificationFlag);
            findAttributeAndAddToContainer(CT_LasDefine::Scanner_Channel, container, m_hInScannerChannel);
            findAttributeAndAddToContainer(CT_LasDefine::Scan_Direction_Flag, container, m_hInScanDirectionFlag);
            findAttributeAndAddToContainer(CT_LasDefine::Edge_of_Flight_Line, container, m_hInEdgeOfFlightLine);
            findAttributeAndAddToContainer(CT_LasDefine::Intensity, container, m_hInIntensity);
            findAttributeAndAddToContainer(CT_LasDefine::Classification, container, m_hInClassification);
            findAttributeAndAddToContainer(CT_LasDefine::User_Data, container, m_hInUserData);
            findAttributeAndAddToContainer(CT_LasDefine::Point_Source_ID, container, m_hInPointSourceID);
            findAttributeAndAddToContainer(CT_LasDefine::Scan_Angle_Rank, container, m_hInScanAngle);
            findAttributeAndAddToContainer(CT_LasDefine::GPS_Time, container, m_hInGPSTime);
            findAttributeAndAddToContainer(CT_LasDefine::Red, container, m_hInRed);
            findAttributeAndAddToContainer(CT_LasDefine::Green, container, m_hInGreen);
            findAttributeAndAddToContainer(CT_LasDefine::Blue, container, m_hInBlue);
            findAttributeAndAddToContainer(CT_LasDefine::Wave_Packet_Descriptor_Index, container, m_hInWavePacketDescriptorIndex);
            findAttributeAndAddToContainer(CT_LasDefine::Byte_offset_to_waveform_data, container, m_hInByteOffsetToWaveformData);
            findAttributeAndAddToContainer(CT_LasDefine::Waveform_packet_size_in_bytes, container, m_hInWaveformPacketSizeInBytes);
            findAttributeAndAddToContainer(CT_LasDefine::Return_Point_Waveform_Location, container, m_hInReturnPointWaveformLocation);
            findAttributeAndAddToContainer(CT_LasDefine::NIR, container, m_hInNIR);

            mLasContainer = container;
        }
    }

    QList<const CT_AbstractPointCloudIndex*> indexes;

    if(mustUseModels())
    {
        if(mIteratorItemBegin == mIteratorItemEnd)
        {
            auto iterator = m_hInItem.iterateInputs(m_handleResultExport);
            mIteratorItemBegin = iterator.begin();
            mIteratorItemEnd = iterator.end();
        }

        int nExported = 0;
        const int totalToExport = maximumItemToExportInFile(std::numeric_limits<int>::max());

        while((mIteratorItemBegin != mIteratorItemEnd)
              && (nExported < totalToExport))
        {
            const CT_AbstractItemDrawableWithPointCloud *item = *mIteratorItemBegin;

            indexes.append(item->pointCloudIndex());

            ++nExported;
            ++mIteratorItemBegin;
        }
    }
    else
    {
        for(const CT_AbstractItemDrawableWithPointCloud* item : mItems)
        {
            indexes.append(item->pointCloudIndex());
        }

        const QList<CT_AbstractCloudIndex*> &pointsSelected = pointsToExport();
        QListIterator<CT_AbstractCloudIndex*> itCI(pointsSelected);

        while(itCI.hasNext())
        {
            indexes.append(dynamic_cast<CT_AbstractPointCloudIndex*>(itCI.next()));
        }
    }

    CT_AbstractPointAttributesScalar* returnNumber = (mLasContainer == nullptr ? nullptr : static_cast<CT_AbstractPointAttributesScalar*>(mLasContainer->pointsAttributesAt(CT_LasDefine::Return_Number)));

    QList<CT_LASPieceByPiecePrivateExporter*> finalPieceByPieceExporters;

    const quint8 pointDataFormat = getPointDataFormat();
    const quint16 pointDataLength = getPointDataLength(pointDataFormat);

    for(CT_AbstractPieceByPieceExporter* ex : pieceByPieceExporters)
    {
        CT_LASPieceByPiecePrivateExporter* lasEx = static_cast<CT_LASPieceByPiecePrivateExporter*>(ex);

        if(lasEx->initializeHeader(returnNumber, pointDataFormat, pointDataLength))
            finalPieceByPieceExporters.append(lasEx);
    }

    if(!finalPieceByPieceExporters.isEmpty())
    {
        for(const CT_AbstractPointCloudIndex* index : indexes)
        {
            CT_PointIterator itP(index);

            while(itP.hasNext())
            {
                const CT_Point& p = itP.next().currentPoint();
                const size_t globalIndex = itP.cIndex();

                for(CT_LASPieceByPiecePrivateExporter* lasEx : finalPieceByPieceExporters)
                {
                    lasEx->computePointForHeader(globalIndex, p);
                }
            }
        }

        QSharedPointer<CT_AbstractLASPointFormat> toolsFormat(createPointDataFormat());
        mCurrentPieceByPieceProgress = 0;
        mPieceByPieceProgressMultiplicator = 1.0 / double(finalPieceByPieceExporters.size());

        for(CT_LASPieceByPiecePrivateExporter* lasEx : finalPieceByPieceExporters)
        {
            connect(lasEx, &CT_LASPieceByPiecePrivateExporter::progressChanged, this, &CT_Exporter_LAS::setPieceByPieceProgress);
            connect(this, &CT_Exporter_LAS::mustCancel, lasEx, &CT_LASPieceByPiecePrivateExporter::cancel);

            lasEx->finalizeHeaderAndWritePoints(toolsFormat);

            disconnect(lasEx, nullptr, this, nullptr);
            disconnect(this, nullptr, lasEx, nullptr);

            mCurrentPieceByPieceProgress += (mPieceByPieceProgressMultiplicator * 100.0);

            if(isStopped())
                break;
        }

        if(!toolsFormat.isNull())
            toolsFormat->clearInfos();
    }

    return true;
}

void CT_Exporter_LAS::clearIterators()
{
    mIteratorItemBegin = HandleItemType::const_iterator();
    mIteratorItemEnd = mIteratorItemBegin;
}

quint8 CT_Exporter_LAS::getPointDataFormat() const
{
    if(mLasContainer == nullptr)
        return 1;

    QList<CT_LasDefine::LASPointAttributesType> list = mLasContainer->lasPointAttributesTypes();

    bool wavePacket = true;
    bool nir = true;
    bool colors = true;

    if(!list.contains(CT_LasDefine::Wave_Packet_Descriptor_Index)
            && !list.contains(CT_LasDefine::Byte_offset_to_waveform_data)
            && !list.contains(CT_LasDefine::Waveform_packet_size_in_bytes)
            && !list.contains(CT_LasDefine::Return_Point_Waveform_Location))
        wavePacket = false;

    if(!list.contains(CT_LasDefine::NIR))
        nir = false;

    if(!list.contains(CT_LasDefine::Red)
            && !list.contains(CT_LasDefine::Green)
            && !list.contains(CT_LasDefine::Blue))
        colors = false;

    if(wavePacket)
    {
        if(nir || colors)
            return 10;

        return 9;
    }

    if(nir)
        return 8;

    if(colors)
        return 7;

    return 1;
}

quint16 CT_Exporter_LAS::getPointDataLength(const int &optFormat) const
{
    int format = optFormat;

    if((format < 0) || (format > 10))
        format = getPointDataFormat();

    if(format == 0)
        return quint16(CT_LASPointFormat0().sizeInBytes());
    else if(format == 1)
        return quint16(CT_LASPointFormat1().sizeInBytes());
    else if(format == 2)
        return quint16(CT_LASPointFormat2().sizeInBytes());
    else if(format == 3)
        return quint16(CT_LASPointFormat3().sizeInBytes());
    else if(format == 4)
        return quint16(CT_LASPointFormat4().sizeInBytes());
    else if(format == 5)
        return quint16(CT_LASPointFormat5().sizeInBytes());
    else if(format == 6)
        return quint16(CT_LASPointFormat6().sizeInBytes());
    else if(format == 7)
        return quint16(CT_LASPointFormat7().sizeInBytes());
    else if(format == 8)
        return quint16(CT_LASPointFormat8().sizeInBytes());
    else if(format == 9)
        return quint16(CT_LASPointFormat9().sizeInBytes());
    else if(format == 10)
        return quint16(CT_LASPointFormat10().sizeInBytes());

    return 0;
}

CT_AbstractLASPointFormat* CT_Exporter_LAS::createPointDataFormat(const int &optFormat) const
{
    int format = optFormat;

    if((format < 0) || (format > 10))
        format = getPointDataFormat();

    if(format == 0)
        return new CT_LASPointFormat0();
    else if(format == 1)
        return new CT_LASPointFormat1();
    else if(format == 2)
        return new CT_LASPointFormat2();
    else if(format == 3)
        return new CT_LASPointFormat3();
    else if(format == 4)
        return new CT_LASPointFormat4();
    else if(format == 5)
        return new CT_LASPointFormat5();
    else if(format == 6)
        return new CT_LASPointFormat6();
    else if(format == 7)
        return new CT_LASPointFormat7();
    else if(format == 8)
        return new CT_LASPointFormat8();
    else if(format == 9)
        return new CT_LASPointFormat9();
    else if(format == 10)
        return new CT_LASPointFormat10();

    return nullptr;
}

void CT_Exporter_LAS::setPieceByPieceProgress(int progress)
{
    const double finalProgress = double(progress) * mPieceByPieceProgressMultiplicator;
    setExportProgress(int(finalProgress + mCurrentPieceByPieceProgress));
}
