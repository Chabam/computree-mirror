#include "dm_amkglpermanentpainter.h"

#include "scene/permanentscene.h"
#include "tools/math.h"

#include "ct_itemdrawable/abstract/ct_abstractmeshmodel.h"
#include "ct_pointcloudindex/abstract/ct_abstractpointcloudindex.h"

DM_AMKglPermanentPainter::DM_AMKglPermanentPainter(GraphicsViewInterface& view) : m_view(view)
{
    m_scene = NULL;
    m_info = NULL;
    m_nPolygonPoints = 0;
    m_enableSetColorCount = 0;
}

DM_AMKglPermanentPainter::~DM_AMKglPermanentPainter()
{
}

void DM_AMKglPermanentPainter::setPermanentScene(const PermanentScene *scene)
{
    m_scene = (PermanentScene*)scene;
}

PermanentScene *DM_AMKglPermanentPainter::getPermanentScene() const
{
    return m_scene;
}

void DM_AMKglPermanentPainter::setItemInformation(const PermanentItemInformation *info)
{
    m_info = (PermanentItemInformation*)info;
}

PermanentItemInformation* DM_AMKglPermanentPainter::getItemInformation() const
{
    return m_info;
}

void DM_AMKglPermanentPainter::setPen(const QPen &pen)
{
    setColor(pen.color());
}

void DM_AMKglPermanentPainter::enableSetColor(bool enable)
{
    if(enable)
        ++m_enableSetColorCount;
    else
        --m_enableSetColorCount;
}

void DM_AMKglPermanentPainter::setColor(int r, int g, int b)
{
    setColor(QColor(r, g, b));
}

void DM_AMKglPermanentPainter::setColor(QColor color)
{
    if(m_enableSetColorCount == 0) {
        m_currentColor = Basic::LocalColor(color);
        m_scene->setCurrentColor(color);
    }
}

QColor DM_AMKglPermanentPainter::getColor()
{
    return m_scene->getCurrentColor();
}

void DM_AMKglPermanentPainter::drawPoint(const double &x, const double &y, const double &z)
{
    IChunk* cI;
    getPermanentScene()->addLocalPoint(Eigen::Vector3d(x, y, z), &cI, m_currentColor);
    m_info->addObject(Scene::Point, 1, cI);
}

void DM_AMKglPermanentPainter::drawPoint(double *p)
{
    drawPoint(p[0], p[1], p[2]);
}

void DM_AMKglPermanentPainter::drawPoint(const size_t &globalIndex)
{
    getPermanentScene()->addPoint(globalIndex, m_info);
}

void DM_AMKglPermanentPainter::drawPointCloud(const CT_AbstractCloudIndex *pci)
{
    getPermanentScene()->addPoints(pci, m_info);
}

void DM_AMKglPermanentPainter::drawCube(const double &x1, const double &y1, const double &z1,
                                const double &x2, const double &y2, const double &z2)
{
    IChunk* cI;

    getPermanentScene()->addBox((x1+x2)/2.0, (y1+y2)/2.0, (z1+z2)/2.0,
                                x2-x1,
                                y2-y1,
                                z2-z1,
                                Eigen::Vector3f(1, 0, 0),
                                Eigen::Vector3f(0, 1, 0),
                                &cI);

    m_info->addObject(Scene::Box, 1, cI);
}

void DM_AMKglPermanentPainter::drawCube(const double &x1, const double &y1, const double &z1,
                                const double &x2, const double &y2, const double &z2,
                                GLenum faces, GLenum mode)
{
    Q_UNUSED(faces)

    if(mode == GL_POINT) {
        drawPoint(x1, y1, z1);
        drawPoint(x1, y1, z2);
        drawPoint(x2, y1, z2);
        drawPoint(x2, y1, z1);

        drawPoint(x1, y2, z1);
        drawPoint(x1, y2, z2);
        drawPoint(x2, y2, z2);
        drawPoint(x2, y2, z1);

        return;
    } else if(mode == GL_LINE) {
        // front
        drawQuadFace(x1, y1, z2,
                     x1, y2, z2,
                     x2, y2, z2,
                     x2, y1, z2);

        // back
        drawQuadFace(x1, y1, z1,
                     x1, y2, z1,
                     x2, y2, z1,
                     x2, y1, z1);

        // left
        drawQuadFace(x1, y1, z2,
                     x1, y2, z2,
                     x1, y2, z1,
                     x1, y1, z1);

        // right
        drawQuadFace(x2, y1, z2,
                     x2, y2, z2,
                     x2, y2, z1,
                     x2, y1, z1);

        // top
        drawQuadFace(x1, y2, z2,
                     x1, y2, z1,
                     x2, y2, z1,
                     x2, y2, z2);

        // bottom
        drawQuadFace(x1, y1, z2,
                     x1, y1, z1,
                     x2, y1, z1,
                     x2, y1, z2);
        return;
    }

    drawCube(x1, y1, z1, x2, y2, z2);
}

void DM_AMKglPermanentPainter::drawQuadFace(const double &x1, const double &y1, const double &z1,
                                    const double &x2, const double &y2, const double &z2,
                                    const double &x3, const double &y3, const double &z3,
                                    const double &x4, const double &y4, const double &z4)
{
    IChunk* cI;

    getPermanentScene()->addLine(Eigen::Vector3d(x1, y1, z1), Eigen::Vector3d(x2, y2, z2), &cI,
                                 m_currentColor,
                                 m_currentColor);
    m_info->addObject(Scene::Line, 1, cI);

    getPermanentScene()->addLine(Eigen::Vector3d(x2, y2, z2), Eigen::Vector3d(x3, y3, z3), &cI,
                                 m_currentColor,
                                 m_currentColor);
    m_info->addObject(Scene::Line, 1, cI);

    getPermanentScene()->addLine(Eigen::Vector3d(x3, y3, z3), Eigen::Vector3d(x4, y4, z4), &cI,
                                 m_currentColor,
                                 m_currentColor);
    m_info->addObject(Scene::Line, 1, cI);

    getPermanentScene()->addLine(Eigen::Vector3d(x4, y4, z4), Eigen::Vector3d(x1, y1, z1), &cI,
                                 m_currentColor,
                                 m_currentColor);
    m_info->addObject(Scene::Line, 1, cI);

}

void DM_AMKglPermanentPainter::fillQuadFace(const double &x1, const double &y1, const double &z1,
                                            const double &x2, const double &y2, const double &z2,
                                            const double &x3, const double &y3, const double &z3,
                                            const double &x4, const double &y4, const double &z4)
{
    /*Q_UNUSED(x4)
    Q_UNUSED(y4)
    Q_UNUSED(z4)

    IChunk* cI;

    getPermanentScene()->addQuads((x1+x3)/2.0, (y1+y3)/2.0, (z1+z3)/2.0,
                                  Math::distancePoints(Eigen::Vector3d(x1, y1, z1), Eigen::Vector3d(x2, y2, z2)),
                                  Math::distancePoints(Eigen::Vector3d(x2, y2, z2), Eigen::Vector3d(x3, y3, z3)),
                                  Eigen::Vector3f(x2-x1, y2-y1, z2-z1),
                                  Eigen::Vector3f(x3-x2, y3-y2, z3-z2),
                                  &cI);

    m_info->addObject(Scene::Quads, 1, cI);*/

    drawTriangle(x1, y1, z1,
                 x2, y2, z2,
                 x3, y3, z3);

    drawTriangle(x3, y3, z3,
                 x4, y4, z4,
                 x1, y1, z1);
}

void DM_AMKglPermanentPainter::drawQuadFace(const double &x1, const double &y1, const double &z1,
                                    int r1, int g1, int b1,
                                    const double &x2, const double &y2, const double &z2,
                                    int r2, int g2, int b2,
                                    const double &x3, const double &y3, const double &z3,
                                    int r3, int g3, int b3, const double &x4,
                                    const double &y4, const double &z4,
                                    int r4, int g4, int b4)
{
    IChunk* cI;

    getPermanentScene()->addLine(Eigen::Vector3d(x1, y1, z1),
                                 Eigen::Vector3d(x2, y2, z2),
                                 &cI,
                                 QColor(r1, g1, b1),
                                 QColor(r2, g2, b2));
    m_info->addObject(Scene::Line, 1, cI);

    getPermanentScene()->addLine(Eigen::Vector3d(x2, y2, z2),
                                 Eigen::Vector3d(x3, y3, z3),
                                 &cI,
                                 QColor(r2, g2, b2),
                                 QColor(r3, g3, b3));
    m_info->addObject(Scene::Line, 1, cI);

    getPermanentScene()->addLine(Eigen::Vector3d(x3, y3, z3),
                                 Eigen::Vector3d(x4, y4, z4),
                                 &cI,
                                 QColor(r3, g3, b3),
                                 QColor(r4, g4, b4));
    m_info->addObject(Scene::Line, 1, cI);

    getPermanentScene()->addLine(Eigen::Vector3d(x4, y4, z4),
                                 Eigen::Vector3d(x1, y1, z1),
                                 &cI,
                                 QColor(r4, g4, b4),
                                 QColor(r1, g1, b1));
    m_info->addObject(Scene::Line, 1, cI);
}

void DM_AMKglPermanentPainter::fillQuadFace(const double &x1, const double &y1, const double &z1,
                                    int r1, int g1, int b1,
                                    const double &x2, const double &y2, const double &z2,
                                    int r2, int g2, int b2,
                                    const double &x3, const double &y3, const double &z3,
                                    int r3, int g3, int b3,
                                    const double &x4, const double &y4, const double &z4,
                                    int r4, int g4, int b4)
{
    IChunk* cI;
    getPermanentScene()->addTriangle(Eigen::Vector3d(x1, y1, z1),
                                     Eigen::Vector3d(x2, y2, z2),
                                     Eigen::Vector3d(x3, y3, z3),
                                     &cI,
                                     QColor(r1, g1, b1),
                                     QColor(r2, g2, b2),
                                     QColor(r3, g3, b3));

    m_info->addObject(Scene::Triangle, 1, cI);

    getPermanentScene()->addTriangle(Eigen::Vector3d(x3, y3, z3),
                                     Eigen::Vector3d(x4, y4, z4),
                                     Eigen::Vector3d(x1, y1, z1),
                                     &cI,
                                     QColor(r3, g3, b3),
                                     QColor(r4, g4, b4),
                                     QColor(r1, g1, b1));

    m_info->addObject(Scene::Triangle, 1, cI);
}

void DM_AMKglPermanentPainter::drawRectXY(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &z)
{
    IChunk* cI;

    getPermanentScene()->addLine(Eigen::Vector3d(topLeft[0], topLeft[1], z), Eigen::Vector3d(topLeft[0], bottomRight[1], z),
            &cI,
            m_currentColor,
            m_currentColor);
    m_info->addObject(Scene::Line, 1, cI);

    getPermanentScene()->addLine(Eigen::Vector3d(topLeft[0], bottomRight[1], z), Eigen::Vector3d(bottomRight[0], bottomRight[1], z), &cI,
            m_currentColor,
            m_currentColor);
    m_info->addObject(Scene::Line, 1, cI);

    getPermanentScene()->addLine(Eigen::Vector3d(bottomRight[0], bottomRight[1], z), Eigen::Vector3d(bottomRight[0], topLeft[1], z), &cI,
            m_currentColor,
            m_currentColor);
    m_info->addObject(Scene::Line, 1, cI);

    getPermanentScene()->addLine(Eigen::Vector3d(bottomRight[0], topLeft[1], z), Eigen::Vector3d(topLeft[0], topLeft[1], z), &cI,
            m_currentColor,
            m_currentColor);
    m_info->addObject(Scene::Line, 1, cI);
}

void DM_AMKglPermanentPainter::fillRectXY(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &z)
{
    fillQuadFace(topLeft[0], topLeft[1], z,
                 bottomRight[0], topLeft[1], z,
                 bottomRight[0], bottomRight[1], z,
                 topLeft[0], bottomRight[1], z);
}

void DM_AMKglPermanentPainter::drawRectXZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &y)
{
    IChunk* cI;

    getPermanentScene()->addLine(Eigen::Vector3d(topLeft[0], y, topLeft[1]), Eigen::Vector3d(bottomRight[0], y, topLeft[1]), &cI,
            m_currentColor,
            m_currentColor);
    m_info->addObject(Scene::Line, 1, cI);

    getPermanentScene()->addLine(Eigen::Vector3d(bottomRight[0], y, topLeft[1]), Eigen::Vector3d(bottomRight[0], y, bottomRight[1]), &cI,
            m_currentColor,
            m_currentColor);
    m_info->addObject(Scene::Line, 1, cI);

    getPermanentScene()->addLine(Eigen::Vector3d(bottomRight[0], y, bottomRight[1]), Eigen::Vector3d(topLeft[0], y, bottomRight[1]), &cI,
            m_currentColor,
            m_currentColor);
    m_info->addObject(Scene::Line, 1, cI);

    getPermanentScene()->addLine(Eigen::Vector3d(topLeft[0], y, bottomRight[1]), Eigen::Vector3d(topLeft[0], y, topLeft[1]), &cI,
            m_currentColor,
            m_currentColor);
    m_info->addObject(Scene::Line, 1, cI);
}

void DM_AMKglPermanentPainter::fillRectXZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &y)
{
    fillQuadFace(topLeft[0], y, topLeft[1],
                 bottomRight[0], y, topLeft[1],
                 bottomRight[0], y, bottomRight[1],
                 topLeft[0], y, bottomRight[1]);
}

void DM_AMKglPermanentPainter::drawRectYZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &x)
{
    IChunk* cI;

    getPermanentScene()->addLine(Eigen::Vector3d(x, topLeft[0], topLeft[1]), Eigen::Vector3d(x, bottomRight[0], topLeft[1]), &cI,
            m_currentColor,
            m_currentColor);
    m_info->addObject(Scene::Line, 1, cI);

    getPermanentScene()->addLine(Eigen::Vector3d(x, bottomRight[0], topLeft[1]), Eigen::Vector3d(x, bottomRight[0], bottomRight[1]), &cI,
            m_currentColor,
            m_currentColor);
    m_info->addObject(Scene::Line, 1, cI);

    getPermanentScene()->addLine(Eigen::Vector3d(x, bottomRight[0], bottomRight[1]), Eigen::Vector3d(x, topLeft[0], bottomRight[1]), &cI,
            m_currentColor,
            m_currentColor);
    m_info->addObject(Scene::Line, 1, cI);

    getPermanentScene()->addLine(Eigen::Vector3d(x, topLeft[0], bottomRight[1]), Eigen::Vector3d(x, topLeft[0], topLeft[1]), &cI,
            m_currentColor,
            m_currentColor);
    m_info->addObject(Scene::Line, 1, cI);
}

void DM_AMKglPermanentPainter::fillRectYZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &x)
{
    fillQuadFace(x, topLeft[0], topLeft[1],
                 x, bottomRight[0], topLeft[1],
                 x, bottomRight[0], bottomRight[1],
                 x, topLeft[0], bottomRight[1]);
}

void DM_AMKglPermanentPainter::drawLine(const double &x1, const double &y1, const double &z1,
                                const double &x2, const double &y2, const double &z2)
{
    IChunk* cI;

    getPermanentScene()->addLine(Eigen::Vector3d(x1, y1, z1), Eigen::Vector3d(x2, y2, z2), &cI,
                                 m_currentColor,
                                 m_currentColor);
    m_info->addObject(Scene::Line, 1, cI);
}

void DM_AMKglPermanentPainter::drawLine(const size_t& globalEdgeIndex,
                                        const size_t &p1GlobalIndex,
                                        const size_t &p2GlobalIndex)
{
    IChunk* cI;

    getPermanentScene()->addMeshEdge(globalEdgeIndex,
                                     p1GlobalIndex,
                                     p2GlobalIndex,
                                     &cI);
    m_info->addObject(Scene::LineGlobalCloud, 1, cI);
}

void DM_AMKglPermanentPainter::drawTriangle(const double &x1, const double &y1, const double &z1,
                                    const double &x2, const double &y2, const double &z2,
                                    const double &x3, const double &y3, const double &z3)
{
    IChunk* cI;
    getPermanentScene()->addTriangle(Eigen::Vector3d(x1, y1, z1),
                                     Eigen::Vector3d(x2, y2, z2),
                                     Eigen::Vector3d(x3, y3, z3),
                                     &cI,
                                     m_currentColor,
                                     m_currentColor,
                                     m_currentColor);

    m_info->addObject(Scene::Triangle, 1, cI);
}

void DM_AMKglPermanentPainter::drawTriangle(const size_t &globalFaceIndex,
                                            const size_t &p1GlobalIndex,
                                            const size_t &p2GlobalIndex,
                                            const size_t &p3GlobalIndex)
{
    IChunk* cI;
    getPermanentScene()->addMeshFace(globalFaceIndex,
                                     p1GlobalIndex,
                                     p2GlobalIndex,
                                     p3GlobalIndex,
                                     &cI);

    m_info->addObject(Scene::TriangleGlobalCloud, 1, cI);
}

void DM_AMKglPermanentPainter::drawCircle(const double &x, const double &y, const double &z, const double &radius)
{
    IChunk* cI;
    getPermanentScene()->addEllipse(x, y, z, radius, radius, Eigen::Vector3f(0, 1, 0), Eigen::Vector3f(0, 0, 1), &cI);
    m_info->addObject(Scene::Ellipse, 1, cI);
}

void DM_AMKglPermanentPainter::drawCircle3D(const Eigen::Vector3d &center, const Eigen::Vector3d &direction, const double &radius)
{
    IChunk* cI;
    getPermanentScene()->addEllipse(center[0], center[1], center[2], radius, radius, Eigen::Vector3f(0, 1, 0), direction.cast<float>(), &cI);
    m_info->addObject(Scene::Ellipse, 1, cI);
}

void DM_AMKglPermanentPainter::drawEllipse(const double &x, const double &y, const double &z,
                                   const double &radiusA,
                                   const double &radiusB)
{
    IChunk* cI;
    getPermanentScene()->addEllipse(x, y, z, radiusA, radiusB, Eigen::Vector3f(1, 0, 0), Eigen::Vector3f(0, 1, 0), &cI);
    m_info->addObject(Scene::Ellipse, 1, cI);
}

void DM_AMKglPermanentPainter::beginPolygon()
{
    m_nPolygonPoints = 0;
}

void DM_AMKglPermanentPainter::addPointToPolygon(const double &x, const double &y, const double &z)
{
    if(m_nPolygonPoints == 0)
        m_firstPolygonPoint = Eigen::Vector3d(x, y, z);
    else {
        IChunk* cI;
        getPermanentScene()->addLine(m_lastPolygonPoint, Eigen::Vector3d(x, y, z), &cI,
                                     m_currentColor,
                                     m_currentColor);
        m_info->addObject(Scene::Line, 1, cI);
        ++m_nPolygonPoints;
    }

    m_lastPolygonPoint = Eigen::Vector3d(x, y, z);
}

void DM_AMKglPermanentPainter::endPolygon()
{
    if(m_nPolygonPoints > 2) {
        IChunk* cI;
        getPermanentScene()->addLine(m_lastPolygonPoint, m_firstPolygonPoint, &cI,
                                     m_currentColor,
                                     m_currentColor);
        m_info->addObject(Scene::Line, 1, cI);
    }

    m_nPolygonPoints = 0;
}

void DM_AMKglPermanentPainter::drawCylinder(const double &x, const double &y, const double &z, const double &radius, const double &height)
{
    IChunk* cI;
    getPermanentScene()->addCylinder(x, y, z, radius, height, radius, Eigen::Vector3f(1, 0, 0), Eigen::Vector3f(0, 1, 0), &cI);
    m_info->addObject(Scene::Cylinder, 1, cI);
}

void DM_AMKglPermanentPainter::drawCylinder3D(const Eigen::Vector3d &center, const Eigen::Vector3d &direction, const double &radius, const double &height)
{
    IChunk* cI;

    Eigen::Vector3d nD = Eigen::Vector3d(0, 1, 0).cross(direction);

    if(nD.norm() == 0)
        nD = Eigen::Vector3d(0, 0, 1).cross(direction);

    getPermanentScene()->addCylinder(center[0], center[1], center[2], radius, height, radius, nD.cast<float>(), direction.cast<float>(), &cI);
    m_info->addObject(Scene::Cylinder, 1, cI);
}

void DM_AMKglPermanentPainter::drawPyramid(const double &topX, const double &topY, const double &topZ,
                                   const double &base1X, const double &base1Y, const double &base1Z,
                                   const double &base2X, const double &base2Y, const double &base2Z,
                                   const double &base3X, const double &base3Y, const double &base3Z,
                                   const double &base4X, const double &base4Y, const double &base4Z)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(base4X)
    Q_UNUSED(base4Y)
    Q_UNUSED(base4Z)

    // TODO : check if base4 is necessary
#endif
    IChunk* cI;

    double cx = (base1X+base3X)/2.0;
    double cy = (base1Y+base3Y)/2.0;
    double cz = (base1Z+base3Z)/2.0;

    Eigen::Vector3d wDir(base2X-base1X, base2Y-base1Y, base2Z-base1Z);
    Eigen::Vector3d hDir(topX-cx, topY-cy, topZ-cz);

    float w = Math::distancePoints(Eigen::Vector3d(base1X, base1Y, base1Z), Eigen::Vector3d(base2X, base2Y, base2Z));
    float h = Math::distancePoints(Eigen::Vector3d(cx, cy, cz), Eigen::Vector3d(topX, topY, topZ));
    float l = Math::distancePoints(Eigen::Vector3d(base2X, base2Y, base2Z), Eigen::Vector3d(base3X, base3Y, base3Z));

    cx = (cx+topX)/2.0;
    cy = (cy+topY)/2.0;
    cz = (cz+topZ)/2.0;

    getPermanentScene()->addPyramid(cx, cy, cz, w, h, l, wDir.cast<float>(), hDir.cast<float>(), &cI);
    m_info->addObject(Scene::Pyramid, 1, cI);
}

void DM_AMKglPermanentPainter::drawPartOfSphere(const double &centerX, const double &centerY, const double &centerZ,
                                        const double &radius,
                                        const double &initTheta, const double &endTheta,
                                        const double &initPhi, const double &endPhi,
                                        bool radians)
{
    IChunk* cI;

    getPermanentScene()->addSphere(centerX, centerY, centerZ, radius, initTheta, endTheta, initPhi, endPhi, radians, &cI);
    m_info->addObject(Scene::Sphere, 1, cI);
}

void DM_AMKglPermanentPainter::drawMesh(const CT_AbstractMeshModel *mesh)
{
    drawFaces(mesh);
}

void DM_AMKglPermanentPainter::drawFaces(const CT_AbstractMeshModel *mesh)
{
    ((CT_AbstractMeshModel*)mesh)->drawFaces(m_view, *this);
}

void DM_AMKglPermanentPainter::drawEdges(const CT_AbstractMeshModel *mesh)
{
    ((CT_AbstractMeshModel*)mesh)->drawEdges(m_view, *this);
}

void DM_AMKglPermanentPainter::drawPoints(const CT_AbstractMeshModel *mesh)
{
    drawPointCloud(mesh->pointCloudIndex());
}
