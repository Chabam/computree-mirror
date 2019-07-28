#include "permanentitemscenebymodel.h"

#include "includemodel.h"
#include "includeitem.h"

#include "visitor/iglobalpointsvisitorbymodel.h"
#include "visitor/iitemsvisitorbymodel.h"
#include "visitor/iobjectsvisitorbymodel.h"

PermanentItemSceneByModel::PermanentItemSceneByModel()
{
    m_mutex = new QMutex(QMutex::Recursive);
    m_autoDeleteItem = false;
    m_maxNumberOfPointsInFastDrawMode = 200000;
    m_mustReduceNumberOfPointsInFastDraw = true;
    m_mustReduceNumberOfPointsInNormalDraw = false;

    setDrawModeToUse(DM_VAO);
}

PermanentItemSceneByModel::~PermanentItemSceneByModel()
{
    removeAllItems();

    delete m_mutex;
}

PermanentItemScene* PermanentItemSceneByModel::getPermanentItemSceneForModel(const ItemModel *model) const
{
    QMutexLocker locker(m_mutex);

    return m_scenes.value((ItemModel*)model, nullptr);
}

PermanentItemScene* PermanentItemSceneByModel::findPermanentItemSceneForItem(const Item* item) const
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(m_scenes);

    while(it.hasNext()) {
        it.next();

        if(it.key()->hasSameModelAsItem(item))
            return it.value();
    }

    return nullptr;
}

void PermanentItemSceneByModel::addedTo(const IGraphicsDocument* doc, const QOpenGLContext* newContext)
{
    QMutexLocker locker(m_mutex);

    Q_ASSERT(newContext != nullptr);

    SceneCollectionIterator it(getPermanentItemScenes());

    while(it.hasNext()) {
        it.next();

        it.value()->addedTo(doc, newContext);
    }

    IContextAccessor<DummyContext>::addedTo(doc, newContext);
}

void PermanentItemSceneByModel::removedFrom(const IGraphicsDocument* doc, const QOpenGLContext* newContext)
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(getPermanentItemScenes());

    while(it.hasNext()) {
        it.next();

        it.value()->removedFrom(doc, newContext);
    }

    IContextAccessor<DummyContext>::removedFrom(doc, newContext);
}

bool PermanentItemSceneByModel::mustBeUpdated() const
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(getPermanentItemScenes());

    while(it.hasNext()) {
        it.next();

        if(it.value()->mustBeUpdated())
            return true;
    }

    return false;
}

void PermanentItemSceneByModel::checkAndUpdateIfItNeedsToBe()
{
    if(mustBeUpdated())
        updateInAllContext();
}

void PermanentItemSceneByModel::updateInAllContext()
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(getPermanentItemScenes());

    while(it.hasNext()) {
        it.next();

        it.value()->updateInAllContext();
    }
}

void PermanentItemSceneByModel::updatePointsInfoInAllContext()
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(getPermanentItemScenes());

    while(it.hasNext()) {
        it.next();

        it.value()->updatePointsInfoInAllContext();
    }
}

void PermanentItemSceneByModel::updatePointsColorInAllContext()
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(getPermanentItemScenes());

    while(it.hasNext()) {
        it.next();

        it.value()->updatePointsColorInAllContext();
    }
}

void PermanentItemSceneByModel::updatePointsNormalInAllContext()
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(getPermanentItemScenes());

    while(it.hasNext()) {
        it.next();

        it.value()->updatePointsNormalInAllContext();
    }
}

void PermanentItemSceneByModel::updateItemsInfoInAllContext()
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(getPermanentItemScenes());

    while(it.hasNext()) {
        it.next();

        it.value()->updateItemsInfoInAllContext();
    }
}

void PermanentItemSceneByModel::updateItemsColorInAllContext()
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(getPermanentItemScenes());

    while(it.hasNext()) {
        it.next();

        it.value()->updateItemsColorInAllContext();
    }
}

void PermanentItemSceneByModel::updateInfoInAllContext()
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(getPermanentItemScenes());

    while(it.hasNext()) {
        it.next();

        it.value()->updateInfoInAllContext();
    }
}

void PermanentItemSceneByModel::updateColorInAllContext()
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(getPermanentItemScenes());

    while(it.hasNext()) {
        it.next();

        it.value()->updateColorInAllContext();
    }
}

void PermanentItemSceneByModel::setDrawModeToUse(DrawMode mode)
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(getPermanentItemScenes());

    while(it.hasNext()) {
        it.next();

        it.value()->setDrawModeToUse(mode);
    }

    for(int i=0; i<(int)Scene::NumberOfElements; ++i) {
        m_drawModeByObjectType.insert((Scene::ObjectType)i, mode);
    }
}

void PermanentItemSceneByModel::setDrawModeToUse(Scene::ObjectType type, DrawMode mode)
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(getPermanentItemScenes());

    while(it.hasNext()) {
        it.next();

        it.value()->setDrawModeToUse(type, mode);
    }

    m_drawModeByObjectType.insert(type, mode);
}

void PermanentItemSceneByModel::setDrawModeToUse(const DrawModeAccessor &accessor)
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(getPermanentItemScenes());

    while(it.hasNext()) {
        it.next();

        for(int i=0; i<(int)Scene::NumberOfElements; ++i) {
            it.value()->setDrawModeToUse((Scene::ObjectType)i, accessor.getDrawModeToUse((Scene::ObjectType)i));
        }
    }

    for(int i=0; i<(int)Scene::NumberOfElements; ++i) {
        m_drawModeByObjectType.insert((Scene::ObjectType)i, accessor.getDrawModeToUse((Scene::ObjectType)i));
    }
}

DrawMode PermanentItemSceneByModel::getDrawModeToUse(Scene::ObjectType type) const
{
    return m_drawModeByObjectType.value(type, DM_NONE);
}

DrawModeAccessor &PermanentItemSceneByModel::operator =(const DrawModeAccessor &other)
{
    int size = Scene::NumberOfElements;

    for(int i=0; i<size; ++i)
        setDrawModeToUse((Scene::ObjectType)i, other.getDrawModeToUse((Scene::ObjectType)i));

    return *this;
}

void PermanentItemSceneByModel::setMustReduceNumberOfPointsInFastDraw(bool enabled)
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(getPermanentItemScenes());

    while(it.hasNext()) {
        it.next();

        it.value()->setMustReduceNumberOfPointsInFastDraw(enabled);
    }

    m_mustReduceNumberOfPointsInFastDraw = enabled;
}

void PermanentItemSceneByModel::setMustReduceNumberOfPointsInNormalDraw(bool enabled)
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(getPermanentItemScenes());

    while(it.hasNext()) {
        it.next();

        it.value()->setMustReduceNumberOfPointsInNormalDraw(enabled);
    }

    m_mustReduceNumberOfPointsInNormalDraw = enabled;
}

bool PermanentItemSceneByModel::mustReduceNumberOfPointsInFastDraw() const
{
    return m_mustReduceNumberOfPointsInFastDraw;
}

bool PermanentItemSceneByModel::mustReduceNumberOfPointsInNormalDraw() const
{
    return m_mustReduceNumberOfPointsInNormalDraw;
}

void PermanentItemSceneByModel::destroyGL(const QOpenGLContext *context)
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(getPermanentItemScenes());

    while(it.hasNext()) {
        it.next();

        it.value()->destroyGL(context);
    }

}

AnyElementAttributesProvider* PermanentItemSceneByModel::getAnyElementsAttributesProvider(const Item *item) const
{
    QMutexLocker locker(m_mutex);

    // TODO : it was a problem here : when itemdrawable was copied it is unable to know if the model return by the
    //        "model()" method was the model used to add it !
    PermanentItemScene* scene = findPermanentItemSceneForItem(item);

    if(scene == nullptr)
        return nullptr;

    return scene->getAnyElementsAttributesProvider();
}

AnyElementAttributesProvider *PermanentItemSceneByModel::getAnyElementsAttributesProvider(const ItemModel *model) const
{
    QMutexLocker locker(m_mutex);

    PermanentItemScene* scene = getPermanentItemSceneForModel(model);

    if(scene == nullptr)
        return nullptr;

    return scene->getAnyElementsAttributesProvider();
}

ObjectAttributesProvider* PermanentItemSceneByModel::getObjectsAttributeProvider(Scene::ObjectType type, const Item *item) const
{
    QMutexLocker locker(m_mutex);

    // TODO : it was a problem here : when itemdrawable was copied it is unable to know if the model return by the
    //        "model()" method was the model used to add it !
    PermanentItemScene* scene = findPermanentItemSceneForItem(item);

    if(scene == nullptr)
        return nullptr;

    return scene->getObjectsAttributeProvider(type);
}

ObjectAttributesProvider* PermanentItemSceneByModel::getObjectsAttributeProvider(Scene::ObjectType type, const ItemModel *model) const
{
    QMutexLocker locker(m_mutex);

    PermanentItemScene* scene = getPermanentItemSceneForModel(model);

    if(scene == nullptr)
        return nullptr;

    return scene->getObjectsAttributeProvider(type);
}

void PermanentItemSceneByModel::addItemsFromModel(const ItemModel* model, GraphicsView &graphicsView)
{
    QMutexLocker locker(m_mutex);

    // TODO : be carreful because it was a problem here : the model used may be different from the model returned by
    //        the "model()" method of the Item class (when it was copied) ! So we must always used
    //        a model to add/remove/etc... item drawable for the moment. The developper must change this system in
    //        pluginshared. One itemdrawable "copied" must have a different model than the original itemdrawable.
    PermanentItemScene& scene = createOrGetPermanentItemSceneForModel(model);

    ItemModel::const_iterator it = model->begin();
    ItemModel::const_iterator end = model->end();

    while(it != end) {
        const Item* item = *it;

        scene.addItem(item, graphicsView);
        ++it;
    }
}

int PermanentItemSceneByModel::countItemsOfModel(const ItemModel *model) const
{
    QMutexLocker locker(m_mutex);

    PermanentItemScene* scene = getPermanentItemSceneForModel(model);

    if(scene == nullptr)
        return 0;

    return scene->countItem();
}

void PermanentItemSceneByModel::setItemsOfModelVisible(const ItemModel *model, bool visible)
{
    QMutexLocker locker(m_mutex);
    PermanentItemScene* scene = getPermanentItemSceneForModel(model);

    if(scene == nullptr)
        return;

    scene->setVisible(visible);
}

void PermanentItemSceneByModel::updateItemsFromModel(const ItemModel *model, GraphicsView &graphicsView)
{
    QMutexLocker locker(m_mutex);

    // TODO : be carreful because it was a problem here : the model used may be different from the model returned by
    //        the "model()" method of the Item class (when it was copied) ! So we must always used
    //        a model to add/remove/etc... item drawable for the moment. The developper must change this system in
    //        pluginshared. One itemdrawable "copied" must have a different model than the original itemdrawable.
    PermanentItemScene& scene = createOrGetPermanentItemSceneForModel(model);

    ItemModel::const_iterator it = model->begin();
    ItemModel::const_iterator end = model->end();

    while(it != end) {
        const Item* item = *it;

        scene.updateItem(item, graphicsView, Qt::white); // TODO : get the color !
        ++it;
    }
}

void PermanentItemSceneByModel::removeItemsFromModel(const ItemModel *model)
{
    QMutexLocker locker(m_mutex);

    delete m_scenes.take((ItemModel*)model);
}

void PermanentItemSceneByModel::removeAllItems()
{
    QMutexLocker locker(m_mutex);

    qDeleteAll(m_scenes.begin(), m_scenes.end());
    m_scenes.clear();
}

QSet<Item *> PermanentItemSceneByModel::getAllItems() const
{
    QMutexLocker locker(m_mutex);

    QSet<Item *> l;

    SceneCollectionIterator it(m_scenes);

    while(it.hasNext()) {
        it.next();

        PermanentItemScene* scene = it.value();

        int count = scene->countItem();

        for(int i=0; i<count; ++i)
            l.insert(scene->getItemAt(i));
    }

    return l;
}

int PermanentItemSceneByModel::countAllItems() const
{
    QMutexLocker locker(m_mutex);

    int n = 0;

    SceneCollectionIterator it(m_scenes);

    while(it.hasNext()) {
        it.next();
        n += it.value()->countItem();
    }

    return n;
}

void PermanentItemSceneByModel::visitPoints(IGlobalPointsVisitorByModel &visitor)
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(m_scenes);

    while(it.hasNext() && visitor.mustContinueVisit()) {
        it.next();

        if(it.value()->isVisible()) {
            visitor.setCurrentInformation(it.key(), it.value());
            it.value()->visitPoints(visitor);
        }
    }
}

void PermanentItemSceneByModel::visitItems(IItemsVisitorByModel &visitor)
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(m_scenes);

    while(it.hasNext() && visitor.mustContinueVisit()) {
        it.next();

        if(it.value()->isVisible()) {
            visitor.setCurrentInformation(it.key(), it.value());
            it.value()->visitItems(visitor);
        }
    }
}

void PermanentItemSceneByModel::visitOneItem(const Item *item, IItemsVisitorByModel &visitor)
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(m_scenes);

    while(it.hasNext() && visitor.mustContinueVisit()) {
        it.next();

        if(it.value()->isVisible()) {
            if(it.value()->containsItem(item)) {
                visitor.setCurrentInformation(it.key(), it.value());
                visitor.addItemDrawableToCompute(item);
            }
        }
    }
}

void PermanentItemSceneByModel::visitObjects(IObjectsVisitorByModel &visitor)
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(m_scenes);

    while(it.hasNext() && visitor.mustContinueVisit()) {
        it.next();

        visitor.setModel(it.key());

        PermanentItemScene* scene = it.value();

        if(scene->isVisible()) {
            visitor.setScene(scene);
            scene->visitObjects(visitor);
        }
    }
}

void PermanentItemSceneByModel::visitObjectsOfTypes(const QList<Scene::ObjectType> &types, IObjectsVisitorByModel &visitor)
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(m_scenes);

    while(it.hasNext() && visitor.mustContinueVisit()) {
        it.next();

        visitor.setModel(it.key());

        PermanentItemScene* scene = it.value();

        if(scene->isVisible()) {
            visitor.setScene(scene);
            scene->visitObjectsOfTypes(types, visitor);
        }
    }
}

void PermanentItemSceneByModel::visitObjectsOfOneItem(const Item *item, IObjectsVisitorByModel &visitor)
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(m_scenes);

    while(it.hasNext() && visitor.mustContinueVisit()) {
        it.next();

        visitor.setModel(it.key());

        PermanentItemScene* scene = it.value();

        if(scene->isVisible()) {
            visitor.setScene(scene);
            visitor.setItem(item);
            scene->visitObjectsOfOneItem(item, visitor);
        }
    }
}

void PermanentItemSceneByModel::visitObjectsOfTypesOfOneItem(const Item *item,
                                                             const QList<Scene::ObjectType> &types,
                                                             IObjectsVisitorByModel &visitor)
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(m_scenes);

    while(it.hasNext() && visitor.mustContinueVisit()) {
        it.next();

        visitor.setModel(it.key());

        PermanentItemScene* scene = it.value();

        if(scene->isVisible()) {
            visitor.setScene(scene);
            visitor.setItem(item);
            scene->visitObjectsOfTypesOfOneItem(item, types, visitor);
        }
    }
}

void PermanentItemSceneByModel::setAutoDeleteItems(bool enable)
{
    QMutexLocker locker(m_mutex);

    m_autoDeleteItem = enable;

    SceneCollectionIterator it(getPermanentItemScenes());

    while(it.hasNext()) {
        it.next();

        it.value()->setAutoDeleteItems(enable);
    }
}

bool PermanentItemSceneByModel::mustAutoDeleteItems() const
{
    return m_autoDeleteItem;
}

void PermanentItemSceneByModel::addItem(const Item *item, const ItemModel *model, GraphicsView& graphicsView)
{
    QMutexLocker locker(m_mutex);

    PermanentItemScene& scene = createOrGetPermanentItemSceneForModel(model);

    scene.addItem(item, graphicsView);
}

void PermanentItemSceneByModel::removeItem(const Item *item, const ItemModel *model)
{
    QMutexLocker locker(m_mutex);

    PermanentItemScene& scene = createOrGetPermanentItemSceneForModel(model);

    scene.removeItem(item);
}

const PermanentItemSceneByModel::SceneCollection &PermanentItemSceneByModel::getPermanentItemScenes() const
{
    return m_scenes;
}

size_t PermanentItemSceneByModel::countPoints() const
{
    QMutexLocker locker(m_mutex);

    size_t total = 0;

    SceneCollectionIterator it(getPermanentItemScenes());

    while(it.hasNext()) {
        it.next();

        total += it.value()->countPoints();
    }

    return total;
}

void PermanentItemSceneByModel::setMaxNumberOfPointsToDrawInFastDraw(const size_t &maxNumber)
{
    QMutexLocker locker(m_mutex);

    m_maxNumberOfPointsInFastDrawMode = qMax(maxNumber, size_t(1));

    size_t total = 0;
    QHash<ItemModel*, size_t> nPoints;

    SceneCollectionIterator it(getPermanentItemScenes());

    while(it.hasNext()) {
        it.next();
        const size_t n = it.value()->countPoints();

        nPoints.insert(it.key(), n);
        total += n;
    }

    it.toFront();

    while(it.hasNext()) {
        it.next();

        const size_t n = nPoints.value(it.key(), 0);
        double ratio = 1;

        if(total > 0)
            ratio = ((double)n)/((double)total);

        it.value()->setMaxNumberOfPointsToDrawInFastDraw(maxNumber*ratio);
    }
}

size_t PermanentItemSceneByModel::getMaxNumberOfPointsToDrawInFastDraw() const
{
    return m_maxNumberOfPointsInFastDrawMode;
}

void PermanentItemSceneByModel::preDraw(DrawInfo &info)
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(getPermanentItemScenes());

    while(it.hasNext()) {
        it.next();
        it.value()->preDraw(info);
    }
}

void PermanentItemSceneByModel::draw(DrawInfo &info)
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(getPermanentItemScenes());

    while(it.hasNext()) {
        it.next();
        it.value()->draw(info);
    }
}

void PermanentItemSceneByModel::fastDraw(DrawInfo &info)
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(getPermanentItemScenes());

    while(it.hasNext()) {
        it.next();
        it.value()->fastDraw(info);
    }
}

void PermanentItemSceneByModel::postDraw(DrawInfo &info)
{
    QMutexLocker locker(m_mutex);

    SceneCollectionIterator it(getPermanentItemScenes());

    while(it.hasNext()) {
        it.next();
        it.value()->postDraw(info);
    }
}

PermanentItemScene& PermanentItemSceneByModel::createOrGetPermanentItemSceneForModel(const ItemModel *model)
{
    QMutexLocker locker(m_mutex);

    PermanentItemScene* scene = m_scenes.value((ItemModel*)model, nullptr);

    if(scene == nullptr)
    {
        Q_ASSERT_X(QThread::currentThread() == qApp->thread(), "PermanentItemSceneByModel", "You cannot create a scene in a different thread that the main thread !");

        scene = new PermanentItemScene();

        connect(scene, SIGNAL(wasEmptied()), this, SLOT(removeScene()), Qt::QueuedConnection);
        connect(scene, SIGNAL(changesHaveOccured()), this, SIGNAL(changesHaveOccured()), Qt::DirectConnection);
        connect(scene, SIGNAL(updateNecessary()), this, SIGNAL(updateNecessary()), Qt::DirectConnection);

        ContextCollectionIterator it(getContexts());

        while(it.hasNext()) {
            it.next();

            scene->addedTo(it.value()->getDocument(), it.key());
        }

        scene->setAutoDeleteItems(mustAutoDeleteItems());

        for(int i=0; i<(int)Scene::NumberOfElements; ++i) {
            scene->setDrawModeToUse((Scene::ObjectType)i, m_drawModeByObjectType.value((Scene::ObjectType)i, DM_VAO));
        }

        scene->setMustReduceNumberOfPointsInFastDraw(mustReduceNumberOfPointsInFastDraw());
        scene->setMustReduceNumberOfPointsInNormalDraw(mustReduceNumberOfPointsInNormalDraw());

        m_scenes.insert((ItemModel*)model, scene);

        setMaxNumberOfPointsToDrawInFastDraw(m_maxNumberOfPointsInFastDrawMode);
    }

    return *scene;
}

void PermanentItemSceneByModel::removeScene()
{
    QMutexLocker locker(m_mutex);

    PermanentItemScene* scene = (PermanentItemScene*)sender();

    if(scene->countItem() == 0) {
        SceneCollectionMutableIterator it(m_scenes);

        while(it.hasNext()) {
            it.next();

            if(it.value() == scene) {
                it.remove();
                delete scene;
                return;
            }
        }
    }
}
