#ifndef CT_EDGEATTRIBUTESNORMAL_H
#define CT_EDGEATTRIBUTESNORMAL_H

#include "ct_itemdrawable/abstract/ct_abstractedgeattributes.h"
#include "ct_attributes/ct_attributesnormal.h"

class CTLIBSTRUCTUREADDON_EXPORT CT_EdgeAttributesNormal : public CT_AbstractEdgeAttributes, public CT_AttributesNormal
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_EdgeAttributesNormal, CT_AbstractEdgeAttributes, Edge normal attributes)

    using SuperClass = CT_AbstractEdgeAttributes;

public:
    CT_EdgeAttributesNormal();
    CT_EdgeAttributesNormal(CT_ECIR pcir);
    CT_EdgeAttributesNormal(CT_ECIR pcir, CT_AbstractNormalCloud *nc);
    CT_EdgeAttributesNormal(const CT_EdgeAttributesNormal& other) = default;

    size_t attributesSize() const override { return CT_AttributesNormal::attributesSize(); }

    CT_ITEM_COPY_IMP(CT_EdgeAttributesNormal)
};

#endif // CT_EDGEATTRIBUTESNORMAL_H
