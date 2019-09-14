#include "ct_abstractedgeattributes.h"

CT_TYPE_IMPL_INIT_MACRO(CT_AbstractEdgeAttributes)

CT_AbstractEdgeAttributes::CT_AbstractEdgeAttributes() : SuperClass()
{
    m_aci = nullptr;
}

CT_AbstractEdgeAttributes::CT_AbstractEdgeAttributes(CT_ECIR cir) : CT_AbstractEdgeAttributes()
{
    m_cir = cir;
    m_aci =(m_cir.data() == nullptr ? nullptr : cir->abstractCloudIndexT());
}

const CT_AbstractEdgeCloudIndex* CT_AbstractEdgeAttributes::edgeCloudIndex() const
{
    return m_aci;
}

CT_ECIR CT_AbstractEdgeAttributes::edgeCloudIndexRegistered() const
{
    return m_cir;
}
