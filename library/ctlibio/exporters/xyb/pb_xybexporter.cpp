#include "pb_xybexporter.h"

#include <math.h>
#include <QMessageBox>
#include <QTextStream>
#include <QEventLoop>
#include <QApplication>
#include <QProgressDialog>
#include <QFileInfo>

#include "ct_itemdrawable/abstract/ct_abstractpointattributesscalar.h"
#include "ct_itemdrawable/ct_pointsattributescolor.h"

#include "ct_itemdrawable/tools/scanner/ct_thetaphishootingpattern.h"
#include "ct_colorcloud/abstract/ct_abstractcolorcloud.h"
#include "ct_coordinates/tools/ct_coordinatesystemmanager.h"
#include "ct_iterator/ct_pointiterator.h"
#include "ct_point.h"

PB_XYBExporter::PB_XYBExporter(int subMenuLevel) : SuperClass(subMenuLevel)
{
    setCanExportPoints(true);
    addNewExportFormat(FileFormat("xyb", tr("Fichiers binaire de points .xyb")));
    setToolTip(tr("Export des points dans un fichier format binaire XYB (FARO)"));
}

PB_XYBExporter::PB_XYBExporter(const PB_XYBExporter& other) : SuperClass(other)
{
}

QString PB_XYBExporter::displayableName() const
{
    return tr("Points, XYB (X,Y,Z,I)");
}

bool PB_XYBExporter::setPointsToExport(const QList<CT_AbstractCloudIndex*> &list)
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

void PB_XYBExporter::setItemsToExport(const QList<const CT_AbstractItemDrawableWithPointCloud*>& items,
                                      const CT_Scanner* scanner,
                                      const QList<const CT_AbstractPointsAttributes*> pointsAttributes)
{
    setMustUseModels(false);

    m_items = items;
    m_scanner = scanner;
    m_attributsColorPointWorker.setAttributes(pointsAttributes);
}

bool PB_XYBExporter::canExportPieceByPiece() const
{
    return true;
}

bool PB_XYBExporter::createExportFileForPieceByPieceExport()
{
    const QFileInfo exportPathInfo = QFileInfo(filePath());
    const QString path = exportPathInfo.path();
    const QString baseName = exportPathInfo.baseName();
    const QString suffix = "xyb";

    m_currentFilePath = QString("%1/%2.%4").arg(path).arg(baseName).arg(suffix);

    if(mustUseModels())
    {
        m_scanner = nullptr;

        for(const CT_Scanner* scanner : m_hInScanner.iterateInputs(m_handleResultExport)) {
            m_scanner = scanner;
            break;
        }
    }

    CT_ThetaPhiShootingPattern* pattern = nullptr;

    if (m_scanner != nullptr)
        pattern = dynamic_cast<CT_ThetaPhiShootingPattern*>(m_scanner->shootingPattern());

    QFile file(m_currentFilePath);

    if(file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream txtStream(&file);

        txtStream << "# SCENE XYZ binary format v1.0\n";

        if(pattern != nullptr)
        {
            double x = pattern->origin().x();
            double y = pattern->origin().y();
            double z = pattern->origin().z();

            txtStream << "ScanPosition " << x << " " << y << " " << z << "\n";
            txtStream << "Rows " << pattern->nVRays() << "\n";
            txtStream << "Cols " << pattern->nHRays() << "\n";
        }
        else
        {
            txtStream << "ScanPosition 0.00000000 0.00000000 0.00000000 \n";
            txtStream << "Rows 0\n";
            txtStream << "Cols 0\n";
        }

        file.close();
    } else {
        return false;
    }

    if(file.open(QFile::Append))
    {
        QDataStream stream(&file);
        stream.setByteOrder(QDataStream::LittleEndian);

        char d_data[8];

        // write header
        d_data[0] = 0;
        d_data[1] = 0;
        d_data[2] = 0;
        d_data[3] = 0;

        stream.writeRawData(d_data, 4);

        file.close();
        return true;
    } else {
        return false;
    }

    return true;
}

bool PB_XYBExporter::exportOnePieceOfDataToFile()
{
    QFile file(m_currentFilePath);

    if(file.open(QFile::Append))
    {
        CT_AbstractColorCloud *cc = createColorCloud();

        QDataStream stream(&file);
        stream.setByteOrder(QDataStream::LittleEndian);

        if(mustUseModels())
        {
            if(m_iteratorItemBegin == m_iteratorItemEnd)
            {
                auto iterator = m_hInItem.iterateInputs(m_handleResultExport);
                m_iteratorItemBegin = iterator.begin();
                m_iteratorItemEnd = iterator.end();
            }

            int nExported = 0;
            const int totalToExport = maximumItemToExportInFile(std::distance(m_iteratorItemBegin, m_iteratorItemEnd));

            // write data
            while((m_iteratorItemBegin != m_iteratorItemEnd)
                  && (nExported < totalToExport))
            {
                const CT_AbstractItemDrawableWithPointCloud *item = *m_iteratorItemBegin;

                exportPoints(stream,
                             dynamic_cast<const CT_IAccessPointCloud*>(item)->pointCloudIndex(),
                             cc,
                             nExported,
                             totalToExport);

                ++nExported;
                ++m_iteratorItemBegin;
            }
        }
        else
        {
            int totalToExport = m_items.size();
            int nExported = 0;

            for(const CT_AbstractItemDrawableWithPointCloud* item : m_items)
            {
                exportPoints(stream,
                             dynamic_cast<const CT_IAccessPointCloud*>(item)->pointCloudIndex(),
                             cc,
                             nExported,
                             totalToExport);

                ++nExported;
                ++m_iteratorItemBegin;
            }

            const QList<CT_AbstractCloudIndex*> &pointsSelected = pointsToExport();
            QListIterator<CT_AbstractCloudIndex*> itCI(pointsSelected);

            nExported = 0;
            totalToExport = pointsSelected.size();

            while(itCI.hasNext())
            {
                exportPoints(stream,
                             dynamic_cast<CT_AbstractPointCloudIndex*>(itCI.next()),
                             cc,
                             nExported,
                             totalToExport);

                ++nExported;
            }
        }

        file.close();
        return true;
    }

    return false;
}


bool PB_XYBExporter::finalizePieceByPieceExport()
{
    m_currentFilePath.clear();

    return true;
}

void PB_XYBExporter::internalDeclareInputModels(CT_ExporterInModelStructureManager& manager)
{
    manager.addGroupToRootGroup(m_hInGroup);
    manager.addItemToGroup(m_hInGroup, m_hInItem, tr("Item à exporter"));
    manager.addItemToGroup(m_hInGroup, m_hInScanner, tr("Scanner"));
    manager.addItemToGroup(m_hInGroup, m_hInPointsAttribute, tr("Couleurs"));
}

bool PB_XYBExporter::internalExportToFile()
{
    if(!createExportFileForPieceByPieceExport())
        return false;

    if(exportOnePieceOfDataToFile())
        return finalizePieceByPieceExport();

    return false;
}

void PB_XYBExporter::clearIterators()
{
    m_iteratorItemBegin = HandleItemType::const_iterator();
    m_iteratorItemBegin = m_iteratorItemEnd;
}

CT_AbstractColorCloud* PB_XYBExporter::createColorCloud()
{
    if(m_attributsColorPointWorker.colorCloud().data() != nullptr)
        return m_attributsColorPointWorker.colorCloud()->abstractColorCloud();

    QList<const CT_AbstractPointsAttributes*> attributesColor;

    for(const CT_AbstractPointsAttributes* pA : m_hInPointsAttribute.iterateInputs(m_handleResultExport)) {
        if((dynamic_cast<const CT_PointsAttributesColor*>(pA) != nullptr)
                || (dynamic_cast<const CT_AbstractPointAttributesScalar*>(pA)))
            attributesColor.append(pA);
    }

    m_attributsColorPointWorker.setAttributes(attributesColor);

    if(!attributesColor.isEmpty())
    {
        m_attributsColorPointWorker.setColorCloud(PS_REPOSITORY->createNewColorCloud(CT_Repository::SyncWithPointCloud));
        m_attributsColorPointWorker.apply();
        return m_attributsColorPointWorker.colorCloud()->abstractColorCloud();
    }

    return nullptr;
}

void PB_XYBExporter::exportPoints(QDataStream &stream,
                                  const CT_AbstractPointCloudIndex *constPCIndex,
                                  const CT_AbstractColorCloud *cc,
                                  const int &nExported,
                                  const int &totalToExport)
{
    size_t i = 0;

    CT_PointIterator it(constPCIndex);
    size_t totalSize = it.size();

    while(it.hasNext())
    {
        const CT_Point &point = it.next().currentPoint();

        stream << point(CT_Point::X);
        stream << point(CT_Point::Y);
        stream << point(CT_Point::Z);

        if(cc == nullptr)
        {
            stream << static_cast<quint16>(0);
        }
        else
        {
            const CT_Color &col = cc->constColorAt(it.cIndex());
            stream << static_cast<quint16>(col.r());
        }

        ++i;

        //const int currentProgress = ((i*100)/totalSize);

        // TODO : export progress problem
        setExportProgress(int((((i*100)/totalSize)+nExported)/totalToExport));
    }
}

