#include "dm_objectsvisitorfollowerbymodel.h"

#include "renderer/ichunk.h"
#include "dm_objectsmodifier.h"
#include "scene/permanentitemscene.h"
#include "scene/tools/dispatchinformation.h"

DM_ObjectsVisitorFollowerByModel::DM_ObjectsVisitorFollowerByModel(ObjectsVisitorInterface& visitor) : m_visitor(visitor)
{
    m_visitor.setFollower(this);
    m_chunk = NULL;
    m_scene = NULL;
    m_item = NULL;
    m_info = NULL;
    m_dispatchInfo = NULL;
}

bool DM_ObjectsVisitorFollowerByModel::mustContinueVisit() const
{
    return m_visitor.continueVisit();
}

void DM_ObjectsVisitorFollowerByModel::setModel(const AMKgl::ItemModel *model)
{
    // TODO
}

void DM_ObjectsVisitorFollowerByModel::setScene(const PermanentItemScene *scene)
{
    m_scene = (PermanentItemScene*)scene;
    m_item = NULL;
    m_info = NULL;
    m_dispatchInfo = NULL;
}

void DM_ObjectsVisitorFollowerByModel::setItem(const AMKgl::Item *item)
{
    m_item = (AMKgl::Item*)item;
    m_info = m_scene->getPermanentItemInformation(m_item);
}

void DM_ObjectsVisitorFollowerByModel::visitChunk(const IChunk *chunk)
{
    if(m_info != NULL)
        m_dispatchInfo = m_info->getObjectInformationForChunk(chunk);

    m_chunk = (IChunk*)chunk;
    m_visitor.newObjectsAvailable();
}

size_t DM_ObjectsVisitorFollowerByModel::getNumberOfObjects() const
{
    if(m_dispatchInfo != NULL)
        return m_dispatchInfo->count();

    return m_chunk->countObjects();
}

size_t DM_ObjectsVisitorFollowerByModel::getNumberOfVertexPerObject() const
{
    return m_chunk->getNumberOfElementPerObject();
}

GraphicsObjectType DM_ObjectsVisitorFollowerByModel::currentObjectType() const
{
    return (GraphicsObjectType)m_chunk->getTypeOfObjectDrawn();
}

CT_OutAbstractItemModel* DM_ObjectsVisitorFollowerByModel::currentItemModel() const
{
    return NULL;
}

QSharedPointer<ObjectStatusQueryiorInterface> DM_ObjectsVisitorFollowerByModel::createObjectsStatusQueryior()
{
    if(m_dispatchInfo != NULL)
        return QSharedPointer<ObjectStatusQueryiorInterface>(new DM_ObjectsModifier(m_chunk, m_dispatchInfo->begin(), m_dispatchInfo->count(), false));

    return QSharedPointer<ObjectStatusQueryiorInterface>(new DM_ObjectsModifier(m_chunk, 0, m_chunk->countObjects(), false));
}

QSharedPointer<ObjectsModifierInterface> DM_ObjectsVisitorFollowerByModel::createObjectsModifier()
{
    if(m_dispatchInfo != NULL)
        return QSharedPointer<ObjectsModifierInterface>(new DM_ObjectsModifier(m_chunk, m_dispatchInfo->begin(), m_dispatchInfo->count()));

    return QSharedPointer<ObjectsModifierInterface>(new DM_ObjectsModifier(m_chunk, 0, m_chunk->countObjects()));
}
