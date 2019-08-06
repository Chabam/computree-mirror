#include "graphicsviewdebugmode.h"

#include <QMouseEvent>
#include <QMenu>

#include "view/DocumentView/GraphicsViews/3D/ggraphicsviewimp.h"

#include "ct_global/ct_context.h"
#include "ct_coordinates/tools/ct_coordinatesystemmanager.h"
#include "ct_accessor/ct_pointaccessor.h"

#include "scene/permanentitemscenebymodel.h"
#include "scene/permanentsceneelementtype.h"
#include "visitor/applycustomfunctiontoobjectvisitor.h"
#include "visitor/applycustomfunctiontoglobalpointsvisitor.h"
#include "actions/picking/actionpickanyelements.h"
#include "tools/opengl/shapetovolume.h"
#include "tools/drawmodeconverter.h"

#include "painterinterface.h"

GraphicsViewDebugMode::GraphicsViewDebugMode() : CT_AbstractActionForGraphicsView()
{
    m_mustShowCoordinateSystem = false;
    m_mustShowCameraPivotPoint = false;
    m_mustShowChunksInformations = false;
}

GraphicsViewDebugMode::~GraphicsViewDebugMode()
{
}

QString GraphicsViewDebugMode::title() const
{
    return tr("Debug mode");
}

QString GraphicsViewDebugMode::description() const
{
    return tr("Permet d'afficher des informations de débogage");
}

QIcon GraphicsViewDebugMode::icon() const
{
    return QIcon(":/Icones/Icones/debug.png");
}

QString GraphicsViewDebugMode::type() const
{
    return CT_AbstractAction::TYPE_OTHER;
}

bool GraphicsViewDebugMode::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::RightButton)
        m_mustShowContextMenu = true;

    return CT_AbstractActionForGraphicsView::mousePressEvent(e);
}

bool GraphicsViewDebugMode::mouseMoveEvent(QMouseEvent *e)
{
    m_mustShowContextMenu = false;

    return CT_AbstractActionForGraphicsView::mouseMoveEvent(e);
}

bool GraphicsViewDebugMode::mouseReleaseEvent(QMouseEvent *e)
{
    if((e->button() == Qt::RightButton) && m_mustShowContextMenu)
        showContextMenu(e->globalPos());

    return CT_AbstractActionForGraphicsView::mouseReleaseEvent(e);
}

void GraphicsViewDebugMode::draw(GraphicsViewInterface &view, PainterInterface &painter)
{
    Q_UNUSED(view)

    const int s = m_polygonToShowIn3D.size();

    for(int i=0; i<s; ++i)
    {
        const TrianglePlane& tp = m_polygonToShowIn3D.at(i);

        for(int j=0; j<3; ++j)
        {
            const Plane& p = tp.planes[j];

            if(j == 0)
                painter.setColor(QColor(255, 0, 0, 127));
            else if(j == 1)
                painter.setColor(QColor(0, 255, 0, 127));
            else
                painter.setColor(QColor(0, 0, 255, 127));

            painter.fillQuadFace(p.m_vertex[0][0], p.m_vertex[0][1], p.m_vertex[0][2],
                    p.m_vertex[1][0], p.m_vertex[1][1], p.m_vertex[1][2],
                    p.m_vertex[2][0], p.m_vertex[2][1], p.m_vertex[2][2],
                    p.m_vertex[3][0], p.m_vertex[3][1], p.m_vertex[3][2]);
        }
    }

    if(m_mustShowCameraPivotPoint) {
        painter.setColor(Qt::green);
        painter.drawPoint(view.camera()->cx(), view.camera()->cy(), view.camera()->cz());
    }
}

void GraphicsViewDebugMode::drawOverlay(GraphicsViewInterface &view, QPainter &painter)
{
    graphicsViewToDebug()->drawText(0, 0, tr("Debug mode - activé"));

    if(m_mustShowCoordinateSystem)
        drawCoordinateSystemInformations(view, painter);

    if(m_mustShowChunksInformations)
        drawChunksInformations(view, painter);
}

void GraphicsViewDebugMode::showContextMenu(const QPoint& pos)
{
    QMenu menu;
    menu.addAction(tr("Colorier les objets de chaque Chunk d'une couleur différente"), this, SLOT(setAutomaticColorToChunk()));

    QAction* act = menu.addAction(tr("Afficher les systèmes de coordonnées"));
    act->setCheckable(true);
    act->setChecked(m_mustShowCoordinateSystem);
    connect(act, SIGNAL(triggered(bool)), this, SLOT(showCoordinateSystem(bool)), Qt::DirectConnection);

    act = menu.addAction(tr("Afficher des informations sur les chunks"));
    act->setCheckable(true);
    act->setChecked(m_mustShowChunksInformations);
    connect(act, SIGNAL(triggered(bool)), this, SLOT(showInformationsOfChunks(bool)), Qt::DirectConnection);

    act = menu.addAction(tr("Afficher le polygon de sélection en 3D"));
    act->setCheckable(true);
    act->setChecked(!m_polygonToShowIn3D.isEmpty());
    connect(act, SIGNAL(triggered(bool)), this, SLOT(showPickingPolygonIn3D(bool)), Qt::DirectConnection);

    if(!m_polygonToShowIn3D.isEmpty())
        menu.addAction(tr("Colorier les points en vert si ils font partie de la sélection"), this, SLOT(showSelectionResult()));

    act = menu.addAction(tr("Affiche le point de pivot de la caméra"));
    act->setCheckable(true);
    act->setChecked(m_mustShowCameraPivotPoint);
    connect(act, SIGNAL(triggered(bool)), this, SLOT(showCameraPivotPoint(bool)), Qt::DirectConnection);

    menu.exec(pos);
}

GGraphicsViewImp *GraphicsViewDebugMode::graphicsViewToDebug() const
{
    return dynamic_cast<GGraphicsViewImp*>(graphicsView());
}

void GraphicsViewDebugMode::drawCoordinateSystemInformations(GraphicsViewInterface &view, QPainter &painter)
{
    Q_UNUSED(view)
    Q_UNUSED(painter)

    GGraphicsViewImp* gv = graphicsViewToDebug();

    int n = PS_COORDINATES_SYS_MANAGER->size();

    Eigen::Vector3d offset;

    for(int i=0; i<n; ++i) {
        CT_AbstractCoordinateSystem* cs = PS_COORDINATES_SYS_MANAGER->coordinateSystemAt(i);
        cs->offset(offset);

        gv->drawText(0, 0, QString("CS %1 [%2;%3;%4]").arg(i).arg(offset[0]).arg(offset[1]).arg(offset[2]));
    }
}

void GraphicsViewDebugMode::drawChunksInformations(GraphicsViewInterface &view, QPainter &painter)
{
    Q_UNUSED(view)
    Q_UNUSED(painter)

    GGraphicsViewImp* gv = graphicsViewToDebug();

    PermanentItemScene* scene = graphicsViewToDebug()->getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr);

    if(scene != nullptr) {
        QString previousType;

        ApplyCustomFunctionToObjectVisitor visitor;
        visitor.setVisitFunction([&gv, &previousType](const IChunk* chunk) {

            const Eigen::Vector3d& offset = chunk->getOffset();

            const QString newType = Scene::objectTypeToString(chunk->getTypeOfObjectDrawn());

            if(newType != previousType) {
                gv->drawText(0, 0, "--- " + newType + " ---");
                previousType = newType;
            }

            const QString drawModeUsed = gv->isFastDrawModeCurrentlyUsed() ? DrawModeConverter::drawModeToQString(chunk->getFastDrawModeUsed()) : DrawModeConverter::drawModeToQString(chunk->getNormalDrawModeUsed());

            gv->drawText(0, 0, QString("Chunk %1 [%2;%3;%4] (%5 objets) {%6}").arg(chunk->getUniqueKey()).arg(offset[0]).arg(offset[1]).arg(offset[2]).arg(chunk->countObjects()).arg(drawModeUsed));
        });

        scene->visitObjects(visitor);
    }
}

void GraphicsViewDebugMode::setAutomaticColorToChunk()
{
    PermanentItemScene* scene = graphicsViewToDebug()->getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr);

    if(scene != nullptr) {
        QVector<Basic::LocalColor> colors(6);
        colors[0] = Qt::magenta;
        colors[1] = Qt::green;
        colors[2] = Qt::blue;
        colors[3] = Qt::yellow;
        colors[4] = Qt::cyan;
        colors[5] = Qt::gray;

        int colorIndex = 0;

        ApplyCustomFunctionToObjectVisitor visitor;
        visitor.setVisitFunction([&colors, &colorIndex](const IChunk* chunk) {
            Basic::AbstractColorCloud* cloud = ((IChunk*)chunk)->createOrGetObjectColorCloud();
            const size_t count = chunk->countObjects();
            const Basic::LocalColor& newColor = colors[colorIndex++];

            for(size_t i=0; i<count; ++i) {
                (*cloud)[i] = newColor;
            }

            if(colorIndex == colors.size())
                colorIndex = 0;
        });

        scene->visitObjects(visitor);
        scene->updateItemsColorInAllContext();
    }
}

void GraphicsViewDebugMode::showCoordinateSystem(bool enabled)
{
    m_mustShowCoordinateSystem = enabled;
}

void GraphicsViewDebugMode::showInformationsOfChunks(bool enabled)
{
    m_mustShowChunksInformations = enabled;
}

void GraphicsViewDebugMode::showPickingPolygonIn3D(bool enabled)
{
    if(enabled) {
        ActionPickAnyElements* picking = graphicsViewToDebug()->getPickingAction();

        if(picking != nullptr) {
            const QPolygon p = picking->getLastPolygon();

            m_polygonToShowIn3D = ShapeToVolume::staticTriangulatePolygonAndConvertItToPlanes(p, *graphicsViewToDebug()->amkglCamera());
        }
    } else {
        m_polygonToShowIn3D.clear();
    }

    setDrawing3DChanged();
}

void GraphicsViewDebugMode::showSelectionResult()
{
    PermanentItemScene* scene = graphicsViewToDebug()->getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr);

    if(scene != nullptr) {
        QVector<TrianglePlane>& polygons = m_polygonToShowIn3D;

        ApplyCustomFunctionToGlobalPointsVisitor visitor;
        visitor.setFunction([&polygons](IChunk* chunk,
                                        const Eigen::Vector3d &offset,
                                        std::vector<GLuint> &indices,
                                        const size_t& firstPointGlobalIndex) {

            Q_UNUSED(offset)

            CT_PointAccessor pAccessor;
            Basic::AbstractColorCloud* cc = chunk->createOrGetObjectColorCloud();

            CT_PointData* firstPoint = &pAccessor.internalPointAt(firstPointGlobalIndex);

            const size_t n = indices.size();
            const int nTriangles = polygons.size();
            int j, k;
            bool ok = false;

            for(size_t i=0; i<n ; ++i) {
                const size_t &index = indices[i];
                const FloatPoint &p = firstPoint[index];

                for(j=0; !ok && (j != nTriangles); ++j) {

                    const TrianglePlane &tr = polygons[j];

                    ok = true;

                    for(k=0; ok && k<3; ++k) {
                        if(tr.planes[k].distance(&p(0)) <= 0.0)
                            ok = false;
                    }
                }

                if(ok) {
                    (*cc)[firstPointGlobalIndex + index] = Basic::LocalColor(0, 255, 0, 255);
                    ok = false;
                } else {
                    (*cc)[firstPointGlobalIndex + index] = Basic::LocalColor(0, 0, 255, 255);
                }
            }
        });

        scene->visitPoints(visitor);
        scene->updatePointsColorInAllContext();
    }
}

void GraphicsViewDebugMode::showCameraPivotPoint(bool enabled)
{
    m_mustShowCameraPivotPoint = enabled;

    setDrawing3DChanged();
}
