#ifndef MESHOBJECTSVISITOR_H
#define MESHOBJECTSVISITOR_H

#include "visitor/iobjectsvisitorbymodel.h"
#include "scene/permanentsceneelementtype.h"

#include <functional>

class PermanentItemSceneByModel;
class AbstractChunkGenericCloudObject;

/**
 * @brief This visitor can help you to visit all objects of a mesh from a scene
 */
class MeshObjectsVisitor : private IObjectsVisitorByModel
{
public:
    /**
     * Set function get the element info of a point and modify it. Use function from
     * class "PointsPickingTools".
     */
    typedef std::function<void (AbstractChunkGenericCloudObject* chunk)> functionForVisit;

    MeshObjectsVisitor(Scene::ObjectType type);

    /**
     * @brief Call this method and set the function that must modify info of points.
     * @example If i want to select all points :
     *
     *          MeshObjectsSetInfoVisitor visitor(Scene::XXXGlobalCloud);
     *          visitor.setFunction([](AbstractChunkGenericCloudObject* chunk) { // do something });
     *
     *          visitor.compute(scene);
     */
    void setFunction(functionForVisit f);

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
    functionForVisit                m_visitFunction;
};

#endif // MESHOBJECTSVISITOR_H
