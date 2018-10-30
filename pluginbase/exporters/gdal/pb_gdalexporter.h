#ifndef PB_GDALEXPORTER_H
#define PB_GDALEXPORTER_H

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
class PB_GDALExporter : public CT_AbstractExporter
{
    Q_OBJECT
    typedef CT_AbstractExporter SuperClass;

public:
    PB_GDALExporter();
    PB_GDALExporter(const PB_GDALExporter& other) = default;

    #ifdef USE_GDAL
    PB_GDALExporter(const GDALDriver *driver);
    #endif

    void init() override;

    QString getExporterCustomName() const override;

    QString getExporterName() const override;

    CT_StepsMenu::LevelPredefined getExporterSubMenuName() const override;

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list) override;

    /**
     * @brief Returns "Raster" or "Vector" depends of the type of the driver used
     */
    QString getTypeOfDriver() const;

    CT_EXPORTER_DECL_COPY(PB_GDALExporter)

protected:
    bool protectedExportToFile() override;

private:
    QList<QString>          _modelsKeys;
    QMap<QString, QString>  _names;
    QMap<QString, QString> _shortNames;

    void replaceBadCharacters(QMap<QString, QString> &names) const;
    QString replaceBadCharacters(const QString &name) const;
    QString replaceAccentCharacters(const QString &name) const;
    QMap<QString, QString> computeShortNames(const QMap<QString, QString> &names) const;

#ifdef USE_GDAL
private:
    GDALDriver                                      *m_driver;
    bool                                            m_exportRaster;

    QMap<QString, OGRFieldType>                     _ogrTypes;

    bool exportRaster(const QString &filepath);

    bool exportVector(const QString &filepath);

    bool exportItemDrawable(CT_AbstractItemDrawable *item, GDALDataset *dataset, OGRLayer *layer);

    bool exportBox2D(const CT_Box2D *box, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature);
    bool exportCircle2D(const CT_Circle2D *circle, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature);
    bool exportLine2D(const CT_Line2D *line, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature);
    bool exportPoint2D(const CT_Point2D *point, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature);
    bool exportPolygon2D(const CT_Polygon2D *polygon, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature);
    bool exportPolyline2D(const CT_Polyline2D *polyline, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature);

    bool exportOGRGeometry(OGRGeometry *geo, GDALDataset *dataset, OGRLayer *layer, OGRFeature *poFeature);
#endif
};

#endif // PB_GDALEXPORTER_H
