#include "permanentiteminformation.h"

#include "scene/permanentsceneelementtype.h"

#include "scene/tools/objectsdispatchinformation.h"
#include "scene/tools/pointsdispatchinformation.h"
#include "renderer/ichunk.h"

PermanentItemInformation::PermanentItemInformation(const quint32 &index)
{
    m_index = index;
}

PermanentItemInformation::~PermanentItemInformation()
{
    clear();
}

void PermanentItemInformation::setIndex(const quint32 &index)
{
    m_index = index;
}

const quint32& PermanentItemInformation::getIndex() const
{
    return m_index;
}

void PermanentItemInformation::addObject(Scene::ObjectType type,
                                         const size_t& size,
                                         const IChunk* chunk,
                                         bool calledAfterInserted)
{
    Q_UNUSED(type)

    DispatchInformation& el = createOrGetObjectForChunk(chunk);

    if(el.begin() == DispatchInformation::INVALID_BEGIN) {
        el.setBegin(chunk->countObjects());

        if(calledAfterInserted)
            el.setBegin(el.begin() - size);
    }

    el.addObjects(size);
}

void PermanentItemInformation::clear()
{
    qDeleteAll(m_objects.begin(), m_objects.end());
    m_objects.clear();
}

void PermanentItemInformation::reset()
{
    ObjectCollectionIterator it(m_objects);

    while(it.hasNext()) {
        it.next();

        IChunk* chunk = it.key();

        chunk->resetObjects();
    }

    qDeleteAll(m_objects.begin(), m_objects.end());
    m_objects.clear();
}

DispatchInformation& PermanentItemInformation::createOrGetObjectForChunk(const IChunk* chunk)
{
    DispatchInformation *info = m_objects.value((IChunk*)chunk, nullptr);

    if(info != nullptr)
        return *info;

    if(chunk->getTypeOfObjectDrawn() == Scene::PointGlobalCloud)
        info = new PointsDispatchInformation();
    else
        info = new ObjectsDispatchInformation();

    m_objects.insert((IChunk*)chunk, info);

    return *info;
}

DispatchInformation* PermanentItemInformation::getObjectInformationForChunk(const IChunk *chunk) const
{
    return m_objects.value((IChunk*)chunk, nullptr);
}

void PermanentItemInformation::removeFromScene()
{
    ObjectCollectionIterator it(m_objects);

    while(it.hasNext()) {
        it.next();

        IChunk* chunk = it.key();
        DispatchInformation* objInfo = it.value();

        chunk->removeObjects(objInfo->begin(), objInfo->count());
    }

    clear();
}

void PermanentItemInformation::setVisible(bool visible)
{
    ObjectCollectionIterator it(m_objects);

    while(it.hasNext()) {
        it.next();

        IChunk* chunk = it.key();
        DispatchInformation* objInfo = it.value();

        chunk->setObjectsVisible(objInfo->begin(), objInfo->count(), visible);
    }
}

bool PermanentItemInformation::isVisible()
{
    ObjectCollectionIterator it(m_objects);

    while(it.hasNext()) {
        it.next();

        IChunk* chunk = it.key();
        DispatchInformation* objInfo = it.value();

        if(chunk->isAtLeastOneObjectVisible(objInfo->begin(), objInfo->count()))
            return true;
    }

    return false;
}

const PermanentItemInformation::ObjectCollection& PermanentItemInformation::getObjectsCollection() const
{
    return m_objects;
}

PermanentItemInformation::ObjectCollection& PermanentItemInformation::getObjectsCollection()
{
    return m_objects;
}
