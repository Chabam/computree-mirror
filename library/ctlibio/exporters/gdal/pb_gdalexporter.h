#ifndef PB_GDALEXPORTER_H
#define PB_GDALEXPORTER_H

#include "ctlibio/ctlibio_global.h"

#include "ct_exporter/abstract/ct_abstractexporter.h"

#include "ct_itemdrawable/ct_box2d.h"
#include "ct_itemdrawable/ct_circle2d.h"
#include "ct_itemdrawable/ct_line2d.h"
#include "ct_itemdrawable/ct_point2d.h"
#include "ct_itemdrawable/ct_polygon2d.h"
#include "ct_itemdrawable/ct_polyline2d.h"
#include "ct_itemdrawable/abstract/ct_abstractimage2d.h"

#ifdef USE_GDAL
#include "gdal_priv.h"

class OGRFeature;
#endif

/**
 * @brief Export ItemDrawable to files that gdal can create
 */
class CTLIBIO_EXPORT PB_GDALExporter : public CT_AbstractExporter
{
    Q_OBJECT
    using SuperClass = CT_AbstractExporter;

public:
    PB_GDALExporter(int subMenuLevelRaster = 0, int subMenuLevelVector = 0, int subMenuLevelDefault = 0);
    PB_GDALExporter(const PB_GDALExporter& other);

    #ifdef USE_GDAL
    PB_GDALExporter(const GDALDriver* driver, int subMenuLevelRaster = 0, int subMenuLevelVector = 0, int subMenuLevelDefault = 0);
    #endif

    QString displayableName() const final;

    bool isExportEachItemInSeparateFileOptionnal() const final;

    QString uniqueName() const final;

    int subMenuLevel() const final;

    void setVectorsToExport(const QList<const CT_AbstractShape2D*>& vectorList);
    void setRastersToExport(const QList<const CT_AbstractImage2D*>& rasterList);

    /**
     * @brief Returns "Raster" or "Vector" depends of the type of the driver used
     */
    QString typeOfDriver() const;

    CT_EXPORTER_DECL_COPY(PB_GDALExporter)

protected:
    void internalDeclareInputModels(CT_ExporterInModelStructureManager& manager) final;

    ExportReturn internalExportToFile() final;

    void clearIterators() final;

    void clearAttributesClouds() final;

#ifdef USE_GDAL
private:
    GDALDriver*                                     m_driver;
    QMap<QString, OGRFieldType>                     _ogrTypes;

    bool exportVector(const CT_AbstractShape2D* shape2d, GDALDataset *dataset, OGRLayer *layer);

    bool exportBox2D(const CT_Box2D *box, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature);
    bool exportCircle2D(const CT_Circle2D *circle, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature);
    bool exportLine2D(const CT_Line2D *line, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature);
    bool exportPoint2D(const CT_Point2D *point, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature);
    bool exportPolygon2D(const CT_Polygon2D *polygon, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature);
    bool exportPolyline2D(const CT_Polyline2D *polyline, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature);

    bool exportOGRGeometry(OGRGeometry *geo, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature);
#endif

private:
    int                                             mSubMenuLevelRaster;
    int                                             mSubMenuLevelVector;
    QList<const CT_AbstractShape2D*>                mVectors;
    QList<const CT_AbstractImage2D*>                mRasters;

    using HandleVectorType = CT_HandleInSingularItem<CT_AbstractShape2D>;
    using HandleRasterType = CT_HandleInSingularItem<CT_AbstractImage2D>;

    CT_HandleInStdGroup<>                           m_hInGroup;
    HandleVectorType                                m_hInVector;
    HandleRasterType                                m_hInRaster;

    HandleVectorType::const_iterator                mIteratorVectorBegin;
    HandleVectorType::const_iterator                mIteratorVectorEnd;

    HandleRasterType::const_iterator                mIteratorRasterBegin;
    HandleRasterType::const_iterator                mIteratorRasterEnd;


    QList<QString>          _modelsKeys;
    QMap<QString, QString>  _names;
    QMap<QString, QString> _shortNames;

    void replaceBadCharacters(QMap<QString, QString> &names) const;
    QString replaceBadCharacters(const QString &name) const;
    QString replaceAccentCharacters(const QString &name) const;
    QMap<QString, QString> computeShortNames(const QMap<QString, QString> &names) const;

    template<typename HandleT, typename IteratorT>
    void internalExportToFileWithModelT(HandleT& handle, IteratorT& begin, IteratorT& end, const QString& prePath, const QString& suffix)
    {
        if(begin == end)
        {
            auto iterator = handle.iterateInputs(m_handleResultExport);
            begin = iterator.begin();
            end = iterator.end();
        }

        int nExported = 0;
        const int totalToExport = maximumItemToExportInFile(std::numeric_limits<int>::max());
        const QString noIndice;

        // write data
        while((begin != end)
              && (nExported < totalToExport))
        {
            const auto item = *begin;

            exportRaster(item, prePath + "." + suffix);

            ++nExported;
            ++begin;
        }
    }

    bool exportVectors(const QList<const CT_AbstractShape2D*> vectors, const QString& filePath);
    bool exportRaster(const CT_AbstractImage2D* grid, const QString& filePath);
};

#endif // PB_GDALEXPORTER_H
