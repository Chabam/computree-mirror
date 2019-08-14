#include "ct_itemdrawable/tools/drawmanager/ct_standardimage2ddrawmanager.h"
#include "painterinterface.h"

#include <stdlib.h>
#include <time.h>

#include "ct_itemdrawable/ct_image2d.h"
#include "ct_tools/ct_typeinfo.h"

#include <ctime>

template< typename DataT > const QString CT_StandardImage2DDrawManager<DataT>::INDEX_CONFIG_3D_MODE_ENABLED = CT_StandardImage2DDrawManager<DataT>::staticInitConfig3DModeEnabled();
template< typename DataT > const QString CT_StandardImage2DDrawManager<DataT>::INDEX_CONFIG_3D_MODE_LINK_POINTS_ENABLED = CT_StandardImage2DDrawManager<DataT>::staticInitConfig3DModeLinkPointsEnabled();
template< typename DataT > const QString CT_StandardImage2DDrawManager<DataT>::INDEX_CONFIG_3D_MODE_HEIGHT_MAP_ENABLED = CT_StandardImage2DDrawManager<DataT>::staticInitConfig3DModeHeightMapEnabled();
template< typename DataT > const QString CT_StandardImage2DDrawManager<DataT>::INDEX_CONFIG_3D_MODE_SCALING_ENABLED = CT_StandardImage2DDrawManager<DataT>::staticInitConfig3DModeScalingEnabled();
template< typename DataT > const QString CT_StandardImage2DDrawManager<DataT>::INDEX_CONFIG_3D_MODE_ZMIN_SCALE_VALUE = CT_StandardImage2DDrawManager<DataT>::staticInitConfig3DModeZMinScaleValue();
template< typename DataT > const QString CT_StandardImage2DDrawManager<DataT>::INDEX_CONFIG_3D_MODE_ZMAX_SCALE_VALUE = CT_StandardImage2DDrawManager<DataT>::staticInitConfig3DModeZMaxScaleValue();
template< typename DataT > const QString CT_StandardImage2DDrawManager<DataT>::INDEX_CONFIG_MAP_MODE_ENABLED = CT_StandardImage2DDrawManager<DataT>::staticInitConfigMapModeEnabled();
template< typename DataT > const QString CT_StandardImage2DDrawManager<DataT>::INDEX_CONFIG_MAP_MODE_ZLEVEL_ENABLED = CT_StandardImage2DDrawManager<DataT>::staticInitConfigMapModeZLevelEnabled();
template< typename DataT > const QString CT_StandardImage2DDrawManager<DataT>::INDEX_CONFIG_MAP_MODE_ZLEVEL_VALUE = CT_StandardImage2DDrawManager<DataT>::staticInitConfigMapModeZLevelValue();
template< typename DataT > const QString CT_StandardImage2DDrawManager<DataT>::INDEX_CONFIG_MAP_MODE_SHOW_GRID = CT_StandardImage2DDrawManager<DataT>::staticInitConfigMapModeShowGrid();
template< typename DataT > const QString CT_StandardImage2DDrawManager<DataT>::INDEX_CONFIG_MAP_MODE_CLUSTER_MODE = CT_StandardImage2DDrawManager<DataT>::staticInitConfigMapModeClusterMode();

template< typename DataT >
CT_StandardImage2DDrawManager<DataT>::CT_StandardImage2DDrawManager(QString drawConfigurationName, bool mapMode, bool scale)
    : SuperClass(drawConfigurationName.isEmpty() ? CT_Image2D<DataT>::staticName() : drawConfigurationName)
{
    _defaultMapMode = mapMode;
    _defaultScaleState = scale;
}

template< typename DataT >
void CT_StandardImage2DDrawManager<DataT>::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    SuperClass::draw(view, painter, itemDrawable);

    const CT_Image2D<DataT> &item = static_cast< const CT_Image2D<DataT>& >(itemDrawable);

    bool mode3D = drawConfiguration()->variableValue(INDEX_CONFIG_3D_MODE_ENABLED).toBool();
    bool relier = drawConfiguration()->variableValue(INDEX_CONFIG_3D_MODE_LINK_POINTS_ENABLED).toBool();
    bool relief = drawConfiguration()->variableValue(INDEX_CONFIG_3D_MODE_HEIGHT_MAP_ENABLED).toBool();
    bool echelle = drawConfiguration()->variableValue(INDEX_CONFIG_3D_MODE_SCALING_ENABLED).toBool();
    double zmin = drawConfiguration()->variableValue(INDEX_CONFIG_3D_MODE_ZMIN_SCALE_VALUE).toDouble();
    double zmax = drawConfiguration()->variableValue(INDEX_CONFIG_3D_MODE_ZMAX_SCALE_VALUE).toDouble();
    bool modeMap = drawConfiguration()->variableValue(INDEX_CONFIG_MAP_MODE_ENABLED).toBool();
    bool fixerZ = drawConfiguration()->variableValue(INDEX_CONFIG_MAP_MODE_ZLEVEL_ENABLED).toBool();
    double z = drawConfiguration()->variableValue(INDEX_CONFIG_MAP_MODE_ZLEVEL_VALUE).toDouble();
    bool show_grid = drawConfiguration()->variableValue(INDEX_CONFIG_MAP_MODE_SHOW_GRID).toBool();
    bool clusterMode = drawConfiguration()->variableValue(INDEX_CONFIG_MAP_MODE_CLUSTER_MODE).toBool();

    double amplitude = item.dataMax() - item.dataMin();
    double scaling = (zmax - zmin) / amplitude;
    double demiRes = item.resolution()/2.0;

    // Create list of colors
    QMap<DataT, QColor> colors;
    if (clusterMode)
    {
        for (int cx = 0 ; cx < item.xdim()-1 ; cx++)
        {
            for (int ly = 0 ; ly < item.ydim()-1 ; ly++)
            {
                DataT value = item.value(cx, ly);
                if (!colors.contains(value))
                {
                    colors.insert(value, QColor());
                }
            }
        }

        std::srand (std::time(nullptr));
        QMutableMapIterator<DataT, QColor> it(colors);
        while (it.hasNext())
        {
            it.next();
            QColor &color = it.value();
            color.setRed(25 + std::rand() % 225);
            color.setGreen(25 + std::rand() % 225);
            color.setBlue(25 + std::rand() % 225);
        }
    }


    if (mode3D)
    {
        if ( relief )
        {
            double value;
            double x0, x1, y0, y1, z0, z1, z2, z3;
            int c0, c1, c2, c3;

            for (int cx = 0 ; cx < item.xdim()-1 ; cx++)
            {
                for (int ly = 0 ; ly < item.ydim()-1 ; ly++)
                {
                    value = double(item.value(cx, ly));

                    // Dessiner le relief
                    if (!qFuzzyCompare(value, double(item.NA())))
                    {

                        if (relief)
                        {
                            x0 = item.getCellCenterColCoord(cx);
                            y0 = item.getCellCenterLinCoord(ly);

                            x1 = item.getCellCenterColCoord(cx+1);
                            y1 = item.getCellCenterLinCoord(ly+1);

                            z0 = item.value(cx, ly);
                            z1 = item.value(cx, ly+1);
                            z2 = item.value(cx+1, ly+1);
                            z3 = item.value(cx+1, ly);

                            c0 = ((z0 - double(item.dataMin()) / amplitude)*255);
                            c1 = ((z1 - double(item.dataMin()) / amplitude)*255);
                            c2 = ((z2 - double(item.dataMin()) / amplitude)*255);
                            c3 = ((z3 - double(item.dataMin()) / amplitude)*255);

                            if (echelle)
                            {
                                z0 = (z0 - item.dataMin()) * scaling + zmin;
                                z1 = (z1 - item.dataMin()) * scaling + zmin;
                                z2 = (z2 - item.dataMin()) * scaling + zmin;
                                z3 = (z3 - item.dataMin()) * scaling + zmin;
                            }

                            painter.fillQuadFace( x0, y0, z0, c0, c0, c0,
                                                  x0, y1, z1, c1, c1, c1,
                                                  x1, y1, z2, c2, c2, c2,
                                                  x1, y0, z3, c3, c3, c3 );


                            // Si le mode relier est active on affiche aussi les contours des carrés
                            // Ca peut aider a visualiser
                            if ( relier )
                            {
                                painter.setColor(0,0,0);
                                painter.drawQuadFace( x0, y0, z0+0.001, 255, 0, 0,
                                                      x0, y1, z1+0.001, 255, 0, 0,
                                                      x1, y1, z2+0.001, 255, 0, 0,
                                                      x1, y0, z3+0.001, 255, 0, 0 );
                            }
                        }
                    }
                }
            }
        }

        else if ( relier )
        {
            double lastx = 0;
            double lasty = 0;
            double lastz = 0;

            for (int ly = 0 ; ly < item.ydim() ; ly++)
            {
                double y = item.getCellCenterLinCoord(ly);

                for (int cx = 0 ; cx < item.xdim() ; cx++)
                {
                    double value = item.value(cx, ly);
                    double x = item.getCellCenterColCoord(cx);

                    if (!qFuzzyCompare(value, double(item.NA())))
                    {
                        if (echelle)
                        {
                            value = (value - double(item.dataMin())) * scaling + zmin;
                        }

                        if (!qFuzzyCompare(lastz, double(item.NA())))
                        {
                            if (cx > 0)
                            {
                                painter.drawLine(lastx, lasty, lastz, x, y, value);
                            }
                        }
                    }

                    lasty = y;
                    lastx = x;
                    lastz = value;
                }
            }

            for (int cx = 0 ; cx < item.xdim() ; cx++)
            {
                double x = item.getCellCenterColCoord(cx);

                for (int ly = 0 ; ly < item.ydim() ; ly++)
                {
                    double value = item.value(cx, ly);
                    double y = item.getCellCenterLinCoord(ly);

                    if (!qFuzzyCompare(value, double(item.NA())))
                    {
                        if (echelle)
                        {
                            value = (value - double(item.dataMin())) * scaling + zmin;
                        }

                        if (!qFuzzyCompare(lastz, double(item.NA())))
                        {
                            if (ly > 0)
                            {
                                painter.drawLine(lastx, lasty, lastz, x, y, value);
                            }
                        }
                    }

                    lasty = y;
                    lastx = x;
                    lastz = value;
                }
            }
        }

        else
        {
            // sinon dessiner seulement les points
            double x, y, value;

            for (int ly = 0 ; ly < item.ydim()-1 ; ly++)
            {
                for (int cx = 0 ; cx < item.xdim()-1 ; cx++)
                {
                    value = item.value(cx, ly);

                    if (echelle)
                    {
                        value = (value - double(item.dataMin())) * scaling + zmin;
                    }

                    if (!qFuzzyCompare(value, double(item.NA())))
                    {
                        x = item.getCellCenterColCoord(cx);
                        y = item.getCellCenterLinCoord(ly);
                        painter.drawPoint(x, y, value);
                    }
                }
            }

        }
    }

    if (modeMap) {
        double z_val = item.level();
        if (fixerZ) {z_val = z;}

        for (int cx = 0 ; cx < item.xdim() ; cx++)
        {
            for (int ly = 0 ; ly < item.ydim() ; ly++)
            {
                double x = item.getCellCenterColCoord(cx);
                double y = item.getCellCenterLinCoord(ly);
                double value = item.value(cx, ly);

                if (qFuzzyCompare(value, double(item.NA())))
                {
                    painter.setColor(QColor(125, 0, 0));
                } else {

                    if (clusterMode)
                    {
                        painter.setColor(colors.value(value, QColor(255, 255, 255)));
                    } else {
                        int colorLevel = ((value - double(item.dataMin())) / amplitude)*255;
                        if (colorLevel < 0) {colorLevel = 0;}
                        if (colorLevel > 255) {colorLevel = 255;}
                        painter.setColor(QColor(colorLevel, colorLevel, colorLevel));
                    }
                }

                Eigen::Vector2d tLeft(x - demiRes, y + demiRes);
                Eigen::Vector2d bRight(tLeft(0)+item.resolution(), tLeft(1)-item.resolution());

                painter.fillRectXY(tLeft, bRight, z_val);

                if (show_grid)
                {
                    painter.setColor(QColor(255, 255, 255));
                    painter.drawRectXY(tLeft, bRight, z_val);
                }
            }
        }
    }
}

template< typename DataT >
CT_ItemDrawableConfiguration CT_StandardImage2DDrawManager<DataT>::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addAllConfigurationOf(SuperClass::createDrawConfiguration(drawConfigurationName));

    // Adding lines to this config dialog box
    item.addNewConfiguration(staticInitConfigMapModeEnabled(), QObject::tr("Mode Raster"), CT_ItemDrawableConfiguration::Bool, _defaultMapMode);
    item.addNewConfiguration(staticInitConfigMapModeZLevelEnabled(), QObject::tr("Mode Raster : Fixer le niveau Z"), CT_ItemDrawableConfiguration::Bool, false);
    item.addNewConfiguration(staticInitConfigMapModeZLevelValue(), QObject::tr("Mode Raster : Niveau Z (m)"), CT_ItemDrawableConfiguration::Double, 0);
    item.addNewConfiguration(staticInitConfigMapModeShowGrid(), QObject::tr("Mode Raster : Afficher grille"), CT_ItemDrawableConfiguration::Bool, false);

    item.addNewConfiguration(staticInitConfig3DModeEnabled(), QObject::tr("Mode 3D"), CT_ItemDrawableConfiguration::Bool, !_defaultMapMode);
    item.addNewConfiguration(staticInitConfig3DModeLinkPointsEnabled(), QObject::tr("Mode 3D    : Relier les centres de cases"), CT_ItemDrawableConfiguration::Bool, false);
    item.addNewConfiguration(staticInitConfig3DModeHeightMapEnabled(), QObject::tr("Mode 3D    : Visualiser le relief"), CT_ItemDrawableConfiguration::Bool, true);
    item.addNewConfiguration(staticInitConfig3DModeScalingEnabled(), QObject::tr("Mode 3D    : Mettre à l'échelle"), CT_ItemDrawableConfiguration::Bool, _defaultScaleState);
    item.addNewConfiguration(staticInitConfig3DModeZMinScaleValue(), QObject::tr("Mode 3D    : Z min de l'échelle (m)"), CT_ItemDrawableConfiguration::Double, 0);
    item.addNewConfiguration(staticInitConfig3DModeZMaxScaleValue(), QObject::tr("Mode 3D    : Z max de l'échelle (m)"), CT_ItemDrawableConfiguration::Double, 5);
    item.addNewConfiguration(staticInitConfigMapModeClusterMode(), QObject::tr("Colorisation par valeurs uniques"), CT_ItemDrawableConfiguration::Bool, false);

    return item;
}

// PROTECTED //

template< typename DataT >
QString CT_StandardImage2DDrawManager<DataT>::staticInitConfig3DModeEnabled()
{
    return "IM2D_3DME";
}

template< typename DataT >
QString CT_StandardImage2DDrawManager<DataT>::staticInitConfig3DModeLinkPointsEnabled()
{
    return "IM2D_3DMLPE";
}

template< typename DataT >
QString CT_StandardImage2DDrawManager<DataT>::staticInitConfig3DModeHeightMapEnabled()
{
    return "IM2D_3DMHME";
}

template< typename DataT >
QString CT_StandardImage2DDrawManager<DataT>::staticInitConfig3DModeScalingEnabled()
{
    return "IM2D_3DMSE";
}

template< typename DataT >
QString CT_StandardImage2DDrawManager<DataT>::staticInitConfig3DModeZMinScaleValue()
{
    return "IM2D_3DMZMINSV";
}

template< typename DataT >
QString CT_StandardImage2DDrawManager<DataT>::staticInitConfig3DModeZMaxScaleValue()
{
    return "IM2D_3DMZMAXSV";
}

template< typename DataT >
QString CT_StandardImage2DDrawManager<DataT>::staticInitConfigMapModeEnabled()
{
    return "IM2D_MME";
}

template< typename DataT >
QString CT_StandardImage2DDrawManager<DataT>::staticInitConfigMapModeZLevelEnabled()
{
    return "IM2D_MMZLE";
}

template< typename DataT >
QString CT_StandardImage2DDrawManager<DataT>::staticInitConfigMapModeZLevelValue()
{
    return "IM2D_MMZLV";
}

template< typename DataT >
QString CT_StandardImage2DDrawManager<DataT>::staticInitConfigMapModeShowGrid()
{
    return "IM2D_SHGRD";
}

template< typename DataT >
QString CT_StandardImage2DDrawManager<DataT>::staticInitConfigMapModeClusterMode()
{
    return "IM2D_COLMD";
}

