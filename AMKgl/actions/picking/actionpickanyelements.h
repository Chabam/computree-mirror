/****************************************************************************

 Copyright (C) 2010-2012 AMVALOR, France. All rights reserved.

 Contact : micha.krebs@gmail.com

 Developers : Michaël KREBS (AMVALOR)

 This file is part of AMKgl

 Computree is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Computree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Amkgl. If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/

#ifndef ACTIONPICKANYELEMENTS_H
#define ACTIONPICKANYELEMENTS_H

#include <QMouseEvent>
#include <QPolygon>

#include "view/amkglviewer.h"

class DrawInfo;
class PolygonForPicking;
class RectangleForPicking;

/**
 * @brief Action to pick any elements (items/points/faces/edges/etc...) in a graphics view
 */
class ActionPickAnyElements : public QObject
{
    Q_OBJECT

public:

    typedef AMKglViewer Viewer;

    enum PickType {
        PT_None,
        PT_Polygone,
        PT_Rectangle
    };

    enum ElementToPick {
        ETP_Items,
        ETP_Points,
        ETP_Edges,
        ETP_Faces
    };

    enum SelectionType {
        ST_Select,
        ST_Unselect
    };

    ActionPickAnyElements();
    ~ActionPickAnyElements();

    /**
     * @brief Called when this action was added to the graphics view
     */
    void setGlViewer(const Viewer *w);

    /**
     * @brief Set the picking type
     */
    void setPickType(PickType t);

    /**
     * @brief Returns the current picking type
     */
    PickType getPickType() const;

    /**
     * @brief Set type of element to pick
     */
    void setElementToPickType(ElementToPick t);

    /**
     * @brief Returns the current type of element to pick
     */
    ElementToPick getElementToPickType() const;

    /**
     * @brief Set the maximum number of elements to pick or 0 if you want that it must be unlimited
     */
    void setMaxNumberOfElementsToPick(const size_t &max);

    /**
     * @brief Returns the maximum number of elements to pick or 0 if it must be unlimited
     */
    size_t getMaxNumberOfElementsToPick() const;

    /**
     * @brief Set the type of selection to use
     */
    void setSelectionType(SelectionType t);

    /**
     * @brief Returns the type of selection used
     */
    SelectionType getSelectionType() const;

    /**
     * @brief Set the polygon. The picking type will switch automatically to "PT_Polygone"
     */
    void setPolygon(const QPolygon& p);

    /**
     * @brief Set the rectangle. The picking type will switch automatically to "PT_Rectangle"
     */
    void setRectangle(const QRect& r);

    /**
     * @brief Returns the polygon. Can be used when picking type is "PT_Rectangle" or "PT_Polygone"
     */
    QPolygon getPolygon() const;

    /**
     * @brief Returns the rectangle. Can be used when picking type is "PT_Rectangle" or "PT_Polygone". If
     *        picking type is "PT_Polygone" it will return the bounding rect of the polygon.
     */
    QRect getRectangle() const;

    /**
     * @brief Returns the last polygon (polygon or rectangle) used for pick elements.
     */
    QPolygon getLastPolygon() const;

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
     * @brief Launch the picking !
     * @return false if picking can not be started
     */
    bool pick();

private:
    Viewer*                 m_glViewer;
    ElementToPick           m_elementToPickType;
    PickType                m_pickType;
    PolygonForPicking*      m_polygonPickingTools;
    RectangleForPicking*    m_rectanglePickingTools;
    size_t                  m_maxElementsToPick;
    SelectionType           m_selectionType;
    QPolygon                m_lastPolygon;

private slots:
    /**
     * @brief Call to redraw only overlay
     */
    void redrawOverlay();

    /**
     * @brief Call to redraw all
     */
    void redrawAll();

signals:
    /**
     * @brief Emitted when the picking type has changed
     */
    void pickingTypeChanged();

    /**
     * @brief Emitted when the element to pick type has changed
     */
    void elementToPickTypeChanged();

    /**
     * @brief Emitted when the picking type change => if picking type is "PT_None" b == false otherwise b == true
     */
    void pickingActivated(bool b);

    /**
     * @brief Emitted when a values of the current shape used to pick points changed
     */
    void valuesOfShapeChanged();
};

#endif // ACTIONPICKANYELEMENTS_H
