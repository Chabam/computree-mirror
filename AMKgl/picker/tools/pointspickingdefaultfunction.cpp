#include "pointspickingdefaultfunction.h"

#include "picker/tools/objectsflagstool.h"
#include "includepoint.h"

void PointsPickingDefaultFunction::staticSelectPoint(const FloatPoint &p, ElementInfo *firstInfo, const size_t &localIndex)
{
    Q_UNUSED(p)

    ObjectsFlagsTool::staticSelectObject(firstInfo[localIndex]);
}

bool PointsPickingDefaultFunction::staticCheckSelectPoint(const FloatPoint &p, ElementInfo *firstInfo, const size_t &localIndex)
{
    Q_UNUSED(p)

    return ObjectsFlagsTool::staticCheckSelectObject(firstInfo[localIndex]);
}

void PointsPickingDefaultFunction::staticUnselectPoint(const FloatPoint &p, ElementInfo *firstInfo, const size_t &localIndex)
{
    Q_UNUSED(p)

    ObjectsFlagsTool::staticUnselectObject(firstInfo[localIndex]);
}

bool PointsPickingDefaultFunction::staticCheckUnselectPoint(const FloatPoint &p, ElementInfo *firstInfo, const size_t &localIndex)
{
    Q_UNUSED(p)

    return ObjectsFlagsTool::staticCheckUnselectObject(firstInfo[localIndex]);
}

bool PointsPickingDefaultFunction::staticIsPointSelected(ElementInfo *firstInfo, const size_t &localIndex)
{
    return ObjectsFlagsTool::staticIsObjectSelected(firstInfo[localIndex]);
}

void PointsPickingDefaultFunction::staticTogglePointSelection(const FloatPoint &p, ElementInfo *firstInfo, const size_t &localIndex)
{
    Q_UNUSED(p)

    return ObjectsFlagsTool::staticToggleObjectSelection(firstInfo[localIndex]);
}
