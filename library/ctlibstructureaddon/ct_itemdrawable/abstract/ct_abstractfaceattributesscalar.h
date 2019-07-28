#ifndef CT_ABSTRACTFACEATTRIBUTESSCALAR_H
#define CT_ABSTRACTFACEATTRIBUTESSCALAR_H

#include "ct_itemdrawable/abstract/ct_abstractfaceattributes.h"

/**
 * @brief Represents a cloud attribute of scalar (int, float, double, etc...) for face.
 */
class CTLIBSTRUCTUREADDON_EXPORT CT_AbstractFaceAttributesScalar : public CT_AbstractFaceAttributes
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_AbstractFaceAttributesScalar, CT_AbstractFaceAttributes, Face attributes)

    using SuperClass = CT_AbstractFaceAttributes;

public:
    CT_AbstractFaceAttributesScalar();
    CT_AbstractFaceAttributesScalar(CT_FCIR cir);

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
     *          - Shared pointer to face cloud index registered
     *
     *        What is initialized differently :
     *          - Parent is set to nullptr
     *          - isSelected and isDisplayed is set to false
     *          - Document list is not copied
     */
    CT_AbstractFaceAttributesScalar(const CT_AbstractFaceAttributesScalar& other) = default;

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

#endif // CT_ABSTRACTFACEATTRIBUTESSCALAR_H
