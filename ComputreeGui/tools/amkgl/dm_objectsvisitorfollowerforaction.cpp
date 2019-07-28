#include "dm_objectsvisitorfollowerforaction.h"

#include "renderer/ichunk.h"
#include "dm_objectsmodifier.h"
#include "scene/tools/dispatchinformation.h"

DM_ObjectsVisitorFollowerForAction::DM_ObjectsVisitorFollowerForAction(ObjectsVisitorInterface &visitor) : m_visitor(visitor)
{
    m_visitor.setFollower(this);
    m_chunk = nullptr;
    m_info = nullptr;
    m_dispatchInfo = nullptr;
}

void DM_ObjectsVisitorFollowerForAction::setCurrentInformation(const PermanentItemInformation *info)
{
    m_info = (PermanentItemInformation*)info;
    m_dispatchInfo = nullptr;
}

bool DM_ObjectsVisitorFollowerForAction::mustContinueVisit() const
{
    return m_visitor.continueVisit();
}

void DM_ObjectsVisitorFollowerForAction::visitChunk(const IChunk *chunk)
{
    if(m_info != nullptr)
        m_dispatchInfo = m_info->getObjectInformationForChunk(chunk);

    m_chunk = (IChunk*)chunk;
    m_visitor.newObjectsAvailable();
}

size_t DM_ObjectsVisitorFollowerForAction::getNumberOfObjects() const
{
    if(m_dispatchInfo != nullptr)
        return m_dispatchInfo->count();

    return m_chunk->countObjects();
}

size_t DM_ObjectsVisitorFollowerForAction::getNumberOfVertexPerObject() const
{
    return m_chunk->getNumberOfElementPerObject();
}

GraphicsObjectType DM_ObjectsVisitorFollowerForAction::currentObjectType() const
{
    return (GraphicsObjectType)m_chunk->getTypeOfObjectDrawn();
}

CT_OutAbstractItemModel* DM_ObjectsVisitorFollowerForAction::currentItemModel() const
{
    return nullptr;
}

QSharedPointer<ObjectStatusQueryiorInterface> DM_ObjectsVisitorFollowerForAction::createObjectsStatusQueryior()
{
    if(m_dispatchInfo != nullptr)
        return QSharedPointer<ObjectStatusQueryiorInterface>(new DM_ObjectsModifier(m_chunk, m_dispatchInfo->begin(), m_dispatchInfo->count(), false));

    return QSharedPointer<ObjectStatusQueryiorInterface>(new DM_ObjectsModifier(m_chunk, 0, m_chunk->countObjects(), false));
}

QSharedPointer<ObjectsModifierInterface> DM_ObjectsVisitorFollowerForAction::createObjectsModifier()
{
    if(m_dispatchInfo != nullptr)
        return QSharedPointer<ObjectsModifierInterface>(new DM_ObjectsModifier(m_chunk, m_dispatchInfo->begin(), m_dispatchInfo->count()));

    return QSharedPointer<ObjectsModifierInterface>(new DM_ObjectsModifier(m_chunk, 0, m_chunk->countObjects()));
}


