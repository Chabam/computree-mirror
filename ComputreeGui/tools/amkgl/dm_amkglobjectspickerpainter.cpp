#include "dm_amkglobjectspickerpainter.h"

#include "picker/objectsprocessorforpicker.h"
#include "tools/math.h"

#include "ct_itemdrawable/abstract/ct_abstractmeshmodel.h"
#include "ct_pointcloudindex/abstract/ct_abstractpointcloudindex.h"

#define MODIFY_SELECTED(XXX) if(!m_selected) { if(XXX) { m_selected = true; } }

DM_AMKglObjectsPickerPainter::DM_AMKglObjectsPickerPainter(const ObjectsProcessorForPicker &processor)
{
    m_processor = (ObjectsProcessorForPicker*)&processor;
    m_selected = false;
}

bool DM_AMKglObjectsPickerPainter::isSelected() const
{
    return m_selected;
}

void DM_AMKglObjectsPickerPainter::drawPoint(const double &x, const double &y, const double &z)
{
    MODIFY_SELECTED(m_processor->computePoint(x, y, z));
}

void DM_AMKglObjectsPickerPainter::drawPoint(double *p)
{
    drawPoint(p[0], p[1], p[2]);
}

void DM_AMKglObjectsPickerPainter::drawPoint(const size_t &globalIndex)
{
    MODIFY_SELECTED(m_processor->computePoint(globalIndex));
}

void DM_AMKglObjectsPickerPainter::drawPointCloud(const CT_AbstractCloudIndex *pci)
{
    MODIFY_SELECTED(m_processor->computePoints(pci));
}

void DM_AMKglObjectsPickerPainter::drawCube(const double &x1, const double &y1, const double &z1,
                                    const double &x2, const double &y2, const double &z2)
{
    MODIFY_SELECTED(m_processor->computeBox((x1+x2)/2.0, (y1+y2)/2.0, (z1+z2)/2.0,
                                         x2-x1,
                                         y2-y1,
                                         z2-z1));
}

void DM_AMKglObjectsPickerPainter::drawCube(const double &x1, const double &y1, const double &z1,
                                    const double &x2, const double &y2, const double &z2,
                                    GLenum faces, GLenum mode)
{
    Q_UNUSED(faces)

    if(mode == GL_POINT) {
        MODIFY_SELECTED(m_processor->computePoint(x1, y1, z1));
        MODIFY_SELECTED(m_processor->computePoint(x1, y1, z2));
        MODIFY_SELECTED(m_processor->computePoint(x2, y1, z2));
        MODIFY_SELECTED(m_processor->computePoint(x2, y1, z1));

        MODIFY_SELECTED(m_processor->computePoint(x1, y2, z1));
        MODIFY_SELECTED(m_processor->computePoint(x1, y2, z2));
        MODIFY_SELECTED(m_processor->computePoint(x2, y2, z2));
        MODIFY_SELECTED(m_processor->computePoint(x2, y2, z1));

        return;
    } else if(mode == GL_LINE) {
        // front
        drawQuadFace(x1, y1, z2,
                     x1, y2, z2,
                     x2, y2, z2,
                     x2, y1, z2);

        if(m_selected)
            return;

        // back
        drawQuadFace(x1, y1, z1,
                     x1, y2, z1,
                     x2, y2, z1,
                     x2, y1, z1);

        if(m_selected)
            return;

        // left
        drawQuadFace(x1, y1, z2,
                     x1, y2, z2,
                     x1, y2, z1,
                     x1, y1, z1);

        if(m_selected)
            return;

        // right
        drawQuadFace(x2, y1, z2,
                     x2, y2, z2,
                     x2, y2, z1,
                     x2, y1, z1);

        if(m_selected)
            return;

        // top
        drawQuadFace(x1, y2, z2,
                     x1, y2, z1,
                     x2, y2, z1,
                     x2, y2, z2);

        if(m_selected)
            return;

        // bottom
        drawQuadFace(x1, y1, z2,
                     x1, y1, z1,
                     x2, y1, z1,
                     x2, y1, z2);
        return;
    }

    MODIFY_SELECTED(m_processor->computeBox((x1+x2)/2.0, (y1+y2)/2.0, (z1+z2)/2.0,
                                         x2-x1,
                                         y2-y1,
                                         z2-z1,
                                         Eigen::Vector3f(1, 0, 0),
                                         Eigen::Vector3f(0, 0, 1),
                                         mode == GL_FILL));
}

void DM_AMKglObjectsPickerPainter::drawQuadFace(const double &x1, const double &y1, const double &z1,
                                        const double &x2, const double &y2, const double &z2,
                                        const double &x3, const double &y3, const double &z3,
                                        const double &x4, const double &y4, const double &z4)
{
    MODIFY_SELECTED(m_processor->computeLine(Eigen::Vector3d(x1, y1, z1), Eigen::Vector3d(x2, y2, z2)));
    MODIFY_SELECTED(m_processor->computeLine(Eigen::Vector3d(x2, y2, z2), Eigen::Vector3d(x3, y3, z3)));
    MODIFY_SELECTED(m_processor->computeLine(Eigen::Vector3d(x3, y3, z3), Eigen::Vector3d(x4, y4, z4)));
    MODIFY_SELECTED(m_processor->computeLine(Eigen::Vector3d(x4, y4, z4), Eigen::Vector3d(x1, y1, z1)));
}

void DM_AMKglObjectsPickerPainter::fillQuadFace(const double &x1, const double &y1, const double &z1,
                                        const double &x2, const double &y2, const double &z2,
                                        const double &x3, const double &y3, const double &z3,
                                        const double &x4, const double &y4, const double &z4)
{
    /*Q_UNUSED(x4)
    Q_UNUSED(y4)
    Q_UNUSED(z4)

    MODIFY_SELECTED(m_processor->computeQuads((x1+x3)/2.0, (y1+y3)/2.0, (z1+z3)/2.0,
                                           Math::distancePoints(Eigen::Vector3d(x1, y1, z1), Eigen::Vector3d(x2, y2, z2)),
                                           Math::distancePoints(Eigen::Vector3d(x2, y2, z2), Eigen::Vector3d(x3, y3, z3)),
                                           Eigen::Vector3f(x2-x1, y2-y1, z2-z1),
                                           Eigen::Vector3f(x3-x2, y3-y2, z3-z2),
                                           true));*/

    drawTriangle(x1, y1, z1,
                 x2, y2, z2,
                 x3, y3, z3);

    drawTriangle(x3, y3, z3,
                 x4, y4, z4,
                 x1, y1, z1);
}

void DM_AMKglObjectsPickerPainter::drawQuadFace(const double &x1, const double &y1, const double &z1,
                                        int r1, int g1, int b1,
                                        const double &x2, const double &y2, const double &z2,
                                        int r2, int g2, int b2,
                                        const double &x3, const double &y3, const double &z3,
                                        int r3, int g3, int b3, const double &x4,
                                        const double &y4, const double &z4,
                                        int r4, int g4, int b4)
{
    Q_UNUSED(r1)
    Q_UNUSED(g1)
    Q_UNUSED(b1)
    Q_UNUSED(r2)
    Q_UNUSED(g2)
    Q_UNUSED(b2)
    Q_UNUSED(r3)
    Q_UNUSED(g3)
    Q_UNUSED(b3)
    Q_UNUSED(r4)
    Q_UNUSED(g4)
    Q_UNUSED(b4)

    drawQuadFace(x1, y1, z1,
                 x2, y2, z2,
                 x3, y3, z3,
                 x4, y4, z4);
}

void DM_AMKglObjectsPickerPainter::fillQuadFace(const double &x1, const double &y1, const double &z1,
                                        int r1, int g1, int b1,
                                        const double &x2, const double &y2, const double &z2,
                                        int r2, int g2, int b2,
                                        const double &x3, const double &y3, const double &z3,
                                        int r3, int g3, int b3,
                                        const double &x4, const double &y4, const double &z4,
                                        int r4, int g4, int b4)
{
    Q_UNUSED(r1)
    Q_UNUSED(g1)
    Q_UNUSED(b1)
    Q_UNUSED(r2)
    Q_UNUSED(g2)
    Q_UNUSED(b2)
    Q_UNUSED(r3)
    Q_UNUSED(g3)
    Q_UNUSED(b3)
    Q_UNUSED(r4)
    Q_UNUSED(g4)
    Q_UNUSED(b4)

    fillQuadFace(x1, y1, z1,
                 x2, y2, z2,
                 x3, y3, z3,
                 x4, y4, z4);
}

void DM_AMKglObjectsPickerPainter::drawRectXY(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &z)
{
    MODIFY_SELECTED(m_processor->computeLine(Eigen::Vector3d(topLeft[0], topLeft[1], z), Eigen::Vector3d(bottomRight[0], topLeft[1], z)));
    MODIFY_SELECTED(m_processor->computeLine(Eigen::Vector3d(bottomRight[0], topLeft[1], z), Eigen::Vector3d(bottomRight[0], bottomRight[1], z)));
    MODIFY_SELECTED(m_processor->computeLine(Eigen::Vector3d(bottomRight[0], bottomRight[1], z), Eigen::Vector3d(topLeft[0], bottomRight[1], z)));
    MODIFY_SELECTED(m_processor->computeLine(Eigen::Vector3d(topLeft[0], bottomRight[1], z), Eigen::Vector3d(topLeft[0], topLeft[1], z)));
}

void DM_AMKglObjectsPickerPainter::fillRectXY(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &z)
{
    fillQuadFace(topLeft[0], topLeft[1], z,
                 bottomRight[0], topLeft[1], z,
                 bottomRight[0], bottomRight[1], z,
                 topLeft[0], bottomRight[1], z);
}

void DM_AMKglObjectsPickerPainter::drawRectXZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &y)
{
    MODIFY_SELECTED(m_processor->computeLine(Eigen::Vector3d(topLeft[0], y, topLeft[1]), Eigen::Vector3d(bottomRight[0], y, topLeft[1])));
    MODIFY_SELECTED(m_processor->computeLine(Eigen::Vector3d(bottomRight[0], y, topLeft[1]), Eigen::Vector3d(bottomRight[0], y, bottomRight[1])));
    MODIFY_SELECTED(m_processor->computeLine(Eigen::Vector3d(bottomRight[0], y, bottomRight[1]), Eigen::Vector3d(topLeft[0], y, bottomRight[1])));
    MODIFY_SELECTED(m_processor->computeLine(Eigen::Vector3d(topLeft[0], y, bottomRight[1]), Eigen::Vector3d(topLeft[0], y, topLeft[1])));
}

void DM_AMKglObjectsPickerPainter::fillRectXZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &y)
{
    fillQuadFace(topLeft[0], y, topLeft[1],
                 bottomRight[0], y, topLeft[1],
                 bottomRight[0], y, bottomRight[1],
                 topLeft[0], y, bottomRight[1]);
}

void DM_AMKglObjectsPickerPainter::drawRectYZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &x)
{
    MODIFY_SELECTED(m_processor->computeLine(Eigen::Vector3d(x, topLeft[0], topLeft[1]), Eigen::Vector3d(x, bottomRight[0], topLeft[1])));
    MODIFY_SELECTED(m_processor->computeLine(Eigen::Vector3d(x, bottomRight[0], topLeft[1]), Eigen::Vector3d(x, bottomRight[0], bottomRight[1])));
    MODIFY_SELECTED(m_processor->computeLine(Eigen::Vector3d(x, bottomRight[0], bottomRight[1]), Eigen::Vector3d(x, topLeft[0], bottomRight[1])));
    MODIFY_SELECTED(m_processor->computeLine(Eigen::Vector3d(x, topLeft[0], bottomRight[1]), Eigen::Vector3d(x, topLeft[0], topLeft[1])));
}

void DM_AMKglObjectsPickerPainter::fillRectYZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &x)
{
    fillQuadFace(x, topLeft[0], topLeft[1],
                 x, bottomRight[0], topLeft[1],
                 x, bottomRight[0], bottomRight[1],
                 x, topLeft[0], bottomRight[1]);
}

void DM_AMKglObjectsPickerPainter::drawLine(const double &x1, const double &y1, const double &z1,
                                    const double &x2, const double &y2, const double &z2)
{
    MODIFY_SELECTED(m_processor->computeLine(Eigen::Vector3d(x1, y1, z1), Eigen::Vector3d(x2, y2, z2)));
}

void DM_AMKglObjectsPickerPainter::drawLine(const size_t& globalEdgeIndex,
                                            const size_t &p1GlobalIndex,
                                            const size_t &p2GlobalIndex)
{
    Q_UNUSED(globalEdgeIndex)

    MODIFY_SELECTED(m_processor->computeLine(p1GlobalIndex, p2GlobalIndex));
}

void DM_AMKglObjectsPickerPainter::drawTriangle(const double &x1, const double &y1, const double &z1,
                                        const double &x2, const double &y2, const double &z2,
                                        const double &x3, const double &y3, const double &z3)
{
    MODIFY_SELECTED(m_processor->computeTriangle(Eigen::Vector3d(x1, y1, z1),
                                              Eigen::Vector3d(x2, y2, z2),
                                              Eigen::Vector3d(x3, y3, z3),
                                              true));
}

void DM_AMKglObjectsPickerPainter::drawTriangle(const size_t &globalFaceIndex, const size_t &p1GlobalIndex, const size_t &p2GlobalIndex, const size_t &p3GlobalIndex)
{
    Q_UNUSED(globalFaceIndex)

    MODIFY_SELECTED(m_processor->computeTriangle(p1GlobalIndex,
                                              p2GlobalIndex,
                                              p3GlobalIndex,
                                              true));
}

void DM_AMKglObjectsPickerPainter::drawCircle(const double &x, const double &y, const double &z, const double &radius)
{
    MODIFY_SELECTED(m_processor->computeEllipse(x, y, z, radius, radius, Eigen::Vector3f(0, 1, 0), Eigen::Vector3f(0, 0, 1)));
}

void DM_AMKglObjectsPickerPainter::drawCircle3D(const Eigen::Vector3d &center, const Eigen::Vector3d &direction, const double &radius)
{
    MODIFY_SELECTED(m_processor->computeEllipse(center[0], center[1], center[2], radius, radius, Eigen::Vector3f(0, 1, 0), direction.cast<float>()));
}

void DM_AMKglObjectsPickerPainter::drawEllipse(const double &x, const double &y, const double &z,
                                       const double &radiusA,
                                       const double &radiusB)
{
    MODIFY_SELECTED(m_processor->computeEllipse(x, y, z, radiusA, radiusB));
}

void DM_AMKglObjectsPickerPainter::beginPolygon()
{
    m_nPolygonPoints = 0;
}

void DM_AMKglObjectsPickerPainter::addPointToPolygon(const double &x, const double &y, const double &z)
{
    if(m_nPolygonPoints == 0)
        m_firstPolygonPoint = Eigen::Vector3d(x, y, z);
    else {
        MODIFY_SELECTED(m_processor->computeLine(m_lastPolygonPoint, Eigen::Vector3d(x, y, z)));
        ++m_nPolygonPoints;
    }

    m_lastPolygonPoint = Eigen::Vector3d(x, y, z);
}

void DM_AMKglObjectsPickerPainter::endPolygon()
{
    if(m_nPolygonPoints > 2)
        MODIFY_SELECTED(m_processor->computeLine(m_lastPolygonPoint, m_firstPolygonPoint));

    m_nPolygonPoints = 0;
}

void DM_AMKglObjectsPickerPainter::drawCylinder(const double &x, const double &y, const double &z, const double &radius, const double &height)
{
    MODIFY_SELECTED(m_processor->computeCylinder(x, y, z, radius, height, radius));
}

void DM_AMKglObjectsPickerPainter::drawCylinder3D(const Eigen::Vector3d &center, const Eigen::Vector3d &direction, const double &radius, const double &height)
{
    Eigen::Vector3d nD = Eigen::Vector3d(0, 1, 0).cross(direction);

    if(nD.norm() == 0)
        nD = Eigen::Vector3d(0, 0, 1).cross(direction);

    MODIFY_SELECTED(m_processor->computeCylinder(center[0], center[1], center[2], radius, height, radius, nD.cast<float>(), direction.cast<float>()));
}

void DM_AMKglObjectsPickerPainter::drawPyramid(const double &topX, const double &topY, const double &topZ, const double &base1X, const double &base1Y, const double &base1Z, const double &base2X, const double &base2Y, const double &base2Z, const double &base3X, const double &base3Y, const double &base3Z, const double &base4X, const double &base4Y, const double &base4Z)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(topX)
    Q_UNUSED(topY)
    Q_UNUSED(topZ)
    Q_UNUSED(base1X)
    Q_UNUSED(base1Y)
    Q_UNUSED(base1Z)
    Q_UNUSED(base2X)
    Q_UNUSED(base2Y)
    Q_UNUSED(base2Z)
    Q_UNUSED(base3X)
    Q_UNUSED(base3Y)
    Q_UNUSED(base3Z)
    Q_UNUSED(base4X)
    Q_UNUSED(base4Y)
    Q_UNUSED(base4Z)

    // TODO
#endif
    //MODIFY_SELECTED(m_picker->computePyramid(topX, topY, topZ, base1X, base1Y, base1Z, base2X, base2Y, base2Z, base3X, base3Y, base3Z, base4X, base4Y, base4Z));
}

void DM_AMKglObjectsPickerPainter::drawPartOfSphere(const double &centerX, const double &centerY, const double &centerZ, const double &radius, const double &initTheta, const double &endTheta, const double &initPhi, const double &endPhi, bool radians)
{
    MODIFY_SELECTED(m_processor->computePartOfSphere(centerX, centerY, centerZ, radius, initTheta, endTheta, initPhi, endPhi, radians));
}

void DM_AMKglObjectsPickerPainter::drawMesh(const CT_AbstractMeshModel *mesh)
{
    drawFaces(mesh);
}

void DM_AMKglObjectsPickerPainter::drawFaces(const CT_AbstractMeshModel *mesh)
{
    ((CT_AbstractMeshModel*)mesh)->drawFaces(*m_processor->getGraphicsView(), *this);
}

void DM_AMKglObjectsPickerPainter::drawEdges(const CT_AbstractMeshModel *mesh)
{
    ((CT_AbstractMeshModel*)mesh)->drawEdges(*m_processor->getGraphicsView(), *this);
}

void DM_AMKglObjectsPickerPainter::drawPoints(const CT_AbstractMeshModel *mesh)
{
    drawPointCloud(mesh->pointCloudIndex());
}

