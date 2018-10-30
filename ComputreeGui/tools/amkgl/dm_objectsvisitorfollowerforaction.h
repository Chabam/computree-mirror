#ifndef DM_OBJECTSVISITORFOLLOWERFORACTION_H
#define DM_OBJECTSVISITORFOLLOWERFORACTION_H

#include "visitor/iobjectsvisitor.h"
#include "scene/tools/permanentiteminformation.h"

#include "interfaces.h"

/**
 * @brief Inherit from interfaces from pluginshared and from amkgl to enable a visit of objects in
 *        a amkgl scene for action by a plugin !
 */
class DM_ObjectsVisitorFollowerForAction : public IObjectsVisitor, public ObjectsVisitorFollowerInterface
{
public:
    DM_ObjectsVisitorFollowerForAction(ObjectsVisitorInterface& visitor);

    /**
     * @brief Set the current permanent item information to retrieve dispatch information
     *        of objects in a specific chunk
     */
    void setCurrentInformation(const PermanentItemInformation* info);

    // ----- IObjectsVisitor ----- //
    bool mustContinueVisit() const;
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
    PermanentItemInformation*   m_info;
    DispatchInformation*        m_dispatchInfo;
};

#endif // DM_OBJECTSVISITORFOLLOWERFORACTION_H
