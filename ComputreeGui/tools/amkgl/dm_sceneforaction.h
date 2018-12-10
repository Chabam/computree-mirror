#ifndef DM_SCENEFORACTION_H
#define DM_SCENEFORACTION_H

#include "scene/permanentscene.h"
#include "scene/tools/permanentiteminformation.h"
#include "graphicsviewinterface.h"

class CT_AbstractActionForGraphicsView;
class DM_AMKglPermanentPainter;

class DM_SceneForAction : public QObject, public IAttributesAccessor
{
    Q_OBJECT

public:
    DM_SceneForAction();
    ~DM_SceneForAction();

    /**
     * @brief Change the current action
     */
    void setAction(const CT_AbstractActionForGraphicsView* action, GraphicsViewInterface &graphicsView);

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
     * @brief Fast draw the scene
     */
    void fastDraw(DrawInfo& info);

    /**
     * @brief Draw normally the scene
     */
    void draw(DrawInfo& info);

    /**
     * @brief Draw overlay (2D)
     */
    void drawOverlay(QPainter& painter, DrawInfo &info);

    /**
     * @brief Returns the attributes provider of any elements
     */
    AnyElementAttributesProvider* getAnyElementsAttributesProvider() const {
        return &const_cast<AnyElementAttributesProvider&>(m_anyElementsAttributesProvider);
    }

    /**
     * @brief Returns the provider of attributes of an element
     * @param type : type of element that you want
     */
    ObjectAttributesProvider* getObjectsAttributeProvider(Scene::ObjectType type) const {
        return m_anyElementsAttributesProvider.getObjectsAttributeProvider(type);
    }

    /**
     * @brief Returns the permanent scene used
     */
    PermanentScene* getPermanentScene() const {
        return m_scene;
    }

    /**
     * @brief Return the element information for the specified element or NULL if it doesn't exist
     */
    PermanentItemInformation* getElementInformation(const quint32& uniqueIndex);

    /**
     * @brief Returns true if at least one renderer is not up to date !
     */
    bool mustBeUpdated() const;

    /**
     * @brief Check if must be updated and update it if it was not up to date.
     */
    void checkAndUpdateIfItNeedsToBe();

private:
    typedef QHash<int, PermanentItemInformation*>           ActionElementsInfoCollection;
    typedef QHashIterator<int, PermanentItemInformation*>   ActionElementsInfoCollectionIterator;

    /**
     * @brief The current action
     */
    CT_AbstractActionForGraphicsView*   m_action;

    /**
     * @brief The graphics view that contains this action
     */
    GraphicsViewInterface*              m_graphicsView;

    /**
     * @brief Scene of objects for the current action
     */
    PermanentScene*                     m_scene;

    /**
     * @brief Information to know where is stored objects of elements for the current action
     */
    ActionElementsInfoCollection        m_elementsInformations;

    /**
     * @brief Array of colors, normals, etc...
     */
    AnyElementAttributesProvider        m_anyElementsAttributesProvider;

    /**
     * @brief Current painter
     */
    DM_AMKglPermanentPainter*           m_currentPainter;

public slots:
    /**
     * @brief Update objects drawned by the action
     */
    void updateActionObjects();

    /**
     * @brief Create a new PermanentItemInformation and add it to collection
     */
    void createNewElementInformation(quint32 uniqueIndex);
};

#endif // DM_SCENEFORACTION_H
