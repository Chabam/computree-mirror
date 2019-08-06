#ifndef GRAPHICSVIEWDEBUGMODE_H
#define GRAPHICSVIEWDEBUGMODE_H

#include <QVector>

#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"

#include "picker/geometry/triangleplane.h"

class GGraphicsViewImp;

/**
 * @brief Tools class to manage the debug mode of a graphics view. (action for graphics view)
 */
class GraphicsViewDebugMode : public CT_AbstractActionForGraphicsView
{
    Q_OBJECT

public:
    GraphicsViewDebugMode();
    virtual ~GraphicsViewDebugMode();

    /**
     * @brief Return the title of the action (added to the view at the side of the icon)
     */
    QString title() const;

    /**
     * @brief Return the description of the action
     */
    QString description() const;

    /**
     * @brief Return the icon of the action
     */
    QIcon icon() const;

    /**
     * @brief Return the type of action
     */
    QString type() const;

    /**
     * @brief called when a mouse press event occurs on the view
     *
     *        Return true if you do not want the event to be propagated.
     */
    bool mousePressEvent(QMouseEvent *e);

    /**
     * @brief called when a mouse move event occurs on the view
     *
     *        Return true if you do not want the event to be propagated.
     */
    bool mouseMoveEvent(QMouseEvent *e);

    /**
     * @brief called when a mouse release event occurs on the view
     *
     *        Return true if you do not want the event to be propagated.
     */
    bool mouseReleaseEvent(QMouseEvent *e);

    /**
     * @brief This method is called when the action is active and when the signal "drawing3DChanged" of
     *        the action is emitted. You can overwrite them if you want to draw some ItemDrawable without
     *        add them in the document. If you want to access objects in the graphics view to modify it you
     *        must not forget to call the method "setNew3DElementDraw(X)" for each item or element you draw.
     *
     * @warning Elements drawn by an action is not exportable or selectable.
     */
    void draw(GraphicsViewInterface &view, PainterInterface &painter);

    /**
     * @brief This method is called (always after draw(...)) when the action is active and when the graphics
     *        view is updated. You can overwrite them if you want to draw some 2D element like text or polygons, etc...
     */
    void drawOverlay(GraphicsViewInterface &view, QPainter &painter);

    CT_ACTION_DECL_COPY(GraphicsViewDebugMode)

private:
    bool    m_mustShowContextMenu;
    bool    m_mustShowCoordinateSystem;
    bool    m_mustShowChunksInformations;
    QVector<TrianglePlane>  m_polygonToShowIn3D;
    bool    m_mustShowCameraPivotPoint;

    /**
     * @brief Show the context menu to let the user do some debug staff
     */
    void showContextMenu(const QPoint &pos);

    /**
     * @brief Returns the graphics view to debug
     */
    GGraphicsViewImp* graphicsViewToDebug() const;

    /**
     * @brief Draw informations of all coordinate system
     */
    void drawCoordinateSystemInformations(GraphicsViewInterface &view, QPainter &painter);

    /**
     * @brief Draw informations of all chunks
     */
    void drawChunksInformations(GraphicsViewInterface &view, QPainter &painter);

private slots:
    /**
     * @brief Change the color of each chunk (all objects inside it) to be different
     */
    void setAutomaticColorToChunk();

    /**
     * @brief Hide/Show coordinate system
     */
    void showCoordinateSystem(bool enabled);

    /**
     * @brief Hide/Show informations for chunks
     */
    void showInformationsOfChunks(bool enabled);

    /**
     * @brief Hide/Show the polygon used for selection in 3D
     */
    void showPickingPolygonIn3D(bool enabled);

    /**
     * @brief Color points if it was in the selection or not
     */
    void showSelectionResult();

    /**
     * @brief Hide/Show the pivot point of the camera
     */
    void showCameraPivotPoint(bool enabled);
};

#endif // GRAPHICSVIEWDEBUGMODE_H
