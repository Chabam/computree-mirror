#include "permanentitemsceneremover.h"

#include "renderer/ichunk.h"
#include "scene/permanentscene.h"
#include "includeitem.h"

PermanentItemSceneRemover::PermanentItemSceneRemover(PermanentItemScene &scene) : m_scene(scene), QObject()
{
}

PermanentItemSceneRemover::~PermanentItemSceneRemover()
{
}

void PermanentItemSceneRemover::addItemToRemove(const Item *item)
{
    PermanentItemInformation *info = m_scene.takePermanentItemInformation(item);

    if(info != NULL) {

        Backup b;
        b.item = (Item*)item;
        b.info = info;
        b.deleteIt = true;
        b.graphicsView = NULL;

        m_itemsBackup.insert(info->getIndex(), b);

        m_scene.internalRemoveItem(item);
    }
}

void PermanentItemSceneRemover::addItemToUpdate(const Item *item, GraphicsView& graphicsView, const QColor& color)
{
    PermanentItemInformation *info = m_scene.takePermanentItemInformation(item);

    if(info != NULL) {

        Backup b;
        b.item = (Item*)item;
        b.info = info;
        b.deleteIt = false;
        b.graphicsView = &graphicsView;
        b.color = color;

        m_itemsBackup.insert(info->getIndex(), b);
    }
}

void PermanentItemSceneRemover::compute()
{
    if(m_itemsBackup.isEmpty())
        return;

    quint32 offsetForIndex = 0;

    QMapIterator<quint32, Backup> it(m_itemsBackup);
    it.toBack();

    quint32 lastIndex = it.previous().key();

    it.toFront();

    while(it.hasNext()) {
        it.next();

        const quint32 newIndex = it.key();

        if(lastIndex < newIndex)  {
            for(quint32 i = lastIndex+1; i < newIndex; ++i) {
                shifItemInformation(m_scene.m_itemsWithInfo.value(m_scene.m_items.at(i)), offsetForIndex);
            }
        }

        const Backup &b = it.value();

        mergeItemInformation(newIndex, b.info);

        lastIndex = newIndex;

        if(b.deleteIt)
            ++offsetForIndex;
    }

    const quint32 newIndex = m_scene.m_items.size();

    for(quint32 i = lastIndex+1; i < newIndex; ++i) {
        shifItemInformation(m_scene.m_itemsWithInfo.value(m_scene.m_items.at(i)), offsetForIndex);
    }

    // remove shapes
    MergedItemInformation::ObjectCollectionIterator itO(getMergedItemInformation().m_objects);

    while(itO.hasNext()) {
        itO.next();

        MergedItemInformation::VectorOfMergedObjectInformation *vector = itO.value();
        itO.key()->removeObjects(vector->collection.begin(), vector->collection.end());
    }

    // remove items
    //quint32 offset = 0;
    offsetForIndex = 0;
    it.toFront();

    bool deleteIt = m_scene.mustAutoDeleteItems();

    while(it.hasNext()) {
        it.next();

        const quint32 infoIndex = it.key()-offsetForIndex;
        const Backup& backup = it.value();

        if(backup.deleteIt) {
            Item* item = m_scene.m_items.takeAt(infoIndex);

            if(deleteIt)
                delete item;

            ++offsetForIndex;
        } else {
            m_scene.addItemUpdated(m_scene.m_items.value(infoIndex, NULL), *backup.graphicsView, m_scene.m_items.indexOf(backup.item), backup.color);
        }
    }

#ifdef QT_DEBUG
    // verify
    offsetForIndex = 0;
    QListIterator<Item*> itI(m_scene.m_items);

    while(itI.hasNext()) {
        Item* item = itI.next();
        PermanentItemInformation* info = m_scene.m_itemsWithInfo.value(item, NULL);
        Q_ASSERT_X(info->getIndex() == offsetForIndex, "PermanentItemSceneRemover", QString("Index Error, must be %1 and is %2").arg(offsetForIndex).arg(info->getIndex()).toLatin1());
        ++offsetForIndex;
    }
#endif

    m_itemsBackup.clear();

    getMergedItemInformation().clear();
}

void PermanentItemSceneRemover::clearItems()
{
    m_scene.lock();

    m_scene.m_objectsScene->clearAllObjects();

    if(m_scene.mustAutoDeleteItems())
        qDeleteAll(m_scene.m_items.begin(), m_scene.m_items.end());

    m_scene.m_items.clear();

    m_scene.m_itemsWithInfo.clear();

    getMergedItemInformation().clear();

    m_scene.unlock();
}

bool PermanentItemSceneRemover::mustBeLaunched() const
{
    return !m_itemsBackup.isEmpty();
}

PermanentItemSceneRemover::MergedItemInformation& PermanentItemSceneRemover::getMergedItemInformation()
{
    return m_infoMerged;
}

void PermanentItemSceneRemover::mergeItemInformation(const quint32& index, PermanentItemInformation *infoToMerge)
{
    QHashIterator<IChunk*, DispatchInformation*> itC(infoToMerge->getObjectsCollection());

    while(itC.hasNext()) {
        itC.next();
        m_infoMerged.mergeChunkWith(itC.key(), *itC.value(), index);
    }

    delete infoToMerge;
}

void PermanentItemSceneRemover::shifItemInformation(PermanentItemInformation *infoToShift, const quint32& offsetForIndex)
{
    infoToShift->setIndex(infoToShift->getIndex()-offsetForIndex);

    PermanentItemInformation::ObjectCollectionIterator itC(infoToShift->getObjectsCollection());

    while(itC.hasNext()) {
        itC.next();
        MergedItemInformation::VectorOfMergedObjectInformation *mergedInfo = m_infoMerged.m_objects.value(itC.key(), NULL);

        if(mergedInfo != NULL)
            itC.value()->setBegin(itC.value()->begin() - mergedInfo->totalCount);
    }
}

PermanentItemSceneRemover::MergedItemInformation::VectorOfMergedObjectInformation* PermanentItemSceneRemover::MergedItemInformation::createOrGetVectorForChunk(const IChunk *chunk)
{
    if(m_lastChunkUsed == chunk)
        return m_lastVectorUsed;

    m_lastChunkUsed = (IChunk*)chunk;
    m_lastVectorUsed = m_objects.value(m_lastChunkUsed, NULL);

    if(m_lastVectorUsed == NULL) {
        m_lastVectorUsed = new VectorOfMergedObjectInformation();
        m_objects.insert(m_lastChunkUsed, m_lastVectorUsed);
    }

    return m_lastVectorUsed;
}

void PermanentItemSceneRemover::MergedItemInformation::mergeChunkWith(const IChunk* chunk,
                                                                      const PermanentItemSceneRemover::MergedItemInformation::MergedObjectInformation &toAdd,
                                                                      const quint32 &index)
{
    Q_UNUSED(index)

    VectorOfMergedObjectInformation* vector = createOrGetVectorForChunk(chunk);

    vector->totalCount += toAdd.count();

    if(!vector->collection.empty()) {
        MergedObjectInformation &lastInfo = vector->collection.back();

        if(toAdd.begin() <= lastInfo.end()+1) {
            lastInfo.mergeWith(toAdd);
        } else {
            vector->collection.push_back(toAdd);
        }

    } else {
       vector->collection.push_back(toAdd);
    }
}

void PermanentItemSceneRemover::MergedItemInformation::clear()
{
    qDeleteAll(m_objects.begin(), m_objects.end());
    m_objects.clear();

    m_lastChunkUsed = NULL;
    m_lastVectorUsed = NULL;
}
