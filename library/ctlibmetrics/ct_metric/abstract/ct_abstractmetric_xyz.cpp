#include "ct_abstractmetric_xyz.h"

CT_AbstractMetric_XYZ::CT_AbstractMetric_XYZ() : CT_AbstractMetricGeneric()
{
    _inCloud = nullptr;
    _plotArea = nullptr;
}

bool CT_AbstractMetric_XYZ::initDatas(const CT_AbstractPointCloudIndex* inCloud, const CT_AreaShape2DData* plotArea)
{
    _inCloud = inCloud;
    _plotArea = plotArea;

    return (_inCloud != nullptr);
}

const CT_AreaShape2DData *CT_AbstractMetric_XYZ::plotArea() const
{
    return _plotArea;
}

const CT_AbstractPointCloudIndex *CT_AbstractMetric_XYZ::pointCloud() const
{
    return _inCloud;
}
