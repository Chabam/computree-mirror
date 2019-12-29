#include "pb_ascrgbexporter.h"

#include <math.h>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QEventLoop>
#include <QApplication>
#include <QProgressDialog>
#include <QFileInfo>

#include "ct_tools/ct_numerictostringconversiont.h"

#include "ct_itemdrawable/abstract/ct_abstractpointattributesscalar.h"
#include "ct_itemdrawable/ct_pointsattributescolor.h"

PB_ASCRGBExporter::PB_ASCRGBExporter(int subMenuLevel) : SuperClass(subMenuLevel)
{
    addNewExportFormat(FileFormat("asc", tr("Fichier asc")));

    setToolTip(tr("Exporte les points au format ASCII, avec les champs suivants :<br>"
                  "- X  : Coordonnée X<br>"
                  "- Y  : Coordonnée Y<br>"
                  "- Z  : Coordonnée Z<br>"
                  "- R  : Composante rouge<br>"
                  "- V  : Composante verte<br>"
                  "- B  : Composante Bleue<br>"
                  "- NX : Coordonnée X de la normale au point<br>"
                  "- NY : Coordonnée X de la normale au point<br>"
                  "- NZ : Coordonnée X de la normale au point<br><br>"));
}

PB_ASCRGBExporter::PB_ASCRGBExporter(const PB_ASCRGBExporter& other) : SuperClass(other),
    mItems(other.mItems)
{
}

QString PB_ASCRGBExporter::displayableName() const
{
    return tr("Points, ASCII(X,Y,Z,R,G,B,NX,NY,NZ)");
}

void PB_ASCRGBExporter::setItemsToExport(const QList<const CT_AbstractItemDrawableWithPointCloud*>& items,
                                         const QList<const CT_AbstractPointsAttributes*> colorsAttributes,
                                         const QList<const CT_PointsAttributesNormal*> normalsAttributes)
{
    setMustUseModels(false);
    mItems = items;

    QList<const CT_AbstractPointsAttributes*> finalAttributes = colorsAttributes;
    finalAttributes.reserve(finalAttributes.size() + normalsAttributes.size());

    for(const CT_PointsAttributesNormal* normal : normalsAttributes)
    {
        finalAttributes.append(normal);
    }

    m_attributsColorAndNormalPointWorker.setAttributes(finalAttributes);
}

void PB_ASCRGBExporter::internalDeclareInputModels(CT_ExporterInModelStructureManager& manager)
{
    manager.addGroupToRootGroup(m_hInGroup);
    manager.addItemToGroup(m_hInGroup, m_hInItem, tr("Item à exporter"));
    manager.addItemToGroup(m_hInGroup, m_hInColorsAttribute, tr("Couleurs"));
    manager.addItemToGroup(m_hInGroup, m_hInNormalsAttribute, tr("Normales"));
}

CT_AbstractExporter::ExportReturn PB_ASCRGBExporter::internalExportToFile()
{
    const QFileInfo exportPathInfo = QFileInfo(filePath());
    const QString path = exportPathInfo.path();
    const QString baseName = exportPathInfo.baseName();
    const QString suffix = "asc";

    const QString currentFilePath = QString("%1/%2.%4").arg(path).arg(baseName).arg(suffix);

    QFile file(currentFilePath);

    if(!file.open(QFile::WriteOnly | QFile::Text))
        return ErrorWhenExport;

    QTextStream txtStream(&file);

    createColorAndNormalCloud();

    CT_AbstractColorCloud* cc = m_attributsColorAndNormalPointWorker.colorCloud() != nullptr ? m_attributsColorAndNormalPointWorker.colorCloud()->abstractColorCloud() : nullptr;
    CT_AbstractNormalCloud* nn = m_attributsColorAndNormalPointWorker.normalCloud() != nullptr ? m_attributsColorAndNormalPointWorker.normalCloud()->abstractNormalCloud() : nullptr;

    txtStream << "X Y Z";

    if(cc != nullptr)
        txtStream << " R G B";

    if(nn != nullptr)
        txtStream << " NX NY NZ";

    txtStream << "\n";

    if(mustUseModels())
    {
        if(mIteratorItemBegin == mIteratorItemEnd)
        {
            auto iterator = m_hInItem.iterateInputs(m_handleResultExport);
            mIteratorItemBegin = iterator.begin();
            mIteratorItemEnd = iterator.end();
        }

        int nExported = 0;
        const int totalToExport = maximumItemToExportInFile(int(std::distance(mIteratorItemBegin, mIteratorItemEnd)));
        const int end = totalToExport*100;

        // write data
        while((mIteratorItemBegin != mIteratorItemEnd)
              && (nExported < end))
        {
            const CT_AbstractItemDrawableWithPointCloud* item = *mIteratorItemBegin;

            exportItem(item, txtStream, cc, nn, nExported, totalToExport);

            nExported += 100;
            ++mIteratorItemBegin;
        }

        file.close();
        return (mIteratorItemBegin == mIteratorItemEnd) ? NoMoreItemToExport : ExportCanContinue;
    }
    else
    {
        const int totalToExport = mItems.size();
        int nExported = 0;

        for(const CT_AbstractItemDrawableWithPointCloud* item : mItems)
        {
            exportItem(item, txtStream, cc, nn, nExported, totalToExport);
            nExported += 100;
        }
    }

    file.close();

    return NoMoreItemToExport;
}

void PB_ASCRGBExporter::clearIterators()
{
    mIteratorItemBegin = HandleItemType::const_iterator();
    mIteratorItemEnd = mIteratorItemBegin;
}

void PB_ASCRGBExporter::clearAttributesClouds()
{
    m_attributsColorAndNormalPointWorker.setColorCloud(nullptr);
    m_attributsColorAndNormalPointWorker.setNormalCloud(nullptr);
}

void PB_ASCRGBExporter::createColorAndNormalCloud()
{
    // cleared in clearAttributesClouds()
    if(!m_attributsColorAndNormalPointWorker.colorCloud().isNull() || !m_attributsColorAndNormalPointWorker.normalCloud().isNull())
        return;

    if(mustUseModels())
    {
        QList<const CT_AbstractPointsAttributes*> attributesColor;
        QList<const CT_AbstractPointsAttributes*> attributesNormal;

        for(const CT_AbstractPointsAttributes* pA : m_hInColorsAttribute.iterateInputs(m_handleResultExport)) {
            if((dynamic_cast<const CT_PointsAttributesColor*>(pA) != nullptr)
                    || (dynamic_cast<const CT_AbstractPointAttributesScalar*>(pA)))
                attributesColor.append(pA);
        }

        for(const CT_AbstractPointsAttributes* pA : m_hInNormalsAttribute.iterateInputs(m_handleResultExport)) {
            if((dynamic_cast<const CT_PointsAttributesNormal*>(pA) != nullptr))
                attributesNormal.append(pA);
        }

        m_attributsColorAndNormalPointWorker.setAttributes(attributesColor + attributesNormal);

        if(!attributesColor.isEmpty())
            m_attributsColorAndNormalPointWorker.setColorCloud(PS_REPOSITORY->createNewColorCloud(CT_Repository::SyncWithPointCloud));

        if(!attributesNormal.isEmpty())
            m_attributsColorAndNormalPointWorker.setNormalCloud(PS_REPOSITORY->createNewNormalCloud(CT_Repository::SyncWithPointCloud));
    }
    else if(!m_attributsColorAndNormalPointWorker.attributes().isEmpty())
    {
        m_attributsColorAndNormalPointWorker.setColorCloud(PS_REPOSITORY->createNewColorCloud(CT_Repository::SyncWithPointCloud));
        m_attributsColorAndNormalPointWorker.setNormalCloud(PS_REPOSITORY->createNewNormalCloud(CT_Repository::SyncWithPointCloud));
    }

    if(!m_attributsColorAndNormalPointWorker.attributes().isEmpty())
        m_attributsColorAndNormalPointWorker.apply();
}

void PB_ASCRGBExporter::exportItem(const CT_AbstractItemDrawableWithPointCloud* item, QTextStream& txtStream, const CT_AbstractColorCloud* cc, const CT_AbstractNormalCloud* nn, const int& nExported, const int& totalToExport)
{
    CT_PointIterator itP(item->pointCloudIndex());

    size_t totalSize = itP.size();
    size_t i = 0;

    quint16 r, g, b;

    while(itP.hasNext())
    {
        const CT_Point &point = itP.next().currentPoint();

        txtStream << CT_NumericToStringConversionT<double>::toString(point(0)) << " ";
        txtStream << CT_NumericToStringConversionT<double>::toString(point(1)) << " ";
        txtStream << CT_NumericToStringConversionT<double>::toString(point(2));

        if(cc != nullptr)
        {
            const CT_Color &co = cc->constColorAt(itP.cIndex());
            r = quint16(co.r() / 255.0);
            g = quint16(co.g() / 255.0);
            b = quint16(co.b() / 255.0);

            txtStream << " " << r << " ";
            txtStream << g << " ";
            txtStream << b;
        }

        if(nn != nullptr)
        {
            const CT_Normal &no = nn->constNormalAt(itP.cIndex());

            txtStream << " " << no[0] << " ";
            txtStream << no[1] << " ";
            txtStream << no[2];
        }

        txtStream << "\n";

        ++i;

        setExportProgress(int((((int(i)*100)/int(totalSize))+nExported)/totalToExport));
    }
}
