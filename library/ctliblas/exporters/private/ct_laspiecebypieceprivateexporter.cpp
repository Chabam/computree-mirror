#include "ct_laspiecebypieceprivateexporter.h"

#include <QFileInfo>
#include <QDir>
#include <QDate>

#include "ctliblas/mergers/ct_sameheaderformatmerger_las.h"

#include "ct_log/ct_logmanager.h"

CT_LASPieceByPiecePrivateExporter::CT_LASPieceByPiecePrivateExporter(CT_Exporter_LAS& exporter, const QString& filePath) :
    mExporter(exporter),
    mFilePath(filePath),
    mHeader(nullptr)
{
    mPointsToWrite.setSortType(CT_AbstractCloudIndex::NotSorted);
}

CT_LASPieceByPiecePrivateExporter::~CT_LASPieceByPiecePrivateExporter()
{
    delete mHeader;
    clearAllBackupedHeaders();
}

bool CT_LASPieceByPiecePrivateExporter::internalCreateFile()
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

bool CT_LASPieceByPiecePrivateExporter::internalOpenFileInAppendMode()
{
    if(mBaseFilePath.isEmpty())
        return false;

    // nothing to do here because the file will be created in finalizeHeaderAndWritePoints() method

    return true;
}

bool CT_LASPieceByPiecePrivateExporter::initializeHeader(const quint8 format, const quint16 pointDataLength)
{
    delete mHeader;
    mHeader = nullptr;

    mReturnNumberSet = false;

    if(format < 11)
    {
        mHeader = new CT_LASHeader();

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

        mExporter.mToolsFormat->initReturnNumber(mExporter.mAllLasAttributes);

        return true;
    }

    return false;
}

void CT_LASPieceByPiecePrivateExporter::computePointForHeader(const size_t& gi, const CT_Point& p)
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

        const CT_LasPointInfo& info = mExporter.mToolsFormat->infoOfPoint(gi);

        if(info.m_rn.first != nullptr)
        {
            mReturnNumberSet = true;
            const int indexInTab = qMax(0, int(info.m_rn.first->dValueAt(info.m_rn.second))-1);

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

bool CT_LASPieceByPiecePrivateExporter::finalizeHeaderAndWritePoints()
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

    QString error;
    QDate date = QDate::currentDate();

    mHeader->m_fileSourceID = 1;
    mHeader->m_globalEncoding = 1;
    mHeader->m_projectIDGuidData1 = 0;
    mHeader->m_projectIDGuidData2 = 0;
    mHeader->m_projectIDGuidData3 = 0;
    mHeader->m_projectIDGuidData4 = 0;
    mHeader->m_versionMajor = 1;  // format 1.2
    mHeader->m_versionMinor = 2;
#ifdef _MSC_VER
    strcpy_s(mHeader->m_systemID, "EXPORT");
    strcpy_s(mHeader->m_sofwareID, "Computree 3.0");
#else
    strcpy(mHeader->m_systemID, "EXPORT");
    strcpy(mHeader->m_sofwareID, "Computree 3.0");
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

    QFile file(filepath);
    QDir().mkpath(fi.dir().path());

    if(file.open(QFile::WriteOnly))
    {
        QDataStream stream(&file);
        stream.setByteOrder(QDataStream::LittleEndian);

        // write it
        if(!writeHeader(stream, mHeader)) {
            delete mHeader;
            mHeader = nullptr;
        }

        if(mHeader == nullptr)
        {
            setErrorMessage(tr("Problème lors de l'écriture de l'en-tête du fichier. %1").arg(errorMessage()));
        }
        else
        {
            mExporter.mToolsFormat->initWrite(mExporter.mAllLasAttributes, mExporter.mColorsAttribute);

            // write points
            const double progressValue = 100.0 / double(mPointsToWrite.size());
            double progress = 0;

            CT_PointIterator itI(&mPointsToWrite);

            while(itI.hasNext()
                  && !mMustCancel)
            {
                itI.next();

                mExporter.mToolsFormat->write(stream, mHeader, itI.currentPoint(), itI.cIndex());

                progress += progressValue;

                setProgress(int(progress));
            }

            mPointsToWrite.resize(0);
            ok = true;
        }

        file.close();
    } else {
        setErrorMessage(tr("Erreur lors de l'ouverture en écriture du fichier %1").arg(filepath));
    }

    saveOrDeleteHeaderBackup(hBackup, mHeader, (ok && !mMustCancel));
    mHeader = nullptr;
    return ok;
}

void CT_LASPieceByPiecePrivateExporter::internalCloseFile()
{
    // nothing to do here because the file is already closed in finalizeHeaderAndWritePoints() method
}

bool CT_LASPieceByPiecePrivateExporter::internalFinalizeFile()
{
    bool ok = false;

    QString finalFilePath = mBaseFilePath;
    finalFilePath.remove(finalFilePath.size()-1, 1);
    finalFilePath += ".las";

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

bool CT_LASPieceByPiecePrivateExporter::writeHeader(QDataStream& stream,
                                                    CT_LASHeader* header)
{
    if(header == nullptr)
        return false;

    if(!header->finalizeBeforeWrite())
        return false;

    QString error;
    bool ok = header->write(stream, error);

    if(!ok)
        setErrorMessage(error);

    return ok;
}

CT_LASPieceByPiecePrivateExporter::HeaderBackup* CT_LASPieceByPiecePrivateExporter::createOrGetHeaderBackupForHeader(const CT_LASHeader* header) const
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

void CT_LASPieceByPiecePrivateExporter::saveOrDeleteHeaderBackup(CT_LASPieceByPiecePrivateExporter::HeaderBackup* backup, CT_LASHeader* header, bool writeOfFileIsOk)
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

void CT_LASPieceByPiecePrivateExporter::clearAllBackupedHeaders()
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

bool CT_LASPieceByPiecePrivateExporter::HeaderBackup::isHeaderSameForExport(const CT_LASHeader* header) const
{
    return this->header->isHeaderAlmostSimilar(*header);
}

QString CT_LASPieceByPiecePrivateExporter::HeaderBackup::previousFilePath() const
{
    return generateFilePath(nFiles-1);
}

QString CT_LASPieceByPiecePrivateExporter::HeaderBackup::currentFilePath() const
{
    return generateFilePath(nFiles);
}

QString CT_LASPieceByPiecePrivateExporter::HeaderBackup::generateFilePath(int n) const
{
    if(n < 0)
        return "";

    return dirPath + QString().setNum(n) + ".las";
}
