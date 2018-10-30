#ifndef DRAWINFO_H
#define DRAWINFO_H

#include <QtOpenGL>
#include <QVector>
#include <QColor>

#include "Eigen/Core"

class AMKglViewer;

/**
 * @brief This class is useful to draw text information on the
 *        view. No text will be painted on top of other text.
 *
 *        It can also be used to get camera information like
 *        the modelview matrix and the camera translation or
 *        graphics view information like if it was in fast draw
 *        or not.
 */
class DrawInfo
{
public:
    typedef AMKglViewer Viewer;

    DrawInfo(QPainter &painter, const Viewer* viewer);

    /**
     * @brief Reset this object. Call it when you begin a new frame.
     */
    void reset();

    /**
     * @brief Set the top left corner where to begin draw of all text
     * @param x : x coordinate of the top left corner used to draw text. If it was < 0 a default coordinate will be used.
     * @param y : y coordinate of the top left corner used to draw text. If it was < 0 a default coordinate will be used.
     */
    void setPositionWhereDrawText(int x,
                                  int y);

    /**
     * @brief Draw all text saved. Call it typically in your post draw method.
     */
    void drawAllTextSaved();

    /**
     * @brief Returns the largest width of the text saved
     */
    int getWidthOfAllTextSaved() const;

    /**
     * @brief Returns the total height of the text saved
     */
    int getHeightOfAllTextSaved() const;

    /**
     * @brief Return true if text must be saved. Otherwise it must be painted
     *        directly
     */
    bool mustTextBeSaved() const;

    /**
     * @brief Draw the text specified. It if must be saved it will be painted
     *        when "drawAllTextSaved" will be called.
     */
    void drawText(QString text);

    /**
     * @brief Returns the current model/view matrix used by the camera
     */
    void getModelViewMatrix(GLdouble mw[16]);

    /**
     * @brief Returns the current camera translation (position)
     */
    Eigen::Vector4d getCameraTranslation() const;

    /**
     * @brief Returns the size of the color list
     */
    int colorSize() const;

    /**
     * @brief Returns the color at specified index. If "i" was
     *        egual or greater than colorSize() it will be shifted
     *        to get the color at 0 + (i-shift).
     */
    QColor colorAt(int i) const;

    /**
     * @brief Returns the color name at specified index. If "i" was
     *        egual or greater than colorSize() it will be shifted
     *        to get the color name at 0 + (i-shift).
     */
    QString colorNameAt(int i) const;

    /**
     * @brief Returns true if the view was currently in fast draw mode
     */
    bool inFastDrawMode() const;

private:
    struct Message {
        int x;
        int y;
        QString text;
    };

    QPainter&           m_painter;
    Viewer*             m_viewer;
    QVector<QColor>     m_colors;
    QVector<QString>    m_colorsName;
    QList<Message>      m_messages;
    bool                m_textMustBeSaved;
    int                 m_lastY;
    QPoint              m_topLeftCornerWhereDrawText;
};

#endif // DRAWINFO_H
