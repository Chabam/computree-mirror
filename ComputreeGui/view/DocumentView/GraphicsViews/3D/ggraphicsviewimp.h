#ifndef GGRAPHICSVIEWIMP_H
#define GGRAPHICSVIEWIMP_H

#include "view/DocumentView/GraphicsViews/ggraphicsview.h"
#include "dm_graphicsviewoptions.h"
#include "view/amkglviewer.h"
#include "defineglobalcloud.h"
#include "scene/permanentscene.h"
#include "tools/amkgl/dm_sceneforaction.h"

class G3DCameraController;
class GGraphicsViewImp;

/**
 * @brief Class to emit signals of graphicsView
 */
class GGraphicsViewImpSignalEmitter : public GraphicsViewSignalEmitterInterface
{
    Q_OBJECT

public:
    void setGraphicsView(GGraphicsViewImp *gv) { m_graphicsView = gv; }
    GraphicsViewInterface* graphicsView() const;

    void emitDrawingStarted() { emit drawingStarted(); }
    void emitDrawingFinished() { emit drawingFinished(); }

protected:
    GGraphicsViewImp *m_graphicsView;
};

class GGraphicsViewImp : public AMKglViewer, public GGraphicsView
{
    Q_OBJECT

public:
    GGraphicsViewImp(const GDocumentViewForGraphics* parentDocument, QWidget* parent);
    ~GGraphicsViewImp();

    void active2DView(bool e);

    /**
     * @brief Call to disable the point selection backup (per example when you pick points, etc...)
     */
    void disablePointsSelectionBackup();

    /**
     * @brief Call to disable the edge selection backup (per example when you pick edges, etc...)
     */
    void disableEdgesSelectionBackup();

    /**
     * @brief Call to disable the face selection backup (per example when you pick faces, etc...)
     */
    void disableFacesSelectionBackup();

    // -------- GraphicsViewSignalEmitterInterface ---------- //

    GraphicsViewInterface* graphicsView() const;

    // -------- GGraphicsView ---------- //

    QOpenGLWidget* getOpenGLWidget() const;
    bool is2DView() const;

    // -------- DM_GraphicsView ---------- //

    void setOptions(const DM_GraphicsViewOptions &newOptions);

    void lockPaint();
    void unlockPaint();

    void redraw();

    DM_GraphicsViewCamera* getCamera() const;

    void takeAndSaveScreenshot();

    void updateDrawing3DOfItemDrawables(const QList<CT_AbstractItemDrawable*> &list);
    void updateItemDrawablesThatColorWasModified();
    void updateColorOfPointsOfItemDrawable(CT_AbstractItemDrawable *item, const QColor &color);

    void getBoundingBoxOfAllItemDrawablePresentInView(Eigen::Vector3d &min, Eigen::Vector3d &max) const;
    void getBoundingBoxOfAllItemDrawableSelectedInView(Eigen::Vector3d &min, Eigen::Vector3d &max) const;

    void getBoundingBoxOfAllPointsSelectedInView(Eigen::Vector3d &min, Eigen::Vector3d &max) const;
    void getBoundingBoxOfAllEdgesSelectedInView(Eigen::Vector3d &min, Eigen::Vector3d &max) const;
    void getBoundingBoxOfAllFacesSelectedInView(Eigen::Vector3d &min, Eigen::Vector3d &max) const;

    using AMKglViewer::setVisible;
    void setVisible(const CT_AbstractItemDrawable* item, bool e);
    bool isVisible(const CT_AbstractItemDrawable *item) const;

    // -------- GraphicsViewInterface ---------- //

    DocumentInterface* document() const;

    GraphicsViewSignalEmitterInterface* signalEmitter() const;
    GraphicsViewOptionsInterface& getOptions();

    QSharedPointer<CT_StandardColorCloudRegistered> colorCloudOf(ColorCloudType type) const;
    QSharedPointer<CT_StandardNormalCloudRegistered> normalCloudOf(NormalCloudType type) const;

    void setColorOfPoint(const size_t &globalIndexOfPoint, const CT_Color &color);
    CT_Color& colorOfPoint(const size_t &globalIndexOfPoint);
    const CT_Color& constColorOfPoint(const size_t &globalIndexOfPoint) const;

    void setNormalOfPoint(const size_t &globalIndexOfPoint, const CT_Normal &normal);
    CT_Normal& normalOfPoint(const size_t &globalIndexOfPoint);
    const CT_Normal& constNormalOfPoint(const size_t &globalIndexOfPoint) const;

    void dirtyColorsOfItemDrawablesWithPoints();
    void dirtyNormalsOfItemDrawablesWithPoints();

    void setFlagProperty(const GraphicsObjectFlag& flag,
                         const GraphicsObjectFlagProperty& property);
    void setFlagProperty(const GraphicsObjectFlag& flag,
                         const GraphicsObjectType& typeOfObject,
                         const GraphicsObjectFlagProperty& property);
    GraphicsObjectFlagProperty getFlagProperty(const GraphicsObjectFlag& flag,
                                               const GraphicsObjectType& typeOfObject) const;
    GraphicsObjectFlagProperty getFlagProperty(const GraphicsObjectFlag& flag,
                                               bool& ok) const;

    CameraInterface* camera() const;
    GraphicsViewType type() const;

    void setSelectionMode(SelectionMode mode);
    GraphicsViewInterface::SelectionMode selectionMode() const;
    bool mustSelectPoints() const;
    bool mustSelectEdges() const;
    bool mustSelectFaces() const;
    bool mustSelectItems() const;
    Q_DECL_DEPRECATED void setSelectRegionWidth(int width);
    Q_DECL_DEPRECATED void setSelectRegionHeight(int height);
    Q_DECL_DEPRECATED void select(const QPoint& point);
    using AMKglViewer::select;
    void select(const QPolygon& polygon);

    CT_SPCIR getSelectedPoints() const;
    CT_SFCIR getSelectedFaces() const;
    CT_SECIR getSelectedEdges() const;
    QList<CT_AbstractItemDrawable*> getSelectedItems() const;

    void addPointsIDToSelection(const size_t &id);
    void addMultiplePointsIDToSelection(const std::vector<size_t> &idList);
    void addFacesIDToSelection(const size_t &id);
    void addMultipleFacesIDToSelection(const std::vector<size_t> &idList);
    void addEdgesIDToSelection(const size_t &id);
    void addMultipleEdgesIDToSelection(const std::vector<size_t> &idList);
    void removePointsIDFromSelection(const size_t &id);
    void removeMultiplePointsIDFromSelection(const std::vector<size_t> &idList);
    void removeFacesIDFromSelection(const size_t &id);
    void removeMultipleFacesIDFromSelection(const std::vector<size_t> &idList);
    void removeEdgesIDFromSelection(const size_t &id);
    void removeMultipleEdgesIDFromSelection(const std::vector<size_t> &idList);
    void setAllPointsSelected(bool select);
    void setAllFacesSelected(bool select);
    void setAllEdgesSelected(bool select);
    void setAllItemsSelected(bool select);
    void toggleItemsSelected();
    void togglePointsSelected();
    void toggleFacesSelected();
    void toggleEdgesSelected();
    size_t countPoints();
    size_t countEdges();
    size_t countFaces();
    size_t countItems();
    void visitObjects(ObjectsVisitorInterface& visitor);
    void visitObjects(const QList<GraphicsObjectType>& types, ObjectsVisitorInterface& visitor);
    QSharedPointer<ObjectsModifierInterface> createObjectsModifier(GraphicsObjectType type);
    QSharedPointer<ObjectStatusQueryiorInterface> createObjectsStatusQueryior(GraphicsObjectType type);
    void visitObjectsOfItem(const CT_AbstractItemDrawable* item, ObjectsVisitorInterface& visitor);
    void visitObjectsOfItem(const CT_AbstractItemDrawable* item, const QList<GraphicsObjectType>& types, ObjectsVisitorInterface& visitor);
    QSharedPointer<ObjectsModifierInterface> createObjectsModifierForItem(const CT_AbstractItemDrawable* item, GraphicsObjectType type);
    void visitObjectsOfCurrentAction(const int& uniqueIndex, ObjectsVisitorInterface& visitor);
    void visitObjectsOfCurrentAction(const int& uniqueIndex, const QList<GraphicsObjectType>& types, ObjectsVisitorInterface& visitor);
    Eigen::Vector3d pointUnderPixel(const QPoint &pixel, bool &found) const;
    void convertClickToLine(const QPoint &pixel, Eigen::Vector3d &orig, Eigen::Vector3d &dir) const;
    void convert3DPositionToPixel(const Eigen::Vector3d &position, QPoint &pixel) const;
    int width() const;
    int height() const;

    // -------- AMKglViewer ---------- //

    AMKgl::GraphicsView* getGraphicsView() const;

    /**
     * @brief Returns the camera of the AMKglViewer
     */
    AMKglViewer::CameraType* amkglCamera() const;

private:
    /**
     * @brief A structure to save all information set
     *        with method before to really pick elements
     */
    struct PickingBackup {
        GraphicsViewInterface::SelectionMode mode;
        QRect   rect;
    };

    /**
     * @brief Signals emitter
     */
    GGraphicsViewImpSignalEmitter   m_signalEmitter;

    /**
     * @brief Options of this view
     */
    DM_GraphicsViewOptions  m_options;

    /**
     * @brief A controller that control the real camera of this viewer
     */
    G3DCameraController*    m_camController;

    /**
     * @brief Unique index of this viewer
     */
    uint                    m_uniqueIndex;

    /**
     * @brief Backup of the picking configuration
     */
    PickingBackup           m_pickingConfiguration;

    /**
     * @brief Parent document
     */
    GDocumentViewForGraphics* m_document;

    /**
     * @brief A static attribute that represent the default filename (without unique index) for viewers
     */
    static QString          DEFAULT_STATE_FILENAME;

    /**
     * @brief A static attribute to know how many views has been created and to let
     *        this class create a unique filename for each viewer
     */
    static uint             NUMBER_OF_VIEWS;

    /**
     * @brief A backup of points selected to allow plugin to query many times the method "getSelectedPoints"
     */
    CT_SPCIR                m_backupPointsSelected;

    /**
     * @brief A backup of points selected to allow plugin to query many times the method "getSelectedEdges"
     */
    CT_SECIR                m_backupEdgesSelected;

    /**
     * @brief A backup of points selected to allow plugin to query many times the method "getSelectedFaces"
     */
    CT_SFCIR                m_backupFacesSelected;

    /**
     * @brief A scene for the current graphics action
     */
    DM_SceneForAction       m_sceneForAction;

    /**
     * @brief Draw camera informations
     */
    void drawCameraInformations();

    /**
     * @brief Compute and set the text position
     */
    void computeAndSetTextPosition();

    /**
     * @brief Use this method to get bounding box of selected edges or faces of meshes
     */
    void getBoundingBoxOfSelectedElementsOfMeshesInView(Scene::ObjectType globalType, Eigen::Vector3d &min, Eigen::Vector3d &max) const;

protected:
    void init();

    void preDraw();
    void draw();
    void fastDraw();
    void postDraw();
    void subPostDraw(DrawInfo& info);
    void drawOverlay(QPainter& painter, DrawInfo &info);

    bool event(QEvent *e);
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

    /**
     * @brief Generate the state filename for this view
     */
    QString stateFileName() const;

    /**
     * @brief Save configuration of the view in a file
     */
    void saveStateToFile();

    /**
     * @brief Restore configuration view from file
     */
    bool restoreStateFromFile();

    /**
     * @brief Convert a selection mode to is basic form
     */
    static GraphicsViewInterface::SelectionMode staticToBasicSelectionMode(GraphicsViewInterface::SelectionMode mode);

private slots:
    /**
     * @brief Called when the current action changed
     */
    void currentActionChanged(CT_AbstractAction *action);

    /**
     * @brief Called when the debug mode changed
     */
    void amkglDebugModeChanged(bool enabled);

    /**
     * @brief Called when the debug mode action is unchecked
     */
    void disableAmkglDebugMode();
};

#endif // GGRAPHICSVIEWIMP_H
