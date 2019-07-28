#ifndef CT_ABSTRACTATTRIBUTES_H
#define CT_ABSTRACTATTRIBUTES_H

#include "ctlibstructureaddon_global.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

/**
 * @brief Represents a cloud attribute. This is a singular item a bit special since
 *        it can also be used to define colors for a cloud of points or normals in a view of a document.
 */
class CTLIBSTRUCTUREADDON_EXPORT CT_AbstractAttributes : public CT_AbstractSingularItemDrawable
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_AbstractAttributes, CT_AbstractSingularItemDrawable, Attributes)

    using SuperClass = CT_AbstractSingularItemDrawable;

public:
    CT_AbstractAttributes();

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
     *
     *        What is initialized differently :
     *          - Parent is set to nullptr
     *          - isSelected and isDisplayed is set to false
     *          - Document list is not copied
     */
    CT_AbstractAttributes(const CT_AbstractAttributes& other) = default;

    /**
     * @brief Returns the size of the cloud of attributes
     */
    virtual size_t attributesSize() const = 0;

};

#endif // CT_ABSTRACTATTRIBUTES_H
