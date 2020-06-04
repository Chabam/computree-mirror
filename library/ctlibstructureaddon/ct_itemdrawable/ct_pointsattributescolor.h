#ifndef CT_POINTSATTRIBUTESCOLOR_H
#define CT_POINTSATTRIBUTESCOLOR_H

#include "ct_itemdrawable/abstract/ct_abstractpointsattributes.h"
#include "ct_attributes/ct_attributescolor.h"

class CTLIBSTRUCTUREADDON_EXPORT CT_PointsAttributesColor : public CT_AttributesColor<CT_AbstractPointsAttributes>
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_PointsAttributesColor, CT_AbstractPointsAttributes, Point color attributes)

    using SuperClass = CT_AttributesColor<CT_AbstractPointsAttributes>;

public:
    CT_PointsAttributesColor() = default;

    template<class ManagerT>
    CT_PointsAttributesColor(CT_PCIR pcir,
                             ManagerT& manager) :
        CT_PointsAttributesColor(pcir,
                                 manager,
                                 std::integral_constant<bool, SFINAE_And_<IsAPointCloudManager(ManagerT), IsABaseOfCT_AbstractXAttributeManager<ManagerT, CT_Color>>::value>())
    {
    }

    /**
     * @brief Copy constructor.
     *
     *        What is copied :
     *          - Pointer of the result and model of the original item.
     *          - Unique ID
     *          - Pointer of base and alternative draw manager
     *          - Displayable name
     *          - Center coordinates
     *          - Default Color
     *          - Shared pointer to point cloud index registered
     *          - Color cloud is copied (call method "copy()") if not nullptr
     *
     *        What is initialized differently :
     *          - Parent is set to nullptr
     *          - isSelected and isDisplayed is set to false
     *          - Document list is not copied
     */
    CT_PointsAttributesColor(const CT_PointsAttributesColor& other) = default;

    size_t numberOfSetValues() const final { return SuperClass::numberOfSetValues(); }

    CT_ITEM_COPY_IMP(CT_PointsAttributesColor)

private:
    CT_DEFAULT_IA_BEGIN(CT_PointsAttributesColor)
    CT_DEFAULT_IA_V2(CT_PointsAttributesColor, CT_AbstractCategory::staticInitDataSize(), &CT_PointsAttributesColor::numberOfSetValues, QObject::tr("Taille"))
    CT_DEFAULT_IA_END(CT_PointsAttributesColor)

    template<class ManagerT>
    CT_PointsAttributesColor(CT_PCIR ,
                             ManagerT&,
                             std::false_type)
    {
        static_assert (false, "The manager you attempt to set in constructor is not a base of CT_AbstractXAttributeManager or is not applicable to point");
    }

    template<class ManagerT>
    CT_PointsAttributesColor(CT_PCIR pcir,
                             ManagerT& manager,
                             std::true_type) :
        SuperClass(pcir, manager)
    {
    }
};

#endif // CT_POINTSATTRIBUTESCOLOR_H
