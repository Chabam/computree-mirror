#ifndef CT_EDGEATTRIBUTESSCALART_H
#define CT_EDGEATTRIBUTESSCALART_H

#include "ct_itemdrawable/abstract/ct_abstractedgeattributesscalar.h"
#include "ct_cloudindex/registered/abstract/ct_abstractcloudindexregisteredt.h"
#include "ct_attributes/ct_attributesscalart.h"

template<typename SCALAR>
class CT_EdgeAttributesScalarT : public CT_AttributesScalarT<SCALAR, CT_AbstractEdgeAttributesScalar>
{
    CT_TYPE_TEMPLATED_IMPL_MACRO(CT_EdgeAttributesScalarT, SCALAR, CT_AbstractEdgeAttributesScalar, Edge %1 attributes)

    using SuperClass = CT_AttributesScalarT<SCALAR, CT_AbstractEdgeAttributesScalar>;

public:
    CT_EdgeAttributesScalarT() = default;

    template<class ManagerT>
    CT_EdgeAttributesScalarT(CT_ECIR ecir,
                             ManagerT& manager) :
        CT_EdgeAttributesScalarT(ecir,
                                 manager,
                                 std::integral_constant<bool, SFINAE_And_<IsAEdgeCloudManager(ManagerT), IsABaseOfCT_AbstractXAttributeManager<ManagerT, SCALAR>>::value>())
    {
    }

    template<class ManagerT>
    CT_EdgeAttributesScalarT(CT_ECIR ecir,
                             ManagerT& manager,
                             const SCALAR& min,
                             const SCALAR& max) :
        CT_EdgeAttributesScalarT(ecir,
                                 manager,
                                 min,
                                 max,
                                 std::integral_constant<bool, SFINAE_And_<IsAEdgeCloudManager(ManagerT), IsABaseOfCT_AbstractXAttributeManager<ManagerT, SCALAR>>::value>())
    {
    }

    CT_EdgeAttributesScalarT(const CT_EdgeAttributesScalarT<SCALAR>& other) = default;

    CT_ITEM_COPY_IMP(CT_EdgeAttributesScalarT<SCALAR>)

private:
    template<class ManagerT>
    CT_EdgeAttributesScalarT(CT_ECIR,
                             ManagerT&,
                             std::false_type)
    {
        static_assert (false, "The manager you attempt to set in constructor is not a base of CT_AbstractXAttributeManager or is not applicable to edge");
    }

    template<class ManagerT>
    CT_EdgeAttributesScalarT(CT_ECIR ecir,
                             ManagerT& manager,
                             std::true_type) :
        SuperClass(ecir,
                   manager)
    {
    }

    template<class ManagerT>
    CT_EdgeAttributesScalarT(CT_ECIR ecir,
                             ManagerT& manager,
                             const SCALAR& min,
                             const SCALAR& max,
                             std::true_type) :
        SuperClass(ecir,
                   manager,
                   min,
                   max)
    {
    }

    template<class ManagerT>
    CT_EdgeAttributesScalarT(CT_ECIR,
                             ManagerT& ,
                             const SCALAR& ,
                             const SCALAR& ,
                             std::false_type)
    {
        static_assert (false, "The manager you attempt to set in constructor is not a base of CT_AbstractXAttributeManager or is not applicable to edge");
    }
};

#endif // CT_EDGEATTRIBUTESSCALART_H
