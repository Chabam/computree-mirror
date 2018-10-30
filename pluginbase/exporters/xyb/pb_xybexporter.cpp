#include "pb_xybexporter.h"

#include <math.h>
#include <QMessageBox>
#include <QTextStream>
#include <QEventLoop>
#include <QApplication>
#include <QProgressDialog>
#include <QFileInfo>

#include "ct_global/ct_context.h"
#include "ct_itemdrawable/abstract/ct_abstractpointsattributes.h"
#include "ct_itemdrawable/ct_scanner.h"
#include "ct_itemdrawable/tools/scanner/ct_thetaphishootingpattern.h"
#include "ct_colorcloud/abstract/ct_abstractcolorcloud.h"
#include "ct_coordinates/tools/ct_coordinatesystemmanager.h"
#include "ct_iterator/ct_pointiterator.h"
#include "ct_point.h"
#include "ct_itemdrawable/tools/ct_itemsearchhelper.h"
#include "ct_model/tools/ct_modelsaverestorehelper.h"

PB_XYBExporter::PB_XYBExporter() : SuperClass()
{
    setCanExportItems(true);
    setCanExportPoints(true);
    setCanExportColors(true);
    setCanExportNormals(false);

    m_scanner = NULL;
    m_scannerModel = NULL;
}

QString PB_XYBExporter::getExporterCustomName() const
{
    return tr("Points, XYB (X,Y,Z,I)");
}

CT_StepsMenu::LevelPredefined PB_XYBExporter::getExporterSubMenuName() const
{
    return CT_StepsMenu::LP_Points;
}

void PB_XYBExporter::init()
{
    addNewExportFormat(FileFormat("xyb", tr("Fichiers binaire de points .xyb")));

    setToolTip(tr("Export des points dans un fichier format binaire XYB (FARO)"));
}

bool PB_XYBExporter::setItemDrawableToExport(const QList<CT_AbstractItemDrawable *> &list)
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

    return SuperClass::setItemDrawableToExport(myList);
}

bool PB_XYBExporter::setPointsToExport(const QList<CT_AbstractCloudIndex*> &list)
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

    return SuperClass::setPointsToExport(myList);
}

void PB_XYBExporter::saveSettings(SettingsWriterInterface &writer) const
{
    SuperClass::saveSettings(writer);

    CT_ModelSaveRestoreHelper::saveSettingsOfOutModel(this, writer, m_scannerModel, "ScannerModel");
}

bool PB_XYBExporter::restoreSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restoreSettings(reader))
        return false;

    bool savedAndFoundedOrNotSaved;
    m_scannerModel = dynamic_cast<CT_OutAbstractSingularItemModel *>(CT_ModelSaveRestoreHelper::restoreOutModelFromSettings(this, reader, myStep(), "ScannerModel", savedAndFoundedOrNotSaved));

    return savedAndFoundedOrNotSaved;
}

QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > PB_XYBExporter::createBuilders() const
{
    QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > l = SuperClass::createBuilders();
    l << QPair<QString, CT_AbstractItemDrawableCollectionBuilder*>(tr("Scanners"), new ScannerBuilder(true));

    return l;
}

bool PB_XYBExporter::useSelection(const CT_ItemDrawableHierarchyCollectionWidget *selectorWidget)
{
    clearWorker();

    if(SuperClass::useSelection(selectorWidget))
    {
        QList<CT_AbstractSingularItemDrawable*> list = selectorWidget->itemDrawableSelected();
        QListIterator<CT_AbstractSingularItemDrawable*> it(list);

        while(it.hasNext() && (m_scanner == NULL))
            m_scanner = dynamic_cast<CT_Scanner*>(it.next());

        // the scanner can not be found if this exporter is used in a step
        if(m_scanner == NULL) {

            // search the model to use
            QList<CT_OutAbstractSingularItemModel*> listM = selectorWidget->itemDrawableModelSelected();
            QListIterator<CT_OutAbstractSingularItemModel*> itM(listM);

            m_scanner = NULL;

            while(itM.hasNext() && (m_scannerModel == NULL)) {
                CT_OutAbstractSingularItemModel *model = itM.next();
                if(dynamic_cast<CT_Scanner*>(model->itemDrawable()) != NULL)
                    m_scannerModel = model;
            }
        }

        return true;
    }

    return false;
}

bool PB_XYBExporter::canExportPieceByPiece() const
{
    return true;
}

bool PB_XYBExporter::createExportFileForPieceByPieceExport()
{
    QFileInfo exportPathInfo = QFileInfo(exportFilePath());
    QString path = exportPathInfo.path();
    QString baseName = exportPathInfo.baseName();
    QString suffix = "xyb";
    m_currentFilePath = QString("%1/%2.%4").arg(path).arg(baseName).arg(suffix);

    CT_ThetaPhiShootingPattern *pattern = nullptr;

    // TODO : MK 21.10.18 ==> i think it was a problem here when we export more that one pointcloud because we can have more that one scanner
    // if we don't have the scanner but his model
    if(m_scannerModel != NULL) {

        // we search the real scanner
        CT_ItemSearchHelper helper;
        CT_ResultItemIterator it = helper.searchSingularItemsForModel(m_scannerModel);

        if(it.hasNext())
            m_scanner = dynamic_cast<CT_Scanner*>((CT_AbstractSingularItemDrawable*)it.next());
    }

    if (m_scanner)
        pattern = dynamic_cast<CT_ThetaPhiShootingPattern*>(m_scanner->getShootingPattern());

    QFile file(m_currentFilePath);

    if(file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream txtStream(&file);

        txtStream << "# SCENE XYZ binary format v1.0\n";

        if(pattern != NULL)
        {
            double x = pattern->getOrigin().x();
            double y = pattern->getOrigin().y();
            double z = pattern->getOrigin().z();

            txtStream << "ScanPosition " << x << " " << y << " " << z << "\n";
            txtStream << "Rows " << pattern->getNVRays() << "\n";
            txtStream << "Cols " << pattern->getNHRays() << "\n";
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
        CT_AbstractColorCloud *cc = createColorCloudBeforeExportToFile();

        QDataStream stream(&file);
        stream.setByteOrder(QDataStream::LittleEndian);

        int totalToExport = itemDrawableToExport().size();

        const QList<CT_AbstractCloudIndex*> &pointsSelected = pointsToExport();

        int nExported = 0;

        // write data
        QListIterator<CT_AbstractItemDrawable*> it(itemDrawableToExport());

        while(it.hasNext())
        {
            CT_AbstractItemDrawable *item = it.next();

            exportPoints(stream,
                         dynamic_cast<CT_IAccessPointCloud*>(item)->getPointCloudIndex(),
                         cc,
                         nExported,
                         totalToExport);

            nExported += 100;
        }

        QListIterator<CT_AbstractCloudIndex*> itCI(pointsSelected);
        totalToExport = pointsSelected.size();

        while(itCI.hasNext())
        {
            exportPoints(stream,
                         dynamic_cast<CT_AbstractPointCloudIndex*>(itCI.next()),
                         cc,
                         nExported,
                         totalToExport);

            nExported += 100;
        }

        file.close();
        return true;
    }

    return false;
}


bool PB_XYBExporter::finalizePieceByPieceExport()
{
    // Nothing to do in this case
    m_currentFilePath.clear();

    return true;
}

bool PB_XYBExporter::protectedExportToFile()
{
    if(!createExportFileForPieceByPieceExport())
        return false;

    if(exportOnePieceOfDataToFile())
        return finalizePieceByPieceExport();

    return false;
}

void PB_XYBExporter::clearWorker()
{
    SuperClass::clearWorker();

    m_scanner = NULL;
    m_scannerModel = NULL;
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

        if(cc == NULL)
        {
            stream << (quint16)0;
        }
        else
        {
            const CT_Color &col = cc->constColorAt(it.cIndex());
            quint16 tmp = (quint16)col.r();
            stream << tmp;
        }

        ++i;

        setExportProgress(int((((i*100)/totalSize)+nExported)/totalToExport));
    }
}

