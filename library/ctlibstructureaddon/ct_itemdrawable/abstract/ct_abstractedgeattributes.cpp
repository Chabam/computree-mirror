#include "ct_abstractedgeattributes.h"

CT_AbstractEdgeAttributes::CT_AbstractEdgeAttributes() : SuperClass()
{
    m_aci = NULL;
}

CT_AbstractEdgeAttributes::CT_AbstractEdgeAttributes(CT_ECIR cir) : CT_AbstractEdgeAttributes()
{
    m_cir = cir;
    m_aci =(m_cir.data() == NULL ? NULL : cir->abstractCloudIndexT());
}

const CT_AbstractEdgeCloudIndex* CT_AbstractEdgeAttributes::edgeCloudIndex() const
{
    return m_aci;
}

CT_ECIR CT_AbstractEdgeAttributes::edgeCloudIndexRegistered() const
{
    return m_cir;
}
