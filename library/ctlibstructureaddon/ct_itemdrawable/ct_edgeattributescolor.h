#ifndef CT_EDGEATTRIBUTESCOLOR_H
#define CT_EDGEATTRIBUTESCOLOR_H

#include "ct_itemdrawable/abstract/ct_abstractedgeattributes.h"
#include "ct_attributes/ct_attributescolor.h"

class CTLIBSTRUCTUREADDON_EXPORT CT_EdgeAttributesColor : public CT_AttributesColor<CT_AbstractEdgeAttributes>
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_EdgeAttributesColor, CT_AbstractEdgeAttributes, Edge color attributes)

    using SuperClass = CT_AttributesColor<CT_AbstractEdgeAttributes>;

public:
    CT_EdgeAttributesColor() = default;

    template<class ManagerT>
    CT_EdgeAttributesColor(CT_ECIR ecir,
                           ManagerT& manager) :
        CT_EdgeAttributesColor(ecir,
                               manager,
                               std::integral_constant<bool, SFINAE_And_<IsAEdgeCloudManager(ManagerT), IsABaseOfCT_AbstractXAttributeManager<ManagerT, CT_Color>>::value>())
    {
    }

    CT_EdgeAttributesColor(const CT_EdgeAttributesColor& other) = default;

    size_t numberOfSetValues() const final { return SuperClass::numberOfSetValues(); }

    CT_ITEM_COPY_IMP(CT_EdgeAttributesColor)

private:
    CT_DEFAULT_IA_BEGIN(CT_EdgeAttributesColor)
    CT_DEFAULT_IA_V2(CT_EdgeAttributesColor, CT_AbstractCategory::staticInitDataSize(), &CT_EdgeAttributesColor::numberOfSetValues, QObject::tr("Taille"))
    CT_DEFAULT_IA_END(CT_EdgeAttributesColor)

    template<class ManagerT>
    CT_EdgeAttributesColor(CT_ECIR,
                           ManagerT&,
                           std::false_type)
    {
        static_assert (false, "The manager you attempt to set in constructor is not a base of CT_AbstractXAttributeManager or is not applicable to edge");
    }

    template<class ManagerT>
    CT_EdgeAttributesColor(CT_ECIR ecir,
                           ManagerT& manager,
                           std::true_type) :
        SuperClass(ecir, manager)
    {
    }
};

#endif // CT_EDGEATTRIBUTESCOLOR_H
