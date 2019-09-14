#include "ct_attributescolor.h"

CT_AttributesColor::CT_AttributesColor(CT_AbstractColorCloud* cloud, bool autoDeleteColorCloud)
{
    m_colorCloud = cloud;
    mAutoDeleteColorCloud = autoDeleteColorCloud;
}

CT_AttributesColor::CT_AttributesColor(const CT_AttributesColor& other) :
    m_colorCloud(other.mAutoDeleteColorCloud ? ((other.m_colorCloud != nullptr) ? dynamic_cast<CT_AbstractColorCloud*>(other.m_colorCloud->copy()) : nullptr) : other.m_colorCloud),
    mAutoDeleteColorCloud(other.mAutoDeleteColorCloud)
{
}

CT_AttributesColor::~CT_AttributesColor()
{
    if(mAutoDeleteColorCloud)
        delete m_colorCloud;
}

const CT_Color& CT_AttributesColor::constColorAt(const size_t& index) const
{
    return m_colorCloud->colorAt(index);
}

CT_AbstractColorCloud* CT_AttributesColor::colorCloud() const
{
    return m_colorCloud;
}

size_t CT_AttributesColor::attributesSize() const
{
    return m_colorCloud->size();
}

void CT_AttributesColor::setColorCloud(CT_AbstractColorCloud* cloud, bool autoDeleteColorCloud)
{
    if(mAutoDeleteColorCloud)
        delete m_colorCloud;

    mAutoDeleteColorCloud = autoDeleteColorCloud;
    m_colorCloud = cloud;
}
