#ifndef CT_STANDARDSCANPATHMANAGER_H
#define CT_STANDARDSCANPATHMANAGER_H

#include "ct_itemdrawable/tools/drawmanager/ct_standardabstractitemdrawablewithoutpointclouddrawmanager.h"

class PLUGINSHAREDSHARED_EXPORT CT_StandardScanPathDrawManager : public CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager
{
public:
    CT_StandardScanPathDrawManager(QString drawConfigurationName = "");
    virtual ~CT_StandardScanPathDrawManager();

    virtual void draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const;

protected:

    const static QString INDEX_CONFIG_DRAW_POINTS;
    const static QString INDEX_CONFIG_DRAW_LINES;
    const static QString INDEX_CONFIG_POINT_SIZE;

    static QString staticInitConfigDrawPoints();
    static QString staticInitConfigDrawLines();
    static QString staticInitConfigPointSize();

    virtual CT_ItemDrawableConfiguration createDrawConfiguration(QString drawConfigurationName) const;
};

#endif // CT_STANDARDSCANPATHMANAGER_H
