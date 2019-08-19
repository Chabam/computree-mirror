#include "ct_reader_lasv2.h"

#include "ct_colorcloud/ct_colorcloudstdvector.h"
#include "ct_iterator/ct_mutablepointiterator.h"

#include "ct_log/ct_logmanager.h"

#include <QMessageBox>
#include <QFile>
#include <QDataStream>
#include <QDate>

#include <QDebug>

CT_Reader_LASV2::CT_Reader_LASV2(int subMenuLevel) : SuperClass(subMenuLevel)
{    
    m_headerFromConfiguration = nullptr;

    addNewReadableFormat(FileFormat("las", tr("Fichiers LAS .las")));

    setToolTip(tr("Charge des points depuis un fichier au format LAS (ASPRS)<br>http://www.asprs.org/Committee-General/LASer-LAS-File-Format-Exchange-Activities.html"));
}

CT_Reader_LASV2::CT_Reader_LASV2(const CT_Reader_LASV2 &other) : SuperClass(other)
{
    m_headerFromConfiguration = nullptr;

    if(other.m_headerFromConfiguration != nullptr)
        m_headerFromConfiguration = (CT_LASHeader*)other.m_headerFromConfiguration->copy(nullptr, nullptr);
}

CT_Reader_LASV2::~CT_Reader_LASV2()
{
    delete m_headerFromConfiguration;
}

QString CT_Reader_LASV2::displayableName() const
{
    return tr("Points, format LAS (V2)");
}

bool CT_Reader_LASV2::setFilePath(const QString& filepath)
{
    QString err;
    CT_LASHeader *header = (CT_LASHeader*)internalReadHeader(filepath, err);

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

bool CT_Reader_LASV2::configure()
{
    QString err;
    CT_LASHeader *header = (CT_LASHeader*)internalReadHeader(filepath(), err);

    if(header != nullptr) {
        delete m_headerFromConfiguration;
        m_headerFromConfiguration = header;
        return true;
    }

    delete header;
    return false;
}

CT_FileHeader* CT_Reader_LASV2::createHeaderPrototype() const
{
    return new CT_LASHeader();
}

bool CT_Reader_LASV2::restoreSettings(SettingsReaderInterface &reader)
{
    delete m_headerFromConfiguration;
    m_headerFromConfiguration = nullptr;

    if(SuperClass::restoreSettings(reader)) {
        QString error;
        m_headerFromConfiguration = (CT_LASHeader*)internalReadHeader(filepath(), error);
    }

    return (m_headerFromConfiguration != nullptr);
}

void CT_Reader_LASV2::internalDeclareOutputModels(CT_ReaderOutModelStructureManager &manager)
{
    manager.addItem(m_hOutScene, tr("Scène"));
    manager.addItem(m_hOutAllAtt, tr("All Attributs"));

    manager.addItem(m_hOutReturnNumber, tr("Return Number"));
    manager.addItem(m_hOutNumberOfReturn, tr("Number of Returns"));
    manager.addItem(m_hOutClassificationFlag, tr("Classification Flags"));
    manager.addItem(m_hOutScannerChannel, tr("Scanner Channel"));
    manager.addItem(m_hOutScanDirectionFlag, tr("Scan Direction Flag"));
    manager.addItem(m_hOutEdgeOfFlightLine, tr("Edge of Flight Line"));

    manager.addItem(m_hOutIntensity, tr("Intensité"));
    manager.addItem(m_hOutClassification, tr("Classification"));
    manager.addItem(m_hOutUserData, tr("User Data"));
    manager.addItem(m_hOutPointSourceID, tr("Point Source "));

    manager.addItem(m_hOutScanAngle, tr("Scan Angle"));

    manager.addItem(m_hOutGPSTime, tr("GPS Time"));

    manager.addItem(m_hOutColor, tr("Color"));
    manager.addItem(m_hOutRed, tr("Red"));
    manager.addItem(m_hOutGreen, tr("Green"));
    manager.addItem(m_hOutBlue, tr("Blue"));

    manager.addItem(m_hOutWavePacketDescriptorIndex, tr("Wave Packet Descriptor Index"));
    manager.addItem(m_hOutByteOffsetToWaveformData, tr("Byte Offset To Waveform Data"));
    manager.addItem(m_hOutWaveformPacketSizeInBytes, tr("Waveform Packet Size In Bytes"));
    manager.addItem(m_hOutReturnPointWaveformLocation, tr("Return Point Waveform Location"));

    manager.addItem(m_hOutNIR, tr("NIR"));
}

CT_FileHeader *CT_Reader_LASV2::internalReadHeader(const QString &filepath, QString &error) const
{
    CT_LASHeader *header = nullptr;

    // Test File validity
    if(QFile::exists(filepath))
    {
        QFile f(filepath);

        if(f.open(QIODevice::ReadOnly))
        {
            QDataStream stream(&f);

            header = new CT_LASHeader();
            header->setFilePath(filepath);

            if(!header->read(stream, error)) {
                delete header;
                header = nullptr;
            }
        }

        f.close();
    }

    return header;
}

bool CT_Reader_LASV2::internalReadFile(CT_StandardItemGroup *group)
{
    bool ok = false;
    QString error;

    CT_LASHeader *header = (CT_LASHeader*)internalReadHeader(filepath(), error);

    if(header == nullptr) {
        PS_LOG->addErrorMessage(LogInterface::reader, tr("Impossible de lire l'en-tête du fichier %1").arg(filepath()));
        return false;
    }

//    if(header->m_pointDataRecordFormat != m_headerFromConfiguration->m_pointDataRecordFormat) {
//        PS_LOG->addErrorMessage(LogInterface::reader, tr("Le format (%1) du fichier %2 ne correspond pas au format (%3) du fichier lu lors de la configuration").arg(header->m_pointDataRecordFormat)
//                                .arg(filepath())
//                                .arg(m_headerFromConfiguration->m_pointDataRecordFormat));
//        delete header;
//        return false;
//    }

    size_t nPoints = header->getPointsRecordCount();

    if(nPoints == 0) {
        PS_LOG->addWarningMessage(LogInterface::reader, tr("Aucun points contenu dans le fichier %1").arg(filepath()));
        delete header;
        return true;
    }

    QFile f(filepath());

    if(f.open(QIODevice::ReadOnly))
    {
        setToolTip(header->toString());

        f.seek(header->m_offsetToPointData);

        QDataStream stream(&f);
        stream.setByteOrder(QDataStream::LittleEndian);

        qint32 x, y, z;
        double xc, yc, zc;
        CT_Point pAdded;

        quint64 pos = f.pos();
        bool mustTransformPoint = header->mustTransformPoints();

        CT_NMPCIR pcir = PS_REPOSITORY->createNewPointCloud(nPoints);

        // VECTOR
        CT_StandardCloudStdVectorT<qint16> *scanAngleRankV6_10 = nullptr;
        CT_StandardCloudStdVectorT<PointCore6_10> *mask6_10V = nullptr;

        mask6_10V = new CT_StandardCloudStdVectorT<PointCore6_10>(nPoints);
        scanAngleRankV6_10 = new CT_StandardCloudStdVectorT<qint16>(nPoints);

        // ItemDrawable 6_10
        CT_PointsAttributesScalarMaskT<PointCore6_10> *rn6_10 = nullptr;
        CT_PointsAttributesScalarMaskT<PointCore6_10> *nor6_10 = nullptr;
        CT_PointsAttributesScalarMaskT<PointCore6_10> *sdf6_10 = nullptr;
        CT_PointsAttributesScalarMaskT<PointCore6_10> *efl6_10 =  nullptr;
        CT_PointsAttributesScalarTemplated<qint16> *scanAngleRank6_10 = nullptr;
        CT_PointsAttributesScalarMaskT<PointCore6_10> *cf = nullptr;
        CT_PointsAttributesScalarMaskT<PointCore6_10> *sc = nullptr;

        // Vector in all point data format :
        CT_StandardCloudStdVectorT<quint16> *intensitiesV = new CT_StandardCloudStdVectorT<quint16>(nPoints);
        CT_StandardCloudStdVectorT<quint8> *classificationsV = new CT_StandardCloudStdVectorT<quint8>(nPoints);
        CT_StandardCloudStdVectorT<quint8> *userDataV = new CT_StandardCloudStdVectorT<quint8>(nPoints);
        CT_StandardCloudStdVectorT<quint16> *pointSourceIDV = new CT_StandardCloudStdVectorT<quint16>(nPoints);

        // Vector not in all point data format :
        CT_StandardCloudStdVectorT<double> *gpsTimeV = nullptr;
        CT_ColorCloudStdVector *colorsV = nullptr;
        CT_StandardCloudStdVectorT<quint16> *colorsRV = nullptr;
        CT_StandardCloudStdVectorT<quint16> *colorsGV = nullptr;
        CT_StandardCloudStdVectorT<quint16> *colorsBV = nullptr;
        CT_StandardCloudStdVectorT<quint8> *wpdiV = nullptr;
        CT_StandardCloudStdVectorT<quint64> *botwdV = nullptr;
        CT_StandardCloudStdVectorT<quint32> *wpsibV = nullptr;
        CT_StandardCloudStdVectorT<float> *rpwlV = nullptr;
        CT_StandardCloudStdVectorT<quint16> *nirV = nullptr;

        if((m_headerFromConfiguration->m_pointDataRecordFormat == 1)
                || (m_headerFromConfiguration->m_pointDataRecordFormat > 2))
        {
            gpsTimeV = new CT_StandardCloudStdVectorT<double>(nPoints);
        }

        if((m_headerFromConfiguration->m_pointDataRecordFormat == 2)
                || (m_headerFromConfiguration->m_pointDataRecordFormat == 3)
                || (m_headerFromConfiguration->m_pointDataRecordFormat == 5)
                || (m_headerFromConfiguration->m_pointDataRecordFormat == 7)
                || (m_headerFromConfiguration->m_pointDataRecordFormat == 8)
                || (m_headerFromConfiguration->m_pointDataRecordFormat == 10))
        {
            colorsV = new CT_ColorCloudStdVector(nPoints);
            colorsRV = new CT_StandardCloudStdVectorT<quint16>(nPoints);
            colorsGV = new CT_StandardCloudStdVectorT<quint16>(nPoints);
            colorsBV = new CT_StandardCloudStdVectorT<quint16>(nPoints);
        }

        if((m_headerFromConfiguration->m_pointDataRecordFormat == 8)
                || (m_headerFromConfiguration->m_pointDataRecordFormat == 10))
        {
            nirV = new CT_StandardCloudStdVectorT<quint16>(nPoints);
        }

        if((m_headerFromConfiguration->m_pointDataRecordFormat == 4)
                || (m_headerFromConfiguration->m_pointDataRecordFormat == 5)
                || (m_headerFromConfiguration->m_pointDataRecordFormat == 9)
                || (m_headerFromConfiguration->m_pointDataRecordFormat == 10))
        {
            wpdiV = new CT_StandardCloudStdVectorT<quint8>(nPoints);
            botwdV = new CT_StandardCloudStdVectorT<quint64>(nPoints);
            wpsibV = new CT_StandardCloudStdVectorT<quint32>(nPoints);
            rpwlV = new CT_StandardCloudStdVectorT<float>();
        }
        CT_MutablePointIterator it(pcir);

        for(size_t i=0; i<nPoints; ++i)
        {
            // READ ALL ATTRIBUTES
            stream >> x >> y >> z >> intensitiesV->tAt(i);

            if (header->m_pointDataRecordFormat < 6)
            {
                quint8 mask8;
                stream >> mask8;

                mask6_10V->tAt(i).entire = mask8;
            } else {
                stream >> mask6_10V->tAt(i).entire;
            }

            stream >> classificationsV->tAt(i);

            if(header->m_pointDataRecordFormat < 6)
            {
                qint8 scan8;
                stream >> scan8;
                scanAngleRankV6_10->tAt(i) = scan8;
                stream >> userDataV->tAt(i);
            }
            else
            {
                // "user data" is before "scan angle" in version 6 to 10
                stream >> userDataV->tAt(i);
                stream >> scanAngleRankV6_10->tAt(i);
            }

            stream >> pointSourceIDV->tAt(i);

            // gps time is always after pointSourceID
            if(gpsTimeV != nullptr)
                stream >> gpsTimeV->tAt(i);

            // color is always after gpsTime if exist otherwise after pointSourceID
            if(colorsRV != nullptr)
            {
                CT_Color &colRGBA = colorsV->tAt(i);
                quint16 &colR = colorsRV->tAt(i);
                quint16 &colG = colorsGV->tAt(i);
                quint16 &colB = colorsBV->tAt(i);

                stream >> colR >> colG >> colB;

                colRGBA.r() = colR/256;
                colRGBA.g() = colG/256;
                colRGBA.b() = colB/256;
                colRGBA.a() = 255;
            }

            // NIR is always after colors if exist
            if(nirV != nullptr)
                stream >> nirV->tAt(i);

            // wave packet is always after NIR if exist
            if(wpdiV != nullptr)
                stream >> wpdiV->tAt(i) >> botwdV->tAt(i) >> wpsibV->tAt(i) >> rpwlV->tAt(i);

            // CONVERT POINT

            if(mustTransformPoint)
                header->transformPoint(x, y, z, xc, yc, zc);

            pAdded(0) = xc;
            pAdded(1) = yc;
            pAdded(2) = zc;

            it.next().replaceCurrentPoint(pAdded);

            pos += header->m_pointDataRecordLength;
            f.seek(pos);

            setProgress((100.0*i)/nPoints);
        }

        CT_Scene *scene = new CT_Scene(pcir);
        scene->updateBoundingBox();

        // add the scene
        group->addSingularItem(m_hOutScene, scene);

        // add attributes
        CT_StdLASPointsAttributesContainer *container = new CT_StdLASPointsAttributesContainer();

        if (header->m_pointDataRecordFormat < 6)
        {

            // 0b00000111
            rn6_10 = new CT_PointsAttributesScalarMaskT<PointCore6_10>(7,0,mask6_10V,pcir,true);

            // 0b00111000
            nor6_10 = new CT_PointsAttributesScalarMaskT<PointCore6_10>(56,3,mask6_10V,pcir,false);

            // 0b01000000
            sdf6_10 = new CT_PointsAttributesScalarMaskT<PointCore6_10>(64,6,mask6_10V,pcir,false);

            // 0b10000000
            efl6_10 = new CT_PointsAttributesScalarMaskT<PointCore6_10>(128,7,mask6_10V,pcir,false);

            scanAngleRank6_10 = new CT_PointsAttributesScalarTemplated<qint16>(pcir, scanAngleRankV6_10);

        }
        else
        {
            // 0b0000 0000 0000 1111
            rn6_10 = new CT_PointsAttributesScalarMaskT<PointCore6_10>(15,0,mask6_10V,pcir,true);

            // 0b0000 0000 1111 0000
            nor6_10 = new CT_PointsAttributesScalarMaskT<PointCore6_10>(240, 4,mask6_10V,pcir,false);

            // 0b0000 0111 0000 0000
            cf = new CT_PointsAttributesScalarMaskT<PointCore6_10>(1792, 8,mask6_10V,pcir,false);

            // 0b0001 1000 0000 0000
            sc = new CT_PointsAttributesScalarMaskT<PointCore6_10>(6144, 11,mask6_10V,pcir,false);

            // 0b0100 0000 0000 0000
            sdf6_10 = new CT_PointsAttributesScalarMaskT<PointCore6_10>(16384, 14,mask6_10V,pcir,false);

            // 0b1000 0000 0000 0000
            efl6_10 = new CT_PointsAttributesScalarMaskT<PointCore6_10>(32768, 15,mask6_10V,pcir,false);

            scanAngleRank6_10 = new CT_PointsAttributesScalarTemplated<qint16>(pcir, scanAngleRankV6_10);

            /*for(int iii=0;iii<10; ++iii)
            {
                double tmp1 = nor6_10->dValueAt(iii);
                double tmp2 = rn6_10->dValueAt(iii);
                double tmp3 = sdf6_10->dValueAt(iii);
                double tmp4 = efl6_10->dValueAt(iii);
                double tmp5 = tmp1;
            }*/
        }



        CT_AbstractPointAttributesScalar* attReturnNumber = rn6_10;
        CT_AbstractPointAttributesScalar* attNumberOfReturn = nor6_10;
        CT_AbstractPointAttributesScalar* attClassificationFlag = cf;
        CT_AbstractPointAttributesScalar* attScannerChannel = sc;
        CT_AbstractPointAttributesScalar* attScanDirectionFlag = sdf6_10;
        CT_AbstractPointAttributesScalar* attEdgeOfFlightLine = efl6_10;
        CT_AbstractPointAttributesScalar* attIntensity = new CT_PointsAttributesScalarTemplated<quint16>(pcir, intensitiesV);
        CT_AbstractPointAttributesScalar* attClassification = new CT_PointsAttributesScalarTemplated<quint8>(pcir, classificationsV);
        CT_AbstractPointAttributesScalar* attUserData = new CT_PointsAttributesScalarTemplated<quint8>(pcir, userDataV);
        CT_AbstractPointAttributesScalar* attPointSourceID = new CT_PointsAttributesScalarTemplated<quint16>(pcir, pointSourceIDV);
        CT_AbstractPointAttributesScalar* attScanAngle = scanAngleRank6_10;
        CT_AbstractPointAttributesScalar* attGPSTime = (gpsTimeV == nullptr ? nullptr : new CT_PointsAttributesScalarTemplated<double>(pcir, gpsTimeV));
        CT_AbstractPointAttributesScalar* attRed = (colorsRV == nullptr ? nullptr : new CT_PointsAttributesScalarTemplated<quint16>(pcir, colorsRV));
        CT_AbstractPointAttributesScalar* attGreen = (colorsGV == nullptr ? nullptr : new CT_PointsAttributesScalarTemplated<quint16>(pcir, colorsGV));
        CT_AbstractPointAttributesScalar* attBlue = (colorsBV == nullptr ? nullptr : new CT_PointsAttributesScalarTemplated<quint16>(pcir, colorsBV));
        CT_AbstractPointAttributesScalar* attWavePacketDescriptorIndex = (wpdiV == nullptr ? nullptr : new CT_PointsAttributesScalarTemplated<quint8>(pcir, wpdiV));
        CT_AbstractPointAttributesScalar* attByteOffsetToWaveformData = (botwdV == nullptr ? nullptr : new CT_PointsAttributesScalarTemplated<quint64>(pcir, botwdV));
        CT_AbstractPointAttributesScalar* attWaveformPacketSizeInBytes = (wpsibV == nullptr ? nullptr : new CT_PointsAttributesScalarTemplated<quint32>(pcir, wpsibV));
        CT_AbstractPointAttributesScalar* attReturnPointWaveformLocation = (rpwlV == nullptr ? nullptr : new CT_PointsAttributesScalarTemplated<float>(pcir, rpwlV));
        CT_AbstractPointAttributesScalar* attNIR = (nirV == nullptr ? nullptr : new CT_PointsAttributesScalarTemplated<quint16>(pcir, nirV));

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
        if (colorsV != nullptr) {group->addSingularItem(m_hOutColor, new CT_PointsAttributesColor(pcir, colorsV));}
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

        ok = true;
    } else {
        PS_LOG->addErrorMessage(LogInterface::reader, tr("Error while loading the file ") + filepath());
    }

    f.close();

    delete header;

    return ok;

}

