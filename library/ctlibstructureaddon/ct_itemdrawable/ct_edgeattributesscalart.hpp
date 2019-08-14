#include "ct_itemdrawable/ct_edgeattributesscalart.h"

template<typename SCALAR>
CT_EdgeAttributesScalarT<SCALAR>::CT_EdgeAttributesScalarT() : SuperClass(), CT_AttributesScalarT<SCALAR>()
{
}

template<typename SCALAR>
CT_EdgeAttributesScalarT<SCALAR>::CT_EdgeAttributesScalarT(CT_ECIR pcir) : SuperClass(pcir),
    CT_AttributesScalarT<SCALAR>(pcir.data() != nullptr ? pcir->size() : 0)
{
}

template<typename SCALAR>
CT_EdgeAttributesScalarT<SCALAR>::CT_EdgeAttributesScalarT(CT_ECIR pcir,
                                                           CT_StandardCloudStdVectorT<SCALAR> *collection) : SuperClass(pcir),
    CT_AttributesScalarT<SCALAR>(collection)
{
}

template<typename SCALAR>
CT_EdgeAttributesScalarT<SCALAR>::CT_EdgeAttributesScalarT(CT_ECIR pcir,
                                                           CT_StandardCloudStdVectorT<SCALAR> *collection,
                                                           const SCALAR &min,
                                                           const SCALAR &max) : SuperClass(pcir),
    CT_AttributesScalarT<SCALAR>(collection, min, max)
{
}

