#include "dm_objectsvisitorfollowerforaction.h"

#include "renderer/ichunk.h"
#include "dm_objectsmodifier.h"
#include "scene/tools/dispatchinformation.h"

DM_ObjectsVisitorFollowerForAction::DM_ObjectsVisitorFollowerForAction(ObjectsVisitorInterface &visitor) : m_visitor(visitor)
{
    m_visitor.setFollower(this);
    m_chunk = NULL;
    m_info = NULL;
    m_dispatchInfo = NULL;
}

void DM_ObjectsVisitorFollowerForAction::setCurrentInformation(const PermanentItemInformation *info)
{
    m_info = (PermanentItemInformation*)info;
    m_dispatchInfo = NULL;
}

bool DM_ObjectsVisitorFollowerForAction::mustContinueVisit() const
{
    return m_visitor.continueVisit();
}

void DM_ObjectsVisitorFollowerForAction::visitChunk(const IChunk *chunk)
{
    if(m_info != NULL)
        m_dispatchInfo = m_info->getObjectInformationForChunk(chunk);

    m_chunk = (IChunk*)chunk;
    m_visitor.newObjectsAvailable();
}

size_t DM_ObjectsVisitorFollowerForAction::getNumberOfObjects() const
{
    if(m_dispatchInfo != NULL)
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
    return NULL;
}

QSharedPointer<ObjectStatusQueryiorInterface> DM_ObjectsVisitorFollowerForAction::createObjectsStatusQueryior()
{
    if(m_dispatchInfo != NULL)
        return QSharedPointer<ObjectStatusQueryiorInterface>(new DM_ObjectsModifier(m_chunk, m_dispatchInfo->begin(), m_dispatchInfo->count(), false));

    return QSharedPointer<ObjectStatusQueryiorInterface>(new DM_ObjectsModifier(m_chunk, 0, m_chunk->countObjects(), false));
}

QSharedPointer<ObjectsModifierInterface> DM_ObjectsVisitorFollowerForAction::createObjectsModifier()
{
    if(m_dispatchInfo != NULL)
        return QSharedPointer<ObjectsModifierInterface>(new DM_ObjectsModifier(m_chunk, m_dispatchInfo->begin(), m_dispatchInfo->count()));

    return QSharedPointer<ObjectsModifierInterface>(new DM_ObjectsModifier(m_chunk, 0, m_chunk->countObjects()));
}


