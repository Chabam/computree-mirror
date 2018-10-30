#include "permanentitemscene.h"

#include "visitor/iitemsvisitor.h"
#include "renderer/object/iobjectcloud.h"
#include "tools/permanentitemsceneremover.h"
#include "permanentscene.h"

#include "defineitempermanentpainter.h"
#include "includeitempermanentpainter.h"
#include "includegraphicsview.h"
#include "includeitem.h"

#include "picker/tools/itemspickingtools.h"

PermanentItemScene::PermanentItemScene()
{
    m_autoDeleteItem = false;
    m_mutex = new QMutex(QMutex::Recursive);

    m_objectsScene = new PermanentScene();
    m_objectsScene->setAttributesAccessor(this);
    m_objectsScene->init();

    m_itemsRemover = new PermanentItemSceneRemover(*this);

    connect(m_objectsScene, SIGNAL(changesHaveOccured()), this, SIGNAL(changesHaveOccured()), Qt::DirectConnection);
    connect(m_objectsScene, SIGNAL(updateNecessary()), this, SIGNAL(updateNecessary()), Qt::DirectConnection);
}

PermanentItemScene::~PermanentItemScene()
{
    clearItems();

    delete m_itemsRemover;
    delete m_objectsScene;
    delete m_mutex;
}

PermanentScene* PermanentItemScene::getPermanentScene() const
{
    return m_objectsScene;
}

void PermanentItemScene::addedTo(const IGraphicsDocument* doc, const QOpenGLContext* newContext)
{
    m_documents.insert(const_cast<IGraphicsDocument*>(doc), const_cast<QOpenGLContext*>(newContext));

    m_objectsScene->addedTo(doc, newContext);
}

void PermanentItemScene::removedFrom(const IGraphicsDocument* doc, const QOpenGLContext* newContext)
{
    IGraphicsDocument* nDoc = const_cast<IGraphicsDocument*>(doc);

    m_documents.remove(nDoc, const_cast<QOpenGLContext*>(newContext));

    if(m_documents.value(nDoc, NULL) == NULL)
        m_documents.remove(nDoc);

    m_objectsScene->removedFrom(doc, newContext);
}

void PermanentItemScene::lock()
{
    m_mutex->lock();
}

void PermanentItemScene::unlock()
{
    m_mutex->unlock();
}

PointCloudAttributesProvider *PermanentItemScene::getPointCloudAttributesProvider() const
{
    return m_objectsScene->getPointCloudAttributesProvider();
}

FaceCloudAttributesProvider *PermanentItemScene::getFaceCloudAttributesProvider() const
{
    return m_objectsScene->getFaceCloudAttributesProvider();
}

EdgeCloudAttributesProvider *PermanentItemScene::getEdgeCloudAttributesProvider() const
{
    return m_objectsScene->getEdgeCloudAttributesProvider();
}

void PermanentItemScene::updateInAllContext()
{
    getPermanentItemSceneRemover()->compute();
    m_objectsScene->updateInAllContext();
}

void PermanentItemScene::updatePointsInfoInAllContext()
{
    getPermanentItemSceneRemover()->compute();
    m_objectsScene->updatePointsInfoInAllContext();
}

void PermanentItemScene::updatePointsColorInAllContext()
{
    getPermanentItemSceneRemover()->compute();
    m_objectsScene->updatePointsColorInAllContext();
}

void PermanentItemScene::updatePointsNormalInAllContext()
{
    getPermanentItemSceneRemover()->compute();
    m_objectsScene->updatePointsNormalInAllContext();
}

void PermanentItemScene::updateItemsInfoInAllContext()
{
    getPermanentItemSceneRemover()->compute();
    m_objectsScene->updateObjectsInfoInAllContext();
}

void PermanentItemScene::updateItemsColorInAllContext()
{
    getPermanentItemSceneRemover()->compute();
    m_objectsScene->updateObjectsColorInAllContext();
}

void PermanentItemScene::updateInfoInAllContext()
{
    //updatePointsInfoInAllContext();
    updateItemsInfoInAllContext();
}

void PermanentItemScene::updateColorInAllContext()
{
    //updatePointsColorInAllContext();
    updateItemsColorInAllContext();
}

void PermanentItemScene::setDrawModeToUse(DrawMode mode)
{
    m_objectsScene->setDrawModeToUse(mode);
}

void PermanentItemScene::setDrawModeToUse(Scene::ObjectType type, DrawMode mode)
{
    m_objectsScene->setDrawModeToUse(type, mode);
}

void PermanentItemScene::setMustReduceNumberOfPointsInFastDraw(bool enabled)
{
    m_objectsScene->setMustReduceNumberOfPointsInFastDraw(enabled);
}

void PermanentItemScene::setMustReduceNumberOfPointsInNormalDraw(bool enabled)
{
    m_objectsScene->setMustReduceNumberOfPointsInNormalDraw(enabled);
}

bool PermanentItemScene::mustReduceNumberOfPointsInFastDraw() const
{
    return m_objectsScene->mustReduceNumberOfPointsInFastDraw();
}

bool PermanentItemScene::mustReduceNumberOfPointsInNormalDraw() const
{
    return m_objectsScene->mustReduceNumberOfPointsInNormalDraw();
}

void PermanentItemScene::destroyGL(const QOpenGLContext *context)
{
    m_objectsScene->destroyGL(context);
}

void PermanentItemScene::setVisible(bool visible)
{
    m_objectsScene->setVisible(visible);
}

bool PermanentItemScene::isVisible() const
{
    return m_objectsScene->isVisible();
}

AnyElementAttributesProvider* PermanentItemScene::getAnyElementsAttributesProvider() const
{
    return &const_cast<AnyElementAttributesProvider&>(m_anyElementsAttributesProvider);
}

ObjectAttributesProvider* PermanentItemScene::getObjectsAttributeProvider(Scene::ObjectType type) const
{
    return m_anyElementsAttributesProvider.getObjectsAttributeProvider(type);
}

void PermanentItemScene::visitPoints(IGlobalPointsVisitor &visitor)
{
    m_objectsScene->visitPoints(visitor);
}

void PermanentItemScene::visitItems(IItemsVisitor &visitor)
{
    ItemCollectionIterator it(m_itemsWithInfo);

    while(it.hasNext() && visitor.mustContinueVisit())
        visitor.addItemDrawableToCompute(it.next().key());
}

void PermanentItemScene::visitObjects(IObjectsVisitor &visitor)
{
    m_objectsScene->visitObjects(visitor);
}

void PermanentItemScene::visitObjectsOfTypes(const QList<Scene::ObjectType> &types, IObjectsVisitor &visitor)
{
    m_objectsScene->visitObjectsOfTypes(types, visitor);
}

void PermanentItemScene::visitObjectsOfOneItem(const Item *item, IObjectsVisitor &visitor)
{
    PermanentItemInformation* info = m_itemsWithInfo.value((Item*)item, NULL);

    if(info != NULL) {
        PermanentItemInformation::ObjectCollectionIterator it(info->getObjectsCollection());

        while(it.hasNext() && visitor.mustContinueVisit()) {
            it.next();

            if(it.value()->count() > 0)
                visitor.visitChunk(it.key());
        }
    }
}

void PermanentItemScene::visitObjectsOfTypesOfOneItem(const Item *item, const QList<Scene::ObjectType> &types, IObjectsVisitor &visitor)
{
    PermanentItemInformation* info = m_itemsWithInfo.value((Item*)item, NULL);

    if(info != NULL) {
        PermanentItemInformation::ObjectCollectionIterator it(info->getObjectsCollection());

        while(it.hasNext() && visitor.mustContinueVisit()) {
            it.next();

            if((it.value()->count() > 0) && types.contains(it.key()->getTypeOfObjectDrawn()))
                visitor.visitChunk(it.key());
        }
    }
}

void PermanentItemScene::setAutoDeleteItems(bool enable)
{
    m_autoDeleteItem = enable;
}

bool PermanentItemScene::mustAutoDeleteItems() const
{
    return m_autoDeleteItem;
}

void PermanentItemScene::clearItems()
{
    getPermanentItemSceneRemover()->clearItems();
}

bool PermanentItemScene::containsItem(const Item *item) const
{
    QMutexLocker locker(getMutex());

    return m_itemsWithInfo.contains((Item*)item);
}

void PermanentItemScene::addItem(const Item *item, GraphicsView& graphicsView)
{
    QMutexLocker locker(getMutex());

    if(internalAddItem(item, graphicsView, m_items.size())) {
        connect(item, &Item::selectChange, this, &PermanentItemScene::itemSelectionChanged, Qt::DirectConnection);

        m_items.push_back((Item*)item);
    }
}

int PermanentItemScene::countItem() const
{
    QMutexLocker locker(getMutex());

    return m_items.size();
}

void PermanentItemScene::setItemVisible(const Item *item, bool visible)
{
    QMutexLocker locker(getMutex());

    PermanentItemInformation *info = m_itemsWithInfo.value((Item*)item, NULL);

    if(info != NULL)
        info->setVisible(visible);
}

bool PermanentItemScene::isItemVisible(const Item *item) const
{
    QMutexLocker locker(getMutex());

    PermanentItemInformation *info = m_itemsWithInfo.value((Item*)item, NULL);

    if(info != NULL)
        return info->isVisible();

    return false;
}

void PermanentItemScene::updateItem(const Item *item, GraphicsView& graphicsView, const QColor& color)
{
    QMutexLocker locker(getMutex());

    m_itemsRemover->addItemToUpdate(item, graphicsView, color);
}

void PermanentItemScene::removeItem(const Item *item)
{
    QMutexLocker locker(getMutex());

    m_itemsRemover->addItemToRemove(item);
}

Item* PermanentItemScene::getItemAt(const int &index)
{
    //QMutexLocker locker(getMutex());

    return m_items.at(index);
}

PermanentItemInformation* PermanentItemScene::getPermanentItemInformation(const Item *item) const
{
    QMutexLocker locker(getMutex());

    return m_itemsWithInfo.value((Item*)item, NULL);
}

size_t PermanentItemScene::countPoints() const
{
    return m_objectsScene->countPoints();
}

void PermanentItemScene::setMaxNumberOfPointsToDrawInFastDraw(const size_t &maxNumber)
{
    m_objectsScene->setMaxNumberOfPointsToDrawInFastDraw(maxNumber);
}

size_t PermanentItemScene::getMaxNumberOfPointsToDrawInFastDraw() const
{
    return m_objectsScene->getMaxNumberOfPointsToDrawInFastDraw();
}

void PermanentItemScene::preDraw(DrawInfo &info)
{
    if(isVisible()) {
        emit anyDrawBegin();

        m_objectsScene->preDraw(info);

        emit anyDrawEnd();
    }
}

void PermanentItemScene::draw(DrawInfo &info)
{
    if(isVisible()) {
        emit anyDrawBegin();

        m_objectsScene->draw(info);

        emit anyDrawEnd();
    }
}

void PermanentItemScene::fastDraw(DrawInfo &info)
{
    if(isVisible()) {
        emit anyDrawBegin();

        m_objectsScene->fastDraw(info);

        emit anyDrawEnd();
    }
}

void PermanentItemScene::postDraw(DrawInfo &info)
{
    if(isVisible()) {
        emit anyDrawBegin();

        m_objectsScene->postDraw(info);

        emit anyDrawEnd();
    }
}

QMutex* PermanentItemScene::getMutex() const
{
    return m_mutex;
}

PermanentItemSceneRemover* PermanentItemScene::getPermanentItemSceneRemover() const
{
    return m_itemsRemover;
}

void PermanentItemScene::addItemUpdated(const Item *item, GraphicsView &graphicsView, const quint32 &uniqueIndexInList, const QColor& color)
{
    internalAddItem(item, graphicsView, uniqueIndexInList, color);
}

bool PermanentItemScene::internalAddItem(const Item *item, GraphicsView &graphicsView, const quint32 &uniqueIndexInList, const QColor& defaultObjectsColor)
{
    QMutexLocker locker(m_mutex);

    if(m_itemsWithInfo.contains((Item*)item))
        return false;

    PermanentItemInformation *info = new PermanentItemInformation(uniqueIndexInList);

    AMKgl::ItemPermanentPainter painter(graphicsView);
    painter.setItemInformation(info);
    painter.setPermanentScene(this->getPermanentScene());
    painter.setColor(defaultObjectsColor);

    ((Item*)item)->draw(graphicsView, painter);

    m_itemsWithInfo.insert((Item*)item, info);

    return true;
}

void PermanentItemScene::internalRemoveItem(const Item *item)
{
    QMutexLocker locker(m_mutex);

    disconnect(item, NULL, this, NULL);
}

PermanentItemInformation* PermanentItemScene::takePermanentItemInformation(const Item *item)
{
    return m_itemsWithInfo.take((Item*)item);
}

bool PermanentItemScene::mustBeUpdated() const
{
    QMutexLocker locker(m_mutex);

    if(m_itemsRemover->mustBeLaunched())
        return true;

    return getPermanentScene()->mustBeUpdated();
}

void PermanentItemScene::itemSelectionChanged(bool selected, Item *item)
{
    PermanentItemInformation* info = m_itemsWithInfo.value(item, NULL);

    if(selected)
        ItemsPickingTools::staticApplyOperationToObjectOfAnItem(item,
                                                                info,
                                                                getAnyElementsAttributesProvider(),
                                                                ItemsPickingTools::staticSelectItem);
    else
        ItemsPickingTools::staticApplyOperationToObjectOfAnItem(item,
                                                                info,
                                                                getAnyElementsAttributesProvider(),
                                                                ItemsPickingTools::staticUnselectItem);
}
