#ifndef PERMANENTITEMSCENEBYMODEL_H
#define PERMANENTITEMSCENEBYMODEL_H

#include "permanentitemscene.h"
#include "renderer/context/icontextaccessor.h"
#include "renderer/drawmodeaccessor.h"

#include "definemodel.h"
#include "definegraphicsview.h"

class IGlobalPointsVisitorByModel;
class IItemsVisitorByModel;
class IObjectsVisitorByModel;

/**
 * @brief Scene to manage item in one or multiple graphics view but sorted by model
 */
class PermanentItemSceneByModel : public QObject, public IContextAccessor<DummyContext>, public DrawModeAccessor
{
    Q_OBJECT

public:
    typedef QHash<ItemModel*, PermanentItemScene*>                  SceneCollection;
    typedef QHashIterator<ItemModel*, PermanentItemScene*>          SceneCollectionIterator;
    typedef QMutableHashIterator<ItemModel*, PermanentItemScene*>   SceneCollectionMutableIterator;

    PermanentItemSceneByModel();
    ~PermanentItemSceneByModel();

    /**
     * @brief Returns, if exist, the scene for the specified model otherwise return nullptr
     */
    PermanentItemScene* getPermanentItemSceneForModel(const ItemModel* model) const;
    Q_DECL_DEPRECATED PermanentItemScene* findPermanentItemSceneForItem(const Item* item) const;

    /**
     * @brief Returns, if exist (if not create it), the scene for the specified model
     */
    PermanentItemScene& createOrGetPermanentItemSceneForModel(const ItemModel *model);

    /**
     * @brief Call this method when you add this scene to
     *        your graphics view and pass it your opengl context
     */
    void addedTo(const IGraphicsDocument *doc, const QOpenGLContext* newContext);

    /**
     * @brief Call this method when you remove this scene from
     *        your graphics view and pass it your opengl context
     */
    void removedFrom(const IGraphicsDocument* doc, const QOpenGLContext* newContext);

    /**
     * @brief Returns true if at least one scene is not up to date !
     */
    bool mustBeUpdated() const;

    /**
     * @brief Check if must be updated and update it if it was not up to date.
     */
    void checkAndUpdateIfItNeedsToBe();

public slots:

    /**
     * @brief Call this method to update all elements in all context. It will create all
     *        necessary VBO, IBO, etc... to draw it
     */
    void updateInAllContext();

    /**
     * @brief Call this method to update infos of points in all context. It will update
     *        necessary VBO, etc... to draw it
     */
    void updatePointsInfoInAllContext();

    /**
     * @brief Call this method to update colors of points in all context. It will update
     *        necessary VBO, etc... to draw it
     */
    void updatePointsColorInAllContext();

    /**
     * @brief Call this method to update normals of points in all context. It will update
     *        necessary VBO, etc... to draw it
     */
    void updatePointsNormalInAllContext();

    /**
     * @brief Call this method to update infos of items in all context. It will update
     *        necessary VBO, etc... to draw it
     */
    void updateItemsInfoInAllContext();

    /**
     * @brief Call this method to update colors of items in all context. It will update
     *        necessary VBO, etc... to draw it
     */
    void updateItemsColorInAllContext();

    /**
     * @brief Call this method to update infos of items/points/etc... in all context. It will update
     *        necessary VBO, etc... to draw it
     */
    void updateInfoInAllContext();

    /**
     * @brief Call this method to update colors of items/points/etc... in all context. It will update
     *        necessary VBO, etc... to draw it
     */
    void updateColorInAllContext();

public:

    /**
     * @brief Define the draw mode to use for all type. If you set it to VBO or VAO and the graphics card
     *        cannot do it, it will be changed to RAW after update.
     */
    void setDrawModeToUse(DrawMode mode);

    /**
     * @brief Define the draw mode to use by type. If you set it to VBO or VAO and the graphics card
     *        cannot do it, it will be changed to RAW after update.
     */
    void setDrawModeToUse(Scene::ObjectType type, DrawMode mode);

    /**
     * @brief Define the draw mode to use by type. If you set it to VBO or VAO and the graphics card
     *        cannot do it, it will be changed to RAW after update.
     */
    void setDrawModeToUse(const DrawModeAccessor &accessor);

    /**
     * @brief Returns the draw mode to use for the type specified.
     * @warning It is not necessary the draw mode used !
     */
    DrawMode getDrawModeToUse(Scene::ObjectType type) const;

    /**
     * @brief Set draw modes from another accessor
     */
    DrawModeAccessor& operator =(const DrawModeAccessor& other);

    /**
     * @brief Set to true if this scene must reduce number of points when method "fastDraw(...)" is called, false otherwise
     */
    void setMustReduceNumberOfPointsInFastDraw(bool enabled);

    /**
     * @brief Set to true if this scene must reduce number of points when method "draw(...)" is called, false otherwise
     */
    void setMustReduceNumberOfPointsInNormalDraw(bool enabled);

    /**
     * @brief Returns true if this scene must reduce number of points when method "fastDraw(...)" is called, false otherwise
     */
    bool mustReduceNumberOfPointsInFastDraw() const;

    /**
     * @brief Returns true if this scene must reduce number of points when method "draw(...)" is called, false otherwise
     */
    bool mustReduceNumberOfPointsInNormalDraw() const;

    /**
     * @brief Returns the draw mode used for the type specified.
     * @warning If it was no point in the renderer the draw mode used can be DM_NONE
     */
    //DrawMode getDrawModeUsed(Scene::ObjectType type) const;

    /**
     * @brief Call it when you want to destroy all opengl elements (when you close your window)
     */
    void destroyGL(const QOpenGLContext *context);

    /**********
     * PROVIDERS
     **********/

    /**
     * @brief Returns the attributes provider of any elements
     */
    AnyElementAttributesProvider* getAnyElementsAttributesProvider(const Item* item) const;
    AnyElementAttributesProvider* getAnyElementsAttributesProvider(const ItemModel* model) const;

    /**
     * @brief Returns the provider of attributes of an element
     * @param type : type of element that you want
     */
    ObjectAttributesProvider* getObjectsAttributeProvider(Scene::ObjectType type, const Item* item) const;
    ObjectAttributesProvider* getObjectsAttributeProvider(Scene::ObjectType type, const ItemModel* model) const;

    /**********
     * MODELS/SCENES
     **********/

    /**
     * @brief Add all items of the model to this scene
     * @param model : model to iterate
     */
    void addItemsFromModel(const ItemModel *model, GraphicsView &graphicsView);

    /**
     * @brief Returns the number of all items of the model present in this scene
     */
    int countItemsOfModel(const ItemModel* model) const;

    /**
     * @brief Show/Hide all items of the model. If you want to hide it : all items is always present in the view but not drawn.
     * @param model :  model to iterate
     * @param visible : true to set visible, false to hide it
     */
    void setItemsOfModelVisible(const ItemModel* model, bool visible);

    /**
     * @brief Update all items of the model from this scene
     * @param model : model to iterate
     */
    void updateItemsFromModel(const ItemModel* model, GraphicsView &graphicsView);

    /**
     * @brief Remove all items of the model from this scene
     * @param model : model to iterate
     */
    void removeItemsFromModel(const ItemModel* model);

    /**
     * @brief Remove all items from this scene
     */
    void removeAllItems();

    /**
     * @brief Returns all items present in this scene
     */
    QSet<Item*> getAllItems() const;

    /**
     * @brief Returns the total numbers of items
     */
    int countAllItems() const;

    /**********
     * VISITING
     **********/

    /**
     * @brief Call to visit points
     * @param visitor : is an object that will receive all points
     */
    void visitPoints(IGlobalPointsVisitorByModel &visitor);

    /**
     * @brief Call to visit items
     * @param visitor : is an object that will compute all shapes draw by items
     */
    void visitItems(IItemsVisitorByModel &visitor);

    /**
     * @brief Call this method if you want to visit an item but in all scenes (per example if it was
     *        present in multiple Model). This can be useful to set a color to the item per example.
     */
    void visitOneItem(const Item *item, IItemsVisitorByModel &visitor);

    /**
     * @brief Call to visit all objects
     * @param visitor : is an object that will receive all objects
     */
    void visitObjects(IObjectsVisitorByModel& visitor);

    /**
     * @brief Call to visit objects of specified type
     * @param types : list of objects to visit
     * @param visitor : is an object that will receive all objects
     */
    void visitObjectsOfTypes(const QList<Scene::ObjectType>& types, IObjectsVisitorByModel& visitor);

    /**
     * @brief Call to visit all objects
     * @param visitor : is an object that will receive all objects
     */
    void visitObjectsOfOneItem(const Item *item, IObjectsVisitorByModel& visitor);

    /**
     * @brief Call to visit objects of specified type
     * @param types : list of objects to visit
     * @param visitor : is an object that will receive all objects
     */
    void visitObjectsOfTypesOfOneItem(const Item *item, const QList<Scene::ObjectType>& types, IObjectsVisitorByModel& visitor);

    /**********
     * ITEMS
     **********/

    /**
     * @brief Set if must auto delete items or not (true by default)
     */
    void setAutoDeleteItems(bool enable);

    /**
     * @brief Returns true if must auto delete items
     */
    bool mustAutoDeleteItems() const;

    /**
     * @brief Add one item to this scene
     * @param item : item to add
     * @param model : model
     * @warning the model must not be the model returned by "getModel" method of the itemdrawable !
     */
    void addItem(const Item *item, const ItemModel *model, GraphicsView &graphicsView);

    /**
     * @brief Remove an item from this scene
     * @param item : item to remove
     * @param model : model
     * @warning the model must not be the model returned by "getModel" method of the itemdrawable !
     */
    void removeItem(const Item *item, const ItemModel *model);

    /**
     * @brief Returns all permanent items scenes
     */
    const SceneCollection& getPermanentItemScenes() const;

    /**********
     * POINTS
     **********/

    /**
     * @brief Returns the total number of points in this scene
     */
    size_t countPoints() const;

    /**
     * @brief Set the maximum number of points to draw when you call the method "fastDraw"
     * @param maxNumber : the maximum number of points
     */
    void setMaxNumberOfPointsToDrawInFastDraw(const size_t &maxNumber);

    /**
     * @brief Returns the maximum number of points to draw when you call the method "fastDraw"
     */
    size_t getMaxNumberOfPointsToDrawInFastDraw() const;

    /**********
     * DRAW
     **********/

    /**
     * @brief Prepare necessary element before draw
     */
    void preDraw(DrawInfo &info);

    /**
     * @brief Draw all normally
     */
    void draw(DrawInfo &info);

    /**
     * @brief Draw all faster
     */
    void fastDraw(DrawInfo &info);

    /**
     * @brief Draw text or other elements
     */
    void postDraw(DrawInfo &info);

private:

    QMutex*                             m_mutex;
    SceneCollection                     m_scenes;
    bool                                m_autoDeleteItem;
    size_t                              m_maxNumberOfPointsInFastDrawMode;

    /**
     * @brief Set if this scene must reduce number of points when method fastDraw() is called
     */
    bool                                m_mustReduceNumberOfPointsInFastDraw;

    /**
     * @brief Set if this scene must reduce number of points when method draw() is called
     */
    bool                                m_mustReduceNumberOfPointsInNormalDraw;

    QHash<Scene::ObjectType, DrawMode>  m_drawModeByObjectType;

private slots:

    /**
     * @brief Called when a scene was emptied
     */
    void removeScene();

signals:
    /**
     * @brief Emitted when changes have occured (add/remove/clear points, change max points, etc...)
     */
    void changesHaveOccured();

    /**
     * @brief Emitted when update was called in a thread different that the main thread
     */
    void updateNecessary();
};

#endif // PERMANENTITEMSCENEBYMODEL_H
