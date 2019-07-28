#ifndef CT_ABSTRACTPOINTATTRIBUTESSCALAR_H
#define CT_ABSTRACTPOINTATTRIBUTESSCALAR_H

#include "ct_itemdrawable/abstract/ct_abstractpointsattributes.h"

/**
 * @brief Represents a cloud attribute of scalar (int, float, double, etc...) for points.
 */
class CTLIBSTRUCTUREADDON_EXPORT CT_AbstractPointAttributesScalar : public CT_AbstractPointsAttributes
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_AbstractPointAttributesScalar, CT_AbstractPointsAttributes, Point attributes)

    using SuperClass = CT_AbstractPointsAttributes;

public:
    CT_AbstractPointAttributesScalar();
    CT_AbstractPointAttributesScalar(CT_PCIR pcir);

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
     *
     *        What is initialized differently :
     *          - Parent is set to nullptr
     *          - isSelected and isDisplayed is set to false
     *          - Document list is not copied
     */
    CT_AbstractPointAttributesScalar(const CT_AbstractPointAttributesScalar& other) = default;

    /**
     * @brief Returns the minimum value
     */
    virtual double dMin() const = 0;

    /**
     * @brief Returns the maximum value
     */
    virtual double dMax() const = 0;

    /**
     * @brief Returns the value at the specified index
     */
    virtual double dValueAt(const size_t &index) const = 0;
};

#endif // CT_ABSTRACTPOINTATTRIBUTESSCALAR_H
