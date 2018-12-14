#ifndef CT_EDGEATTRIBUTESSCALART_H
#define CT_EDGEATTRIBUTESSCALART_H

#include "ct_itemdrawable/abstract/ct_abstractedgeattributesscalar.h"
#include "ct_attributes/ct_attributesscalart.h"

template<typename SCALAR>
class CT_EdgeAttributesScalarT : public CT_AbstractEdgeAttributesScalar, public CT_AttributesScalarT<SCALAR>
{
    CT_TYPE_TEMPLATED_IMPL_MACRO(CT_EdgeAttributesScalarT, SCALAR, CT_AbstractEdgeAttributesScalar, Edge %1 attributes)
    using SuperClass = CT_AbstractEdgeAttributesScalar;

public:
    CT_EdgeAttributesScalarT();
    CT_EdgeAttributesScalarT(CT_ECIR pcir);
    CT_EdgeAttributesScalarT(CT_ECIR pcir, CT_StandardCloudStdVectorT<SCALAR> *collection);
    CT_EdgeAttributesScalarT(CT_ECIR pcir,
                             CT_StandardCloudStdVectorT<SCALAR> *collection,
                             const SCALAR &min,
                             const SCALAR &max);
    CT_EdgeAttributesScalarT(const CT_EdgeAttributesScalarT<SCALAR>& other) = default;

    double dMin() const override { return CT_AttributesScalarT<SCALAR>::dMin(); }
    double dMax() const override { return CT_AttributesScalarT<SCALAR>::dMax(); }

    double dValueAt(const size_t& index) const override { return CT_AttributesScalarT<SCALAR>::dValueAt(index); }

    size_t attributesSize() const override { return CT_AttributesScalarT<SCALAR>::attributesSize(); }

    CT_ITEM_COPY_IMP(CT_EdgeAttributesScalarT<SCALAR>)
};

#include "ct_itemdrawable/ct_edgeattributesscalart.hpp"

#endif // CT_EDGEATTRIBUTESSCALART_H
