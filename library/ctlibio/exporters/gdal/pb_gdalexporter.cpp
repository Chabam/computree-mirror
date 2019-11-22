#include "pb_gdalexporter.h"

#ifdef USE_GDAL
#include "gdal.h"
#include "ogrsf_frmts.h"
#endif

#include "readers/tools/gdal/ct_gdaltools.h"

#include <QFileInfo>
#include <QDebug>

#define VECTOR_DRIVER_TYPE "Vector"
#define RASTER_DRIVER_TYPE "Raster"

// TODO : choose raster type (what is R, G or B, etc...) and export it in multiple bands
// TODO : Group raster by dimension and resolution

PB_GDALExporter::PB_GDALExporter(int subMenuLevelRaster, int subMenuLevelVector, int subMenuLevelDefault) : SuperClass(subMenuLevelDefault),
    mSubMenuLevelRaster(subMenuLevelRaster),
    mSubMenuLevelVector(subMenuLevelVector)
{
}

PB_GDALExporter::PB_GDALExporter(const PB_GDALExporter& other) : SuperClass(other),
    #ifdef USE_GDAL
    m_driver(other.m_driver),
    #endif
    mSubMenuLevelRaster(other.mSubMenuLevelRaster),
    mSubMenuLevelVector(other.mSubMenuLevelVector),
    mVectors(other.mVectors),
    mRasters(other.mRasters)
{
}

#ifdef USE_GDAL
PB_GDALExporter::PB_GDALExporter(const GDALDriver *driver, int subMenuLevelRaster, int subMenuLevelVector, int subMenuLevelDefault) : SuperClass(subMenuLevelDefault),
    m_driver(const_cast<GDALDriver*>(driver)),
    mSubMenuLevelRaster(subMenuLevelRaster),
    mSubMenuLevelVector(subMenuLevelVector)
{
    QStringList ext = CT_GdalTools::staticGdalDriverExtension(m_driver);

    if(ext.isEmpty())
        ext.append("gdal");

    addNewExportFormat(FileFormat(ext, CT_GdalTools::staticGdalDriverName((m_driver))));

    const QString driverType = typeOfDriver();

    QString toolTip = tr("Exporter GDAL de type : %1").arg(driverType.isEmpty()?tr("Inconnu"):driverType);

    if (driverType == RASTER_DRIVER_TYPE)
    {
        toolTip.append("<br>");
        toolTip.append("<br>");
        toolTip.append(tr("Pour plus de détails voir : http://www.gdal.org/formats_list.html"));
    }
    else if (driverType == VECTOR_DRIVER_TYPE)
    {
        toolTip.append("<br>");
        toolTip.append("<br>");
        toolTip.append(tr("Pour plus de détails voir : http://www.gdal.org/ogr_formats.html"));
    }

    toolTip.append("<br>");
    toolTip.append("<br>");
    toolTip.append(tr("Extension : "));

    for (int i = 0 ; i < ext.size() ; i++)
    {
        toolTip.append("*.");
        toolTip.append(ext.at(i));
        if ((i + 1) < ext.size()) {toolTip.append(" ");}
    }

    setToolTip(toolTip);
}
#endif

QString PB_GDALExporter::displayableName() const
{
#ifdef USE_GDAL
    if(m_driver != nullptr)
        return CT_GdalTools::staticGdalDriverName(m_driver);
#endif

    return SuperClass::displayableName();
}

bool PB_GDALExporter::isExportEachItemInSeparateFileOptionnal() const
{
#ifdef USE_GDAL
    const QString driverType = typeOfDriver();

    if (driverType == RASTER_DRIVER_TYPE) // raster can only be exported one per file
        return false;

    // multiple vectors or one vector per file is allowed
    return true;
#else
    return SuperClass::isExportEachItemInSeparateFileOptionnal();
#endif
}

QString PB_GDALExporter::uniqueName() const
{
#ifdef USE_GDAL
    if(m_driver != nullptr)
        return CT_GdalTools::staticGdalDriverName(m_driver);
#endif
    return SuperClass::uniqueName();
}

int PB_GDALExporter::subMenuLevel() const
{
    const QString driverType = typeOfDriver();

    if (driverType == RASTER_DRIVER_TYPE)
        return mSubMenuLevelRaster;

    if (driverType == VECTOR_DRIVER_TYPE)
        return mSubMenuLevelVector;

    return SuperClass::subMenuLevel();
}

QString PB_GDALExporter::typeOfDriver() const
{
#ifdef USE_GDAL
    if (m_driver->GetMetadataItem(GDAL_DCAP_RASTER) != nullptr && m_driver->GetMetadataItem(GDAL_DCAP_VECTOR) == nullptr)
        return RASTER_DRIVER_TYPE;

    if (m_driver->GetMetadataItem(GDAL_DCAP_VECTOR) != nullptr && m_driver->GetMetadataItem(GDAL_DCAP_RASTER) == nullptr)
        return VECTOR_DRIVER_TYPE;
#endif

    return QString();
}

void PB_GDALExporter::setVectorsToExport(const QList<const CT_AbstractShape2D*>& vectorList)
{
    setMustUseModels(false);
    mRasters.clear();
    mVectors = vectorList;
}

void PB_GDALExporter::setRastersToExport(const QList<const CT_AbstractImage2D*>& rasterList)
{
    setMustUseModels(false);
    mVectors.clear();
    mRasters = rasterList;
}

#ifdef USE_GDAL2

    QList<CT_AbstractItemAttribute *> attributes = shape2d->itemAttributes();
    for (int i = 0 ; i < attributes.size() ; i++)
    {
        CT_AbstractItemAttribute* att = attributes.at(i);

        CT_OutAbstractSingularItemModel  *itemModel = (CT_OutAbstractSingularItemModel*) shape2d->model();
        CT_OutAbstractItemAttributeModel *attrModel = att->model();

        //QString itemDN = itemModel->displayableName();
        QString itemUN = itemModel->uniqueName();

        QString attrDN = attrModel->displayableName();
        QString attrUN = attrModel->uniqueName();

        if (attrModel->isADefaultItemAttributeModel() && attrModel->rootOriginalModel() != nullptr) {attrUN = attrModel->rootOriginalModel()->uniqueName();}

        QString key = QString("ITEM_%1_ATTR_%2").arg(itemUN).arg(attrUN);

        if (!_modelsKeys.contains(key))
        {
            _modelsKeys.append(key);
            _names.insert(key, attrDN);

            CT_AbstractCategory::ValueType type = attrModel->itemAttribute()->type();

            if      (type == CT_AbstractCategory::BOOLEAN) {_ogrTypes.insert(key, OFTInteger);}
            else if (type == CT_AbstractCategory::STRING)  {_ogrTypes.insert(key, OFTString);}
            else if (type == CT_AbstractCategory::STRING)  {_ogrTypes.insert(key, OFTString);}
            else if (type == CT_AbstractCategory::INT8)    {_ogrTypes.insert(key, OFTInteger);}
            else if (type == CT_AbstractCategory::UINT8)   {_ogrTypes.insert(key, OFTInteger);}
            else if (type == CT_AbstractCategory::INT16)   {_ogrTypes.insert(key, OFTInteger);}
            else if (type == CT_AbstractCategory::UINT16)  {_ogrTypes.insert(key, OFTInteger);}
            else if (type == CT_AbstractCategory::INT32)   {_ogrTypes.insert(key, OFTInteger);}
            //                else if (type == CT_AbstractCategory::UINT32)  {_ogrTypes.insert(key, OFTInteger64);}
            //                else if (type == CT_AbstractCategory::INT64)   {_ogrTypes.insert(key, OFTInteger64);}
            //                else if (type == CT_AbstractCategory::INT32)   {_ogrTypes.insert(key, OFTInteger64);}
            else                                           {_ogrTypes.insert(key, OFTReal);}
        }
    }

    if (!_names.isEmpty())
    {
        replaceBadCharacters(_names);
        qSort(_modelsKeys.begin(), _modelsKeys.end());
        _shortNames = computeShortNames(_names);


        QFileInfo exportPathInfo = QFileInfo(exportFilePath());

        QFile ffields(QString("%1/fields_names.txt").arg(exportPathInfo.absolutePath()));
        QTextStream fstream(&ffields);
        if (ffields.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMapIterator<QString, QString> itF(_shortNames);
            while (itF.hasNext())
            {
                itF.next();
                QString key = itF.key();
                QString shortName = itF.value();
                QString longName = _names.value(key);
                fstream << shortName << "\t";
                fstream << longName << "\n";
            }
            ffields.close();
        }

    }
#endif


void PB_GDALExporter::internalDeclareInputModels(CT_ExporterInModelStructureManager& manager)
{
    manager.addGroupToRootGroup(m_hInGroup);

    const QString driverType = typeOfDriver();

    if(driverType == RASTER_DRIVER_TYPE)
        manager.addItemToGroup(m_hInGroup, m_hInRaster, tr("Rasters"));
    else if(driverType == VECTOR_DRIVER_TYPE)
        manager.addItemToGroup(m_hInGroup, m_hInVector, tr("Vectors"));
}

CT_AbstractExporter::ExportReturn PB_GDALExporter::internalExportToFile()
{
    const QFileInfo exportPathInfo = QFileInfo(filePath());
    const QString prePath = exportPathInfo.path() + "/" + exportPathInfo.baseName();
    const QString suffix = exportFormats().first().suffixes().first();

    if(mustUseModels())
    {
        if(m_hInVector.isValid())
        {
            if(mIteratorVectorBegin == mIteratorVectorEnd)
            {
                auto iterator = m_hInVector.iterateInputs(m_handleResultExport);
                mIteratorVectorBegin = iterator.begin();
                mIteratorVectorEnd = iterator.end();
            }

            int nExported = 0;
            const int totalToExport = maximumItemToExportInFile(std::numeric_limits<int>::max());
            const QString noIndice;

            QList<const CT_AbstractShape2D*> vectors;

            while((mIteratorVectorBegin != mIteratorVectorEnd)
                  && (nExported < totalToExport))
            {
                vectors.append(*mIteratorVectorBegin);

                ++nExported;
                ++mIteratorVectorBegin;
            }

            // write data
            if(!exportVectors(vectors, prePath + "." + suffix))
                return ErrorWhenExport;

            return (mIteratorVectorBegin == mIteratorVectorEnd) ? NoMoreItemToExport : ExportCanContinue;
        }
        else if(m_hInRaster.isValid())
        {
            if(mIteratorRasterBegin == mIteratorRasterEnd)
            {
                auto iterator = m_hInRaster.iterateInputs(m_handleResultExport);
                mIteratorRasterBegin = iterator.begin();
                mIteratorRasterEnd = iterator.end();
            }

            if(mIteratorRasterBegin != mIteratorRasterEnd)
            {
                if(!exportRaster(*mIteratorRasterBegin, prePath + "." + suffix))
                    return ErrorWhenExport;

                ++mIteratorRasterBegin;
            }

            return (mIteratorRasterBegin == mIteratorRasterEnd) ? NoMoreItemToExport : ExportCanContinue;
        }
    }
    else if(!mVectors.isEmpty())
    {
        if(!exportVectors(mVectors, prePath + "." + suffix))
            return ErrorWhenExport;
    }
    else if(!mRasters.isEmpty())
    {
        int cpt = 0;
        for(auto raster : mRasters)
        {
            if(!exportRaster(raster, prePath + QString("_%1").arg(cpt) + "." + suffix))
                return ErrorWhenExport;

            ++cpt;
        }
    }

    return NoMoreItemToExport;
}

void PB_GDALExporter::clearIterators()
{
    mIteratorVectorBegin = HandleVectorType::const_iterator();
    mIteratorVectorEnd = mIteratorVectorBegin;

    mIteratorRasterBegin = HandleRasterType::const_iterator();
    mIteratorRasterEnd = mIteratorRasterBegin;
}

void PB_GDALExporter::clearAttributesClouds()
{
}

// (multiple vector per file)
bool PB_GDALExporter::exportVectors(const QList<const CT_AbstractShape2D*> vectors, const QString& filePath)
{
#ifdef USE_GDAL
    const std::string fp = filePath.toStdString();
    GDALDataset* dataset = m_driver->Create(fp.data(), 0, 0, 0, GDT_Unknown, nullptr);

    if( dataset == nullptr )
    {
        setErrorMessage(tr("Creation of output file failed."));
        return false;
    }

    OGRLayer *layer = dataset->CreateLayer("layer", nullptr, wkbUnknown, nullptr);

    // TODO : models ???!
    for (int i = 0 ; layer != nullptr && i < _modelsKeys.size() ; i++)
    {
        QString key = _modelsKeys.at(i);
        if (_ogrTypes.contains(key))
        {
            OGRFieldType ogrType = _ogrTypes.value(key);

            QByteArray fieldNameBA = _shortNames.value(key).toLatin1();
            const char* fieldName = fieldNameBA;

            OGRFieldDefn oField(fieldName, ogrType );

            if (layer->CreateField( &oField ) != OGRERR_NONE)
            {
                //  erreur
            }
        }
    }

    if( layer == nullptr )
    {
        GDALClose(dataset);
        setErrorMessage(tr("Layer creation failed."));
        return false;
    }

    for(const CT_AbstractShape2D* vector : vectors)
        exportVector(vector, dataset, layer);

    GDALClose(dataset);

    return true;
#else
    return false;
#endif
}

// RASTER (one raster per file)
bool PB_GDALExporter::exportRaster(const CT_AbstractImage2D* grid, const QString& filePath)
{
#ifdef USE_GDAL
    const int nXSize = grid->xdim();
    const int nYSize = grid->ydim();

    const std::string fp = filePath.toStdString();
    GDALDataset* dataset = m_driver->Create(fp.data(), nXSize, nYSize, 1, GDT_Float32, nullptr);

    if( dataset == nullptr )
    {
        setErrorMessage(tr("Creation of output file failed."));
        return false;
    }

    if( dataset->GetRasterBand(1) == nullptr )
    {
        GDALClose(dataset);
        setErrorMessage(tr("Creation of output file failed."));
        return false;
    }

    GDALRasterBand *poBand = dataset->GetRasterBand(1);

    double padfTransform[6] = { 0, 0, 0, 0, 0, 0 };

    padfTransform[0] = grid->minX();
    padfTransform[1] = grid->resolution();
    padfTransform[5] = -grid->resolution();
    padfTransform[3] = grid->minY() + nYSize*padfTransform[1];

    dataset->SetGeoTransform( padfTransform );

    poBand->SetNoDataValue(grid->NAAsDouble());

    float* pafScanline = static_cast<float*>(CPLMalloc(sizeof(float)*size_t(nXSize)));

    size_t index = 0;

    for(int y = 0 ;  y < nYSize ; ++y)
    {
        for(int x = 0 ; x < nXSize ; ++x)
        {
            grid->index(x, y, index);
            pafScanline[x] = float(grid->valueAtIndexAsDouble(index));
        }
        poBand->RasterIO( GF_Write, 0, y, nXSize, 1, pafScanline, nXSize, 1, GDT_Float32, 0, 0);
    }

    CPLFree(pafScanline);

    double min, max, mean, stdDev;
    poBand->FlushCache();
    poBand->ComputeStatistics(0, &min, &max, &mean, &stdDev, nullptr, nullptr);
    poBand->SetStatistics(min, max, mean, stdDev);
    poBand->FlushCache();

    GDALClose(dataset);

    return true;
#else
    return false;
#endif
}

#ifdef USE_GDAL

bool PB_GDALExporter::exportVector(const CT_AbstractShape2D* shape2d, GDALDataset *dataset, OGRLayer *layer)
{
    OGRFeature *poFeature;
    poFeature = OGRFeature::CreateFeature( layer->GetLayerDefn() );

    /*QList<CT_AbstractItemAttribute *> attributes = shape2d->itemAttributes();
    for (int i = 0 ; i < attributes.size() ; i++)
    {
        CT_AbstractItemAttribute* att = attributes.at(i);

        CT_OutAbstractSingularItemModel  *itemModel = (CT_OutAbstractSingularItemModel*) shape2d->model();
        CT_OutAbstractItemAttributeModel *attrModel = att->model();

        QString itemUN = itemModel->uniqueName();
        QString attrUN = attrModel->uniqueName();

        if (attrModel->isADefaultItemAttributeModel() && attrModel->rootOriginalModel() != nullptr) {attrUN = attrModel->rootOriginalModel()->uniqueName();}

        QString key = QString("ITEM_%1_ATTR_%2").arg(itemUN).arg(attrUN);


        QByteArray fieldNameBA = _shortNames.value(key).toLatin1();
        const char* fieldName = fieldNameBA;

        if      (_ogrTypes.value(key) == OFTBinary)
        {
            poFeature->SetField(fieldName, att->toInt(shape2d, nullptr));
        } else if (_ogrTypes.value(key) == OFTString)
        {
            //QString text = replaceAccentCharacters(att->toString(shape2d, nullptr));
            QString text = att->toString(shape2d, nullptr);
            QByteArray textBA = text.toLatin1();
            const char* textChar = textBA;
            poFeature->SetField(fieldName, textChar);
        } else if (_ogrTypes.value(key) == OFTInteger)
        {
            poFeature->SetField(fieldName, att->toInt(shape2d, nullptr));
        } else
        {
            poFeature->SetField(fieldName, att->toDouble(shape2d, nullptr));
        }
    }*/

    shape2d->visitItemAttributes([shape2d, &poFeature](const CT_AbstractItemAttribute* att) -> bool
    {
        const CT_AbstractCategory::ValueType vt = att->valueType();

        /*if(_ogrTypes.value(key) == OFTBinary)
        {
            poFeature->SetField(fieldName, att->toInt(shape2d, nullptr));
        }
        else if (vt == CT_AbstractCategory::STRING) // OFTString
        {
            //QString text = replaceAccentCharacters(att->toString(shape2d, nullptr));
            const std::string text = att->toString(shape2d, nullptr).toStdString();
            poFeature->SetField(fieldName, text.data());
        }
        else if((vt & CT_AbstractCategory::NUMBER_INT)
                && (vt != CT_AbstractCategory::INT64)
                && (vt != CT_AbstractCategory::UINT64)
                && (vt != CT_AbstractCategory::UINT32)) // OFTInteger
        {
            poFeature->SetField(fieldName, att->toInt(shape2d, nullptr));
        }
        else
        {
            poFeature->SetField(fieldName, att->toDouble(shape2d, nullptr));
        }*/
        return true;
    });

    const CT_Box2D *box = dynamic_cast<const CT_Box2D*>(shape2d);
    if(box != nullptr)
        return exportBox2D(box, dataset, layer, poFeature);

    const CT_Circle2D *circle = dynamic_cast<const CT_Circle2D*>(shape2d);
    if(circle != nullptr)
        return exportCircle2D(circle, dataset, layer, poFeature);

    const CT_Line2D *line = dynamic_cast<const CT_Line2D*>(shape2d);
    if(line != nullptr)
        return exportLine2D(line, dataset, layer, poFeature);

    const CT_Point2D *point = dynamic_cast<const CT_Point2D*>(shape2d);
    if(point != nullptr)
        return exportPoint2D(point, dataset, layer, poFeature);

    const CT_Polygon2D *polyg = dynamic_cast<const CT_Polygon2D*>(shape2d);
    if(polyg != nullptr)
        return exportPolygon2D(polyg, dataset, layer, poFeature);

    const CT_Polyline2D *polyl = dynamic_cast<const CT_Polyline2D*>(shape2d);
    if(polyl != nullptr)
        return exportPolyline2D(polyl, dataset, layer, poFeature);

    OGRFeature::DestroyFeature( poFeature );

    return false;
}

bool PB_GDALExporter::exportBox2D(const CT_Box2D *box, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature)
{
    OGRPolygon po;

    Eigen::Vector3d min, max;
    box->boundingBox(min, max);

    OGRLinearRing extRing;
    extRing.addPoint(min[0], min[1], 0);
    extRing.addPoint(max[0], min[1], 0);
    extRing.addPoint(max[0], max[1], 0);
    extRing.addPoint(min[0], max[1], 0);
    extRing.addPoint(min[0], min[1], 0);

    po.addRing(&extRing);

    return exportOGRGeometry(&po, dataset, layer, poFeature);
}

bool PB_GDALExporter::exportCircle2D(const CT_Circle2D *circle, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature)
{
    Eigen::Vector2d c = circle->getCenter();
    c[0] -= circle->getRadius();

    Eigen::Vector2d c2 = circle->getCenter();
    c2[0] += circle->getRadius();

    OGRLineString *ogrl = OGRGeometryFactory::curveToLineString(c[0], c[1], 0, c2[0], c2[1], 0, c[0], c[1], 0, false, 0, nullptr);

    bool r = exportOGRGeometry(ogrl, dataset, layer, poFeature);

    OGRGeometryFactory::destroyGeometry(ogrl);

    return r;
}

bool PB_GDALExporter::exportLine2D(const CT_Line2D *line, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature)
{
    OGRLineString ogrl;

    ogrl.addPoint(line->x1(), line->y1(), 0);
    ogrl.addPoint(line->x2(), line->y2(), 0);

    return exportOGRGeometry(&ogrl, dataset, layer, poFeature);
}

bool PB_GDALExporter::exportPoint2D(const CT_Point2D *point, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature)
{
    OGRPoint ogrp;

    ogrp.setX(point->x());
    ogrp.setX(point->y());
    ogrp.setZ(0);

    return exportOGRGeometry(&ogrp, dataset, layer, poFeature);
}

bool PB_GDALExporter::exportPolygon2D(const CT_Polygon2D *polygon, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature)
{
    OGRPolygon po;
    OGRLinearRing extRing;

    QVectorIterator<Eigen::Vector2d> it(polygon->getVertices());

    while(it.hasNext())
    {
        const Eigen::Vector2d& v = it.next();
        extRing.addPoint(v[0], v[1], 0);
    }

    it.toFront();
    if (it.hasNext())
    {
        const Eigen::Vector2d& v = it.next();
        extRing.addPoint(v[0], v[1], 0);
    }

    po.addRing(&extRing);

    return exportOGRGeometry(&po, dataset, layer, poFeature);
}

bool PB_GDALExporter::exportPolyline2D(const CT_Polyline2D *polyline, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature)
{
    OGRLineString ogrl;

    QVectorIterator<Eigen::Vector2d> it(polyline->getVertices());

    while(it.hasNext())
    {
        const Eigen::Vector2d& v = it.next();
        ogrl.addPoint(v[0], v[1], 0);
    }

    return exportOGRGeometry(&ogrl, dataset, layer, poFeature);
}

bool PB_GDALExporter::exportOGRGeometry(OGRGeometry *geo, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature)
{
    Q_UNUSED(dataset)

    poFeature->SetGeometry( geo );

    if( layer->CreateFeature( poFeature ) != OGRERR_NONE )
    {
        OGRFeature::DestroyFeature( poFeature );
        setErrorMessage(tr("Failed to create feature in shapefile."));
        return false;
    }

    OGRFeature::DestroyFeature( poFeature );

    return true;
}

void PB_GDALExporter::replaceBadCharacters(QMap<QString, QString> &names) const
{
    QMutableMapIterator<QString, QString> it(names);
    while (it.hasNext())
    {
        it.next();
        it.setValue(replaceBadCharacters(it.value()));
    }
}

QString PB_GDALExporter::replaceBadCharacters(const QString &name) const
{
    QString value = name;
    value.replace(QRegExp("[àáâãäå]"), "a");
    value.replace(QRegExp("[ÀÁÂÃÄÅ]"), "A");
    value.replace(QRegExp("[éèëê]"), "e");
    value.replace(QRegExp("[ÈÉÊË]"), "E");
    value.replace(QRegExp("[ìíîï]"), "i");
    value.replace(QRegExp("[ÌÍÎÏ]"), "I");
    value.replace(QRegExp("[òóôõöø]"), "o");
    value.replace(QRegExp("[ÒÓÔÕÖØ]"), "O");
    value.replace(QRegExp("[ùúûü]"), "u");
    value.replace(QRegExp("[ÙÚÛÜ]"), "U");
    value.replace(QRegExp("[ñ]"), "n");
    value.replace(QRegExp("[Ñ]"), "N");
    value.replace(QRegExp("[ç]"), "c");
    value.replace(QRegExp("[Ç]"), "C");
    value.replace(QRegExp("[\\W]"), "_");
    return value;
}

QString PB_GDALExporter::replaceAccentCharacters(const QString &name) const
{
        QString value = name;
        value.replace(QRegExp("[àáâãäå]"), "a");
        value.replace(QRegExp("[ÀÁÂÃÄÅ]"), "A");
        value.replace(QRegExp("[éèëê]"), "e");
        value.replace(QRegExp("[ÈÉÊË]"), "E");
        value.replace(QRegExp("[ìíîï]"), "i");
        value.replace(QRegExp("[ÌÍÎÏ]"), "I");
        value.replace(QRegExp("[òóôõöø]"), "o");
        value.replace(QRegExp("[ÒÓÔÕÖØ]"), "O");
        value.replace(QRegExp("[ùúûü]"), "u");
        value.replace(QRegExp("[ÙÚÛÜ]"), "U");
        value.replace(QRegExp("[ñ]"), "n");
        value.replace(QRegExp("[Ñ]"), "N");
        value.replace(QRegExp("[ç]"), "c");
        value.replace(QRegExp("[Ç]"), "C");
        return value;
}


QMap<QString, QString> PB_GDALExporter::computeShortNames(const QMap<QString, QString> &names) const
{
    QMap<QString, QString> shortNames;
    QList<QString> existing;

    QMapIterator<QString, QString> it(names);
    while (it.hasNext())
    {
        it.next();
        QString key = it.key();
        QString value = it.value();

        if (value.size() <= 10)
        {
            shortNames.insert(key, value);
            existing.append(value.toLower());
        } else {
            QString newValue = value.left(10);
            int cpt = 2;
            while (existing.contains(newValue.toLower()))
            {
                QString number = QVariant(cpt++).toString();
                newValue = QString("%1%2").arg(value.left(10 - number.length())).arg(number);
            }
            shortNames.insert(key, newValue);
            existing.append(newValue.toLower());
        }
    }

    return shortNames;
}

#endif
