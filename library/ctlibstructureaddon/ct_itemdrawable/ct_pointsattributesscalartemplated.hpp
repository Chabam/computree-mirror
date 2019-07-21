#ifndef CT_POINTSATTRIBUTESSCALARTEMPLATED_HPP
#define CT_POINTSATTRIBUTESSCALARTEMPLATED_HPP

#include "ct_pointsattributesscalartemplated.h"

template<typename SCALAR>
CT_PointsAttributesScalarTemplated<SCALAR>::CT_PointsAttributesScalarTemplated() :
    SuperClass1(),
    SuperClass2()
{
}

template<typename SCALAR>
CT_PointsAttributesScalarTemplated<SCALAR>::CT_PointsAttributesScalarTemplated(CT_PCIR pcir,
                                                                               CT_StandardCloudStdVectorT<SCALAR>* collection) :
    SuperClass1(pcir),
    SuperClass2(collection)
{
}

template<typename SCALAR>
CT_PointsAttributesScalarTemplated<SCALAR>::CT_PointsAttributesScalarTemplated(CT_PCIR pcir,
                                                                               CT_StandardCloudStdVectorT<SCALAR>* collection,
                                                                               const SCALAR& min,
                                                                               const SCALAR& max) :
    SuperClass1(pcir),
    SuperClass2(collection, min, max)
{
}

#endif // CT_POINTSATTRIBUTESSCALARTEMPLATED_HPP
