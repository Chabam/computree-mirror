#ifndef GDOCUMENTVIEWFORGRAPHICS_H
#define GDOCUMENTVIEWFORGRAPHICS_H

#include "gdocumentview.h"

#include "GraphicsViews/ggraphicsview.h"
#include "GraphicsViews/ggraphicsviewoptions.h"
#include "gcameragraphicsoptions.h"
#include "gpointofviewdocumentmanager.h"
#include "tools/attributes/worker/dm_attributesmanager.h"

#include "ct_itemdrawable/abstract/ct_abstractpointsattributes.h"
#include "ct_itemdrawable/abstract/ct_abstractfaceattributes.h"
#include "ct_itemdrawable/abstract/ct_abstractedgeattributes.h"

#include "ct_colorcloud/registered/ct_standardcolorcloudregistered.h"

#include "view/DocumentView/GraphicsViews/3D/ggraphicsviewimp.h"

#include "tools/document/dm_resultgroupgui.h"

#include <QHBoxLayout>
#include <QToolButton>

class GDocumentViewForGraphics : public GDocumentView, public IGraphicsDocument
{
    Q_OBJECT

public:

    struct NormalsConfiguration {
        QColor normalColor;
        float normalLength;
    };

    typedef std::function<void (const CT_AbstractItemDrawable* item, const QColor& color)> visitItemColorModifiedFunction;

    GDocumentViewForGraphics(GDocumentManagerView &manager, QString title, QString type);
    virtual ~GDocumentViewForGraphics();

    // ------------- IGraphicsDocument ------------ //

    PermanentSceneToRender* getPermanentSceneToRender() const;
    QOpenGLWidget* getOpengWidgetWithContext(const QOpenGLContext* context) const;
    QColor getSelectionColor() const;
    QColor getNormalsColor() const;
    float getNormalsLength() const;
    bool mustShowNormals() const;
    void setMustShowNormals(bool enabled);
    bool mustReduceNumberOfPointsInFastDraw() const;
    bool mustReduceNumberOfPointsInNormalDraw() const;

    // ------------- GDocumentView ------------ //

    void init();

    void addGraphics(GGraphicsView *graphics);
    QList<GGraphicsView*> getGraphicsList() const;

    void beginAddMultipleItemDrawable() {}
    void endAddMultipleItemDrawable() {}
    void addItemDrawable(CT_AbstractItemDrawable &item);


    void beginRemoveMultipleItemDrawable() {}
    void endRemoveMultipleItemDrawable() {}
    void removeItemDrawable(CT_AbstractItemDrawable &item);

    void updateDrawing3DOfItemDrawablesInGraphicsView(const QList<CT_AbstractItemDrawable*> &items);

    QList<InDocumentViewInterface*> views() const;
    void redrawGraphics();
    void updateItems(const QList<CT_AbstractItemDrawable*> &items);

    void dirtyColorsOfPoints();
    void dirtyNormalsOfPoints();
    void applyNormalsConfiguration(GDocumentViewForGraphics::NormalsConfiguration c);
    GDocumentViewForGraphics::NormalsConfiguration getNormalsConfiguration() const;

    bool acceptAction(const CT_AbstractAction *action) const;
    bool setCurrentAction(CT_AbstractAction *action, bool deleteAction = true);
    bool setDefaultAction(CT_AbstractAction *action, bool deleteAction = true);
    /**
     * @brief Call this method if you want set an action from GUI
     */
    bool setCurrentInternalAction(CT_AbstractAction *action, bool deleteAction = true);
    CT_AbstractAction* currentAction() const;
    CT_AbstractAction* defaultAction() const;
    void removeActions(const QString &uniqueName) const;

    QString getType() const;

    /**
     * @brief Returns true if this document use item color.
     */
    virtual bool useItemColor() const;

    /**
     * @brief Set the color of the item passed in parameter.
     */
    virtual void setColor(const CT_AbstractItemDrawable *item, const QColor &color);

    /**
     * @brief Returns true if the color of the itemdrawable passed in parameter is modified.
     */
    virtual bool isColorModified(const CT_AbstractItemDrawable *item);

    /**
     * @brief Return the color of the item passed in parameter.
     */
    virtual QColor getColor(const CT_AbstractItemDrawable *item);

    /**
     * @brief Call to visit items that colors was modified
     */
    void visitItemsThatColorWasModified(visitItemColorModifiedFunction f);

    /**
     * @brief Returns true if this document has the capacity of change the visibility of an itemdrawable
     */
    bool canChangeVisibility() const;

    /**
     * @brief Change the visibility of the item in the document
     */
    void setVisible(const CT_AbstractItemDrawable *item, bool visible);

    /**
     * @brief Returns the visibility of the item in the document.
     */
    bool isVisible(const CT_AbstractItemDrawable *item) const;

    /**
     * @brief Apply the attributes (scalar, color or normal for points/faces/edges) to array of this document.
     */
    void applyAttributes(DM_AbstractAttributes *dpa);

    /**
     * @brief Set graphics options
     */
    void setOptions(const DM_GraphicsViewOptions& options);

    /**
     * @brief Returns current options
     */
    DM_GraphicsViewOptions getOptions() const;

    /**
     * @brief Update the attributes manager to be sync with elements in the view
     */
    void updateAttributesManager();

    /**
     * @brief Returns the attributes manager used in this graphics view
     */
    const DM_AttributesManager& attributesManager() const;

private:
    GGraphicsViewOptions        *_graphicsOptionsView;
    GCameraGraphicsOptions      *_cameraOptionsView;

    DM_AttributesManager        m_attributesManager;

    QTimer                      m_timerUpdateColors;
    QTimer                      m_timerDirtyColorsOfPoints;
    QTimer                      m_timerDirtyNormalsOfPoints;

    GGraphicsView*              m_graphics;
    PermanentSceneToRender*     m_scene;

    QHBoxLayout                 *_layoutGraphics;

    QToolButton                 *_pointOfViewButton;
    GPointOfViewDocumentManager *_pointOfViewMenu;
    DM_PointOfViewManager       _pofManager;

    QPushButton                 *_buttonExport;
    QPushButton                 *m_buttonTransferToDocument;
    QPushButton                 *_buttonPixelSizeUp;
    QPushButton                 *_buttonPixelSizeDown;
    QPushButton                 *_buttonCameraType;

    QPushButton*                m_buttonFastDrawMode;

    QString                     _type;

    bool                        _viewDetached;
    QWidget*                    _previousParent;
    Qt::WindowFlags             _flags;

    DM_ResultGroupGui           mResultGui;

    void createAndAddCameraAndGraphicsOptions(QWidget *parent);
    void createAndAddItemDrawableWidgetContainer(QWidget *parent);
    void createAndAddGraphicsWidgetContainer(QWidget *parent);

    QString getKeyForPointOfViewManager();

    bool setCurrentAction(bool internal, CT_AbstractAction *action, bool deleteAction);

protected:

    /**
     * @brief Overloaded to create DM_ItemInfoForGraphics;
     */
    virtual DM_AbstractInfo* createNewItemInformation(const CT_AbstractItemDrawable *item) const;

    /**
     * @brief Recursive set color of childrens of this group
     */
    void recursiveSetColor(CT_StandardItemGroup *group,
                              const QColor &color);

public slots:

    /**
      * \brief Affiche les options du document
      */
    void showOptions();
    void validateOptions();

    /**
     * @brief Save a screenshot to disk
     */
    void takeAndSaveScreenshot();

    /**
      * \brief Ajoute le point de vue actuel du graphique au gestionnaire de point de vue
      */
    void addActualPointOfView();

    /**
      * \brief Dfini le point de vue pass en paramtre au graphique du document
      */
    void setPointOfView(DM_PointOfView *pof);

    /**
     * @brief Show points attributes manager
     */
    void showAttributesOptions();

    void changePixelSizeUp();
    void changePixelSizeDown();
    void changeFastDrawMode();
    void changeCameraType();
    void setTransparencyActivated(bool activated);
    void setCameraType(bool orthographic);


    void detachView();
protected slots:
    virtual void closeEvent(QCloseEvent *closeEvent);

private slots:

    void syncChanged(bool enable);
    void pluginExporterManagerReloaded();
    void exporterActionTriggered();
    void mustUpdateItemDrawablesThatColorWasModified();
    void mustDirtyColorsOfItemDrawablesWithPoints();
    void mustDirtyNormalsOfItemDrawablesWithPoints();

    void updateButtonFastDrawMode();
    void updateButtonCameraType();

    void updateMenuOfTransferButton();
    void addSelectedItemsToDocument();

    void removeItemDrawableFromScene(CT_AbstractItemDrawable& item);
signals:

    void syncEnabled(const GDocumentViewForGraphics *view);
    void syncDisabled(const GDocumentViewForGraphics *view);

    void startDirtyColorsOfPointTimer();
    void startDirtyNormalsOfPointTimer();
    void startUpdateColorsTimer();
    void startUpdateNormalsTimer();
};

#endif // GDOCUMENTVIEWFORGRAPHICS_H
