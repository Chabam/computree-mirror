#ifndef CT_POINTSATTRIBUTESSCALARTEMPLATED_HPP
#define CT_POINTSATTRIBUTESSCALARTEMPLATED_HPP

#include "ct_pointsattributesscalartemplated.h"

template<typename SCALAR>
CT_PointsAttributesScalarTemplated<SCALAR>::CT_PointsAttributesScalarTemplated() :
    CT_AbstractPointAttributesScalar(),
    CT_AttributesScalarT<SCALAR>()
{
}

template<typename SCALAR>
CT_PointsAttributesScalarTemplated<SCALAR>::CT_PointsAttributesScalarTemplated(CT_PCIR pcir,
                                                                               CT_StandardCloudStdVectorT<SCALAR>* collection) :
    CT_AbstractPointAttributesScalar(pcir),
    CT_AttributesScalarT<SCALAR>(collection)
{
}

template<typename SCALAR>
CT_PointsAttributesScalarTemplated<SCALAR>::CT_PointsAttributesScalarTemplated(CT_PCIR pcir,
                                                                               CT_StandardCloudStdVectorT<SCALAR>* collection,
                                                                               const SCALAR& min,
                                                                               const SCALAR& max) :
    CT_AbstractPointAttributesScalar(pcir),
    CT_AttributesScalarT<SCALAR>(collection, min, max)
{
}

#endif // CT_POINTSATTRIBUTESSCALARTEMPLATED_HPP
