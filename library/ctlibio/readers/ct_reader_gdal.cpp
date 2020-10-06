#include "ct_reader_gdal.h"

#ifdef USE_GDAL
#include "gdal_priv.h"
#include "gdal.h"
#include "ogrsf_frmts.h"
#include "readers/tools/gdal/ct_gdaltools.h"
#endif

#include "ct_log/ct_logmanager.h"

CT_Reader_GDAL::CT_Reader_GDAL() : SuperClass()
{
#ifdef USE_GDAL
    m_driver = nullptr;
#endif
}

CT_Reader_GDAL::CT_Reader_GDAL(const CT_Reader_GDAL &other) : SuperClass(other)
{
#ifdef USE_GDAL
    m_driver = other.m_driver;
    m_nameFromDriver = other.m_nameFromDriver;
#endif
}

#ifdef USE_GDAL
CT_Reader_GDAL::CT_Reader_GDAL(const GDALDriver* driver, int defaultMenuLevel, int rasterMenuLevel, int vectorMenuLevel)
{
    m_driver = const_cast<GDALDriver*>(driver);

    const QString driverType = getTypeOfDriver();

    QStringList ext = QString(m_driver->GetMetadataItem(GDAL_DMD_EXTENSION)).split("/");

    if (ext.size() == 1 && ext.first().isEmpty())
    {
        setSubMenuLevel(defaultMenuLevel);

    }
    else if (driverType == "Raster")
    {
        setSubMenuLevel(rasterMenuLevel == -1 ? defaultMenuLevel : rasterMenuLevel);
    }
    else if (driverType == "Vector")
    {
        setSubMenuLevel(vectorMenuLevel == -1 ? defaultMenuLevel : vectorMenuLevel);
    }
    else
    {
        setSubMenuLevel(defaultMenuLevel);
    }

    init();
}

#endif

QString CT_Reader_GDAL::getTypeOfDriver() const
{
#ifdef USE_GDAL
    if(m_driver == nullptr)
    {
        return QString();
    }

    if((m_driver->GetMetadataItem(GDAL_DCAP_RASTER) != nullptr) && (m_driver->GetMetadataItem(GDAL_DCAP_VECTOR) == nullptr))
    {
        return "Raster";
    }

    if((m_driver->GetMetadataItem(GDAL_DCAP_VECTOR) != nullptr) && (m_driver->GetMetadataItem(GDAL_DCAP_RASTER) == nullptr))
    {
        return "Vector";
    }
#endif

    return QString();
}

bool CT_Reader_GDAL::postVerifyFile(const QString& filepath)
{
    #ifdef USE_GDAL
    if(m_driver != nullptr)
        return canBeOpened(filepath);
    #else
    Q_UNUSED(filepath)
    #endif

    return false;
}

void CT_Reader_GDAL::internalDeclareOutputModels(CT_ReaderOutModelStructureManager& manager)
{
    #ifdef USE_GDAL
    GDALDataset *data = getDataSet(filepath());

    if(data != nullptr)
    {
        int n = data->GetRasterCount();

        if(n != 0) {
            for(int i=0; i<n; ++i) {
                const QString name = GDALGetColorInterpretationName(data->GetRasterBand(i+1)->GetColorInterpretation());

                RasterHandleType* handle = new RasterHandleType();
                registerHandlePtr(QString("r%1").arg(i), handle);

                manager.addItem(*handle, (name.isEmpty() || (name == "Undefined")) ? QString("Raster %1").arg(i) : name);
            }
        } else {
            n = data->GetLayerCount();

            for(int i=0; i<n; ++i) {
                LayerHandleType* layerHandle = new LayerHandleType();
                registerHandlePtr(QString("l%1").arg(i), layerHandle);
                manager.addGroup(*layerHandle, tr("Layer %1").arg(i));

                OGRLayer *poLayer = data->GetLayer(i);

                OGRFeature *poFeature;

                poLayer->ResetReading();

                OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();

                QStringList canNotBeLoaded;
                Point2DHandleType* point2DHandle = nullptr;
                Polygon2DHandleType* polygon2DHandle = nullptr;
                Polyline2DHandleType* polyline2DHandle = nullptr;

                while(((poFeature = poLayer->GetNextFeature()) != nullptr)
                      && ((point2DHandle == nullptr) || (polygon2DHandle == nullptr) || (polyline2DHandle == nullptr)))
                {
                    OGRGeometry *poGeometry = poFeature->GetGeometryRef();

                    if(poGeometry != nullptr)
                        createGeometryModel(poFDefn, poGeometry, canNotBeLoaded, manager, *layerHandle, point2DHandle, polygon2DHandle, polyline2DHandle);

                    OGRFeature::DestroyFeature( poFeature );
                }
            }
        }

        GDALClose(data);
    }
    #endif
}

#ifdef USE_GDAL
bool CT_Reader_GDAL::createGeometryModel(OGRFeatureDefn *poFDefn,
                                         OGRGeometry *poGeometry,
                                         QStringList &canNotBeLoaded,
                                         CT_ReaderOutModelStructureManager& manager,
                                         LayerHandleType& layerHandle,
                                         Point2DHandleType*& point2DHandle,
                                         Polygon2DHandleType*& polygon2DHandle,
                                         Polyline2DHandleType*& polyline2DHandle)
{
    OGRwkbGeometryType geoT = wkbFlatten(poGeometry->getGeometryType());

    if((geoT == wkbPoint) || (geoT == wkbMultiPoint)) {

        if(point2DHandle == nullptr) {
            point2DHandle = new Point2DHandleType();
            registerHandlePtr(QString("p%1").arg(size_t(&layerHandle)), point2DHandle);

            manager.addItem(layerHandle, *point2DHandle, tr("Points"));
            createAttributesModel(poFDefn, manager, layerHandle, *point2DHandle);
        }

        return true;
    }
    else if((geoT == wkbPolygon) || (geoT == wkbMultiPolygon)) {

        if(polygon2DHandle == nullptr) {
            polygon2DHandle = new Polygon2DHandleType();
            registerHandlePtr(QString("plg%1").arg(size_t(&layerHandle)), polygon2DHandle);

            manager.addItem(layerHandle, *polygon2DHandle, tr("Polygones"));
            createAttributesModel(poFDefn, manager, layerHandle, *polygon2DHandle);
        }

        return true;
    }
    else if((geoT == wkbLineString) || (geoT == wkbMultiLineString)) {

        if(polyline2DHandle == nullptr) {
            polyline2DHandle = new Polyline2DHandleType();
            registerHandlePtr(QString("pll%1").arg(size_t(&layerHandle)), polyline2DHandle);

            manager.addItem(layerHandle, *polyline2DHandle, tr("Polylignes"));
            createAttributesModel(poFDefn, manager, layerHandle, *polyline2DHandle);
        }

        return true;
    } else if(geoT == wkbGeometryCollection) {

        OGRGeometryCollection *multi = (OGRGeometryCollection*)poGeometry;

        int n = multi->getNumGeometries();

        for(int j=0; (j<n) && ((point2DHandle == nullptr) || (polygon2DHandle == nullptr) || (polyline2DHandle == nullptr)); ++j) {

            OGRGeometry *poGeometry2 = multi->getGeometryRef(j);

            createGeometryModel(poFDefn,
                                poGeometry2,
                                canNotBeLoaded,
                                manager,
                                layerHandle,
                                point2DHandle,
                                polygon2DHandle,
                                polyline2DHandle);
        }

        return true;
    }

    const QString geoName = poGeometry->getGeometryName();

    if(!canNotBeLoaded.contains(geoName)) {
        canNotBeLoaded.append(geoName);
        PS_LOG->addDebugMessage(LogInterface::reader, tr("Impossible de charger des éléments du type : ") + geoName);
    }

    return false;
}

bool CT_Reader_GDAL::createPolyline(OGRFeature *poFeature,
                                    OGRFeatureDefn *poFDefn,
                                    OGRLineString *line,
                                    LayerHandleType* layerHandle,
                                    CT_StandardItemGroup *rootGroup)
{
    const int n = line->getNumPoints();

    if(n > 0) {

        QVector<Eigen::Vector2d> vertices(n);

        OGRPoint point;

        for(int j=0; j<n; ++j) {
            line->getPoint(j, &point);
            vertices[j] = Eigen::Vector2d(point.getX(), point.getY());
        }

        CT_StandardItemGroup* layer = layerHandle->createInstance();
        rootGroup->addGroup(*layerHandle, layer);

        Polyline2DHandleType* itemHandle = registeredHandlePtr<Polyline2DHandleType>(QString("pll%1").arg(size_t(layerHandle)));
        CT_Polyline2D* item = itemHandle->createInstance(new CT_Polyline2DData(vertices));
        layer->addSingularItem(*itemHandle, item);

        createAttributes(poFeature, poFDefn, layerHandle, itemHandle, item);

        return true;
    }

    return false;
}

bool CT_Reader_GDAL::createPolygon(OGRFeature *poFeature,
                   OGRFeatureDefn *poFDefn,
                   OGRPolygon *polygon,
                   LayerHandleType* layerHandle,
                   CT_StandardItemGroup *rootGroup)
{
    const OGRLinearRing *ring = polygon->getExteriorRing();

    if(ring != nullptr) {
        int n = ring->getNumPoints();

        QVector<Eigen::Vector2d> vertices(n);

        OGRPoint point;

        for(int j=0; j<n; ++j) {
            ring->getPoint(j, &point);
            vertices[j] = Eigen::Vector2d(point.getX(), point.getY());
        }

        CT_StandardItemGroup* layer = layerHandle->createInstance();
        rootGroup->addGroup(*layerHandle, layer);

        Polygon2DHandleType* itemHandle = registeredHandlePtr<Polygon2DHandleType>(QString("plg%1").arg(size_t(layerHandle)));
        CT_Polygon2D* item = itemHandle->createInstance(new CT_Polygon2DData(vertices));
        layer->addSingularItem(*itemHandle, item);

        createAttributes(poFeature, poFDefn, layerHandle, itemHandle, item);

        return true;
    }

    return false;
}

bool CT_Reader_GDAL::createPoint(OGRFeature *poFeature,
                 OGRFeatureDefn *poFDefn,
                 OGRPoint *point,
                 LayerHandleType* layerHandle,
                 CT_StandardItemGroup *rootGroup)
{

    CT_StandardItemGroup* layer = layerHandle->createInstance();
    rootGroup->addGroup(*layerHandle, layer);

    Point2DHandleType* itemHandle = registeredHandlePtr<Point2DHandleType>(QString("p%1").arg(size_t(layerHandle)));
    CT_Point2D* item = itemHandle->createInstance(new CT_Point2DData(point->getX(), point->getY()));
    layer->addSingularItem(*itemHandle, item);

    createAttributes(poFeature, poFDefn, layerHandle, itemHandle, item);

    return true;
}
#endif

bool CT_Reader_GDAL::internalReadFile(CT_StandardItemGroup* group)
{
    #ifdef USE_GDAL

    GDALDataset *data = getDataSet(filepath());

    if(data == nullptr)
        return false;

    int count = data->GetRasterCount();

    if(count > 0) {
        double padfTransform[6];

        for(int i=0; i<count; ++i) {

            GDALRasterBand *poBand = data->GetRasterBand(i+1);

            poBand->GetDataset()->GetGeoTransform(&padfTransform[0]);

            const int nXSize = poBand->GetXSize();
            const int nYSize = poBand->GetYSize();

            const double xMin = padfTransform[0];
            const double yMin = padfTransform[3] - nYSize*padfTransform[1];

            const double na = poBand->GetNoDataValue();

            RasterHandleType* rasterHandle = registeredHandlePtr<RasterHandleType>(QString("r%1").arg(i));
            auto raster = rasterHandle->createInstance(xMin,
                                                       yMin,
                                                       nXSize,
                                                       nYSize,
                                                       padfTransform[1], 0, na, na);

            float *pafScanline = (float *) CPLMalloc(sizeof(float)*nXSize);

            size_t index = 0;

            for(int y=0; y<nYSize; ++y) {
#if defined(_WIN32) && defined(_MSC_VER) // Microsoft Visual Studio Compiler
#elif (defined(__linux__) || defined(_WIN32)) && defined(__GNUC__) // GNU Compiler (gcc,g++) for Linux, Unix, and MinGW (Windows)
#pragma GCC diagnostic ignored "-Wunused-result"
#elif defined(__APPLE__) // Clang Compiler (Apple)
#pragma GCC diagnostic ignored "-Wunused-result"
#endif
                poBand->RasterIO( GF_Read, 0, y, nXSize, 1, pafScanline, nXSize, 1, GDT_Float32, 0, 0 );
#if defined(_WIN32) && defined(_MSC_VER)
#elif (defined(__linux__) || defined(_WIN32)) && defined(__GNUC__)
#pragma GCC diagnostic warning "-Wunused-result"
#elif defined(__APPLE__)
#pragma GCC diagnostic warning "-Wunused-result"
#endif

                for(int x=0; x<nXSize; ++x) {
                    raster->index(x, y, index);
                    raster->setValueAtIndex(index, pafScanline[x]);
                }
            }

            CPLFree(pafScanline);

            raster->computeMinMax();

            group->addSingularItem(*rasterHandle, raster);
        }
    } else {
        count = data->GetLayerCount();

        for(int i=0; i<count; ++i) {

            OGRLayer *poLayer = data->GetLayer(i);

            LayerHandleType* layerHandle = registeredHandlePtr<LayerHandleType>(QString("l%1").arg(i));

            OGRFeature *poFeature;

            poLayer->ResetReading();

            OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();

            while( (poFeature = poLayer->GetNextFeature()) != nullptr )
            {
                OGRGeometry *poGeometry = poFeature->GetGeometryRef();

                if(poGeometry != nullptr)
                    convertGeometryToCT(poGeometry, poFeature, poFDefn, layerHandle, group);

                OGRFeature::DestroyFeature( poFeature );
            }
        }
    }

    GDALClose(data);

    return true;
    #else

    return false;
    #endif
}

QString CT_Reader_GDAL::displayableName() const
{
    #ifdef USE_GDAL
    if(!m_nameFromDriver.isEmpty())
        return m_nameFromDriver;
    #endif
    return metaObject()->className();
}

QString CT_Reader_GDAL::uniqueName() const
{
    return displayableName();
}

void CT_Reader_GDAL::init()
{
    #ifdef USE_GDAL
    if(m_driver != nullptr) {

        updateNameFromDriver();

        QStringList ext = CT_GdalTools::staticGdalDriverExtension(m_driver);

        if(ext.isEmpty())
            ext.append("*");

        addNewReadableFormat(FileFormat(ext, CT_GdalTools::staticGdalDriverName(m_driver)));

        const QString driverType = getTypeOfDriver();

        QString toolTip = tr("Reader GDAL de type : %1").arg(driverType.isEmpty()?tr("Inconnu"):driverType);

        if (driverType == "Raster")
        {
            toolTip.append("<br>");
            toolTip.append("<br>");
            toolTip.append(tr("Pour plus de détails voir : http://www.gdal.org/formats_list.html"));
        } else if (driverType == "Vector")
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
}

#ifdef USE_GDAL
CT_AbstractItemAttribute* CT_Reader_GDAL::createAttributeWithGoodType(OGRFeatureDefn *poFDefn,
                                                                      int iField,
                                                                      OGRFeature *poFeature)
{
    OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );

    switch( poFieldDefn->GetType() )
    {
    case OFTInteger:
        return new CT_StdItemAttributeT<int>(CT_AbstractCategory::DATA_VALUE, ((poFeature != nullptr) ? poFeature->GetFieldAsInteger(iField) : 0));

    case OFTReal:
        return new CT_StdItemAttributeT<double>(CT_AbstractCategory::DATA_VALUE, ((poFeature != nullptr) ? poFeature->GetFieldAsDouble(iField) : 0));

    default:
        return new CT_StdItemAttributeT<QString>(CT_AbstractCategory::DATA_VALUE, QString((poFeature != nullptr) ? poFeature->GetFieldAsString(iField) : QString()));
    }
}

bool CT_Reader_GDAL::canBeOpened(const QString &filepath) const
{
    GDALDataset *d = getDataSet(filepath);

    if(d == nullptr)
        return false;

    GDALClose(d);
    return true;
}

GDALDataset* CT_Reader_GDAL::getDataSet(const QString &filepath) const
{
    GDALDataset *poDataset = (GDALDataset *) GDALOpenEx(filepath.toStdString().data(), GDAL_OF_RASTER | GDAL_OF_VECTOR | GDAL_OF_READONLY, nullptr, nullptr, nullptr);

    if((poDataset != nullptr) && (poDataset->GetDriver() == m_driver))
        return poDataset;

    if(poDataset != nullptr)
        GDALClose(poDataset);

    return nullptr;
}

bool CT_Reader_GDAL::convertGeometryToCT(OGRGeometry *poGeometry, OGRFeature *poFeature, OGRFeatureDefn *poFDefn, LayerHandleType* layerHandle, CT_StandardItemGroup* rootGroup)
{
    OGRwkbGeometryType geoT = wkbFlatten(poGeometry->getGeometryType());

    bool ok = false;

    if(geoT == wkbLineString) {
        ok = createPolyline(poFeature, poFDefn, (OGRLineString*)poGeometry, layerHandle, rootGroup);
    } else if(geoT == wkbPolygon) {
        ok = createPolygon(poFeature, poFDefn, (OGRPolygon*)poGeometry, layerHandle, rootGroup);
    } else if(geoT == wkbPoint) {
        ok = createPoint(poFeature, poFDefn, (OGRPoint*)poGeometry, layerHandle, rootGroup);
    } else if((geoT == wkbGeometryCollection) || (geoT == wkbMultiLineString) || (geoT == wkbMultiPolygon) || (geoT == wkbMultiPoint)) {

        OGRGeometryCollection *multi = (OGRGeometryCollection*)poGeometry;

        const int n = multi->getNumGeometries();

        for(int j=0; j<n; ++j)
            convertGeometryToCT(multi->getGeometryRef(j), poFeature, poFDefn, layerHandle, rootGroup);
    }

    return ok;
}

void CT_Reader_GDAL::updateNameFromDriver()
{
    if(m_driver != nullptr)
        m_nameFromDriver = QString("%1 %2").arg(getTypeOfDriver()).arg(CT_GdalTools::staticGdalDriverName(m_driver));
    else
        m_nameFromDriver = QString();
}
#endif
