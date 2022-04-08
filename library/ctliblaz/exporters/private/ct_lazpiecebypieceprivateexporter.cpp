#include "ct_lazpiecebypieceprivateexporter.h"

#include <QFileInfo>
#include <QDir>
#include <QDate>

#include "ctliblas/mergers/ct_sameheaderformatmerger_las.h"
#include "ctliblas/tools/las/abstract/ct_abstractlaspointformat.h"

#include "ct_log/ct_logmanager.h"

#include "laszip/laszip_api.h"

CT_LAZPieceByPiecePrivateExporter::CT_LAZPieceByPiecePrivateExporter(CT_Exporter_LAZ& exporter, const QString& filePath) :
    mExporter(exporter),
    mFilePath(filePath),
    mHeader(nullptr)
{
    mPointsToWrite.setSortType(CT_AbstractCloudIndex::NotSorted);
}

CT_LAZPieceByPiecePrivateExporter::~CT_LAZPieceByPiecePrivateExporter()
{
    delete mHeader;
    clearAllBackupedHeaders();
}

bool CT_LAZPieceByPiecePrivateExporter::internalCreateFile()
{
    delete mHeader;
    mHeader = nullptr;
    clearAllBackupedHeaders();

    const QFileInfo exportPathInfo = QFileInfo(mFilePath);
    const QString path = exportPathInfo.path();
    const QString baseName = exportPathInfo.baseName();

    mBaseFilePath = QDir::toNativeSeparators(path + QDir::separator() + baseName + QDir::separator());

    return true;
}

bool CT_LAZPieceByPiecePrivateExporter::internalOpenFileInAppendMode()
{
    if(mBaseFilePath.isEmpty())
        return false;

    // nothing to do here because the file will be created in finalizeHeaderAndWritePoints() method

    return true;
}

bool CT_LAZPieceByPiecePrivateExporter::initializeHeader(const quint8 format, const quint16 pointDataLength)
{
    delete mHeader;
    mHeader = nullptr;

    mReturnNumberSet = false;

    if(format < 11)
    {
        mHeader = new CT_LAZHeader();

        mHeader->m_pointDataRecordFormat = format;
        mHeader->m_pointDataRecordLength = pointDataLength;

        CT_Point min;
        min.setX(std::numeric_limits<double>::max());
        min.setY(min.x());
        min.setZ(min.x());

        CT_Point max;
        max.setX(-min.x());
        max.setY(max.x());
        max.setZ(max.x());

        mHeader->setBoundingBox(min, max);

        for(int i=0; i<5; ++i)
            mHeader->m_legacyNumberOfPointsByReturn[i] = 0; // deprecated in 1.4

        for(int i=0; i<15; ++i)
            mHeader->m_numberOfPointsByReturn[i] = 0;

        mExporter.mToolsFormat->initReturnNumber(mExporter.mLasAttributeByType);

        return true;
    }

    return false;
}

void CT_LAZPieceByPiecePrivateExporter::computePointForHeader(const CT_AbstractPointAttributesScalar* returnNumberValues,
                                                              const size_t& gi,
                                                              const CT_Point& p)
{
    if(mPointFilter(gi, p))
    {
        // save point index in a list so it will be used in finalizeHeaderAndWritePoints()
        mPointsToWrite.addIndex(gi);

        if(p(0) > mHeader->m_maxCoordinates(0))
            mHeader->m_maxCoordinates(0) = p(0);

        if(p(1) > mHeader->m_maxCoordinates(1))
            mHeader->m_maxCoordinates(1) = p(1);

        if(p(2) > mHeader->m_maxCoordinates(2))
            mHeader->m_maxCoordinates(2) = p(2);

        if(p(0) < mHeader->m_minCoordinates(0))
            mHeader->m_minCoordinates(0) = p(0);

        if(p(1) < mHeader->m_minCoordinates(1))
            mHeader->m_minCoordinates(1) = p(1);

        if(p(2) < mHeader->m_minCoordinates(2))
            mHeader->m_minCoordinates(2) = p(2);

        if((returnNumberValues != nullptr) && (returnNumberValues->hasBeenSet(gi)))
        {
            mReturnNumberSet = true;
            const int indexInTab = qMax(0, int(returnNumberValues->scalarAsDoubleAt(gi))-1);

            quint64 &val = mHeader->m_numberOfPointsByReturn[indexInTab];
            ++val;

            if(indexInTab < 5)
            {
                quint32 &legacyVal = mHeader->m_legacyNumberOfPointsByReturn[indexInTab];
                ++legacyVal;
            }
        }

        ++mHeader->m_numberOfPointRecords;
    }
}

bool CT_LAZPieceByPiecePrivateExporter::finalizeHeaderAndWritePoints()
{
    if((mHeader->m_numberOfPointRecords == 0) || mExporter.mToolsFormat.isNull()) {
        delete mHeader;
        mHeader = nullptr;
        return false;
    }

    if(!mReturnNumberSet)
    {
        mHeader->m_numberOfPointsByReturn[0] = mHeader->m_numberOfPointRecords;
        mHeader->m_legacyNumberOfPointsByReturn[0] = quint32(mHeader->m_numberOfPointRecords);
    }

    QDate date = QDate::currentDate();

    mHeader->m_fileSourceID = 1;
    mHeader->m_globalEncoding = 1;
    mHeader->m_projectIDGuidData1 = 0;
    mHeader->m_projectIDGuidData2 = 0;
    mHeader->m_projectIDGuidData3 = 0;
    mHeader->m_projectIDGuidData4 = 0;
    mHeader->m_versionMajor = 1;  // format 1.4
    mHeader->m_versionMinor = 4;
#ifdef _MSC_VER
    strcpy_s(mHeader->m_systemID, "EXPORT");
    strcpy_s(mHeader->m_sofwareID, "Computree 6.0");
#else
    strcpy(mHeader->m_systemID, "EXPORT");
    strcpy(mHeader->m_sofwareID, "Computree 6.0");
#endif
    mHeader->m_fileCreationDayOfYear = quint16(date.dayOfYear());
    mHeader->m_fileCreationYear = quint16(date.year());
    mHeader->m_numberOfVariableLengthRecords = 0; // TODO : write the good value

    if(mHeader->m_numberOfPointRecords < std::numeric_limits<quint32>::max())
        mHeader->m_legacyNumberOfPointRecord = quint32(mHeader->m_numberOfPointRecords); // deprecated in 1.4
    else
        mHeader->m_legacyNumberOfPointRecord = 0;

    mHeader->m_xOffset = 0;
    mHeader->m_yOffset = 0;
    mHeader->m_zOffset = 0;
    mHeader->setBoundingBox(mHeader->m_minCoordinates,  mHeader->m_maxCoordinates);
    mHeader->updateScaleFactorFromBoundingBox();
    mHeader->m_startOfWaveformDataPacketRecord = 0; // TODO : write the good value
    mHeader->m_startOfFirstExtendedVariableLengthRecord = 0; // TODO : write the good value
    mHeader->m_numberOfExtendedVariableLengthRecords = 0; // TODO : write the good value

    // find the backup or create a new backup for this header
    HeaderBackup* hBackup = createOrGetHeaderBackupForHeader(mHeader);

    bool ok = false;

    // The filepath will have this structure :
    // - output directory path (choosed by user) + a directory with the name of the file to export + a directory with a unique index for each header (index in the list)
    //
    // So in the last directory we can have multiple files if it has the same header (same format, version, etc...)

    const QString filepath = hBackup->currentFilePath();

    QFileInfo fi(filepath);
    QDir().mkpath(fi.dir().path());

    // create the writer
    laszip_POINTER laszip_writer = nullptr;
    if (laszip_create(&laszip_writer))
    {
        PS_LOG->addErrorMessage(LogInterface::exporter, tr("Impossible de créer le writer LASZIP"));
        return false;
    }

    // initialize the header for the writer using the header of the reader
    laszip_header header;
    if(!writeHeader(mHeader, header))
    {
        delete mHeader;
        mHeader = nullptr;
    }
    if (laszip_set_header(laszip_writer, &header))
    {
        PS_LOG->addErrorMessage(LogInterface::exporter, tr("Impossible de charger le header LASZIP"));
        return false;
    }

    // open the writer
    laszip_BOOL compress = true;
    if (laszip_open_writer(laszip_writer, filepath.toStdString().c_str(), compress))
    {
        PS_LOG->addErrorMessage(LogInterface::exporter, tr("Impossible d'ouvrir le fichier LASZIP en écriture"));
        return false;
    }

    if(mHeader == nullptr)
    {
        setErrorMessage(tr("Problème lors de l'écriture de l'en-tête du fichier. %1").arg(errorMessage()));
    }
    else
    {
        mExporter.mToolsFormat->initWrite(mExporter.mLasAttributeByType, mExporter.mColorAttribute);

        // write points
        const double progressValue = 100.0 / double(mPointsToWrite.size());
        double progress = 0;

        CT_PointIterator itI(&mPointsToWrite);
        const CT_AbstractPointAttributesScalar* sc;
        const CT_PointsAttributesColor* colorValues;

        while(itI.hasNext()
              && !mMustCancel)
        {
            itI.next();

            //QDataStream stream;
            //stream.setByteOrder(QDataStream::LittleEndian);
            //mExporter.mToolsFormat->write(stream, mHeader, itI.currentPoint(), itI.cIndex());

            // Convert CT point to LASZIP point
            laszip_point point;

            CT_Point p = itI.currentPoint();
            size_t globalIndex = itI.cIndex();

            if(mHeader->mustTransformPoints())
            {
                qint32 m_x, m_y, m_z;
                mHeader->inverseTransformPoint(p(CT_Point::X), p(CT_Point::Y), p(CT_Point::Z), m_x, m_y, m_z);

                point.X = m_x;
                point.Y = m_y;
                point.Z = m_z;
            }
            else
            {
                point.X = p(CT_Point::X);
                point.Y = p(CT_Point::Y);
                point.Z = p(CT_Point::Z);
            }

            // All other attributes are set to 0 by default in the structure.
            // Here we save the attributes that are found for the current point/index.

            sc = mExporter.mToolsFormat->scalars(CT_LasDefine::Intensity);
            if((sc != nullptr) && sc->hasBeenSet(globalIndex))
                point.intensity = static_cast<quint16>(sc->scalarAsDoubleAt(globalIndex));

            sc = mExporter.mToolsFormat->scalars(CT_LasDefine::Return_Number);
            if((sc != nullptr) && sc->hasBeenSet(globalIndex))
                point.return_number = static_cast<quint8>(sc->scalarAsDoubleAt(globalIndex));

            sc = mExporter.mToolsFormat->scalars(CT_LasDefine::Number_of_Returns);
            if((sc != nullptr) && sc->hasBeenSet(globalIndex))
                point.number_of_returns = static_cast<quint8>(sc->scalarAsDoubleAt(globalIndex));

            sc = mExporter.mToolsFormat->scalars(CT_LasDefine::Classification_Flags);
            if((sc != nullptr) && sc->hasBeenSet(globalIndex))
                point.extended_classification_flags = static_cast<quint8>(sc->scalarAsDoubleAt(globalIndex));

            sc = mExporter.mToolsFormat->scalars(CT_LasDefine::Scanner_Channel);
            if((sc != nullptr) && sc->hasBeenSet(globalIndex))
                point.extended_scanner_channel = static_cast<quint8>(sc->scalarAsDoubleAt(globalIndex));

            sc = mExporter.mToolsFormat->scalars(CT_LasDefine::Scan_Direction_Flag);
            if((sc != nullptr) && sc->hasBeenSet(globalIndex))
                point.scan_direction_flag = static_cast<quint8>(sc->scalarAsDoubleAt(globalIndex));

            sc = mExporter.mToolsFormat->scalars(CT_LasDefine::Edge_of_Flight_Line);
            if((sc != nullptr) && sc->hasBeenSet(globalIndex))
                point.edge_of_flight_line = static_cast<quint8>(sc->scalarAsDoubleAt(globalIndex));

            sc = mExporter.mToolsFormat->scalars(CT_LasDefine::Classification);
            if((sc != nullptr) && sc->hasBeenSet(globalIndex))
                point.classification = static_cast<quint8>(sc->scalarAsDoubleAt(globalIndex));

            sc = mExporter.mToolsFormat->scalars(CT_LasDefine::User_Data);
            if((sc != nullptr) && sc->hasBeenSet(globalIndex))
                point.user_data = static_cast<quint8>(sc->scalarAsDoubleAt(globalIndex));

            sc = mExporter.mToolsFormat->scalars(CT_LasDefine::Scan_Angle_Rank);
            if((sc != nullptr) && sc->hasBeenSet(globalIndex))
                point.scan_angle_rank = static_cast<quint16>(sc->scalarAsDoubleAt(globalIndex));

            sc = mExporter.mToolsFormat->scalars(CT_LasDefine::Point_Source_ID);
            if((sc != nullptr) && sc->hasBeenSet(globalIndex))
                point.point_source_ID = static_cast<quint16>(sc->scalarAsDoubleAt(globalIndex));

            sc = mExporter.mToolsFormat->scalars(CT_LasDefine::GPS_Time);
            if((sc != nullptr) && sc->hasBeenSet(globalIndex))
                point.gps_time = static_cast<double>(sc->scalarAsDoubleAt(globalIndex));

            colorValues = mExporter.mToolsFormat->colors();
            // If CT color has been set manually
            if((colorValues != nullptr) && colorValues->hasBeenSet(globalIndex))
            {
                // TODO : *257 ???
                const CT_Color& color = colorValues->constColorAt(globalIndex);
                point.rgb[0] = quint16(color.r()*257);
                point.rgb[1] = quint16(color.g()*257);
                point.rgb[2] = quint16(color.b()*257);
            }
            // Otherwise we use the original point colors
            else
            {
                sc = mExporter.mToolsFormat->scalars(CT_LasDefine::Red);
                if((sc != nullptr) && sc->hasBeenSet(globalIndex))
                    point.rgb[0] = static_cast<quint8>(sc->scalarAsDoubleAt(globalIndex));

                sc = mExporter.mToolsFormat->scalars(CT_LasDefine::Green);
                if((sc != nullptr) && sc->hasBeenSet(globalIndex))
                    point.rgb[1] = static_cast<quint8>(sc->scalarAsDoubleAt(globalIndex));

                sc = mExporter.mToolsFormat->scalars(CT_LasDefine::Blue);
                if((sc != nullptr) && sc->hasBeenSet(globalIndex))
                    point.rgb[2] = static_cast<quint8>(sc->scalarAsDoubleAt(globalIndex));
            }

            sc = mExporter.mToolsFormat->scalars(CT_LasDefine::NIR);
            if((sc != nullptr) && sc->hasBeenSet(globalIndex))
                point.rgb[3] = static_cast<quint8>(sc->scalarAsDoubleAt(globalIndex));

            // For the moùent, we don't use the wave form.
//            sc = mExporter.mToolsFormat->scalars(CT_LasDefine::Wave_Packet_Descriptor_Index);
//            if((sc != nullptr) && sc->hasBeenSet(globalIndex))
//                point.wave_packet[0] = static_cast<quint8>(sc->scalarAsDoubleAt(globalIndex));

//            sc = mExporter.mToolsFormat->scalars(CT_LasDefine::Byte_offset_to_waveform_data);
//            if((sc != nullptr) && sc->hasBeenSet(globalIndex))
//                point.wave_packet[1] = static_cast<quint64>(sc->scalarAsDoubleAt(globalIndex));

//            sc = mExporter.mToolsFormat->scalars(CT_LasDefine::Waveform_packet_size_in_bytes);
//            if((sc != nullptr) && sc->hasBeenSet(globalIndex))
//                point.wave_packet[9] = static_cast<quint32>(sc->scalarAsDoubleAt(globalIndex));

//            sc = mExporter.mToolsFormat->scalars(CT_LasDefine::Return_Point_Waveform_Location);
//            if((sc != nullptr) && sc->hasBeenSet(globalIndex))
//                point.wave_packet[13] = static_cast<float>(sc->scalarAsDoubleAt(globalIndex));

            // copy the point
            if (laszip_set_point(laszip_writer, &point))
            {
                PS_LOG->addErrorMessage(LogInterface::exporter, tr("Impossible de copier le point vers le writer"));
                delete mHeader;
                return false;
            }

            // write the point
            if (laszip_write_point(laszip_writer))
            {
                PS_LOG->addErrorMessage(LogInterface::exporter, tr("Impossible d'écrire le point dans le fichier"));
                delete mHeader;
                return false;
            }

            progress += progressValue;

            setProgress(int(progress));
        }

        mPointsToWrite.resize(0);
        ok = true;

        // close the writer
        if (laszip_close_writer(laszip_writer))
        {
            PS_LOG->addErrorMessage(LogInterface::exporter, tr("Impossible de fermer le fichier"));
            delete mHeader;
            return false;
        }

        // destroy the writer
        if (laszip_destroy(laszip_writer))
        {
            PS_LOG->addErrorMessage(LogInterface::exporter, tr("Impossible de détruire le writer"));
            delete mHeader;
            return false;
        }
    }

    saveOrDeleteHeaderBackup(hBackup, mHeader, (ok && !mMustCancel));
    mHeader = nullptr;
    return ok;
}

void CT_LAZPieceByPiecePrivateExporter::internalCloseFile()
{
    // nothing to do here because the file is already closed in finalizeHeaderAndWritePoints() method
}

bool CT_LAZPieceByPiecePrivateExporter::internalFinalizeFile()
{
    bool ok = false;

    QString finalFilePath = mBaseFilePath;
    finalFilePath.remove(finalFilePath.size()-1, 1);
    finalFilePath += ".laz";

    PS_LOG->addInfoMessage(LogInterface::exporter, tr("Export du fichier %1 terminé").arg(finalFilePath));

    QFile::remove(finalFilePath);

    if((mHeaders.size() == 1) && (mHeaders.first()->nFiles == 1))
    {
        const QString fp = mHeaders.first()->previousFilePath();

        QFile f(fp);
        ok = f.copy(finalFilePath);

    }
    else if(!mHeaders.isEmpty())
    {
        QStringList filesPath;

        for(const HeaderBackup* b : mHeaders)
        {
            for(int i=0; i<b->nFiles; ++i)
            {
               filesPath.append(b->generateFilePath(i));
            }
        }

        CT_SameHeaderFormatMerger_LAS merger;
        merger.setInputFilesPath(filesPath);
        merger.setOutputFilePath(finalFilePath);
        merger.process();
        ok = !merger.hasError();

        if(!ok)
            setErrorMessage(merger.errorMessage());
    }

    clearAllBackupedHeaders();

    return ok;
}

bool CT_LAZPieceByPiecePrivateExporter::writeHeader(CT_LAZHeader* header,
                                                    laszip_header &zipHeader)
{
    if(header == nullptr)
        return false;

    if(!header->finalizeBeforeWrite())
        return false;

    QString error;
    bool ok = header->writezip(zipHeader);

    if(!ok)
        setErrorMessage(error);

    return ok;
}

CT_LAZPieceByPiecePrivateExporter::HeaderBackup* CT_LAZPieceByPiecePrivateExporter::createOrGetHeaderBackupForHeader(const CT_LAZHeader* header) const
{
    for(HeaderBackup* b : mHeaders)
    {
        if(b->isHeaderSameForExport(header))
            return b;
    }

    HeaderBackup* b = new HeaderBackup();
    b->header = header;
    b->nFiles = 0;
    b->dirPath = QDir::toNativeSeparators(mBaseFilePath + QString().setNum(mHeaders.size()) + QDir::separator());

    return b;
}

void CT_LAZPieceByPiecePrivateExporter::saveOrDeleteHeaderBackup(CT_LAZPieceByPiecePrivateExporter::HeaderBackup* backup, CT_LAZHeader* header, bool writeOfFileIsOk)
{
    if(backup == nullptr)
        return;

    if(writeOfFileIsOk)
        ++backup->nFiles;

    if(!mHeaders.contains(backup))
    {
        if(writeOfFileIsOk)
            mHeaders.append(backup);
        else
            delete backup;
    }
    else if(!writeOfFileIsOk && (backup->header != header))
    {
        delete header;
    }
}

void CT_LAZPieceByPiecePrivateExporter::clearAllBackupedHeaders()
{
    for(HeaderBackup* b : mHeaders)
    {
        for(int i=0; i<b->nFiles; ++i) {
            QFile::remove(b->generateFilePath(i));
        }

        QDir d(b->generateFilePath(0));
        d.cd("..");

        if(d.entryList(QDir::NoDotAndDotDot | QDir::AllEntries).isEmpty()) {
            if(d.removeRecursively()) {
                d.cd("..");

                if(d.entryList(QDir::NoDotAndDotDot | QDir::AllEntries).isEmpty())
                    d.removeRecursively();
            }
        }

        delete b;
    }

    mHeaders.clear();
}

bool CT_LAZPieceByPiecePrivateExporter::HeaderBackup::isHeaderSameForExport(const CT_LAZHeader* header) const
{
    return this->header->isHeaderAlmostSimilar(*header);
}

QString CT_LAZPieceByPiecePrivateExporter::HeaderBackup::previousFilePath() const
{
    return generateFilePath(nFiles-1);
}

QString CT_LAZPieceByPiecePrivateExporter::HeaderBackup::currentFilePath() const
{
    return generateFilePath(nFiles);
}

QString CT_LAZPieceByPiecePrivateExporter::HeaderBackup::generateFilePath(int n) const
{
    if(n < 0)
        return "";

    return dirPath + QString().setNum(n) + ".laz";
}
