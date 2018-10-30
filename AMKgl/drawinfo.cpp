#include "drawinfo.h"

#include "view/amkglviewer.h"

DrawInfo::DrawInfo(QPainter &painter, const Viewer *viewer) : m_painter(painter)
{
    m_viewer = (Viewer*)viewer;

    m_colors.append(Qt::white);
    m_colors.append(Qt::red);
    m_colors.append(Qt::green);
    m_colors.append(Qt::blue);
    m_colors.append(Qt::yellow);
    m_colors.append(Qt::gray);
    m_colors.append(Qt::cyan);
    m_colors.append(Qt::magenta);


    m_colorsName.append("white");
    m_colorsName.append("red");
    m_colorsName.append("green");
    m_colorsName.append("blue");
    m_colorsName.append("yellow");
    m_colorsName.append("gray");
    m_colorsName.append("cyan");
    m_colorsName.append("magenta");

    m_topLeftCornerWhereDrawText.setX(m_painter.fontMetrics().height());
    m_topLeftCornerWhereDrawText.setY(m_topLeftCornerWhereDrawText.x());

    reset();
}

void DrawInfo::reset()
{
    m_textMustBeSaved = true;
    m_lastY = 0;
}

void DrawInfo::setPositionWhereDrawText(int x, int y)
{
    if(x >= 0)
        m_topLeftCornerWhereDrawText.setX(x);

    if(y >= 0)
        m_topLeftCornerWhereDrawText.setY(y);
}

void DrawInfo::getModelViewMatrix(GLdouble mw[])
{
    m_viewer->camera()->getModelViewMatrix(mw);
}

Eigen::Vector4d DrawInfo::getCameraTranslation() const
{
    return Eigen::Vector4d(m_viewer->camera()->position().x,
                           m_viewer->camera()->position().y,
                           m_viewer->camera()->position().z,
                           1);
}

void DrawInfo::drawAllTextSaved()
{
    m_textMustBeSaved = false;

    m_painter.save();
    m_painter.setPen(Qt::white);
    foreach (const Message &m, m_messages) {
        m_painter.drawText(m_topLeftCornerWhereDrawText.x(),
                           m_topLeftCornerWhereDrawText.y()+m.y,
                           m.text);
    }
    m_painter.restore();

    m_messages.clear();
}

int DrawInfo::getWidthOfAllTextSaved() const
{
    int max = 0;

    QFontMetrics fm = m_painter.fontMetrics();

    foreach (const Message &m, m_messages) {
        max = qMax(max, fm.width(m.text));
    }

    return max;
}

int DrawInfo::getHeightOfAllTextSaved() const
{
    return m_painter.fontMetrics().height() * m_messages.size();
}

bool DrawInfo::mustTextBeSaved() const
{
    return m_textMustBeSaved;
}

void DrawInfo::drawText(QString text)
{
    if(!mustTextBeSaved()) {
        m_painter.save();
        m_painter.setPen(Qt::white);
        m_painter.drawText(m_topLeftCornerWhereDrawText.x(),
                           m_topLeftCornerWhereDrawText.y() + m_lastY,
                           text);
        m_painter.restore();
    } else {
        Message m;
        m.x = 0;
        m.y = m_lastY;
        m.text = text;

        m_messages.append(m);
    }

    m_lastY += m_painter.fontMetrics().height();
}

QColor DrawInfo::colorAt(int i) const
{
    while(i >= m_colors.size())
        i -= m_colors.size();

    return m_colors.at(i);
}

QString DrawInfo::colorNameAt(int i) const
{
    while(i >= m_colorsName.size())
        i -= m_colorsName.size();

    return m_colorsName.at(i);
}

bool DrawInfo::inFastDrawMode() const
{
    return m_viewer->isFastDrawModeCurrentlyUsed();
}
