#ifndef MESHOBJECTSSETINFOVISITOR_H
#define MESHOBJECTSSETINFOVISITOR_H

#include "visitor/iobjectsvisitorbymodel.h"
#include "scene/permanentsceneelementtype.h"
#include "elementinfo.h"

#include <functional>

class PermanentItemSceneByModel;

/**
 * @brief This visitor can help you to set all objects of a mesh from a scene selected/unselected, visible/unvisible, etc...
 */
class MeshObjectsSetInfoVisitor : private IObjectsVisitorByModel
{
public:
    /**
     * Set function get the element info of a point and modify it. Use function from
     * class "PointsPickingTools".
     */
    typedef std::function<void (ElementInfo& info)> functionForSet;

    MeshObjectsSetInfoVisitor(Scene::ObjectType type);

    /**
     * @brief Call this method and set the function that must modify info of points.
     * @example If i want to select all points :
     *
     *          MeshObjectsSetInfoVisitor visitor(Scene::XXXGlobalCloud);
     *          visitor.setFunction(PointsPickingTools::staticSelectPoint);
     *
     *          visitor.compute(scene);
     */
    void setFunction(functionForSet f);

    /**
     * @brief Set to true if you want to create info cloud if it was not yet created (true by default)
     */
    void setCreateInfoItNotCreated(bool createIt);

    /**
     * @brief Call it to launch the process
     */
    void compute(PermanentItemSceneByModel* scene);

    // --------- IObjectsVisitorByModel --------- //

    bool mustContinueVisit() const { return true; }
    void setModel(const AMKgl::ItemModel* model) { Q_UNUSED(model); }
    void setScene(const PermanentItemScene* scene) { Q_UNUSED(scene); }
    void setItem(const AMKgl::Item* item) { Q_UNUSED(item); }
    AMKgl::ItemModel* getCurrentModel() const { return nullptr; }
    PermanentItemScene* getCurrentScene() const { return nullptr; }

    void visitChunk(const IChunk* chunk);

private:
    Scene::ObjectType               m_objectType;
    bool                            m_createInfoCloud;
    functionForSet                  m_setFunction;
};

#endif // MESHOBJECTSSETINFOVISITOR_H
