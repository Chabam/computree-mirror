#if !defined (_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif
#include <QtMath>

#include "shapetovolume.h"

#include "poly2tri.h"
#include "picker/tools/intersectionchecker.h"
#include "tools/qglviewer/qglviewertools.h"

QVector<QPolygon> ShapeToVolume::staticConvertPolygonToTriangle(const QPolygon &polygon)
{
    QVector<QPolygon> result;

    int nP = polygon.size();

    if(nP == 3)
        result.append(polygon);

    if(nP <= 3)
        return result;

    std::vector<p2t::Point*> polyline;

    QVectorIterator<QPoint> it(polygon);

    while(it.hasNext()) {
        const QPoint &p = it.next();
        polyline.push_back(new p2t::Point(p.x(),p.y()));
    }

    p2t::CDT cdt(polyline);
    cdt.Triangulate();

    std::vector<p2t::Triangle*> triangles = cdt.GetTriangles();

    foreach (p2t::Triangle *t, triangles) {
        QPolygon poly;

        for(int i=0; i<3; ++i) {
            p2t::Point *p = t->GetPoint(i);
            poly.push_back(QPoint(p->x, p->y));
        }

        result.push_back(poly);
    }

    qDeleteAll(polyline.begin(), polyline.end());

    return result;
}

QVector<TrianglePlane> ShapeToVolume::staticTriangulatePolygonAndConvertItToPlanes(const QPolygon &polygon,
                                                                                   const QGLCamera &camera)
{
    QVector<TrianglePlane> result;

    QVector<QPolygon> triangles = staticConvertPolygonToTriangle(polygon);

    foreach (const QPolygon &triangle, triangles) {

        QVector<Plane> planes = staticConvertPolygonToPlanes(triangle, camera);
        TrianglePlane tr;
        tr.planes[0] = planes[0];
        tr.planes[1] = planes[1];
        tr.planes[2] = planes[2];

        result.append(tr);
    }

    return result;
}

QVector<Plane> ShapeToVolume::staticConvertPolygonToPlanes(const QPolygon &polygon,
                                                           const QGLCamera &camera)
{
    QVector<Plane> v;

    if(polygon.size() > 1) {

        QVectorIterator<QPoint> it(polygon);

        QPoint lastPos = it.next();
        Eigen::Vector3d lastNearV, lastFarV;
        Eigen::Vector3d nearV, farV;

        staticConvert2DPointTo3DLine(camera, lastPos, lastNearV, lastFarV);

        while(it.hasNext()) {

            const QPoint &pos = it.next();

            staticConvert2DPointTo3DLine(camera, pos, nearV, farV);

            v.append(Plane(lastNearV, lastFarV, farV, nearV));

            lastNearV = nearV;
            lastFarV = farV;
            lastPos = pos;
        }

        if(polygon.size() > 2) {
            staticConvert2DPointTo3DLine(camera, polygon.first(), nearV, farV);

            v.append(Plane(lastNearV, lastFarV, farV, nearV));
        }
    }

    return v;
}

void ShapeToVolume::staticConvert2DPointTo3DLine(const QGLCamera &camera,
                                                 const QPoint &pos,
                                                 Eigen::Vector3d &nearV,
                                                 Eigen::Vector3d &farV)
{
    Eigen::Vector3d orig;
    Eigen::Vector3d dir;
    camera.convertClickToLine(pos, orig, dir);

    Plane nearPlane, farPlane;
    staticGetCameraPlane(camera, nearPlane, farPlane);

    Eigen::Vector3d nearPointIntersect, farPointIntersect;

    bool ok = IntersectionChecker::staticDoesASegmentIntersectWithAPlane(Eigen::Vector3d(orig),
                                                               Eigen::Vector3d(orig+dir),
                                                               nearPlane.m_vertex[0],
                                                               nearPlane.m_vertex[1],
                                                               nearPlane.m_vertex[2],
                                                               nearPlane.m_vertex[3],
                                                               &nearPointIntersect,
                                                               true,
                                                               true);

    Q_ASSERT_X(ok, "ShapeToVolume", "Segment don't intersect Near plane");

    ok = IntersectionChecker::staticDoesASegmentIntersectWithAPlane(Eigen::Vector3d(orig),
                                                               Eigen::Vector3d(orig+dir),
                                                               farPlane.m_vertex[0],
                                                               farPlane.m_vertex[1],
                                                               farPlane.m_vertex[2],
                                                               farPlane.m_vertex[3],
                                                               &farPointIntersect,
                                                               true,
                                                               true);

    Q_ASSERT_X(ok, "ShapeToVolume", "Segment don't intersect Far plane");

    nearV = nearPointIntersect;
    farV = farPointIntersect;
}

void ShapeToVolume::staticGetCameraPlane(const QGLCamera &camera,
                                         Plane &nearPlane,
                                         Plane &farPlane)
{
    Eigen::Vector3d points[2];
    Eigen::Vector3d orig = camera.position()*2;

    points[0].z = camera.zNear();
    points[1].z = camera.zFar();

    switch (camera.type())
    {
        case QGLCamera::PERSPECTIVE:
        {
            points[0][1] = points[0][2] * tan(camera.fieldOfView()*0.5);
            points[0][0] = points[0][1] * camera.aspectRatio();

            const qreal ratio = points[1][2] / points[0][2];

            points[1][1] = ratio * points[0][1];
            points[1][0] = ratio * points[0][0];
            break;
        }
        default:
        case QGLCamera::ORTHOGRAPHIC:
        {
            GLdouble hw, hh;
            camera.getOrthoWidthHeight(hw, hh);
            points[0][0] = points[1][0] = qreal(hw);
            points[0][1] = points[1][1] = qreal(hh);
            break;
        }
    }

    Eigen::Vector3d tmp[2];
    Eigen::Vector3d p1[2], p2[2], p3[2], p4[2];

    tmp[0] = camera.worldCoordinatesOf(points[0]);
    tmp[1] = camera.worldCoordinatesOf(points[1]);

    p1[0] = Eigen::Vector3d(-tmp[0][0], -tmp[0][1], -tmp[0][2]) + orig;
    p1[1] = Eigen::Vector3d(-tmp[1][0], -tmp[1][1], -tmp[1][2]) + orig;

    points[0][1] = -points[0][1];
    points[1][1] = -points[1][1];

    tmp[0] = camera.worldCoordinatesOf(points[0]);
    tmp[1] = camera.worldCoordinatesOf(points[1]);

    p2[0] = Eigen::Vector3d(-tmp[0][0], -tmp[0][1], -tmp[0][2]) + orig;
    p2[1] = Eigen::Vector3d(-tmp[1][0], -tmp[1][1], -tmp[1][2]) + orig;

    points[0][0] = -points[0][0];
    points[1][0] = -points[1][0];

    tmp[0] = camera.worldCoordinatesOf(points[0]);
    tmp[1] = camera.worldCoordinatesOf(points[1]);

    p3[0] = Eigen::Vector3d(-tmp[0][0], -tmp[0][1], -tmp[0][2]) + orig;
    p3[1] = Eigen::Vector3d(-tmp[1][0], -tmp[1][1], -tmp[1][2]) + orig;

    points[0][1] = -points[0][1];
    points[1][1] = -points[1][1];

    tmp[0] = camera.worldCoordinatesOf(points[0]);
    tmp[1] = camera.worldCoordinatesOf(points[1]);

    p4[0] = Eigen::Vector3d(-tmp[0][0], -tmp[0][1], -tmp[0][2]) + orig;
    p4[1] = Eigen::Vector3d(-tmp[1][0], -tmp[1][1], -tmp[1][2]) + orig;

    nearPlane.set(p1[0], p2[0], p3[0], p4[0]);
    farPlane.set(p1[1], p2[1], p3[1], p4[1]);
}
