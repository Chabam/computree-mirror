#include "ct_actionselectitemdrawablegv.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QIcon>
#include <QPainter>
#include <math.h>

#include "views/actions/ct_actionselectitemdrawablegvoptions.h"

CT_ActionSelectItemDrawableGV::CT_ActionSelectItemDrawableGV() : CT_AbstractActionForGraphicsView()
{    
    m_selectTool = NULL;
}

CT_ActionSelectItemDrawableGV::~CT_ActionSelectItemDrawableGV()
{
    if (m_selectTool != NULL)
    {
        delete m_selectTool;
    }
}

QString CT_ActionSelectItemDrawableGV::uniqueName() const
{
    return "CT_ActionSelectItemDrawableGV";
}

QString CT_ActionSelectItemDrawableGV::title() const
{
    return tr("Sélection");
}

QString CT_ActionSelectItemDrawableGV::description() const
{
    return tr("Sélection d'éléments");
}

QIcon CT_ActionSelectItemDrawableGV::icon() const
{
    return QIcon(":/icons/cursor.png");
}

QString CT_ActionSelectItemDrawableGV::type() const
{
    return CT_AbstractAction::TYPE_SELECTION;
}

void CT_ActionSelectItemDrawableGV::init()
{
    CT_AbstractActionForGraphicsView::init();

    if(nOptions() == 0)
    {
        m_selectTool = new CT_ActionSelectTool();
        // create the option widget if it was not already created
        CT_ActionSelectItemDrawableGVOptions *option = new CT_ActionSelectItemDrawableGVOptions(this, m_selectTool);
        m_selectTool->init(graphicsView(), option);

        // add the options to the graphics view²
        graphicsView()->addActionOptions(option);

        connect(m_selectTool, SIGNAL(selectionModeChanged(GraphicsViewInterface::SelectionMode)), option, SLOT(setSelectionMode(GraphicsViewInterface::SelectionMode)));

        // register the option to the superclass, so the hideOptions and showOptions
        // is managed automatically
        registerOption(option);

        connect(m_selectTool, SIGNAL(needRedrawOverlay()), this, SLOT(redrawOverlay()));
    }
}

bool CT_ActionSelectItemDrawableGV::mousePressEvent(QMouseEvent *e)
{    
    return m_selectTool->mousePressEvent(e);
}

bool CT_ActionSelectItemDrawableGV::mouseMoveEvent(QMouseEvent *e)
{
    return m_selectTool->mouseMoveEvent(e);
}

bool CT_ActionSelectItemDrawableGV::mouseReleaseEvent(QMouseEvent *e)
{
    return m_selectTool->mouseReleaseEvent(e);
}

bool CT_ActionSelectItemDrawableGV::wheelEvent(QWheelEvent *e)
{
    return m_selectTool->wheelEvent(e);
}

bool CT_ActionSelectItemDrawableGV::keyPressEvent(QKeyEvent *e)
{
    return m_selectTool->keyPressEvent(e);
}

bool CT_ActionSelectItemDrawableGV::keyReleaseEvent(QKeyEvent *e)
{
    return m_selectTool->keyReleaseEvent(e);
}

void CT_ActionSelectItemDrawableGV::drawOverlay(GraphicsViewInterface &view, QPainter &painter)
{   
    m_selectTool->drawOverlay(view, painter);
}

CT_AbstractAction* CT_ActionSelectItemDrawableGV::copy() const
{
    return new CT_ActionSelectItemDrawableGV();
}

void CT_ActionSelectItemDrawableGV::redrawOverlay()
{
    document()->redrawGraphics();
}
