#ifndef CT_ACTIONSELECTITEMDRAWABLEGV_H
#define CT_ACTIONSELECTITEMDRAWABLEGV_H

#include "ctlibstdactions/ctlibstdactions_global.h"
#include <QRect>

#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"
#include "ctlibstdactions/action/tools/ct_actionselecttool.h"

class CTLIBSTDACTIONS_EXPORT CT_ActionSelectItemDrawableGV : public CT_AbstractActionForGraphicsView
{
    Q_OBJECT
public:

    enum SelectionTool {
        Point = 0,
        Rectangle,
        Polygon
    };

    CT_ActionSelectItemDrawableGV();

    ~CT_ActionSelectItemDrawableGV();

    QString uniqueName() const;
    QString title() const;
    QString description() const;
    QIcon icon() const;
    QString type() const;

    void init();

    bool mousePressEvent(QMouseEvent *e);
    bool mouseMoveEvent(QMouseEvent *e);
    bool mouseReleaseEvent(QMouseEvent *e);
    bool wheelEvent(QWheelEvent *e);

    bool keyPressEvent(QKeyEvent *e);
    bool keyReleaseEvent(QKeyEvent *e);

    void drawOverlay(GraphicsViewInterface &view, QPainter &painter);

    CT_AbstractAction* copy() const;

private:
    CT_ActionSelectTool*                            m_selectTool;

private slots:
    void redrawOverlay();

};

#endif // CT_ACTIONSELECTITEMDRAWABLEGV_H
