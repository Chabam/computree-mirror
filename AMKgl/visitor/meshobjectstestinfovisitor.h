#ifndef MESHOBJECTSTESTINFOVISITOR_H
#define MESHOBJECTSTESTINFOVISITOR_H

#include "visitor/iobjectsvisitorbymodel.h"
#include "renderer/mesh/abstractchunkgenericcloudobject.h"
#include "scene/permanentitemscenebymodel.h"

/**
 * @brief This visitor can help you to get all selected/unselected, visible/unvisible, etc... mesh objects from a scene
 */
template<typename FoundedCloudIndex>
class MeshObjectsTestInfoVisitor : private IObjectsVisitorByModel
{
public:
    /**
     * Test function get the element info of a point and return true if we must keep it. Use function from
     * class "PointsPickingTools".
     */
    typedef std::function<bool (const ElementInfo& info)> testFunction;

    MeshObjectsTestInfoVisitor(Scene::ObjectType type);
    ~MeshObjectsTestInfoVisitor();

    /**
     * @brief Call this method and set the function that must return true when you want to keep a point.
     * @example If i want to keep only selected objects :
     *
     *          MeshObjectsTestInfoVisitor< std::vector<GLuint> > visitor(Scene::XXXXGlobalCloud);
     *          visitor.setFunction(PointsPickingTools::staticIsPointSelected);
     *
     *          visitor.compute(scene);
     *
     *          MeshObjectsTestInfoVisitor< std::vector<GLuint> >::FoundedCloudIndex indices = visitor.takeFoundedObjects();
     */
    void setFunction(testFunction f);

    /**
     * @brief Call it to launch the process
     */
    void compute(PermanentItemSceneByModel* scene);

    /**
     * @brief Get the cloud index of founded objects from this visitor. If you don't take it, it will be deleted
     *        from memory in the destructor.
     */
    FoundedCloudIndex* getFoundedObjects() const;

    /**
     * @brief Take the cloud index of founded objects from this visitor. If you don't take it, it will be deleted
     *        from memory in the destructor.
     */
    FoundedCloudIndex* takeFoundedObjects();

    // --------- IObjectsVisitorByModel --------- //

    bool mustContinueVisit() const { return true; }
    void setModel(const AMKgl::ItemModel* model) { Q_UNUSED(model); }
    void setScene(const PermanentItemScene* scene) { Q_UNUSED(scene); }
    void setItem(const AMKgl::Item* item) { Q_UNUSED(item); }
    AMKgl::ItemModel* getCurrentModel() const { return NULL; }
    PermanentItemScene* getCurrentScene() const { return NULL; }

    void visitChunk(const IChunk* chunk);

private:
    Scene::ObjectType               m_objectType;
    testFunction                    m_testFunction;
    FoundedCloudIndex*              m_foundedObjects;
};

#include "meshobjectstestinfovisitor.hpp"

#endif // MESHOBJECTSTESTINFOVISITOR_H
