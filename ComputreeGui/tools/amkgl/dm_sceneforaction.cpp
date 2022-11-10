#include "dm_sceneforaction.h"

#include "tools/amkgl/dm_amkglpermanentpainter.h"
#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"

DM_SceneForAction::DM_SceneForAction()
{
    m_action = nullptr;
    m_graphicsView = nullptr;
    m_currentPainter = nullptr;

    m_scene = new PermanentScene();

    m_scene->setAttributesAccessor(this);
    m_scene->init();
    m_scene->setDrawModeToUse(DM_RAW);
}

DM_SceneForAction::~DM_SceneForAction()
{
    qDeleteAll(m_elementsInformations.begin(), m_elementsInformations.end());
    delete m_scene;
}

void DM_SceneForAction::setAction(const CT_AbstractActionForGraphicsView *action, GraphicsViewInterface& graphicsView)
{
    if(m_action != nullptr) {
        disconnect(action, nullptr, this, nullptr);

        qDeleteAll(m_elementsInformations.begin(), m_elementsInformations.end());
        m_elementsInformations.clear();

        m_scene->clearAllObjects();
        m_scene->updateInAllContext();
    }

    m_action = (CT_AbstractActionForGraphicsView*)action;
    m_graphicsView = (GraphicsViewInterface*)&graphicsView;

    if(m_action != nullptr) {
        connect(m_action, SIGNAL(drawing3DChanged()), this, SLOT(updateActionObjects()));
        connect(m_action, SIGNAL(new3DElementDraw(quint32)), this, SLOT(createNewElementInformation(quint32)), Qt::DirectConnection);
    }

    updateActionObjects();
}

void DM_SceneForAction::addedTo(const IGraphicsDocument *doc, const QOpenGLContext *newContext)
{
    m_scene->addedTo(doc, newContext);
}

void DM_SceneForAction::removedFrom(const IGraphicsDocument *doc, const QOpenGLContext *newContext)
{
    m_scene->removedFrom(doc, newContext);
}

void DM_SceneForAction::fastDraw(DrawInfo &info)
{
    m_scene->fastDraw(info);
}

void DM_SceneForAction::draw(DrawInfo &info)
{
    m_scene->draw(info);
}

void DM_SceneForAction::drawOverlay(QPainter &painter, DrawInfo &info)
{
    Q_UNUSED(info)

    if(m_action != nullptr)
        m_action->drawOverlay(*m_graphicsView, painter);
}

PermanentItemInformation* DM_SceneForAction::getElementInformation(const quint32 &uniqueIndex)
{
    return m_elementsInformations.value(uniqueIndex, nullptr);
}

bool DM_SceneForAction::mustBeUpdated() const
{
    return m_scene->mustBeUpdated();
}

void DM_SceneForAction::checkAndUpdateIfItNeedsToBe()
{
    if(m_scene->mustBeUpdated())
        m_scene->updateInAllContext();
}

void DM_SceneForAction::updateActionObjects()
{
    if(m_action != nullptr) {
        ActionElementsInfoCollectionIterator it(m_elementsInformations);

        while(it.hasNext())
            it.next().value()->reset();

        qDeleteAll(m_elementsInformations.begin(), m_elementsInformations.end());
        m_elementsInformations.clear();

        DM_AMKglPermanentPainter painter(*m_graphicsView);
        m_currentPainter = &painter;

        painter.setPermanentScene(m_scene);
        createNewElementInformation(0);

        m_action->draw(*m_graphicsView, painter);

        m_currentPainter = nullptr;

        m_scene->updateInAllContext();
    }
}

void DM_SceneForAction::createNewElementInformation(quint32 uniqueIndex)
{
    Q_ASSERT(m_currentPainter != nullptr);
    if (m_currentPainter == nullptr) {qDebug() << "DM_SceneForAction::createNewElementInformation" << ", " <<  "m_currentPainter == nullptr"; return;}

    PermanentItemInformation* info = m_elementsInformations.value(uniqueIndex, nullptr);

    if(info == nullptr) {
        info = new PermanentItemInformation(uniqueIndex);
        m_elementsInformations.insert(uniqueIndex, info);
    }

    m_currentPainter->setItemInformation(info);
}
