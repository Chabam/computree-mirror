#include "ct_exporter_buffer.h"

#include <QFileInfo>
#include <QFile>
#include <QTextStream>

#include "ct_tools/ct_numerictostringconversiont.h"
#include "ct_iterator/ct_pointiterator.h"

CT_Exporter_BUFFER::CT_Exporter_BUFFER(int subMenuLevel) : SuperClass(subMenuLevel)
{
    setCanExportPoints(true);
    addNewExportFormat(FileFormat("cti", tr("Fichiers binaire de buffer .cti")));
    setToolTip(tr("Export du buffer dans un fichier format binaire CTI (CompuTree Indexes for buffer)"));
}

CT_Exporter_BUFFER::CT_Exporter_BUFFER(const CT_Exporter_BUFFER& other) : SuperClass(other),
    m_items(other.m_items)
{
}

QString CT_Exporter_BUFFER::displayableName() const
{
    return tr("Export de buffer points, CTI (idx)");
}

bool CT_Exporter_BUFFER::setPointsToExport(const QList<CT_AbstractCloudIndex*> &list)
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

void CT_Exporter_BUFFER::setItemsToExport(const QList<const CT_AbstractItemDrawableWithPointCloud*>& items)
{
    setMustUseModels(false);
    m_items = items;
}

void CT_Exporter_BUFFER::internalDeclareInputModels(CT_ExporterInModelStructureManager& manager)
{
    manager.addGroupToRootGroup(m_hInGroup);
    manager.addItemToGroup(m_hInGroup, m_hInItem, tr("Item à exporter"));
}

CT_AbstractExporter::ExportReturn CT_Exporter_BUFFER::internalExportToFile()
{
// 1/ Get point format and write it
// 2/ Start with multiple point cloud from selected (buffered) area and create the list of files with their list of indexes
//    >>> (if Npoint == Ntotal, then just set Npoint = 0 because this is a reference file)
// 3/ Write the buffer file in the selected path
// 4/ >>> In the end if the selected path is not the same as the data path, place a side-car file with the abolute path beside

    const QFileInfo exportPathInfo = QFileInfo(filePath());
    const QString path = exportPathInfo.path();
    const QString baseName = exportPathInfo.baseName();
    const QString suffix = "cti";
    const QString currentFilePath = QString("%1/%2.%3").arg(path, baseName, suffix);
    QString pathTarget;

    QFile file(currentFilePath);

    if(!file.open(QFile::WriteOnly | QFile::Text))
        return ErrorWhenExport;

    QDataStream datStream(&file);
    datStream.setByteOrder(QDataStream::LittleEndian);

    if(mustUseModels())
    {
        if(mIteratorItemBegin == mIteratorItemEnd)
        {
            auto iterator = m_hInItem.iterateInputs(m_handleResultExport);
            mIteratorItemBegin = iterator.begin();
            mIteratorItemEnd = iterator.end();
        }

        // Get point format from file name and write it
        const CT_AbstractItemDrawableWithPointCloud* itemInfo = *mIteratorItemBegin;
        const QFileInfo inFileInfo = QFileInfo(itemInfo->displayableName());  // DISPLAYABLENAME ?
        pathTarget = inFileInfo.path();
        QString ext = inFileInfo.suffix();
        ext.truncate(3); // We keep only the 3 first character of the file suffix.
        datStream << ext;

        int nExported = 0;
        const int totalToExport = maximumItemToExportInFile(int(std::distance(mIteratorItemBegin, mIteratorItemEnd)));
        const int end = totalToExport*100;

        // write data
        while((mIteratorItemBegin != mIteratorItemEnd)
              && (nExported < end))
        {
            const CT_AbstractItemDrawableWithPointCloud* item = *mIteratorItemBegin;

            // Write the current item's file name
            datStream << item->displayableName();

            // Test if point cloud is complete or not
            bool allPoints = false;
            // TO BE DONE

            if(allPoints)
                datStream << (quint64)0;
            else
                exportItem(item, datStream, nExported, totalToExport);

            nExported += 100;
            ++mIteratorItemBegin;
        }

        file.close();
        return (mIteratorItemBegin == mIteratorItemEnd) ? NoMoreItemToExport : ExportCanContinue;
    }
    else
    {
        // Get point format from file name and write it
        const QFileInfo inFileInfo = QFileInfo(m_items.constFirst()->displayableName());
        pathTarget = inFileInfo.path();
        QString ext = inFileInfo.suffix();
        ext.truncate(3); // We keep only the 3 first character of the file suffix.
        datStream << ext;

        const int totalToExport = m_items.size();
        int nExported = 0;

        for(const CT_AbstractItemDrawableWithPointCloud* item : qAsConst(m_items))
        {
            // Write the current item's file name
            datStream << item->displayableName();

            // Test if point cloud is complete or not
            bool allPoints = false;
            // TO BE DONE

            if(allPoints)
                datStream << (quint64)0;
            else
                exportItem(item, datStream, nExported, totalToExport);

            nExported += 100;
        }
    }

    file.close();

    // In the end if the selected path is not the same as the data path, place a side-car file with the abolute path beside
    if(path != pathTarget)
    {
        // TO BE DONE
    }

    return NoMoreItemToExport;
}

void CT_Exporter_BUFFER::clearIterators()
{
    mIteratorItemBegin = HandleItemType::const_iterator();
    mIteratorItemEnd = mIteratorItemBegin;
}

void CT_Exporter_BUFFER::clearAttributesClouds()
{
}

void CT_Exporter_BUFFER::exportItem(const CT_AbstractItemDrawableWithPointCloud* item, QDataStream& stream, const int& nExported, const int& totalToExport)
{
    CT_PointIterator itP(item->pointCloudIndex());
    const size_t totalSize = itP.size();
    size_t i = 0;

    // Write the whole point list information
    while(itP.hasNext())
    {
        // Write current point global index from point cloud
        stream << static_cast<quint64>(itP.next().currentGlobalIndex());
        ++i;
        setExportProgress(int((((i*100)/totalSize)+size_t(nExported))/size_t(totalToExport)));
    }
}
