#ifndef GLOBALPOINTSTESTINFOVISITOR_H
#define GLOBALPOINTSTESTINFOVISITOR_H

#include "visitor/iglobalpointsvisitorbymodel.h"

#include "tools/anyelementattributesprovider.h"
#include "tools/pointcloudattributesprovider.h"
#include "scene/permanentitemscene.h"

/**
 * @brief This visitor can help you to get all selected/unselected, visible/unvisible, etc... global points from a scene
 */
template<typename FoundedCloudIndex>
class GlobalPointsTestInfoVisitor : public IGlobalPointsVisitorByModel
{
public:
    /**
     * Test function get the element info of a point and return true if we must keep it. Use function from
     * class "PointsPickingTools".
     */
    typedef std::function<bool (const ElementInfo& info)> testFunction;

    GlobalPointsTestInfoVisitor();
    ~GlobalPointsTestInfoVisitor();

    /**
     * @brief Call this method and set the function that must return true when you want to keep a point.
     * @example If i want to keep only selected points :
     *
     *          GlobalPointsInfoVisitor visitor;
     *          visitor.setFunction(PointsPickingTools::staticIsPointSelected);
     *
     *          scene.visitPoints(visitor);
     *
     *          GlobalPointsInfoVisitor::FoundedCloudIndex indices = visitor.takeFoundedPoints();
     */
    void setFunction(testFunction f);

    /**
     * @brief Get the cloud index of founded points from this visitor. If you don't take it, it will be deleted
     *        from memory in the destructor.
     */
    FoundedCloudIndex* getFoundedPoints() const;

    /**
     * @brief Take the cloud index of founded points from this visitor. If you don't take it, it will be deleted
     *        from memory in the destructor.
     */
    FoundedCloudIndex* takeFoundedPoints();

    // --------- IGlobalPointsVisitorByModel --------- //

    bool mustContinueVisit() const { return true; }
    void setCurrentInformation(const ItemModel *model, const PermanentItemScene *scene);
    AMKgl::ItemModel* getCurrentModel() const { return NULL; }
    PermanentItemScene* getCurrentScene() const { return NULL; }

    void addPointsToCompute(const Eigen::Vector3d &offset, std::vector<GLuint> &indices, const size_t& firstPointGlobalIndex);

private:
    AMKgl::GlobalInfoCloud*         m_infoCloud;
    testFunction                    m_testFunction;
    FoundedCloudIndex*              m_foundedPoints;
};

#include "globalpointstestinfovisitor.hpp"

#endif // GLOBALPOINTSTESTINFOVISITOR_H
