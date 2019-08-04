#ifndef CT_FACEATTRIBUTESCOLOR_H
#define CT_FACEATTRIBUTESCOLOR_H

#include "ct_itemdrawable/abstract/ct_abstractfaceattributes.h"
#include "ct_attributes/ct_attributescolor.h"
#include "ct_colorcloud/ct_colorcloudstdvector.h"


class CTLIBSTRUCTUREADDON_EXPORT CT_FaceAttributesColor : public CT_AbstractFaceAttributes, public CT_AttributesColor
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_FaceAttributesColor, CT_AbstractFaceAttributes, Face color attributes)
    using SuperClass = CT_AbstractFaceAttributes;

public:
    CT_FaceAttributesColor();
    CT_FaceAttributesColor(CT_FCIR pcir);
    CT_FaceAttributesColor(CT_FCIR pcir,
                           CT_AbstractColorCloud *cc);
    CT_FaceAttributesColor(const CT_FaceAttributesColor& other) = default;

    size_t attributesSize() const override { return CT_AttributesColor::attributesSize(); }

    CT_ITEM_COPY_IMP(CT_FaceAttributesColor)

private:
    CT_DEFAULT_IA_BEGIN(CT_FaceAttributesColor)
    CT_DEFAULT_IA_V2(CT_FaceAttributesColor, CT_AbstractCategory::staticInitDataSize(), &CT_FaceAttributesColor::attributesSize, QObject::tr("Taille"))
    CT_DEFAULT_IA_END(CT_FaceAttributesColor)
};

#endif // CT_FACEATTRIBUTESCOLOR_H
