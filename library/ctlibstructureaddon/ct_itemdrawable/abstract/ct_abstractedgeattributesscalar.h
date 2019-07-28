#ifndef CT_ABSTRACTEDGEATTRIBUTESSCALAR_H
#define CT_ABSTRACTEDGEATTRIBUTESSCALAR_H

#include "ct_itemdrawable/abstract/ct_abstractedgeattributes.h"

/**
 * @brief Represents a cloud attribute of scalar (int, float, double, etc...) for edge.
 */
class CTLIBSTRUCTUREADDON_EXPORT CT_AbstractEdgeAttributesScalar : public CT_AbstractEdgeAttributes
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_AbstractEdgeAttributesScalar, CT_AbstractEdgeAttributes, Edge attributes)

    using SuperClass = CT_AbstractEdgeAttributes;

public:
    CT_AbstractEdgeAttributesScalar();
    CT_AbstractEdgeAttributesScalar(CT_ECIR cir);

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
     *          - Shared pointer to edge cloud index registered
     *
     *        What is initialized differently :
     *          - Parent is set to nullptr
     *          - isSelected and isDisplayed is set to false
     *          - Document list is not copied
     */
    CT_AbstractEdgeAttributesScalar(const CT_AbstractEdgeAttributesScalar& other) = default;

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

#endif // CT_ABSTRACTEDGEATTRIBUTESSCALAR_H
