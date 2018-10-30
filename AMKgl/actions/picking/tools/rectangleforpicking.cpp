#include "rectangleforpicking.h"

#include <QPainter>

#include <limits>

#include "drawinfo.h"
#include "tools/math.h"

#define MIN_DISTANCE 5

RectangleForPicking::RectangleForPicking()
{
    m_moveWhat = MoveNone;
    m_pointToMoveIndex = -1;
    m_authorizeMouseInteractionWithViewWhenPicking = false;
}

void RectangleForPicking::clear()
{
    m_polygon.clear();
    m_pointToMoveIndex = -1;
    m_moveWhat = MoveNone;
    emit polygonChanged();
    redrawOverlay();
}

void RectangleForPicking::setRectangle(const QRect &r)
{
    m_polygon = QPolygon(r);
    m_pointToMoveIndex = -1;
    m_moveWhat = MoveNone;
    emit polygonChanged();
    redrawOverlay();
}

const QPolygon& RectangleForPicking::getPolygon() const
{
    return m_polygon;
}

bool RectangleForPicking::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton) {
        if(m_polygon.isEmpty()) {

            QPoint pos = e->pos();

            m_polygon.append(pos);
            m_polygon.append(QPoint(pos.x()+1, pos.y()));
            m_polygon.append(QPoint(pos.x()+1, pos.y()+1));
            m_polygon.append(QPoint(pos.x(), pos.y()+1));

            m_moveWhat = MovePointOfPolygon;
            m_pointToMoveIndex = 2;

            redrawOverlay();
        } else {
            double dist;
            int index;
            Math::getClosestPolygonPointToPoint(m_polygon, m_mousePos, dist, &index);

            if(dist < MIN_DISTANCE) {
                m_moveWhat = MovePointOfPolygon;
                m_pointToMoveIndex = index;

                if(isMouseInteractionWithViewAuthorizedWhenPickingIsEnabled())
                    return true;

            } else if(isMousePosInPolygon()) {
                m_moveWhat = MovePolygon;
                m_originPolygon = m_polygon;
                m_originPointMove = e->pos();
                return true;
            }
        }
    }

    if(isMouseInteractionWithViewAuthorizedWhenPickingIsEnabled())
        return false;

    return true;
}

bool RectangleForPicking::mouseMoveEvent(QMouseEvent *e)
{
    Q_UNUSED(e)

    m_mousePos = e->pos();

    if(m_moveWhat == MovePointOfPolygon) {

        if((m_pointToMoveIndex == 0) || (m_pointToMoveIndex == 3)) {
            if(e->pos().x() < m_polygon.at(1).x()) {
                m_polygon[0].setX(e->pos().x());
                m_polygon[3].setX(e->pos().x());
            }
        } else if(m_pointToMoveIndex != -1) {
            if(e->pos().x() > m_polygon.at(0).x()) {
                m_polygon[1].setX(e->pos().x());
                m_polygon[2].setX(e->pos().x());
            }
        }

        if((m_pointToMoveIndex == 0) || (m_pointToMoveIndex == 1)) {
            if(e->pos().y() < m_polygon.at(3).y()) {
                m_polygon[0].setY(e->pos().y());
                m_polygon[1].setY(e->pos().y());
            }
        } else if(m_pointToMoveIndex != -1) {
            if(e->pos().y() > m_polygon.at(0).y()) {
                m_polygon[3].setY(e->pos().y());
                m_polygon[2].setY(e->pos().y());
            }
        }

        redrawOverlay();

        if(m_pointToMoveIndex != -1)
            return true;

    } else if(m_moveWhat == MovePolygon) {
        m_polygon = m_originPolygon;
        m_polygon.translate(e->pos() - m_originPointMove);
        emit polygonChanged();
        redrawOverlay();
        return true;
    }

    redrawOverlay();

    if(isMouseInteractionWithViewAuthorizedWhenPickingIsEnabled())
        return false;

    return true;
}

bool RectangleForPicking::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton) {
        m_moveWhat = MoveNone;
        m_pointToMoveIndex = -1;
    }

    return false;
}

bool RectangleForPicking::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_UNUSED(e)

    return false;
}

bool RectangleForPicking::wheelEvent(QWheelEvent *e)
{
    Q_UNUSED(e)

    if(isMouseInteractionWithViewAuthorizedWhenPickingIsEnabled())
        return false;

    return true;
}

bool RectangleForPicking::keyPressEvent(QKeyEvent *e)
{
    Q_UNUSED(e)

    return false;
}

bool RectangleForPicking::keyReleaseEvent(QKeyEvent *e)
{
    Q_UNUSED(e)

    return false;
}

void RectangleForPicking::drawOverlay(QPainter &painter, DrawInfo &info)
{
    Q_UNUSED(info)

    painter.save();

        // draw just lines of the polygon
        painter.setPen(createPenToUse(Qt::blue));
        painter.setBrush(QColor(0, 0, 255, 50));
        drawPolygon(m_polygon, painter, info);

        double dist = 0;
        QPoint p;

        if(m_pointToMoveIndex != -1)
            p = m_polygon.at(m_pointToMoveIndex);
        else
            p = Math::getClosestPolygonPointToPoint(m_polygon, m_mousePos, dist);

        if(dist < MIN_DISTANCE) {
            painter.setPen(createPenToUse(Qt::yellow));
            painter.setBrush(Qt::yellow);
            painter.drawEllipse(p, 5, 5);

            painter.setPen(createPenToUse(Qt::red));
            painter.setBrush(Qt::red);
            painter.drawEllipse(p, 1, 1);
        }

        painter.restore();
}

bool RectangleForPicking::isMouseInteractionWithViewAuthorizedWhenPickingIsEnabled() const
{
    return m_authorizeMouseInteractionWithViewWhenPicking;
}

void RectangleForPicking::drawPolygon(const QPolygon &pol, QPainter &painter, DrawInfo &info)
{
    Q_UNUSED(info)

    painter.drawPolygon(pol);
}

bool RectangleForPicking::isMousePosInPolygon() const
{
    return Math::isPointInPolygon(m_polygon, m_mousePos);
}

void RectangleForPicking::redrawOverlay()
{
    emit mustBeRedraw();
}

QPen RectangleForPicking::createPenToUse(const QColor &color)
{
    return QPen(QBrush(color), 5);
}
