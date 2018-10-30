#ifndef RECTANGLEFORPICKING_H
#define RECTANGLEFORPICKING_H

#include <QMouseEvent>
#include <QPolygon>

class DrawInfo;

class RectangleForPicking : public QObject
{
    Q_OBJECT

public:
    RectangleForPicking();

    /**
     * @brief Clear the current rectangle
     */
    void clear();

    /**
     * @brief Set the rectangle.
     */
    void setRectangle(const QRect& r);

    /**
     * @brief Returns the polygon
     */
    const QPolygon& getPolygon() const;

    /**
     * @brief Called from graphics view when a mouse press event occured
     * @return Returns true if the graphics view must not use the mouse event
     */
    bool mousePressEvent(QMouseEvent *e);

    /**
     * @brief Called from graphics view when a mouse move event occured
     * @return Returns true if the graphics view must not use the mouse event
     */
    bool mouseMoveEvent(QMouseEvent *e);

    /**
     * @brief Called from graphics view when a mouse release event occured
     * @return Returns true if the graphics view must not use the mouse event
     */
    bool mouseReleaseEvent(QMouseEvent *e);

    /**
     * @brief Called from graphics view when a mouse double click event occured
     * @return Returns true if the graphics view must not use the mouse event
     */
    bool mouseDoubleClickEvent(QMouseEvent *e);

    /**
     * @brief Called from graphics view when a mouse wheel event occured
     * @return Returns true if the graphics view must not use the mouse event
     */
    bool wheelEvent(QWheelEvent *e);

    /**
     * @brief Called from graphics view when a key press event occured
     * @return Returns true if the graphics view must not use the key event
     */
    bool keyPressEvent(QKeyEvent *e);

    /**
     * @brief Called from graphics view when a key release event occured
     * @return Returns true if the graphics view must not use the key event
     */
    bool keyReleaseEvent(QKeyEvent *e);

    /**
     * @brief Called from graphics view when we must draw it
     */
    void drawOverlay(QPainter &painter, DrawInfo &info);

    /**
     * @brief Returns true if the mouse interaction with view is authorized when piking is enabled
     */
    bool isMouseInteractionWithViewAuthorizedWhenPickingIsEnabled() const;

private:
    enum MoveWhat {
        MoveNone,
        MovePointOfPolygon,
        MovePolygon
    };

    struct ClosestResult {
        QPoint  point;
        double  distance;
        int     index;
    };

    QPolygon        m_polygon;
    QPoint          m_mousePos;

    QPoint          m_originPointMove;
    QPolygon        m_originPolygon;

    MoveWhat        m_moveWhat;
    int             m_pointToMoveIndex;

    bool            m_authorizeMouseInteractionWithViewWhenPicking;

    /**
     * @brief Draw the polygon
     */
    void drawPolygon(const QPolygon &pol, QPainter &painter, DrawInfo &info);

    /**
     * @brief Returns true if the mouse pos is in the polygon
     */
    bool isMousePosInPolygon() const;

    /**
     * @brief Call to redraw only overlay
     */
    void redrawOverlay();

    /**
     * @brief Returns the pen to use
     */
    QPen createPenToUse(const QColor &color);

signals:
    /**
     * @brief Emitted when you must redraw this object
     */
    void mustBeRedraw();

    /**
     * @brief Emitted when the polygon changed
     */
    void polygonChanged();
};

#endif // RECTANGLEFORPICKING_H
