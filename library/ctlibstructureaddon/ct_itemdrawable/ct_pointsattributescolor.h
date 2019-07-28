#ifndef CT_POINTSATTRIBUTESCOLOR_H
#define CT_POINTSATTRIBUTESCOLOR_H

#include "ct_itemdrawable/abstract/ct_abstractpointsattributes.h"
#include "ct_itemdrawable/tools/drawmanager/ct_standardpointsattributescolordrawmanager.h"
#include "ct_attributes/ct_attributescolor.h"

class CTLIBSTRUCTUREADDON_EXPORT CT_PointsAttributesColor : public CT_AbstractPointsAttributes, public CT_AttributesColor
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_PointsAttributesColor, CT_AbstractPointsAttributes, Point color attributes)

    using SuperClass1 = CT_AbstractPointsAttributes;
    using SuperClass2 = CT_AttributesColor;

public:
    CT_PointsAttributesColor();

    CT_PointsAttributesColor(CT_PCIR pcir);

    CT_PointsAttributesColor(CT_PCIR pcir,
                             CT_AbstractColorCloud* cc);

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

    size_t attributesSize() const override { return CT_AttributesColor::attributesSize(); }

    CT_ITEM_COPY_IMP(CT_PointsAttributesColor)

private:
    static CT_StandardPointsAttributesColorDrawManager PAC_DRAW_MANAGER;
};

#endif // CT_POINTSATTRIBUTESCOLOR_H
