#ifndef CT_FACEATTRIBUTESSCALART_H
#define CT_FACEATTRIBUTESSCALART_H

#include "ct_itemdrawable/abstract/ct_abstractfaceattributesscalar.h"
#include "ct_attributes/ct_attributesscalart.h"

template<typename SCALAR>
class CT_FaceAttributesScalarT : public CT_AbstractFaceAttributesScalar, public CT_AttributesScalarT<SCALAR>
{
    CT_TYPE_TEMPLATED_IMPL_MACRO(CT_FaceAttributesScalarT, SCALAR, CT_AbstractFaceAttributesScalar, Face %1 attributes)
    using SuperClass = CT_AbstractFaceAttributesScalar;

public:
    CT_FaceAttributesScalarT();
    CT_FaceAttributesScalarT(CT_FCIR pcir);
    CT_FaceAttributesScalarT(CT_FCIR pcir,
                             CT_StandardCloudStdVectorT<SCALAR> *collection);
    CT_FaceAttributesScalarT(CT_FCIR pcir,
                             CT_StandardCloudStdVectorT<SCALAR> *collection,
                             const SCALAR &min,
                             const SCALAR &max);
    CT_FaceAttributesScalarT(const CT_FaceAttributesScalarT<SCALAR>& other) = default;


    double dMin() const override { return CT_AttributesScalarT<SCALAR>::dMin(); }
    double dMax() const override { return CT_AttributesScalarT<SCALAR>::dMax(); }

    double dValueAt(const size_t &index) const override { return CT_AttributesScalarT<SCALAR>::dValueAt(index); }

    size_t attributesSize() const override { return CT_AttributesScalarT<SCALAR>::attributesSize(); }

    CT_ITEM_COPY_IMP(CT_FaceAttributesScalarT<SCALAR>)
};

#include "ct_itemdrawable/ct_faceattributesscalart.hpp"

#endif // CT_FACEATTRIBUTESSCALART_H
