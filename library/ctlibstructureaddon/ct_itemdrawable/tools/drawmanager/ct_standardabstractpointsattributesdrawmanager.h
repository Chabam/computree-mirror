#ifndef CT_STANDARDABSTRACTPOINTSATTRIBUTESDRAWMANAGER_H
#define CT_STANDARDABSTRACTPOINTSATTRIBUTESDRAWMANAGER_H

#include "ctlibstructureaddon_global.h"

#include "ct_itemdrawable/tools/drawmanager/abstract/ct_abstractitemdrawabledrawmanager.h"

class CTLIBSTRUCTUREADDON_EXPORT CT_StandardAbstractPointsAttributesDrawManager : public CT_AbstractItemDrawableDrawManager
{
    using SuperClass = CT_AbstractItemDrawableDrawManager;

public:
    CT_StandardAbstractPointsAttributesDrawManager();

    void draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const override;
};

#endif // CT_STANDARDABSTRACTPOINTSATTRIBUTESDRAWMANAGER_H
