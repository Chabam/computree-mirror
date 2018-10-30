#include "ct_exporter_las.h"

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

#include "ctliblas/mergers/ct_sameheaderformatmerger_las.h"

#include "ct_iterator/ct_pointiterator.h"

#include "ct_itemdrawable/tools/ct_itemsearchhelper.h"

#include "ct_model/tools/ct_modelsaverestorehelper.h"

#include <QFileInfo>
#include <QDateTime>
#include <QInputDialog>
#include <QDir>
#include <limits>
#include <string.h>
#include <QDebug>

CT_Exporter_LAS::CT_Exporter_LAS() : SuperClass()
{
    setCanExportItems(true);

    setCanExportPoints(true);
    setCanExportEdges(false);
    setCanExportFaces(false);

    setCanExportColors(false);
    setCanExportNormals(false);

    m_lasContainer = NULL;
    m_lasContainerModel = NULL;
}

CT_Exporter_LAS::CT_Exporter_LAS(const CT_Exporter_LAS &other) : SuperClass(other)
{
    m_lasContainer = other.m_lasContainer;
    m_lasContainerModel = other.m_lasContainerModel;
    m_baseFilePath = other.m_baseFilePath;

    // don't copy header backup ! because not really necessary.
    // it will be created in the method "exportOnePieceOfDataToFile"
}

CT_Exporter_LAS::~CT_Exporter_LAS()
{
    clearAllBackupedHeaders();
}

QString CT_Exporter_LAS::getExporterCustomName() const
{
    return tr("Points, LAS");
}

CT_StepsMenu::LevelPredefined CT_Exporter_LAS::getExporterSubMenuName() const
{
    return CT_StepsMenu::LP_Points;
}

void CT_Exporter_LAS::init()
{
    addNewExportFormat(FileFormat("las", tr("Fichiers LAS .las")));

    setToolTip(tr("Exporte tous les points au format LAS, ASPRS<br>http://www.asprs.org/Committee-General/LASer-LAS-File-Format-Exchange-Activities.html"));
}

bool CT_Exporter_LAS::setItemDrawableToExport(const QList<CT_AbstractItemDrawable *> &list)
{
    QList<CT_AbstractItemDrawable*> myList;
    QListIterator<CT_AbstractItemDrawable*> it(list);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

        if(dynamic_cast<CT_IAccessPointCloud*>(item) != NULL)
            myList.append(item);
    }

    if(myList.isEmpty())
    {
        setErrorMessage(errorMessage() + "\r\n" + tr("- Aucun ItemDrawable du type CT_IAccessPointCloud"));
        return false;
    }

    return CT_AbstractExporter::setItemDrawableToExport(myList);
}

bool CT_Exporter_LAS::setPointsToExport(const QList<CT_AbstractCloudIndex*> &list)
{
    QList<CT_AbstractCloudIndex*> myList;
    QListIterator<CT_AbstractCloudIndex*> it(list);

    while(it.hasNext())
    {
        CT_AbstractCloudIndex *item = it.next();

        if(dynamic_cast<CT_AbstractPointCloudIndex*>(item) != NULL)
            myList.append(item);
    }

    if(myList.isEmpty())
    {
        setErrorMessage(errorMessage() + "\r\n" + tr("- Aucun points à exporter"));
        return false;
    }

    return CT_AbstractExporter::setPointsToExport(myList);
}

void CT_Exporter_LAS::saveSettings(SettingsWriterInterface &writer) const
{
    SuperClass::saveSettings(writer);

    CT_ModelSaveRestoreHelper::saveSettingsOfOutModel(this, writer, m_lasContainerModel, "LasContainerModel");
}

bool CT_Exporter_LAS::restoreSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restoreSettings(reader))
        return false;

    clearSelection();

    bool savedAndFoundedOrNotSaved;
    m_lasContainerModel = dynamic_cast<CT_OutAbstractSingularItemModel *>(CT_ModelSaveRestoreHelper::restoreOutModelFromSettings(this, reader, myStep(), "LasContainerModel", savedAndFoundedOrNotSaved));

    return savedAndFoundedOrNotSaved;
}

QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > CT_Exporter_LAS::createBuilders() const
{
    QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > l;
    l << QPair<QString, CT_AbstractItemDrawableCollectionBuilder*>(QObject::tr("Attributs"), new LasAttributesBuilder(true));

    return l;
}

bool CT_Exporter_LAS::useSelection(const CT_ItemDrawableHierarchyCollectionWidget *selectorWidget)
{
    clearSelection();

    const QList<CT_StdLASPointsAttributesContainer*> list = selectorWidget->itemDrawableSelectedT<CT_StdLASPointsAttributesContainer>();

    if(!list.isEmpty()) {
        m_lasContainer = list.first();
    } else {

        // search the model to use
        const QList<CT_OutAbstractSingularItemModel*> listM = selectorWidget->itemDrawableModelSelectedT<CT_StdLASPointsAttributesContainer>();

        if(!listM.isEmpty())
            m_lasContainerModel = listM.first();
    }

    return true;
}

bool CT_Exporter_LAS::canExportPieceByPiece() const
{
    return true;
}

bool CT_Exporter_LAS::createExportFileForPieceByPieceExport()
{
    clearAllBackupedHeaders();

    QFileInfo exportPathInfo = QFileInfo(exportFilePath());
    QString path = exportPathInfo.path();
    QString baseName = exportPathInfo.baseName();

    m_baseFilePath = QDir::toNativeSeparators(path + QDir::separator() + baseName + QDir::separator());

    return true;
}

bool CT_Exporter_LAS::exportOnePieceOfDataToFile()
{
    if(m_lasContainerModel != NULL) {

        CT_ItemSearchHelper helper;
        CT_ResultItemIterator it = helper.searchSingularItemsForModel(m_lasContainerModel);

        if(it.hasNext())
            m_lasContainer = dynamic_cast<CT_StdLASPointsAttributesContainer*>((CT_AbstractSingularItemDrawable*)it.next());
    }

    bool ok = false;

    if(getPointDataFormat() > 0)
    {
        QList<CT_AbstractPointCloudIndex*> indexes;

        QListIterator<CT_AbstractItemDrawable*> it(itemDrawableToExport());

        while(it.hasNext())
        {
            CT_AbstractItemDrawable *item = it.next();

            const CT_AbstractPointCloudIndex *index = dynamic_cast<CT_IAccessPointCloud*>(item)->getPointCloudIndex();

            if(index != NULL)
                indexes.append((CT_AbstractPointCloudIndex*)index);
        }

        QListIterator<CT_AbstractCloudIndex*> itP(pointsToExport());

        while(itP.hasNext())
            indexes.append(dynamic_cast<CT_AbstractPointCloudIndex*>(itP.next()));

        size_t nPoints = 0;

        // create the header
        CT_LASHeader* header = createHeader((m_lasContainer == NULL ? NULL : (CT_AbstractPointAttributesScalar*)m_lasContainer->pointsAttributesAt(CT_LasDefine::Return_Number)),
                                            indexes,
                                            nPoints);

        if(nPoints == 0) {
            delete header;
            header = NULL;
        }

        if(header != NULL) {
            // find the backup or create a new backup for this header
            HeaderBackup* hBackup = createOrGetHeaderBackupForHeader(header);

            if(hBackup->toolsFormat != NULL)
            {
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
                    if(!writeHeader(stream, header)) {
                        delete header;
                        header = NULL;
                    }

                    if(header == NULL)
                    {
                        setErrorMessage(tr("Problème lors de l'écriture de l'en-tête du fichier. %1").arg(errorMessage()));
                    }
                    else
                    {
                        // write points
                        hBackup->toolsFormat->setHeader(header);

                        if(m_lasContainer != NULL)
                            hBackup->toolsFormat->setAttributes(m_lasContainer->lasPointsAttributes());

                        // pass "true" if you want to be fastest but consume more memory, false to be slower but consume less memory !
                        hBackup->toolsFormat->initWrite(true);

                        size_t pCount = 0;

                        QListIterator<CT_AbstractPointCloudIndex*> itIL(indexes);

                        while(itIL.hasNext())
                        {
                            CT_AbstractPointCloudIndex *index = itIL.next();

                            CT_PointIterator itI(index);

                            while(itI.hasNext()) {
                                itI.next();
                                hBackup->toolsFormat->write(stream, itI.currentPoint(), itI.cIndex());

                                ++pCount;

                                setExportProgress(int((pCount*100)/nPoints));
                            }
                        }

                        hBackup->toolsFormat->clearInfos();
                        ok = true;
                    }

                    file.close();
                } else {
                    setErrorMessage(tr("Erreur lors de l'ouverture en écriture du fichier %1").arg(filepath));
                }
            }

            saveOrDeleteHeaderBackup(hBackup, header, ok);
        } else if(nPoints != 0){
            setErrorMessage(tr("Problème lors de la création de l'en-tête du fichier."));
        }

    } else {
        setErrorMessage(tr("Erreur le format de point \"%1\" n'est pas disponible au sein de cet exporter de fichier LAS.").arg(getPointDataFormat()));
    }

    m_lasContainerModel = NULL;
    m_lasContainer = NULL;

    return ok;
}

bool CT_Exporter_LAS::finalizePieceByPieceExport()
{
    bool ok = false;

    QString finalFilePath = m_baseFilePath;
    finalFilePath.remove(finalFilePath.size()-1, 1);
    finalFilePath += ".las";

    QFile::remove(finalFilePath);

    if((m_headers.size() == 1) && (m_headers.first()->nFiles == 1)) {
        const QString fp = m_headers.first()->previousFilePath();

        QFile f(fp);
        ok = f.copy(finalFilePath);

    } else if(!m_headers.isEmpty()) {
        QStringList filesPath;

        foreach (const HeaderBackup* b, m_headers) {
            for(int i=0; i<b->nFiles; ++i) {
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

bool CT_Exporter_LAS::protectedExportToFile()
{
    if(!createExportFileForPieceByPieceExport())
        return false;

    if(exportOnePieceOfDataToFile())
        return finalizePieceByPieceExport();

    return false;
}

void CT_Exporter_LAS::clearSelection()
{
    m_lasContainer = NULL;
    m_lasContainerModel = NULL;
}

CT_LASHeader* CT_Exporter_LAS::createHeader(const CT_AbstractPointAttributesScalar* rn,
                                            const QList<CT_AbstractPointCloudIndex *>& indexes,
                                            size_t &nPoints) const
{
    CT_LASHeader *header = NULL;
    nPoints = 0;

    int format = getPointDataFormat();

    if(format >= 0 && format < 11)
    {
        header = new CT_LASHeader();

        CT_Point min;
        min.setX(std::numeric_limits<double>::max());
        min.setY(min.x());
        min.setZ(min.x());

        CT_Point max;
        max.setX(-min.x());
        max.setY(max.x());
        max.setZ(max.x());

        for(int i=0; i<5; ++i)
            header->m_legacyNumberOfPointsByReturn[i] = 0; // deprecated in 1.4

        for(int i=0; i<15; ++i)
            header->m_numberOfPointsByReturn[i] = 0;

        const CT_AbstractPointCloudIndex *rnIndex = NULL;
        size_t rnSize = 0;

        if(rn != NULL)
        {
            rnIndex = rn->getPointCloudIndex();

            if(rnIndex != NULL)
                rnSize = rnIndex->size();
        }

        QListIterator<CT_AbstractPointCloudIndex*> it(indexes);

        while(it.hasNext())
        {
            CT_AbstractPointCloudIndex *index = it.next();

            CT_PointIterator itP(index);

            // count the number of points
            nPoints += index->size();

            // compute max, min and return number
            while(itP.hasNext()) {

                const CT_Point &p = itP.next().currentPoint();

                if(p(0) > max(0))
                    max.setX(p(0));

                if(p(1) > max(1))
                    max.setY(p(1));

                if(p(2) > max(2))
                    max.setZ(p(2));

                if(p(0) < min(0))
                    min.setX(p(0));

                if(p(1) < min(1))
                    min.setY(p(1));

                if(p(2) < min(2))
                    min.setZ(p(2));

                if(rnIndex != NULL) {
                    size_t i = rnIndex->indexOf(itP.cIndex());

                    if(i < rnSize)
                    {
                        const int indexInTab = rn->dValueAt(i)-1;

                        quint64 &val = header->m_numberOfPointsByReturn[indexInTab];
                        ++val;

                        if(indexInTab < 5) {
                            quint32 &legacyVal = header->m_legacyNumberOfPointsByReturn[(int)rn->dValueAt(i)-1];
                            ++legacyVal;
                        }
                    }
                }
            }
        }

        if(rn == NULL) {
            header->m_numberOfPointsByReturn[0] = nPoints;
            header->m_legacyNumberOfPointsByReturn[0] = quint32(nPoints);
        }

        QString error;
        QDate date = QDate::currentDate();

        header->m_fileSourceID = 1;
        header->m_globalEncoding = 1;
        header->m_projectIDGuidData1 = 0;
        header->m_projectIDGuidData2 = 0;
        header->m_projectIDGuidData3 = 0;
        header->m_projectIDGuidData4 = 0;
        header->m_versionMajor = 1;  // format 1.2
        header->m_versionMinor = 2;
        #ifdef _MSC_VER
        strcpy_s(header->m_systemID, "EXPORT");
        strcpy_s(header->m_sofwareID, "Computree 3.0");
        #else
        strcpy(header->m_systemID, "EXPORT");
        strcpy(header->m_sofwareID, "Computree 3.0");
        #endif
        header->m_fileCreationDayOfYear = date.dayOfYear();
        header->m_fileCreationYear = date.year();
        header->m_numberOfVariableLengthRecords = 0; // TODO : write the good value
        header->m_pointDataRecordFormat = format;
        header->m_pointDataRecordLength = getPointDataLength(format);

        if(nPoints < std::numeric_limits<quint32>::max())
            header->m_legacyNumberOfPointRecord = quint32(nPoints); // deprecated in 1.4
        else
            header->m_legacyNumberOfPointRecord = 0;

        header->m_xOffset = 0;
        header->m_yOffset = 0;
        header->m_zOffset = 0;
        header->setBoundingBox(min(0), min(1), min(2), max(0), max(1), max(2));
        header->updateScaleFactorFromBoundingBox();
        header->m_startOfWaveformDataPacketRecord = 0; // TODO : write the good value
        header->m_startOfFirstExtendedVariableLengthRecord = 0; // TODO : write the good value
        header->m_numberOfExtendedVariableLengthRecords = 0; // TODO : write the good value
        header->m_numberOfPointRecords = nPoints;
    }

    return header;
}

bool CT_Exporter_LAS::writeHeader(QDataStream& stream,
                                  CT_LASHeader* header)
{
    if(header == NULL)
        return false;

    if(!header->finalizeBeforeWrite())
        return false;

    QString error;
    bool ok = header->write(stream, error);

    if(!ok)
        setErrorMessage(error);

    return ok;
}

int CT_Exporter_LAS::getPointDataFormat() const
{
    if(m_lasContainer == NULL)
        return 1;

    QList<CT_LasDefine::LASPointAttributesType> list = m_lasContainer->lasPointAttributesTypes();

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

    return NULL;
}

CT_Exporter_LAS::HeaderBackup* CT_Exporter_LAS::createOrGetHeaderBackupForHeader(const CT_LASHeader* header) const
{
    foreach (HeaderBackup* b, m_headers) {
        if(b->isHeaderSameForExport(header))
            return b;
    }

    HeaderBackup* b = new HeaderBackup();
    b->header = (CT_LASHeader*)header;
    b->nFiles = 0;
    b->dirPath = QDir::toNativeSeparators(m_baseFilePath + QString().setNum(m_headers.size()) + QDir::separator());
    b->toolsFormat = createPointDataFormat();

    return b;
}

void CT_Exporter_LAS::saveOrDeleteHeaderBackup(CT_Exporter_LAS::HeaderBackup* backup, CT_LASHeader* header, bool writeOfFileIsOk)
{
    if(backup == NULL)
        return;

    if(writeOfFileIsOk)
        ++backup->nFiles;

    if(!m_headers.contains(backup)) {
        if(writeOfFileIsOk)
            m_headers.append(backup);
        else
            delete backup;
    } else if(!writeOfFileIsOk && (backup->header != header)) {
        delete header;
    }
}

void CT_Exporter_LAS::clearAllBackupedHeaders()
{
    foreach (HeaderBackup* b, m_headers) {

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

    m_headers.clear();
}

bool CT_Exporter_LAS::HeaderBackup::isHeaderSameForExport(const CT_LASHeader* header) const
{
    return this->header->isHeaderAlmostSimilar(*header);
}

QString CT_Exporter_LAS::HeaderBackup::previousFilePath() const
{
    return generateFilePath(nFiles-1);
}

QString CT_Exporter_LAS::HeaderBackup::currentFilePath() const
{
    return generateFilePath(nFiles);
}

QString CT_Exporter_LAS::HeaderBackup::generateFilePath(int n) const
{
    if(n < 0)
        return "";

    return dirPath + QString().setNum(n) + ".las";
}
