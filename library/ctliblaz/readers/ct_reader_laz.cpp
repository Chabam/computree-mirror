#include "ct_reader_laz.h"

#include "ct_colorcloud/ct_colorcloudstdvector.h"
#include "ct_iterator/ct_mutablepointiterator.h"

#include "ct_log/ct_logmanager.h"

#include <QMessageBox>
#include <QFile>
#include <QDataStream>
#include <QDate>
#include <QDebug>

#include "laszip/laszip_api.h"

CT_Reader_LAZ::CT_Reader_LAZ(int subMenuLevel) : SuperClass(subMenuLevel)
{    
    m_headerFromConfiguration = nullptr;

    addNewReadableFormat(FileFormat("laz", tr("Fichiers LAZ .laz")));

    setToolTip(tr("Charge des points depuis un fichier au format LAZ (rapidlasso GmbH)<br>https://laszip.org/"));
}

CT_Reader_LAZ::CT_Reader_LAZ(const CT_Reader_LAZ &other) : SuperClass(other)
{
    m_headerFromConfiguration = nullptr;

    if(other.m_headerFromConfiguration != nullptr)
        m_headerFromConfiguration = static_cast<CT_LAZHeader*>(other.m_headerFromConfiguration->copy(nullptr, nullptr));
}

CT_Reader_LAZ::~CT_Reader_LAZ()
{
    delete m_headerFromConfiguration;
}

QString CT_Reader_LAZ::displayableName() const
{
    return tr("Points, format LAZ");
}

bool CT_Reader_LAZ::setFilePath(const QString& filepath)
{
    QString err;
    CT_LAZHeader *header = static_cast<CT_LAZHeader*>(internalReadHeader(filepath, err));

    if(header != nullptr) {
        if(CT_AbstractReader::setFilePath(filepath)) {
            setToolTip(header->toString());
            delete header;
            return true;
        }
    } else if(!err.isEmpty()) {
        PS_LOG->addErrorMessage(LogInterface::reader, err);
    }

    delete header;
    return false;
}

bool CT_Reader_LAZ::configure()
{
    QString err;
    CT_LAZHeader *header = static_cast<CT_LAZHeader*>(internalReadHeader(filepath(), err));

    if(header != nullptr) {
        delete m_headerFromConfiguration;
        m_headerFromConfiguration = header;
        return true;
    }

    delete header;
    return false;
}

CT_FileHeader* CT_Reader_LAZ::createHeaderPrototype() const
{
    return new CT_LAZHeader();
}

bool CT_Reader_LAZ::restoreSettings(SettingsReaderInterface &reader)
{
    delete m_headerFromConfiguration;
    m_headerFromConfiguration = nullptr;

    if(SuperClass::restoreSettings(reader)) {
        QString error;
        m_headerFromConfiguration = static_cast<CT_LAZHeader*>(internalReadHeader(filepath(), error));
    }

    return (m_headerFromConfiguration != nullptr);
}

void CT_Reader_LAZ::internalDeclareOutputModels(CT_ReaderOutModelStructureManager &manager)
{
    manager.addItem(m_hOutScene, tr("Scène"));
    manager.addItem(m_hOutAllAtt, tr("All Attributs"));

    manager.addPointAttribute(m_hOutReturnNumber, tr("Return Number"));
    manager.addPointAttribute(m_hOutNumberOfReturn, tr("Number of Returns"));
    manager.addPointAttribute(m_hOutClassificationFlag, tr("Classification Flags"));
    manager.addPointAttribute(m_hOutScannerChannel, tr("Scanner Channel"));
    manager.addPointAttribute(m_hOutScanDirectionFlag, tr("Scan Direction Flag"));
    manager.addPointAttribute(m_hOutEdgeOfFlightLine, tr("Edge of Flight Line"));

    manager.addPointAttribute(m_hOutIntensity, tr("Intensité"));
    manager.addPointAttribute(m_hOutClassification, tr("Classification"));
    manager.addPointAttribute(m_hOutUserData, tr("User Data"));
    manager.addPointAttribute(m_hOutPointSourceID, tr("Point Source "));

    manager.addPointAttribute(m_hOutScanAngle, tr("Scan Angle"));

    manager.addPointAttribute(m_hOutGPSTime, tr("GPS Time"));

    manager.addPointAttribute(m_hOutColor, tr("Color"));
    manager.addPointAttribute(m_hOutRed, tr("Red"));
    manager.addPointAttribute(m_hOutGreen, tr("Green"));
    manager.addPointAttribute(m_hOutBlue, tr("Blue"));

    manager.addPointAttribute(m_hOutWavePacketDescriptorIndex, tr("Wave Packet Descriptor Index"));
    manager.addPointAttribute(m_hOutByteOffsetToWaveformData, tr("Byte Offset To Waveform Data"));
    manager.addPointAttribute(m_hOutWaveformPacketSizeInBytes, tr("Waveform Packet Size In Bytes"));
    manager.addPointAttribute(m_hOutReturnPointWaveformLocation, tr("Return Point Waveform Location"));

    manager.addPointAttribute(m_hOutNIR, tr("NIR"));
}

CT_FileHeader *CT_Reader_LAZ::internalReadHeader(const QString &filepath, QString &error) const
{
    CT_LAZHeader *header = nullptr;

    // Test File validity
    if(QFile::exists(filepath))
    {
        QFile f(filepath);

        if(f.open(QIODevice::ReadOnly))
        {
            QDataStream stream(&f);

            header = new CT_LAZHeader();
            header->setFilePath(filepath);

            if(!header->readzip(filepath, error)) {
                delete header;
                header = nullptr;
            }
        }

        f.close();
    }

    return header;
}

bool CT_Reader_LAZ::internalReadFile(CT_StandardItemGroup *group)
{
    QString error;

    CT_LAZHeader *header = static_cast<CT_LAZHeader*>(internalReadHeader(filepath(), error));
    if(header == nullptr)
    {
        PS_LOG->addErrorMessage(LogInterface::reader, tr("Impossible de lire l'en-tête du fichier %1").arg(filepath()));
        return false;
    }

    size_t nPoints = header->getPointsRecordCount();
    if(nPoints == 0)
    {
        PS_LOG->addWarningMessage(LogInterface::reader, tr("Aucun points contenu dans le fichier %1").arg(filepath()));
        delete header;
        return true;
    }

    // create the reader
    laszip_POINTER laszip_reader = nullptr;
    if (laszip_create(&laszip_reader))
    {
        PS_LOG->addErrorMessage(LogInterface::reader, tr("Impossible de lire le fichier"));
        delete header;
        return false;
    }

    // open the reader
    auto filepath = CT_AbstractReader::filepath();
    laszip_BOOL is_compressed = 0;
    if (laszip_open_reader(laszip_reader, filepath.toStdString().c_str(), &is_compressed))
    {
        PS_LOG->addErrorMessage(LogInterface::reader, tr("Impossible de lire le fichier"));
        return false;
    }

    // get a pointer to the points that will be read
    laszip_point* point;
    if (laszip_get_point_pointer(laszip_reader, &point))
    {
        PS_LOG->addErrorMessage(LogInterface::reader, tr("Impossible de lire le fichier"));
        delete header;
        return false;
    }

    setToolTip(header->toString());

    bool mustTransformPoint = header->mustTransformPoints();

    CT_NMPCIR pcir = PS_REPOSITORY->createNewPointCloud(nPoints);

    // VECTOR
    auto scanAngleSetter = m_hOutScanAngle.createAttributesSetter(pcir);
    auto core6_10Setter = mCore6_10Manager.createAttributesSetter(pcir);

    // Vector in all point data format :
    auto intensitySetter = m_hOutIntensity.createAttributesSetter(pcir);
    auto classificationSetter = m_hOutClassification.createAttributesSetter(pcir);
    auto userDataSetter = m_hOutUserData.createAttributesSetter(pcir);
    auto pointSourceIDSetter = m_hOutPointSourceID.createAttributesSetter(pcir);

    // Vector not in all point data format :
    decltype (m_hOutGPSTime)::SetterPtr gpsTimeSetter = nullptr;
    decltype (m_hOutColor)::SetterPtr colorSetter = nullptr;
    decltype (m_hOutRed)::SetterPtr redSetter = nullptr;
    decltype (m_hOutGreen)::SetterPtr greenSetter = nullptr;
    decltype (m_hOutBlue)::SetterPtr blueSetter = nullptr;
    decltype (m_hOutWaveformPacketSizeInBytes)::SetterPtr wpsibSetter = nullptr;
    decltype (m_hOutByteOffsetToWaveformData)::SetterPtr botwdSetter = nullptr;
    decltype (m_hOutWavePacketDescriptorIndex)::SetterPtr wpdiSetter = nullptr;
    decltype (m_hOutReturnPointWaveformLocation)::SetterPtr rpwlSetter = nullptr;
    decltype (m_hOutNIR)::SetterPtr nirSetter = nullptr;

    if((m_headerFromConfiguration->m_pointDataRecordFormat == 1)
            || (m_headerFromConfiguration->m_pointDataRecordFormat > 2))
    {
        gpsTimeSetter = m_hOutGPSTime.createAttributesSetterPtr(pcir);
    }

    if((m_headerFromConfiguration->m_pointDataRecordFormat == 2)
            || (m_headerFromConfiguration->m_pointDataRecordFormat == 3)
            || (m_headerFromConfiguration->m_pointDataRecordFormat == 5)
            || (m_headerFromConfiguration->m_pointDataRecordFormat == 7)
            || (m_headerFromConfiguration->m_pointDataRecordFormat == 8)
            || (m_headerFromConfiguration->m_pointDataRecordFormat == 10))
    {
        colorSetter = m_hOutColor.createAttributesSetterPtr(pcir);
        redSetter = m_hOutRed.createAttributesSetterPtr(pcir);
        greenSetter = m_hOutGreen.createAttributesSetterPtr(pcir);
        blueSetter = m_hOutBlue.createAttributesSetterPtr(pcir);
    }

    if((m_headerFromConfiguration->m_pointDataRecordFormat == 8)
            || (m_headerFromConfiguration->m_pointDataRecordFormat == 10))
    {
        nirSetter = m_hOutNIR.createAttributesSetterPtr(pcir);
    }

    if((m_headerFromConfiguration->m_pointDataRecordFormat == 4)
            || (m_headerFromConfiguration->m_pointDataRecordFormat == 5)
            || (m_headerFromConfiguration->m_pointDataRecordFormat == 9)
            || (m_headerFromConfiguration->m_pointDataRecordFormat == 10))
    {
        wpdiSetter = m_hOutWavePacketDescriptorIndex.createAttributesSetterPtr(pcir);
        botwdSetter = m_hOutByteOffsetToWaveformData.createAttributesSetterPtr(pcir);
        wpsibSetter = m_hOutWaveformPacketSizeInBytes.createAttributesSetterPtr(pcir);
        rpwlSetter = m_hOutReturnPointWaveformLocation.createAttributesSetterPtr(pcir);
    }

    CT_MutablePointIterator it(pcir);

    // read the points and convert them from LASZIP to CT format
    for(size_t i=0; i<nPoints; i++)
    {
        // READ THE POINT
        if (laszip_read_point(laszip_reader))
        {
            PS_LOG->addErrorMessage(LogInterface::reader, tr("Impossible de lire le point"));
            delete header;
            return false;
        }

        // READ ALL ATTRIBUTES
        qint32 x = point->X;
        qint32 y = point->Y;
        qint32 z = point->Z;

        intensitySetter.setValueWithLocalIndex(i, point->intensity);

        PointCore6_10 valuePointCore6_10;
        if (header->m_pointDataRecordFormat < 6) // LAS <= 1.3
        {
            valuePointCore6_10.entire = point->return_number +
                    (point->number_of_returns << 3) +
                    (point->scan_direction_flag << 6) +
                    (point->edge_of_flight_line << 7);
        }
        else // LAS 1.4
        {
            valuePointCore6_10.entire = (quint16)point->extended_return_number +
                    ((quint16)point->extended_number_of_returns << 4) +
                    ((quint16)point->extended_classification_flags << 8) +
                    ((quint16)point->extended_scanner_channel << 12) +
                    ((quint16)point->scan_direction_flag << 14) +
                    ((quint16)point->edge_of_flight_line << 15);
        }

        core6_10Setter.setValueWithLocalIndex(i, valuePointCore6_10);

        if (header->m_pointDataRecordFormat < 6) // LAS <= 1.3
        {
            classificationSetter.setValueWithLocalIndex(i, point->classification);
        }
        else // LAS 1.4
        {
            classificationSetter.setValueWithLocalIndex(i, point->extended_classification);
        }

        if(header->m_pointDataRecordFormat < 6) // LAS <= 1.3
        {
            scanAngleSetter.setValueWithLocalIndex(i, point->scan_angle_rank);
            userDataSetter.setValueWithLocalIndex(i, point->user_data);
        }
        else // LAS 1.4
        {
            // "user data" is before "scan angle" in version 6 to 10
            userDataSetter.setValueWithLocalIndex(i, point->user_data);
            scanAngleSetter.setValueWithLocalIndex(i, point->extended_scan_angle);
        }

        pointSourceIDSetter.setValueWithLocalIndex(i, point->point_source_ID);

        // gps time is always after pointSourceID
        if(gpsTimeSetter)
            gpsTimeSetter->setValueWithLocalIndex(i, point->gps_time);

        // color is always after gpsTime if exist otherwise after pointSourceID
        if(colorSetter)
        {
            redSetter->setValueWithLocalIndex(i, point->rgb[0]);
            greenSetter->setValueWithLocalIndex(i, point->rgb[1]);
            blueSetter->setValueWithLocalIndex(i, point->rgb[2]);

            CT_Color colRGBA;
            colRGBA.r() = point->rgb[0]/256;
            colRGBA.g() = point->rgb[1]/256;
            colRGBA.b() = point->rgb[2]/256;
            colRGBA.a() = 255;

            colorSetter->setValueWithLocalIndex(i, colRGBA);
        }

        // NIR is always after colors if exist
        if(nirSetter)
            nirSetter->setValueWithLocalIndex(i, point->rgb[3]);

        // wave packet is always after NIR if exist
        if(wpdiSetter)
        {
            quint8 wpdi = point->wave_packet[0];
            quint64 botwd = (quint64)point->wave_packet[1] +
                    ((quint64)point->wave_packet[2] << 8) +
                    ((quint64)point->wave_packet[3] << 16) +
                    ((quint64)point->wave_packet[4] << 24) +
                    ((quint64)point->wave_packet[5] << 32) +
                    ((quint64)point->wave_packet[6] << 40) +
                    ((quint64)point->wave_packet[7] << 48) +
                    ((quint64)point->wave_packet[8] << 56);
            quint32 wpsi = (quint32)point->wave_packet[9] +
                    ((quint32)point->wave_packet[10] << 8) +
                    ((quint32)point->wave_packet[11] << 16) +
                    ((quint32)point->wave_packet[12] << 24);
            float rpwl = static_cast<float>((quint32)point->wave_packet[13] +
                    ((quint32)point->wave_packet[14] << 8) +
                    ((quint32)point->wave_packet[15] << 16) +
                    ((quint32)point->wave_packet[16] << 24));

            wpdiSetter->setValueWithLocalIndex(i, wpdi);
            botwdSetter->setValueWithLocalIndex(i, botwd);
            wpsibSetter->setValueWithLocalIndex(i, wpsi);
            rpwlSetter->setValueWithLocalIndex(i, rpwl);
        }

        // CONVERT POINT
        CT_Point pAdded;
        if(mustTransformPoint)
        {
            double xc = 0;
            double yc = 0;
            double zc = 0;
            header->transformPoint(x, y, z, xc, yc, zc);
            pAdded(0) = xc;
            pAdded(1) = yc;
            pAdded(2) = zc;
        }
        else
        {
            pAdded(0) = x;
            pAdded(1) = y;
            pAdded(2) = z;
        }

        it.next().replaceCurrentPoint(pAdded);

        setProgress(int((100.0*i)/nPoints));
    }

    // close the reader
    if (laszip_close_reader(laszip_reader))
    {
        PS_LOG->addErrorMessage(LogInterface::reader, tr("Impossible de fermer le fichier"));
        delete header;
        return false;
    }

    // destroy the reader
    if (laszip_destroy(laszip_reader))
    {
        PS_LOG->addErrorMessage(LogInterface::reader, tr("Impossible de detruire le reader"));
        delete header;
        return false;
    }

    CT_Scene *scene = new CT_Scene(pcir);
    scene->updateBoundingBox();

    // add the scene
    group->addSingularItem(m_hOutScene, scene);

    // add attributes
    CT_StdLASPointsAttributesContainer *container = new CT_StdLASPointsAttributesContainer();

    // ItemDrawable 6_10
    CT_AbstractPointAttributesScalar* rn6_10 = nullptr;
    CT_AbstractPointAttributesScalar* nor6_10 = nullptr;
    CT_AbstractPointAttributesScalar* sdf6_10 = nullptr;
    CT_AbstractPointAttributesScalar* efl6_10 =  nullptr;
    CT_AbstractPointAttributesScalar* cf = nullptr;
    CT_AbstractPointAttributesScalar* sc = nullptr;

    if (header->m_pointDataRecordFormat < 6)
    {
        // 0b00000111
        rn6_10 = m_hOutReturnNumber.createInstance(7, 0, pcir, mCore6_10Manager);

        // 0b00111000
        nor6_10 = m_hOutNumberOfReturn.createInstance(56, 3, pcir, mCore6_10Manager);

        // 0b01000000
        sdf6_10 = m_hOutScanDirectionFlag.createInstance(64, 6, pcir, mCore6_10Manager);

        // 0b10000000
        efl6_10 = m_hOutEdgeOfFlightLine.createInstance(128, 7, pcir, mCore6_10Manager);
    }
    else
    {
        // 0b0000 0000 0000 1111
        rn6_10 = m_hOutReturnNumber.createInstance(15, 0, pcir, mCore6_10Manager);

        // 0b0000 0000 1111 0000
        nor6_10 = m_hOutNumberOfReturn.createInstance(240, 4, pcir, mCore6_10Manager);

        // 0b0000 1111 0000 0000
        cf = m_hOutClassificationFlag.createInstance(3840, 8, pcir, mCore6_10Manager);

        // 0b0011 0000 0000 0000
        sc = m_hOutScannerChannel.createInstance(12288, 12, pcir, mCore6_10Manager);

        // 0b0100 0000 0000 0000
        sdf6_10 = m_hOutScanDirectionFlag.createInstance(16384, 14, pcir, mCore6_10Manager);

        // 0b1000 0000 0000 0000
        efl6_10 = m_hOutEdgeOfFlightLine.createInstance(32768, 15, pcir, mCore6_10Manager);
    }

    CT_AbstractPointAttributesScalar* attReturnNumber = rn6_10;
    CT_AbstractPointAttributesScalar* attNumberOfReturn = nor6_10;
    CT_AbstractPointAttributesScalar* attClassificationFlag = cf;
    CT_AbstractPointAttributesScalar* attScannerChannel = sc;
    CT_AbstractPointAttributesScalar* attScanDirectionFlag = sdf6_10;
    CT_AbstractPointAttributesScalar* attEdgeOfFlightLine = efl6_10;
    CT_AbstractPointAttributesScalar* attIntensity = m_hOutIntensity.createAttributeInstance(pcir);
    CT_AbstractPointAttributesScalar* attClassification = m_hOutClassification.createAttributeInstance(pcir);
    CT_AbstractPointAttributesScalar* attUserData = m_hOutUserData.createAttributeInstance(pcir);
    CT_AbstractPointAttributesScalar* attPointSourceID = m_hOutPointSourceID.createAttributeInstance(pcir);
    CT_AbstractPointAttributesScalar* attScanAngle = m_hOutScanAngle.createAttributeInstance(pcir);
    CT_AbstractPointAttributesScalar* attGPSTime = (gpsTimeSetter ? m_hOutGPSTime.createAttributeInstance(pcir) : nullptr);
    CT_AbstractPointAttributesScalar* attRed = (redSetter ? m_hOutRed.createAttributeInstance(pcir) : nullptr);
    CT_AbstractPointAttributesScalar* attGreen = (greenSetter ? m_hOutGreen.createAttributeInstance(pcir) : nullptr);
    CT_AbstractPointAttributesScalar* attBlue = (blueSetter ? m_hOutBlue.createAttributeInstance(pcir) : nullptr);
    CT_AbstractPointAttributesScalar* attWavePacketDescriptorIndex = (wpdiSetter ? m_hOutWavePacketDescriptorIndex.createAttributeInstance(pcir) : nullptr);
    CT_AbstractPointAttributesScalar* attByteOffsetToWaveformData = (botwdSetter ? m_hOutByteOffsetToWaveformData.createAttributeInstance(pcir) : nullptr);
    CT_AbstractPointAttributesScalar* attWaveformPacketSizeInBytes = (wpsibSetter ? m_hOutWaveformPacketSizeInBytes.createAttributeInstance(pcir) : nullptr);
    CT_AbstractPointAttributesScalar* attReturnPointWaveformLocation = (rpwlSetter ? m_hOutReturnPointWaveformLocation.createAttributeInstance(pcir) : nullptr);
    CT_AbstractPointAttributesScalar* attNIR = (nirSetter ? m_hOutNIR.createAttributeInstance(pcir) : nullptr);

    if (container != nullptr) {group->addSingularItem(m_hOutAllAtt, container);}
    if (attIntensity != nullptr) {group->addSingularItem(m_hOutIntensity, attIntensity);}
    if (attReturnNumber != nullptr) {group->addSingularItem(m_hOutReturnNumber, attReturnNumber);}
    if (attNumberOfReturn != nullptr) {group->addSingularItem(m_hOutNumberOfReturn, attNumberOfReturn);}
    if (attClassificationFlag != nullptr) {group->addSingularItem(m_hOutClassificationFlag, attClassificationFlag);}
    if (attScannerChannel != nullptr) {group->addSingularItem(m_hOutScannerChannel, attScannerChannel);}
    if (attScanDirectionFlag != nullptr) {group->addSingularItem(m_hOutScanDirectionFlag, attScanDirectionFlag);}
    if (attEdgeOfFlightLine != nullptr) {group->addSingularItem(m_hOutEdgeOfFlightLine, attEdgeOfFlightLine);}
    if (attClassification != nullptr) {group->addSingularItem(m_hOutClassification, attClassification);}
    if (attScanAngle != nullptr) {group->addSingularItem(m_hOutScanAngle, attScanAngle);}
    if (attUserData != nullptr) {group->addSingularItem(m_hOutUserData, attUserData);}
    if (attPointSourceID != nullptr) {group->addSingularItem(m_hOutPointSourceID, attPointSourceID);}
    if (attGPSTime != nullptr) {group->addSingularItem(m_hOutGPSTime, attGPSTime);}
    if (colorSetter) {group->addSingularItem(m_hOutColor, m_hOutColor.createAttributeInstance(pcir));}
    if (attRed != nullptr) {group->addSingularItem(m_hOutRed, attRed);}
    if (attGreen != nullptr) {group->addSingularItem(m_hOutGreen, attGreen);}
    if (attBlue != nullptr) {group->addSingularItem(m_hOutBlue, attBlue);}
    if (attWavePacketDescriptorIndex != nullptr) {group->addSingularItem(m_hOutWavePacketDescriptorIndex, attWavePacketDescriptorIndex);}
    if (attByteOffsetToWaveformData != nullptr) {group->addSingularItem(m_hOutByteOffsetToWaveformData, attByteOffsetToWaveformData);}
    if (attWaveformPacketSizeInBytes != nullptr) {group->addSingularItem(m_hOutWaveformPacketSizeInBytes, attWaveformPacketSizeInBytes);}
    if (attReturnPointWaveformLocation != nullptr) {group->addSingularItem(m_hOutReturnPointWaveformLocation, attReturnPointWaveformLocation);}
    if (attNIR != nullptr) {group->addSingularItem(m_hOutNIR, attNIR);}

    container->insertPointsAttributesAt(CT_LasDefine::Intensity, attIntensity);
    container->insertPointsAttributesAt(CT_LasDefine::Return_Number, attReturnNumber);
    container->insertPointsAttributesAt(CT_LasDefine::Number_of_Returns, attNumberOfReturn);
    container->insertPointsAttributesAt(CT_LasDefine::Classification_Flags, attClassificationFlag);
    container->insertPointsAttributesAt(CT_LasDefine::Scanner_Channel, attScannerChannel);
    container->insertPointsAttributesAt(CT_LasDefine::Scan_Direction_Flag, attScanDirectionFlag);
    container->insertPointsAttributesAt(CT_LasDefine::Edge_of_Flight_Line, attEdgeOfFlightLine);
    container->insertPointsAttributesAt(CT_LasDefine::Classification, attClassification);
    container->insertPointsAttributesAt(CT_LasDefine::Scan_Angle_Rank, attScanAngle);
    container->insertPointsAttributesAt(CT_LasDefine::User_Data, attUserData);
    container->insertPointsAttributesAt(CT_LasDefine::Point_Source_ID, attPointSourceID);
    container->insertPointsAttributesAt(CT_LasDefine::GPS_Time, attGPSTime);
    container->insertPointsAttributesAt(CT_LasDefine::Red, attRed);
    container->insertPointsAttributesAt(CT_LasDefine::Green, attGreen);
    container->insertPointsAttributesAt(CT_LasDefine::Blue, attBlue);
    container->insertPointsAttributesAt(CT_LasDefine::NIR, attNIR);
    container->insertPointsAttributesAt(CT_LasDefine::Wave_Packet_Descriptor_Index, attWavePacketDescriptorIndex);
    container->insertPointsAttributesAt(CT_LasDefine::Byte_offset_to_waveform_data, attByteOffsetToWaveformData);
    container->insertPointsAttributesAt(CT_LasDefine::Waveform_packet_size_in_bytes, attWaveformPacketSizeInBytes);
    container->insertPointsAttributesAt(CT_LasDefine::Return_Point_Waveform_Location, attReturnPointWaveformLocation);

    delete header;
    return true;
}