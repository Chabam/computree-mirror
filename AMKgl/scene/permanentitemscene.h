#ifndef PERMANENTITEMSCENE_H
#define PERMANENTITEMSCENE_H

#include "renderer/drawmode.h"
#include "renderer/attributes/iattributesaccessor.h"
#include "tools/permanentiteminformation.h"
#include "tools/objectattributesprovider.h"
#include "tools/anyelementattributesprovider.h"

#include "defineitem.h"
#include "definegraphicsview.h"

#include <QMultiHash>

class PermanentItemSceneRemover;
class PermanentScene;
class IGraphicsDocument;
class DrawInfo;
class QOpenGLContext;
class IGlobalPointsVisitor;
class IItemsVisitor;
class IObjectsVisitor;

using namespace AMKgl;

/**
 * @brief Scene to manage item in one or multiple graphics view
 */
class PermanentItemScene : public QObject, public IAttributesAccessor
{
    Q_OBJECT

public:
    typedef QHash<Item*, PermanentItemInformation*>         ItemCollection;
    typedef QHashIterator<Item*, PermanentItemInformation*> ItemCollectionIterator;

    typedef QList<Item*>                                    ItemLCollection;
    typedef QListIterator<Item*>                            ItemLCollectionIterator;

    PermanentItemScene();
    virtual ~PermanentItemScene();

    /**
     * @brief Returns the permanent scene used
     */
    PermanentScene* getPermanentScene() const;

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
     * @brief Lock it when you want to add/remove/count in a thread
     */
    void lock();

    /**
     * @brief Unlock it when you have finished
     */
    void unlock();

    /**
     * @brief Returns the point cloud attributes provider
     */
    PointCloudAttributesProvider* getPointCloudAttributesProvider() const;

    /**
     * @brief Returns the face cloud attributes provider
     */
    FaceCloudAttributesProvider* getFaceCloudAttributesProvider() const;

    /**
     * @brief Returns the edge cloud attributes provider
     */
    EdgeCloudAttributesProvider* getEdgeCloudAttributesProvider() const;

    /**
     * @brief Returns true if at least one renderer in the scene is not up to date !
     */
    bool mustBeUpdated() const;

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

    /**
     * @brief Set visible or not (if not the "XXXdraw" method don't draw anything !
     */
    void setVisible(bool visible);

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
     * @brief Call it when you want to destroy all opengl elements (when you close your window)
     */
    void destroyGL(const QOpenGLContext *context);

    /**
     * @brief Returns true if this scene is visible
     */
    bool isVisible() const;

    /**********
     * PROVIDERS
     **********/

    /**
     * @brief Returns the attributes provider of any elements
     */
    AnyElementAttributesProvider* getAnyElementsAttributesProvider() const;

    /**
     * @brief Returns the provider of attributes of an element
     * @param type : type of element that you want
     */
    ObjectAttributesProvider* getObjectsAttributeProvider(Scene::ObjectType type) const;

    /**********
     * VISITING
     **********/

    /**
     * @brief Call to visit points
     * @param visitor : is an object that will receive all points
     */
    void visitPoints(IGlobalPointsVisitor &visitor);

    /**
     * @brief Call to visit items
     * @param visitor : is an object that will compute all shapes draw by items
     */
    void visitItems(IItemsVisitor &visitor);

    /**
     * @brief Call to visit all objects of an item
     * @param visitor : is an object that will receive all objects
     */
    void visitObjects(IObjectsVisitor& visitor);

    /**
     * @brief Call to visit objects of specified type of an item
     * @param types : use Scene::ObjectType elements
     * @param visitor : is an object that will receive all objects
     */
    void visitObjectsOfTypes(const QList<Scene::ObjectType>& types, IObjectsVisitor& visitor);

    /**
     * @brief Call to visit all objects of an item
     * @param visitor : is an object that will receive all objects
     */
    void visitObjectsOfOneItem(const Item *item, IObjectsVisitor& visitor);

    /**
     * @brief Call to visit objects of specified type of an item
     * @param types : use Scene::ObjectType elements
     * @param visitor : is an object that will receive all objects
     */
    void visitObjectsOfTypesOfOneItem(const Item *item, const QList<Scene::ObjectType>& types, IObjectsVisitor& visitor);

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
     * @brief Returns the number of item present in this scene
     */
    int countItem() const;

    /**
     * @brief Remove all items from this scene
     */
    void clearItems();

    /**
     * @brief Returns true if this item is in this scene
     */
    bool containsItem(const Item *item) const;

    /**
     * @brief Add one item to this scene
     * @param item : item to add
     */
    void addItem(const Item *item, GraphicsView& graphicsView);

    /**
     * @brief Show/Hide the item. If you want to hide it : the item is always present in the view but not drawn.
     * @param item : item to show/hide
     * @param visible : true to set visible, false to hide it
     */
    void setItemVisible(const Item *item, bool visible);

    /**
     * @brief Returns true if the item is visible or not
     */
    bool isItemVisible(const Item *item) const;

    /**
     * @brief Update an item
     * @param item : item to update
     * @param graphicsView : graphics view that will be used to draw the item
     * @param color : color set when draw the item
     */
    void updateItem(const Item *item, GraphicsView& graphicsView, const QColor &color);

    /**
     * @brief Remove an item from this scene
     * @param item : item to remove
     */
    void removeItem(const Item *item);

    /**
     * @brief Returns the item at index specified
     */
    Item* getItemAt(const int& index);

    /**
     * @brief Returns information of item passed in parameter
     */
    PermanentItemInformation* getPermanentItemInformation(const Item *item) const;

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
    friend class PermanentItemSceneRemover;

    typedef QMultiHash<IGraphicsDocument*, QOpenGLContext*>                          DocCollection;
    typedef QHashIterator<IGraphicsDocument*, QOpenGLContext*>                       DocCollectionIterator;

    /**
     * @brief Mutex to enable this class to be thread safe when add/remove items
     */
    QMutex*                         m_mutex;
    bool                            m_autoDeleteItem;
    DocCollection                   m_documents;
    AnyElementAttributesProvider    m_anyElementsAttributesProvider;
    PermanentScene*                 m_objectsScene;

    /**
     * @brief Collection of items (list) to keep the order of its insertion
     */
    ItemLCollection                 m_items;

    /**
     * @brief Collection of items with information (hashtable)
     */
    ItemCollection                  m_itemsWithInfo;

    /**
     * @brief Remover to remove efficiently items from this class
     */
    PermanentItemSceneRemover*      m_itemsRemover;

    /**
     * @brief Add one item that was updated by the PermanentItemSceneRemover
     * @param item : item updated
     */
    void addItemUpdated(const Item *item, GraphicsView& graphicsView, const quint32& uniqueIndexInList, const QColor &color);

    /**
     * @brief Add one item
     * @param item : item to add
     */
    bool internalAddItem(const Item *item, GraphicsView& graphicsView, const quint32& uniqueIndexInList, const QColor& defaultObjectsColor = Qt::white);

    /**
     * @brief Remove an item
     */
    void internalRemoveItem(const Item *item);

protected:
    friend class PermanentItemSceneRemover;

    /**
     * @brief Returns the mutex used to be thread safe
     */
    QMutex* getMutex() const;

    /**
     * @brief Returns the item scene remover
     */
    PermanentItemSceneRemover* getPermanentItemSceneRemover() const;

    /**
     * @brief Remove from item with info collection the information and return it
     * @param item : item to get information
     * @return information founded
     */
    PermanentItemInformation* takePermanentItemInformation(const Item *item);

private slots:
    /**
     * @brief Called when a item's selection changed
     */
    void itemSelectionChanged(bool selected, Item* item);

signals:
    /**
     * @brief Emitted when changes have occured (add/remove/clear points, change max points, etc...)
     */
    void changesHaveOccured();

    /**
     * @brief Emitted when any draw begin
     */
    void anyDrawBegin();

    /**
     * @brief Emitted when any draw end
     */
    void anyDrawEnd();

    /**
     * @brief Emitted when any update begin
     */
    //void anyUpdateBegin();

    /**
     * @brief Emitted when any update end
     */
    //void anyUpdateEnd();

    /**
     * @brief Emitted when update was called in a thread different that the main thread
     */
    void updateNecessary();

    /**
     * @brief Emitted when no items was present in this scene
     */
    void wasEmptied();
};

#endif // PERMANENTITEMSCENE_H
