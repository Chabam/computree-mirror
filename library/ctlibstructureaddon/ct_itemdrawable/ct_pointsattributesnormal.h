#ifndef CT_POINTSATTRIBUTESNORMAL_H
#define CT_POINTSATTRIBUTESNORMAL_H

#include "ct_itemdrawable/tools/drawmanager/ct_standardpointsattributesnormaldrawmanager.h"
#include "ct_itemdrawable/abstract/ct_abstractpointsattributes.h"
#include "ct_attributes/ct_attributesnormal.h"

class CTLIBSTRUCTUREADDON_EXPORT CT_PointsAttributesNormal : public CT_AbstractPointsAttributes, public CT_AttributesNormal
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_PointsAttributesNormal, CT_AbstractPointsAttributes, Normal point attributes)

    using SuperClass1 = CT_AbstractPointsAttributes;
    using SuperClass2 = CT_AttributesNormal;

public:
    CT_PointsAttributesNormal();

    CT_PointsAttributesNormal(CT_PCIR pcir);

    CT_PointsAttributesNormal(CT_PCIR pcir,
                              CT_AbstractNormalCloud* nc);

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
     *          - Normal cloud is copied (call method "copy()") if not nullptr
     *
     *        What is initialized differently :
     *          - Parent is set to nullptr
     *          - isSelected and isDisplayed is set to false
     *          - Document list is not copied
     */
    CT_PointsAttributesNormal(const CT_PointsAttributesNormal& other) = default;

    size_t attributesSize() const override { return CT_AttributesNormal::attributesSize(); }

    CT_ITEM_COPY_IMP(CT_PointsAttributesNormal)

private:
    static CT_StandardPointsAttributesNormalDrawManager PAN_DRAW_MANAGER;
};

#endif // CT_POINTSATTRIBUTESNORMAL_H
