#ifndef DM_AMKGLPERMANENTPAINTER_H
#define DM_AMKGLPERMANENTPAINTER_H

#include "painterinterface.h"
#include "graphicsviewinterface.h"
#include "scene/tools/permanentiteminformation.h"
#include "renderer/generic/generictypedef.h"

/**
 * @brief Use this painter to fill the permanent scene with shapes (objects) draw
 *        by an CT_AbstractItemDrawable.
 */
class DM_AMKglPermanentPainter : public PainterInterface
{
public:
    DM_AMKglPermanentPainter(GraphicsViewInterface &view);
    ~DM_AMKglPermanentPainter();

    /**
     * @brief Set the permanent scene to fill
     * @param scene : scene to use
     */
    void setPermanentScene(const PermanentScene* scene);

    /**
     * @brief Returns the permanent scene used
     */
    PermanentScene* getPermanentScene() const;

    /**
     * @brief Set the information to modify
     */
    void setItemInformation(const PermanentItemInformation* info);

    /**
     * @brief Returns the information used
     */
    PermanentItemInformation* getItemInformation() const;

    void statistic();

    void save() {}
    void restore() {}

    void enableSetPointSize(bool enable) { Q_UNUSED(enable) }
    void setPointSize(float size) { Q_UNUSED(size) }
    void restoreDefaultPointSize() {}
    void setPen(const QPen &pen);
    void restoreDefaultPen() {}
    void enableSetColor(bool enable);
    void setColor(int r, int g, int b);
    void setColor(QColor color);
    QColor getColor();

    void drawPoint(const double &x, const double &y, const double &z);
    void drawPoint(double *p);
    void drawPoint(const size_t &globalIndex);

    void drawPointCloud(const CT_AbstractCloudIndex *pci);

    void drawCube(const double &x1, const double &y1, const double &z1, const double &x2, const double &y2, const double &z2);
    void drawCube(const double &x1, const double &y1, const double &z1, const double &x2, const double &y2, const double &z2, GLenum faces, GLenum mode);


    void drawQuadFace( const double &x1, const double &y1, const double &z1,
                               const double &x2, const double &y2, const double &z2,
                               const double &x3, const double &y3, const double &z3,
                               const double &x4, const double &y4, const double &z4 );

    void fillQuadFace( const double &x1, const double &y1, const double &z1,
                               const double &x2, const double &y2, const double &z2,
                               const double &x3, const double &y3, const double &z3,
                               const double &x4, const double &y4, const double &z4 );

    void drawQuadFace( const double &x1, const double &y1, const double &z1, int r1, int g1, int b1,
                               const double &x2, const double &y2, const double &z2, int r2, int g2, int b2,
                               const double &x3, const double &y3, const double &z3, int r3, int g3, int b3,
                               const double &x4, const double &y4, const double &z4, int r4, int g4, int b4 );

    void fillQuadFace( const double &x1, const double &y1, const double &z1, int r1, int g1, int b1,
                               const double &x2, const double &y2, const double &z2, int r2, int g2, int b2,
                               const double &x3, const double &y3, const double &z3, int r3, int g3, int b3,
                               const double &x4, const double &y4, const double &z4, int r4, int g4, int b4 );


    void drawRectXY(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &z);
    void fillRectXY(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &z);
    void drawRectXZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &y);
    void fillRectXZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &y);
    void drawRectYZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &x);
    void fillRectYZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &x);

    void drawLine(const double &x1, const double &y1, const double &z1, const double &x2, const double &y2, const double &z2);
    void drawLine(const size_t& globalEdgeIndex,
                  const size_t &p1GlobalIndex,
                  const size_t &p2GlobalIndex);

    void drawTriangle(const double &x1, const double &y1, const double &z1,
                              const double &x2, const double &y2, const double &z2,
                              const double &x3, const double &y3, const double &z3);

    void drawTriangle(const size_t& globalFaceIndex,
                      const size_t &p1GlobalIndex,
                      const size_t &p2GlobalIndex,
                      const size_t &p3GlobalIndex);

    void drawCircle(const double &x, const double &y, const double &z, const double &radius);
    void drawCircle3D(const Eigen::Vector3d &center, const Eigen::Vector3d &direction, const double &radius);
    void drawEllipse(const double &x, const double &y, const double &z, const double &radiusA, const double &radiusB);

    void beginPolygon();
    void addPointToPolygon(const double &x, const double &y, const double &z);
    void endPolygon();

    void drawCylinder(const double & x, const double & y, const double & z, const double & radius, const double & height);
    void drawCylinder3D(const Eigen::Vector3d &center, const Eigen::Vector3d &direction, const double & radius, const double & height);

    void drawPyramid(const double &topX, const double &topY, const double &topZ,
                             const double &base1X, const double &base1Y, const double &base1Z,
                             const double &base2X, const double &base2Y, const double &base2Z,
                             const double &base3X, const double &base3Y, const double &base3Z,
                             const double &base4X, const double &base4Y, const double &base4Z);

    void drawPartOfSphere (const double &centerX, const double &centerY, const double &centerZ,
                                   const double &radius,
                                   const double &initTheta, const double &endTheta,
                                   const double &initPhi, const double &endPhi,
                                   bool radians = true);

    void drawMesh(const CT_AbstractMeshModel *mesh);
    void drawFaces(const CT_AbstractMeshModel *mesh);
    void drawEdges(const CT_AbstractMeshModel *mesh);
    void drawPoints(const CT_AbstractMeshModel *mesh);

private:
    GraphicsViewInterface&                  m_view;
    PermanentScene*                         m_scene;
    PermanentItemInformation*               m_info;
    int                                     m_nPolygonPoints;
    Eigen::Vector3d                         m_lastPolygonPoint;
    Eigen::Vector3d                         m_firstPolygonPoint;
    int                                     m_enableSetColorCount;
    Basic::LocalColor                       m_currentColor;
};

#endif // DM_AMKGLPERMANENTPAINTER_H
