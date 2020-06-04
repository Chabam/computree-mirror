#ifndef CT_FACEATTRIBUTESCOLOR_H
#define CT_FACEATTRIBUTESCOLOR_H

#include "ct_itemdrawable/abstract/ct_abstractfaceattributes.h"
#include "ct_attributes/ct_attributescolor.h"

class CTLIBSTRUCTUREADDON_EXPORT CT_FaceAttributesColor : public CT_AttributesColor<CT_AbstractFaceAttributes>
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_FaceAttributesColor, CT_AbstractFaceAttributes, Face color attributes)
    using SuperClass = CT_AttributesColor<CT_AbstractFaceAttributes>;

public:
    CT_FaceAttributesColor() = default;

    template<class ManagerT>
    CT_FaceAttributesColor(CT_FCIR fcir,
                           ManagerT& manager) :
        CT_FaceAttributesColor(fcir,
                               manager,
                               std::integral_constant<bool, SFINAE_And_<IsAFaceCloudManager(ManagerT), IsABaseOfCT_AbstractXAttributeManager<ManagerT, CT_Color>>::value>())
    {
    }

    CT_FaceAttributesColor(const CT_FaceAttributesColor& other) = default;

    size_t numberOfSetValues() const final { return SuperClass::numberOfSetValues(); }

    CT_ITEM_COPY_IMP(CT_FaceAttributesColor)

private:
    CT_DEFAULT_IA_BEGIN(CT_FaceAttributesColor)
    CT_DEFAULT_IA_V2(CT_FaceAttributesColor, CT_AbstractCategory::staticInitDataSize(), &CT_FaceAttributesColor::numberOfSetValues, QObject::tr("Taille"))
    CT_DEFAULT_IA_END(CT_FaceAttributesColor)

    template<class ManagerT>
    CT_FaceAttributesColor(CT_FCIR,
                           ManagerT&,
                           std::false_type)
    {
        static_assert (false, "The manager you attempt to set in constructor is not a base of CT_AbstractXAttributeManager or is not applicable to face");
    }

    template<class ManagerT>
    CT_FaceAttributesColor(CT_FCIR fcir,
                           ManagerT& manager,
                           std::true_type) :
        SuperClass(fcir, manager)
    {
    }
};

#endif // CT_FACEATTRIBUTESCOLOR_H
