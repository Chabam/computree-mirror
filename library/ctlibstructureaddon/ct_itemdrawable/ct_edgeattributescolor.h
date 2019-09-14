#ifndef CT_EDGEATTRIBUTESCOLOR_H
#define CT_EDGEATTRIBUTESCOLOR_H

#include "ct_itemdrawable/abstract/ct_abstractedgeattributes.h"
#include "ct_attributes/ct_attributescolor.h"

class CTLIBSTRUCTUREADDON_EXPORT CT_EdgeAttributesColor : public CT_AbstractEdgeAttributes, public CT_AttributesColor
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_EdgeAttributesColor, CT_AbstractEdgeAttributes, Edge color attributes)

    using SuperClass = CT_AbstractEdgeAttributes;

public:
    CT_EdgeAttributesColor();
    CT_EdgeAttributesColor(CT_ECIR pcir);
    CT_EdgeAttributesColor(CT_ECIR pcir, CT_AbstractColorCloud *cc, bool autoDeleteColorCloud = true);
    CT_EdgeAttributesColor(const CT_EdgeAttributesColor& other) = default;

    size_t attributesSize() const override { return CT_AttributesColor::attributesSize(); }

    CT_ITEM_COPY_IMP(CT_EdgeAttributesColor)
};

#endif // CT_EDGEATTRIBUTESCOLOR_H
