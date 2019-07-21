#ifndef CT_READER_GDAL_H
#define CT_READER_GDAL_H

#include "ctlibio/ctlibio_global.h"

#include "ct_reader/abstract/ct_abstractreader.h"

#ifdef USE_GDAL
#include "ct_itemdrawable/ct_image2d.h"
#include "ct_itemdrawable/ct_point2d.h"
#include "ct_itemdrawable/ct_polygon2d.h"
#include "ct_itemdrawable/ct_polyline2d.h"
#include "gdal_priv.h"
#include "ogr_geometry.h"
#include "ogr_feature.h"
#endif

/**
 * @brief Read files that gdal can read
 */
class CTLIBIO_EXPORT CT_Reader_GDAL : public CT_AbstractReader
{
    Q_OBJECT
    using SuperClass = CT_AbstractReader;

public:
    CT_Reader_GDAL();
    CT_Reader_GDAL(const CT_Reader_GDAL &other);
    #ifdef USE_GDAL
    CT_Reader_GDAL(const GDALDriver *driver);
    ~CT_Reader_GDAL() override;

    GDALDriver* getDriver() const { return m_driver; }
    #endif

    /**
     * @brief Returns a displayable name of the reader
     */
    QString displayableName() const override;

    /**
     * @brief Returns the unique name (you can use it to compare readers because it was unique).
     */
    QString uniqueName() const override;

    /**
     * @brief Returns the sub menu level where we can store this reader
     */
    //CT_StepsMenu::LevelPredefined getReaderSubMenuName() const;

    /**
     * @brief Returns "Raster" for raster and "Vector" for vector otherwise returns an empty string
     */
    QString getTypeOfDriver() const;

    READER_ALL_COPY_IMP(CT_Reader_GDAL)

protected:

    /**
     * @brief Redefined to verify that file can be opened by gdal and returns false if not.
     */
    bool postVerifyFile(const QString &filepath) override;

    void internalDeclareOutputModels(CT_ReaderOutModelStructureManager& manager) override;

    bool internalReadFile(CT_StandardItemGroup* group) override;

private:
    /**
     * @brief Called in constructor when a driver is set. Update the name of driver, readable format and tooltip
     */
    void init();

#ifdef USE_GDAL
private:
    using RasterHandleType = CT_HandleOutSingularItem<CT_Image2D<float>>;
    using Point2DHandleType = CT_HandleOutSingularItem<CT_Point2D>;
    using Polygon2DHandleType = CT_HandleOutSingularItem<CT_Polygon2D>;
    using Polyline2DHandleType = CT_HandleOutSingularItem<CT_Polyline2D>;
    using AbstractItemHandle = CT_AbstractHandleOutItem;

    using AttributeIntHandleType = CT_HandleOutStdItemAttribute<int>;
    using AttributeDoubleHandleType = CT_HandleOutStdItemAttribute<double>;
    using AttributeStringHandleType = CT_HandleOutStdItemAttribute<QString>;
    using AbstractAttributeHandle = CT_AbstractHandleOutItemAttribute;

    using LayerHandleType = CT_HandleOutStdGroup;

    GDALDriver                                      *m_driver;
    QString                                         m_nameFromDriver;

    QList<RasterHandleType*>                        m_rasterHandles;
    QList<LayerHandleType*>                         m_layerHandles;
    QHash<LayerHandleType*, Point2DHandleType*>     m_pointHandles;
    QHash<LayerHandleType*, Polygon2DHandleType*>   m_polygoneHandles;
    QHash<LayerHandleType*, Polyline2DHandleType*>  m_polylineHandles;
    QHash<QPair<AbstractItemHandle*, int>, AbstractAttributeHandle*>     m_itemAttributeHandles;

    /**
     * @brief Clear all collection of handles and delete handles from memory
     */
    void clearHandles();

    /**
     * @brief Return true if the file can be opened by this driver
     */
    bool canBeOpened(const QString &filepath) const;

    /**
     * @brief Get the dataset from the file.
     * @return NULL if the file can not be read or is was an error
     */
    GDALDataset* getDataSet(const QString &filepath) const;

    bool createGeometryModel(OGRFeatureDefn *poFDefn,
                             OGRGeometry *poGeometry,
                             QStringList &canNotBeLoaded,
                             CT_ReaderOutModelStructureManager& manager,
                             LayerHandleType& layerHandle,
                             Point2DHandleType*& point2DHandle,
                             Polygon2DHandleType*& polygon2DHandle,
                             Polyline2DHandleType*& polyline2DHandle);

    template<typename ItemHandleType>
    void createAttributesModel(OGRFeatureDefn *poFDefn,
                               CT_ReaderOutModelStructureManager& manager,
                               LayerHandleType& /*layerHandle*/,
                               ItemHandleType& itemHandle)
    {
        for( int iField = 0; iField < poFDefn->GetFieldCount(); iField++ )
        {
            OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );

            switch( poFieldDefn->GetType() )
            {
            case OFTInteger: {
                AttributeIntHandleType* itemAttributeHandle = new AttributeIntHandleType();
                manager.addItemAttributeAndFindCategory(itemHandle, *itemAttributeHandle, CT_AbstractCategory::DATA_VALUE);
                m_itemAttributeHandles.insert(qMakePair((AbstractItemHandle*)&itemHandle, iField), itemAttributeHandle);
                break;
            }

            case OFTReal:{
                AttributeDoubleHandleType* itemAttributeHandle = new AttributeDoubleHandleType();
                manager.addItemAttributeAndFindCategory(itemHandle, *itemAttributeHandle, CT_AbstractCategory::DATA_VALUE);
                m_itemAttributeHandles.insert(qMakePair((AbstractItemHandle*)&itemHandle, iField), itemAttributeHandle);
                break;
            }

            default:{
                AttributeStringHandleType* itemAttributeHandle = new AttributeStringHandleType();
                manager.addItemAttributeAndFindCategory(itemHandle, *itemAttributeHandle, CT_AbstractCategory::DATA_VALUE);
                m_itemAttributeHandles.insert(qMakePair((AbstractItemHandle*)&itemHandle, iField), itemAttributeHandle);
                break;
            }
            }
        }
    }

    bool createPolyline(OGRFeature *poFeature,
                        OGRFeatureDefn *poFDefn,
                        OGRLineString *line,
                        LayerHandleType* layerHandle,
                        CT_StandardItemGroup *layer);

    bool createPolygon(OGRFeature *poFeature,
                       OGRFeatureDefn *poFDefn,
                       OGRPolygon *polygon,
                       LayerHandleType* layerHandle,
                       CT_StandardItemGroup *layer);

    bool createPoint(OGRFeature *poFeature,
                     OGRFeatureDefn *poFDefn,
                     OGRPoint *point,
                     LayerHandleType* layerHandle,
                     CT_StandardItemGroup *layer);

    template<typename ItemHandleType>
    bool createAttributes(OGRFeature *poFeature,
                          OGRFeatureDefn *poFDefn,
                          LayerHandleType* /*layerHandle*/,
                          ItemHandleType* itemHandle,
                          CT_AbstractSingularItemDrawable* item)
    {
        for( int iField = 0; iField < poFDefn->GetFieldCount(); iField++ )
        {
            // create the item attribute
            CT_AbstractItemAttribute *att = createAttributeWithGoodType(poFDefn, iField, poFeature);
            AbstractAttributeHandle* attributeHandle = m_itemAttributeHandles.value(qMakePair((AbstractItemHandle*)itemHandle, iField), nullptr);
            item->addItemAttribute(*attributeHandle, att);
        }

        return true;
    }

    CT_AbstractItemAttribute* createAttributeWithGoodType(OGRFeatureDefn *poFDefn,
                                                          int iField,
                                                          OGRFeature *poFeature);

    /**
     * @brief Convert the poGeometry to a CT_XXX object and add it to a new group for the layer 'layerIndex'
     */
    bool convertGeometryToCT(OGRGeometry *poGeometry, OGRFeature *poFeature, OGRFeatureDefn *poFDefn, LayerHandleType* layerHandle, CT_StandardItemGroup* rootGroup);

    /**
     * @brief Update the name from the driver
     */
    void updateNameFromDriver();
#endif
};

#endif // CT_READER_GDAL_H
