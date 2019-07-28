#include "ct_abstractfaceattributes.h"

#include "ct_cloudindex/abstract/ct_abstractcloudindext.h"

CT_AbstractFaceAttributes::CT_AbstractFaceAttributes() : SuperClass()
{
    m_aci = nullptr;
}

CT_AbstractFaceAttributes::CT_AbstractFaceAttributes(CT_FCIR cir) : CT_AbstractFaceAttributes()
{
    m_cir = cir;
    m_aci =(m_cir.data() == nullptr ? nullptr : cir->abstractCloudIndexT());
}

const CT_AbstractFaceCloudIndex* CT_AbstractFaceAttributes::faceCloudIndex() const
{
    return m_aci;
}

CT_FCIR CT_AbstractFaceAttributes::faceCloudIndexRegistered() const
{
    return m_cir;
}
