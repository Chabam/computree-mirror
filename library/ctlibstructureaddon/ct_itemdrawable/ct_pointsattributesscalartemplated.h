#ifndef CT_POINTSATTRIBUTESSCALARTEMPLATED_H
#define CT_POINTSATTRIBUTESSCALARTEMPLATED_H

#include "ct_itemdrawable/abstract/ct_abstractpointattributesscalar.h"
#include "ct_attributes/ct_attributesscalart.h"

/**
 *  Points attributes of scalar type (int, float, double, etc...)
 */
template<typename SCALAR>
class CT_PointsAttributesScalarTemplated : public CT_AbstractPointAttributesScalar, public CT_AttributesScalarT<SCALAR>
{
    CT_TYPE_TEMPLATED_IMPL_MACRO(CT_PointsAttributesScalarTemplated, SCALAR, CT_AbstractPointAttributesScalar, Point %1 attributes)

    using SuperClass1 = CT_AbstractPointAttributesScalar;
    using SuperClass2 = CT_AttributesScalarT<SCALAR>;

public:
    CT_PointsAttributesScalarTemplated();

    /**
     * @brief Create a collection of SCALAR. Attention the size of the collection passed in parameter must be the same that the size of points cloud index.
     */
    CT_PointsAttributesScalarTemplated(CT_PCIR pcir,
                                       CT_StandardCloudStdVectorT<SCALAR>* collection);

    /**
     * @brief Create a collection of SCALAR. Attention the size of the collection passed in parameter must be the same that the size of points cloud index.
     */
    CT_PointsAttributesScalarTemplated(CT_PCIR pcir,
                                       CT_StandardCloudStdVectorT<SCALAR>* collection,
                                       const SCALAR& min,
                                       const SCALAR& max);

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
     *          - Collection is copied if not NULL
     *          - Min and max value are copied
     *
     *        What is initialized differently :
     *          - Parent is set to NULL
     *          - isSelected and isDisplayed is set to false
     *          - Document list is not copied
     */
    CT_PointsAttributesScalarTemplated(const CT_PointsAttributesScalarTemplated<SCALAR>& other) = default;

    double dMin() const override { return CT_AttributesScalarT<SCALAR>::dMin(); }
    double dMax() const override { return CT_AttributesScalarT<SCALAR>::dMax(); }

    double dValueAt(const size_t &index) const override { return CT_AttributesScalarT<SCALAR>::dValueAt(index); }

    size_t attributesSize() const override { return CT_AttributesScalarT<SCALAR>::attributesSize(); }

    CT_ITEM_COPY_IMP(CT_PointsAttributesScalarTemplated<SCALAR>)
};

#include "ct_itemdrawable/ct_pointsattributesscalartemplated.hpp"

#endif // CT_POINTSATTRIBUTESSCALARTEMPLATED_H
