#include "ct_attributesnormal.h"

CT_AttributesNormal::CT_AttributesNormal(CT_AbstractNormalCloud* cloud,
                                         bool autoDeleteNormalCloud)
{
    m_normalCloud = cloud;
    mAutoDeleteNormalCloud = autoDeleteNormalCloud;
}

CT_AttributesNormal::CT_AttributesNormal(const CT_AttributesNormal& other) :
    m_normalCloud(other.mAutoDeleteNormalCloud ? ((other.m_normalCloud != nullptr) ? dynamic_cast<CT_AbstractNormalCloud*>(other.m_normalCloud->copy()) : nullptr) : other.m_normalCloud),
    mAutoDeleteNormalCloud(other.mAutoDeleteNormalCloud)
{
}

CT_AttributesNormal::~CT_AttributesNormal()
{
    if(mAutoDeleteNormalCloud)
        delete m_normalCloud;
}

const CT_Normal& CT_AttributesNormal::constNormalAt(const size_t& index) const
{
    return m_normalCloud->constNormalAt(index);
}

void CT_AttributesNormal::setNormalCloud(CT_AbstractNormalCloud* cloud,
                                         bool autoDeleteNormalCloud)
{
    if(mAutoDeleteNormalCloud)
        delete m_normalCloud;

    mAutoDeleteNormalCloud = autoDeleteNormalCloud;
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
