#include "ct_abstractmetric_las.h"

CT_AbstractMetric_LAS::CT_AbstractMetric_LAS() : CT_AbstractMetric_XYZ()
{
    _lasAttributes = nullptr;
    m_lasPointCloudIndex = nullptr;
}

bool CT_AbstractMetric_LAS::initLasDatas(const CT_AbstractPointCloudIndex *inCloud, const CT_AreaShape2DData *plotArea, const CT_StdLASPointsAttributesContainer *lasAttributes)
{
    _lasAttributes = const_cast<CT_StdLASPointsAttributesContainer*>(lasAttributes);
    m_lasPointCloudIndex = nullptr;

    if (_lasAttributes == nullptr)
        return false;

    QHashIterator<CT_LasDefine::LASPointAttributesType, CT_AbstractPointAttributesScalar *> it(_lasAttributes->lasPointsAttributes());

    if(!it.hasNext())
        return false;

    CT_AbstractPointAttributesScalar *firstAttribute = it.next().value();

    if((m_lasPointCloudIndex = const_cast<CT_AbstractPointCloudIndex*>(firstAttribute->pointCloudIndex())) == nullptr)
        _lasAttributes = nullptr;

    if((_lasAttributes != nullptr) && !initDatas(inCloud, plotArea)) {
        _lasAttributes = nullptr;
        m_lasPointCloudIndex = nullptr;
    }

    return (_lasAttributes != nullptr);
}

CT_StdLASPointsAttributesContainer *CT_AbstractMetric_LAS::lasAttributes() const
{
    return _lasAttributes;
}

CT_AbstractPointCloudIndex *CT_AbstractMetric_LAS::lasPointCloudIndex() const
{
    return m_lasPointCloudIndex;
}
