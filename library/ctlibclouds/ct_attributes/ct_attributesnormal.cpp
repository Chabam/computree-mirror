#include "ct_attributesnormal.h"

CT_AttributesNormal::CT_AttributesNormal(CT_AbstractNormalCloud* cloud)
{
    m_normalCloud = cloud;
}

CT_AttributesNormal::CT_AttributesNormal(const CT_AttributesNormal& other) :
    m_normalCloud((other.m_normalCloud != nullptr) ? dynamic_cast<CT_AbstractNormalCloud*>(other.m_normalCloud->copy()) : nullptr)
{
}

CT_AttributesNormal::~CT_AttributesNormal()
{
    delete m_normalCloud;
}

const CT_Normal& CT_AttributesNormal::constNormalAt(const size_t& index) const
{
    return m_normalCloud->constNormalAt(index);
}

void CT_AttributesNormal::setNormalCloud(CT_AbstractNormalCloud* cloud)
{
    delete m_normalCloud;
    m_normalCloud = cloud;
}

CT_AbstractNormalCloud* CT_AttributesNormal::normalCloud() const
{
    return m_normalCloud;
}

size_t CT_AttributesNormal::attributesSize() const
{
    return m_normalCloud->size();
}
