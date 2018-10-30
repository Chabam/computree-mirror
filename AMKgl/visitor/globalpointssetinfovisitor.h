#ifndef GLOBALPOINTSSETINFOVISITOR_H
#define GLOBALPOINTSSETINFOVISITOR_H

#include "visitor/iglobalpointsvisitorbymodel.h"
#include "scene/permanentitemscene.h"
#include "defineglobalcloud.h"

/**
 * @brief This visitor can help you to set all global points from a scene selected/unselected, visible/unvisible, etc...
 */
class GlobalPointsSetInfoVisitor : public IGlobalPointsVisitorByModel
{
public:
    /**
     * Set function get the element info of a point and modify it. Use function from
     * class "PointsPickingTools".
     */
    typedef std::function<void (ElementInfo& info)> functionForSet;

    GlobalPointsSetInfoVisitor();

    /**
     * @brief Call this method and set the function that must modify info of points.
     * @example If i want to select all points :
     *
     *          GlobalPointsInfoVisitor visitor;
     *          visitor.setFunction(PointsPickingTools::staticSelectPoint);
     *
     *          scene.visitPoints(visitor);
     */
    void setFunction(functionForSet f);

    // --------- IGlobalPointsVisitorByModel --------- //

    bool mustContinueVisit() const { return true; }

    void setCurrentInformation(const ItemModel *model, const PermanentItemScene *scene);
    AMKgl::ItemModel* getCurrentModel() const { return NULL; } \
    PermanentItemScene* getCurrentScene() const { return NULL; }

    void addPointsToCompute(const Eigen::Vector3d &offset, std::vector<GLuint> &indices, const size_t& firstPointGlobalIndex);

private:
    AMKgl::GlobalInfoCloud*         m_infoCloud;
    functionForSet                  m_setFunction;
};

#include "visitor/globalpointstestinfovisitor.hpp"

#endif // GLOBALPOINTSSETINFOVISITOR_H
