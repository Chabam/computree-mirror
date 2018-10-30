#ifndef POINTSPICKINGDEFAULTFUNCTION_H
#define POINTSPICKINGDEFAULTFUNCTION_H

#include "definepoint.h"

#include "elementinfo.h"

using namespace AMKgl;

class PointsPickingDefaultFunction
{
public:
    /**
     * @brief Select the point
     */
    static void staticSelectPoint(const FloatPoint &p, ElementInfo* firstInfo, const size_t &localIndex);

    /**
     * @brief Check if must test the point when select it
     */
    static bool staticCheckSelectPoint(const FloatPoint &p, ElementInfo* firstInfo, const size_t &localIndex);

    /**
     * @brief Unselect the point
     */
    static void staticUnselectPoint(const FloatPoint &p, ElementInfo* firstInfo, const size_t &localIndex);

    /**
     * @brief Check if must test the point when unselect it
     */
    static bool staticCheckUnselectPoint(const FloatPoint &p, ElementInfo* firstInfo, const size_t &localIndex);

    /**
     * @brief Returns true if the point is selected
     */
    static bool staticIsPointSelected(ElementInfo* firstInfo, const size_t &localIndex);

    /**
     * @brief Toggle the selection of the point (if selected it will be unselected and vice/versa)
     */
    static void staticTogglePointSelection(const FloatPoint &p, ElementInfo* firstInfo, const size_t &localIndex);
};

#endif // POINTSPICKINGDEFAULTFUNCTION_H
