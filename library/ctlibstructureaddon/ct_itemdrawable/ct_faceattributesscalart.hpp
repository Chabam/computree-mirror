#ifndef CT_FACEATTRIBUTESSCALART_HPP
#define CT_FACEATTRIBUTESSCALART_HPP

#include "ct_itemdrawable/ct_faceattributesscalart.h"

template<typename SCALAR>
CT_FaceAttributesScalarT<SCALAR>::CT_FaceAttributesScalarT() : SuperClass(),
    CT_AttributesScalarT<SCALAR>()
{
}

template<typename SCALAR>
CT_FaceAttributesScalarT<SCALAR>::CT_FaceAttributesScalarT(CT_FCIR pcir) : SuperClass(pcir),
    CT_AttributesScalarT<SCALAR>(pcir.data() != nullptr ? pcir->size() : 0)
{
}

template<typename SCALAR>
CT_FaceAttributesScalarT<SCALAR>::CT_FaceAttributesScalarT(CT_FCIR pcir,
                                                           CT_StandardCloudStdVectorT<SCALAR> *collection) : SuperClass(pcir),
    CT_AttributesScalarT<SCALAR>(collection)
{
}

template<typename SCALAR>
CT_FaceAttributesScalarT<SCALAR>::CT_FaceAttributesScalarT(CT_FCIR pcir,
                                                           CT_StandardCloudStdVectorT<SCALAR> *collection,
                                                           const SCALAR &min,
                                                           const SCALAR &max) : SuperClass(pcir),
    CT_AttributesScalarT<SCALAR>(collection, min, max)
{
}

#endif // CT_FACEATTRIBUTESSCALART_HPP
