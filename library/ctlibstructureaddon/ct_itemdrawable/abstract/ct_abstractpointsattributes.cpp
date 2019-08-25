#include "ct_abstractpointsattributes.h"

#include "ct_cloudindex/registered/abstract/ct_abstractcloudindexregisteredt.h"

CT_StandardAbstractPointsAttributesDrawManager CT_AbstractPointsAttributes::APA_DRAW_MANAGER;

CT_AbstractPointsAttributes::CT_AbstractPointsAttributes() : SuperClass()
{
    m_apci = nullptr;
    setBaseDrawManager(&APA_DRAW_MANAGER);
}

CT_AbstractPointsAttributes::CT_AbstractPointsAttributes(CT_PCIR pcir) : CT_AbstractPointsAttributes()
{
    m_pcir = pcir;
    m_apci =(m_pcir.data() == nullptr ? nullptr : m_pcir->abstractCloudIndexT());
}

const CT_AbstractPointCloudIndex* CT_AbstractPointsAttributes::pointCloudIndex() const
{
    return m_apci;
}

void CT_AbstractPointsAttributes::setPointCloudIndexRegistered(CT_PCIR pcir)
{
    m_pcir = pcir;
    m_apci =(m_pcir.data() == NULL ? NULL : m_pcir->abstractCloudIndexT());
}

CT_PCIR CT_AbstractPointsAttributes::pointCloudIndexRegistered() const
{
    return m_pcir;
}
