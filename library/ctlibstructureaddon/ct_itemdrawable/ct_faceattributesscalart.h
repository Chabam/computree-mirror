#ifndef CT_FACEATTRIBUTESSCALART_H
#define CT_FACEATTRIBUTESSCALART_H

#include "ct_itemdrawable/abstract/ct_abstractfaceattributesscalar.h"
#include "ct_cloudindex/registered/abstract/ct_abstractcloudindexregisteredt.h"
#include "ct_attributes/ct_attributesscalart.h"

template<typename SCALAR>
class CT_FaceAttributesScalarT : public CT_AttributesScalarT<SCALAR, CT_AbstractFaceAttributesScalar>
{
    CT_TYPE_TEMPLATED_IMPL_MACRO(CT_FaceAttributesScalarT, SCALAR, CT_AbstractFaceAttributesScalar, Face %1 attributes)
    using SuperClass = CT_AttributesScalarT<SCALAR, CT_AbstractFaceAttributesScalar>;

public:
    CT_FaceAttributesScalarT();

    template<class ManagerT>
    CT_FaceAttributesScalarT(CT_FCIR fcir,
                             ManagerT& manager) :
        CT_FaceAttributesScalarT(fcir,
                                 manager,
                                 std::integral_constant<bool, SFINAE_And_<IsAFaceCloudManager(ManagerT), IsABaseOfCT_AbstractXAttributeManager<ManagerT, SCALAR>>::value>())
    {
    }

    template<class ManagerT>
    CT_FaceAttributesScalarT(CT_FCIR fcir,
                             ManagerT& manager,
                             const SCALAR& min,
                             const SCALAR& max) :
        CT_FaceAttributesScalarT(fcir,
                                 manager,
                                 min,
                                 max,
                                 std::integral_constant<bool, SFINAE_And_<IsAFaceCloudManager(ManagerT), IsABaseOfCT_AbstractXAttributeManager<ManagerT, SCALAR>>::value>())
    {
    }

    CT_FaceAttributesScalarT(const CT_FaceAttributesScalarT<SCALAR>& other) = default;

    CT_ITEM_COPY_IMP(CT_FaceAttributesScalarT<SCALAR>)

    private:
        template<class ManagerT>
                       CT_FaceAttributesScalarT(CT_FCIR,
                                                ManagerT&,
                                                std::false_type)
    {
        static_assert (false, "The manager you attempt to set in constructor is not a base of CT_AbstractXAttributeManager or is not applicable to face");
    }

    template<class ManagerT>
    CT_FaceAttributesScalarT(CT_FCIR fcir,
                             ManagerT& manager,
                             std::true_type) :
        SuperClass(fcir,
                   manager)
    {
    }

    template<class ManagerT>
    CT_FaceAttributesScalarT(CT_FCIR fcir,
                             ManagerT& manager,
                             const SCALAR& min,
                             const SCALAR& max,
                             std::true_type) :
        SuperClass(fcir,
                   manager,
                   min,
                   max)
    {
    }

    template<class ManagerT>
    CT_FaceAttributesScalarT(CT_FCIR,
                             ManagerT& ,
                             const SCALAR& ,
                             const SCALAR& ,
                             std::false_type)
    {
        static_assert (false, "The manager you attempt to set in constructor is not a base of CT_AbstractXAttributeManager or is not applicable to face");
    }
};

#endif // CT_FACEATTRIBUTESSCALART_H
