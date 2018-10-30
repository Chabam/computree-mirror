#ifndef DM_OBJECTSVISITORFOLLOWERBYMODEL_H
#define DM_OBJECTSVISITORFOLLOWERBYMODEL_H

#include "visitor/iobjectsvisitorbymodel.h"
#include "scene/tools/permanentiteminformation.h"

#include "interfaces.h"

/**
 * @brief Inherit from interfaces from pluginshared and from amkgl to enable a visit of objects in
 *        a amkgl scene by a plugin !
 */
class DM_ObjectsVisitorFollowerByModel : public IObjectsVisitorByModel, public ObjectsVisitorFollowerInterface
{
public:
    DM_ObjectsVisitorFollowerByModel(ObjectsVisitorInterface& visitor);

    // ----- IObjectsVisitorByModel ----- //
    bool mustContinueVisit() const;
    void setModel(const AMKgl::ItemModel* model);
    void setScene(const PermanentItemScene* scene);
    void setItem(const AMKgl::Item* item);
    void visitChunk(const IChunk* chunk);

    // ----- ObjectsVisitorFollowerInterface ----- //
    size_t getNumberOfObjects() const;
    size_t getNumberOfVertexPerObject() const;
    GraphicsObjectType currentObjectType() const;
    CT_OutAbstractItemModel* currentItemModel() const;
    QSharedPointer<ObjectStatusQueryiorInterface> createObjectsStatusQueryior();
    QSharedPointer<ObjectsModifierInterface> createObjectsModifier();

private:
    ObjectsVisitorInterface&    m_visitor;
    IChunk*                     m_chunk;
    AMKgl::Item*                m_item;
    PermanentItemScene*         m_scene;
    PermanentItemInformation*   m_info;
    DispatchInformation*        m_dispatchInfo;
};

#endif // DM_OBJECTSVISITORFOLLOWERBYMODEL_H
