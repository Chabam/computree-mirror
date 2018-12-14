#ifndef CT_FACEATTRIBUTESNORMAL_H
#define CT_FACEATTRIBUTESNORMAL_H

#include "ct_itemdrawable/abstract/ct_abstractfaceattributes.h"
#include "ct_attributes/ct_attributesnormal.h"

class CTLIBSTRUCTUREADDON_EXPORT CT_FaceAttributesNormal : public CT_AbstractFaceAttributes, public CT_AttributesNormal
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_FaceAttributesNormal, CT_AbstractFaceAttributes, Face normal attributes)
    using SuperClass = CT_AbstractFaceAttributes;

public:
    CT_FaceAttributesNormal();
    CT_FaceAttributesNormal(CT_FCIR pcir);
    CT_FaceAttributesNormal(CT_FCIR pcir,
                            CT_AbstractNormalCloud *nc);
    CT_FaceAttributesNormal(const CT_FaceAttributesNormal& other) = default;

    size_t attributesSize() const override { return CT_AttributesNormal::attributesSize(); }

    CT_ITEM_COPY_IMP(CT_FaceAttributesNormal)
};

#endif // CT_FACEATTRIBUTESNORMAL_H
