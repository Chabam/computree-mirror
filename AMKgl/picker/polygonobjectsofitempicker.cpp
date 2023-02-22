#include "polygonobjectsofitempicker.h"

#include "amkglapp.h"

#include "includepoint.h"
#include "includecloudindex.h"
#include "interfaces/iglobalpointcloudmanager.h"
#include "renderer/ellipse/ellipseshaderparams.h"
#include "picker/tools/intersectionchecker.h"
#include "tools/math.h"

#define CALL_CHECK_OPERATION getCheckOperation()(getItem(), getItemInformation(), getPermanentItemScene(), getAnyElementsAttributesProvider())
#define CALL_SELECT_OPERATION getSelectionOperation()(getItem(), getItemInformation(), getPermanentItemScene(), getAnyElementsAttributesProvider())

Eigen::Matrix4d AffineInverse(const Eigen::Matrix4d& m)
{
    Eigen::Matrix3d RotSclInv = (
        m.block<3, 3>(0, 0).array().rowwise()
        / m.block<3, 3>(0, 0).colwise().squaredNorm().array() //scaling
        ).transpose(); //rotation

    return (Eigen::Matrix4d() << RotSclInv
        , -RotSclInv * m.block<3, 1>(0, 3) //translation
        , 0, 0, 0, 1).finished();
}

PolygonObjectsOfItemPicker::PolygonObjectsOfItemPicker(const QVector<Plane> &polygonPlanes,
                                           const QVector<TrianglePlane> &triangles) : ObjectsOfItemPicker(),
                                                                                      m_polygonPlanes(polygonPlanes),
                                                                                      m_triangles(triangles)
{
    m_pointAccessor = AMKglEA->getGlobalPointCloudManager();
    m_hasEnoughTriangle = m_triangles.size() > 0;
}

bool PolygonObjectsOfItemPicker::isValid() const
{
    if(!m_hasEnoughTriangle)
        return false;

    return ObjectsOfItemPicker::isValid();
}

bool PolygonObjectsOfItemPicker::computePoint(const double &x, const double &y, const double &z)
{
    if(!CALL_CHECK_OPERATION)
        return false;

    if(IntersectionChecker::staticIsAPointInsideAPolygon(x, y, z, m_triangles, m_triangles.size())) {
        // point included in the polygon : apply selection operation
        CALL_SELECT_OPERATION;

        return true;
    }

    return false;
}

bool PolygonObjectsOfItemPicker::computePoint(const Eigen::Vector3d &p)
{
    return computePoint(p(0), p(1), p(2));
}

bool PolygonObjectsOfItemPicker::computePoint(const size_t &globalIndex)
{
    if(!CALL_CHECK_OPERATION)
        return false;

    DoublePoint p;
    getPointAtGlobalIndex(globalIndex, p);

    return computePoint(p);
}

bool PolygonObjectsOfItemPicker::computePoints(const CloudIndex *cloud)
{
    if(!CALL_CHECK_OPERATION)
        return false;

    int nTriangles = m_triangles.size();
    //bool ok = false;
    bool selected = false;
    size_t nPoints = cloud->size();

    for(size_t i = 0; i < nPoints; ++i) {

        const DoublePoint& p = getPointAtGlobalIndex((*cloud)[i]);

        if(IntersectionChecker::staticIsAPointInsideAPolygon(p(0), p(1), p(2), m_triangles, nTriangles)) {
            // point included in the polygon : apply selection operation
            CALL_SELECT_OPERATION;

            selected = true;
            //ok = false;

            if(!CALL_CHECK_OPERATION)
                return selected;
        }
    }

    return selected;
}

bool PolygonObjectsOfItemPicker::computeEllipse(const double &cx, const double &cy, const double &cz,
                                          const float &majorAxisRadius, const float &minorAxisRadius,
                                          const Eigen::Vector3f &majorAxisDirection,
                                          const Eigen::Vector3f &normal,
                                          bool filled)
{
    if(!CALL_CHECK_OPERATION)
        return false;

    if(checkEllipse(cx, cy, cz,
                    majorAxisRadius, minorAxisRadius,
                    majorAxisDirection, normal,
                    filled)) {
        CALL_SELECT_OPERATION;
        return true;
    }

    return false;
}

bool PolygonObjectsOfItemPicker::computeBox(const double &cx, const double &cy, const double &cz,
                                      const float &width, const float &height, const float &length,
                                      const Eigen::Vector3f &widthAxisDirection,
                                      const Eigen::Vector3f &lengthAxisDirection,
                                      bool filled)
{
    if(!CALL_CHECK_OPERATION)
        return false;

    if(checkBox(cx, cy, cz,
                width, height, length,
                widthAxisDirection, lengthAxisDirection,
                filled)) {
        CALL_SELECT_OPERATION;
        return true;
    }

    return false;
}

bool PolygonObjectsOfItemPicker::computeQuads(const double &cx, const double &cy, const double &cz,
                                        const float &width, const float &length,
                                        const Eigen::Vector3f &widthAxisDirection,
                                        const Eigen::Vector3f &lengthAxisDirection,
                                        bool filled)
{
    return computeBox(cx, cy, cz, width, 0, length, widthAxisDirection, lengthAxisDirection, filled);
}

bool PolygonObjectsOfItemPicker::computeLine(const Eigen::Vector3d &p1, const Eigen::Vector3d &p2)
{
    if(!CALL_CHECK_OPERATION)
        return false;

    if(checkLine(p1, p2)) {
        CALL_SELECT_OPERATION;
        return true;
    }

    return false;
}

bool PolygonObjectsOfItemPicker::computeLine(const size_t &p1GlobalIndex, const size_t &p2GlobalIndex)
{
    if(!CALL_CHECK_OPERATION)
        return false;

    DoublePoint p1, p2;
    getPointAtGlobalIndex(p1GlobalIndex, p1);
    getPointAtGlobalIndex(p2GlobalIndex, p2);

    return computeLine(p1,
                       p2);
}

bool PolygonObjectsOfItemPicker::computeTriangle(const size_t &p1GlobalIndex, const size_t &p2GlobalIndex, const size_t &p3GlobalIndex,
                                           bool filled)
{
    if(!CALL_CHECK_OPERATION)
        return false;

    DoublePoint p1, p2, p3;
    getPointAtGlobalIndex(p1GlobalIndex, p1);
    getPointAtGlobalIndex(p2GlobalIndex, p2);
    getPointAtGlobalIndex(p3GlobalIndex, p3);

    return computeTriangle(p1, p2, p3, filled);
}

bool PolygonObjectsOfItemPicker::computeTriangle(const Eigen::Vector3d &p1, const Eigen::Vector3d &p2, const Eigen::Vector3d &p3,
                                           bool filled)
{
    if(!CALL_CHECK_OPERATION)
        return false;

    if(checkTriangle(p1, p2, p3, filled)) {
        CALL_SELECT_OPERATION;
        return true;
    }

    return false;
}

bool PolygonObjectsOfItemPicker::computeCylinder(const double &cx, const double &cy, const double &cz,
                                           const float &width, const float &height, const float &length,
                                           const Eigen::Vector3f &widthAxisDirection, const Eigen::Vector3f &heightAxisDirection)
{
    if(!CALL_CHECK_OPERATION)
        return false;

    if(checkPoint(cx, cy, cz)) {
        CALL_SELECT_OPERATION;
        return true;
    }

    const double middleHeight = height/2.0;

    if(checkEllipse(cx+(heightAxisDirection.x()*middleHeight),
                    cy+(heightAxisDirection.y()*middleHeight),
                    cz+(heightAxisDirection.z()*middleHeight),
                    width, length,
                    widthAxisDirection, heightAxisDirection,
                    true)) {
        CALL_SELECT_OPERATION;
        return true;
    }

    if(checkEllipse(cx+(-heightAxisDirection.x()*middleHeight),
                    cy+(-heightAxisDirection.y()*middleHeight),
                    cz+(-heightAxisDirection.z()*middleHeight),
                    width, length,
                    widthAxisDirection, heightAxisDirection,
                    true)) {
        CALL_SELECT_OPERATION;
        return true;
    }

    // TODO : when we check if the polygon intersect the cylinder (no caps) is false ! correct it !!!!
    const Eigen::Vector3d center(cx, cy, cz);

    Eigen::Vector3d wDirection = widthAxisDirection.normalized().cast<double>();
    Eigen::Vector3d hDirection = heightAxisDirection.normalized().cast<double>();
    Eigen::Vector3d lDirection = wDirection.cross(hDirection);

    wDirection = wDirection * width;
    hDirection = hDirection * height;
    lDirection = lDirection * length;

    Eigen::Matrix4d matrix = Eigen::Matrix4d::Identity();
    matrix.block(0,3,3,1) = center;
    matrix(0,0) = wDirection[0];    // Xx
    matrix(1,0) = wDirection[1];    // Xy
    matrix(2,0) = wDirection[2];    // Xz
    matrix(0,1) = hDirection[0];    // Yx
    matrix(1,1) = hDirection[1];    // Yy
    matrix(2,1) = hDirection[2];    // Yz
    matrix(0,2) = lDirection[0];    // Zx
    matrix(1,2) = lDirection[1];    // Zy
    matrix(2,2) = lDirection[2];    // Zz

    matrix = AffineInverse(matrix);

    Eigen::Vector3d closestPoint[2];

    int nTriangles = m_triangles.size();

    for(int j=0; (j != nTriangles); ++j) {

        const TrianglePlane& tr = m_triangles[j];

        for(int k=0; k<3; ++k) {
            const Plane& plane = tr.planes[k];

            Math::getClosestPointBetweenPointAndTriangle(center,
                                                         plane.m_vertex[0],
                                                         plane.m_vertex[1],
                                                         plane.m_vertex[2],
                                                         closestPoint[0]);

            if(Math::isPointInCylinder(closestPoint[0], matrix)) {
                addIntersectionPoint(closestPoint[0]);
                CALL_SELECT_OPERATION;
                return true;
            }

            Math::getClosestPointBetweenPointAndTriangle(center,
                                                         plane.m_vertex[0],
                                                         plane.m_vertex[2],
                                                         plane.m_vertex[3],
                                                         closestPoint[1]);

            if(Math::isPointInCylinder(closestPoint[1], matrix)) {
                addIntersectionPoint(closestPoint[1]);
                CALL_SELECT_OPERATION;
                return true;
            }
        }
    }

    return false;
}

bool PolygonObjectsOfItemPicker::computePartOfSphere(const double &cx, const double &cy, const double &cz,
                                               const double &radius,
                                               const double &initTheta, const double &endTheta,
                                               const double &initPhi, const double &endPhi,
                                               bool radians)
{
    if(!CALL_CHECK_OPERATION)
        return false;

    if(checkPoint(cx, cy, cz)) {
        CALL_SELECT_OPERATION;
        return true;
    }

    const double initThetaRad = radians ? initTheta : qRadiansToDegrees(initTheta);
    const double endThetaRad = radians ? endTheta : qRadiansToDegrees(endTheta);
    const double initPhiRad = radians ? initPhi : qRadiansToDegrees(initPhi);
    const double endPhiRad = radians ? endPhi : qRadiansToDegrees(endPhi);

    const double sinInitPhi = sin(initPhiRad);
    const double cosInitPhi = cos(initPhiRad);

    const double cosInitTheta = cos(initThetaRad);
    const double sinInitTheta = sin(initThetaRad);

    const double cosEndTheta = cos(endThetaRad);
    const double sinEndTheta = sin(endThetaRad);

    const double sinEndPhi = sin(endPhiRad);
    const double cosEndPhi = cos(endPhiRad);

    const Eigen::Vector3f p1(sinInitPhi*cosInitTheta,
                             sinInitPhi*sinInitTheta,
                             cosInitPhi);

    const Eigen::Vector3f p2(sinInitPhi*cosEndTheta,
                             sinInitPhi*sinEndTheta,
                             cosInitPhi);

    const Eigen::Vector3f p3(sinEndPhi*cosEndTheta,
                             sinEndPhi*sinEndTheta,
                             cosEndPhi);

    const Eigen::Vector3f p4(sinEndPhi*cosInitTheta,
                             sinEndPhi*sinInitTheta,
                             cosEndPhi);

    const Eigen::Vector3d center(cx, cy, cz);

    // TODO : check part of ellipse !
    if(checkEllipse(cx, cy, cz, radius, radius, p1, p1.cross(p2), true)) {
        CALL_SELECT_OPERATION;
        return true;
    }

    // TODO : check part of ellipse !
    if(checkEllipse(cx, cy, cz, radius, radius, p2, p2.cross(p3), true)) {
        CALL_SELECT_OPERATION;
        return true;
    }

    // TODO : check part of ellipse !
    if(checkEllipse(cx, cy, cz, radius, radius, p3, p3.cross(p4), true)) {
        CALL_SELECT_OPERATION;
        return true;
    }

    // TODO : check part of ellipse !
    if(checkEllipse(cx, cy, cz, radius, radius, p1, p1.cross(p4), true)) {
        CALL_SELECT_OPERATION;
        return true;
    }

    return false;

    Eigen::Matrix4d matrix = Eigen::Matrix4d::Identity();
    matrix.block(0,3,3,1) = center;
    matrix(0,0) = radius;    // Xx
    matrix(1,1) = radius;    // Yy
    matrix(2,2) = radius;    // Zz

    matrix = AffineInverse(matrix);

    Eigen::Vector3d closestPoint[2];

    int nTriangles = m_triangles.size();

    for(int j=0; (j != nTriangles); ++j) {

        const TrianglePlane& tr = m_triangles[j];

        for(int k=0; k<3; ++k) {
            const Plane& plane = tr.planes[k];

            Math::getClosestPointBetweenPointAndTriangle(center,
                                                         plane.m_vertex[0],
                                                         plane.m_vertex[1],
                                                         plane.m_vertex[2],
                                                         closestPoint[0]);

            if(Math::isPointInSphere(closestPoint[0], matrix)) {
                bool ok = true;

                /*double theta = qAcos(closestPoint[0].x());
                double phi = qAsin(closestPoint[0].y());

                if(theta >= initThetaRad && theta <= endThetaRad) {
                    if(phi >= initPhiRad && phi <= endPhiRad)
                        ok = true;
                }*/

                if(ok) {
                    addIntersectionPoint(closestPoint[0]);
                    CALL_SELECT_OPERATION;
                    return true;
                }
            }

            Math::getClosestPointBetweenPointAndTriangle(center,
                                                         plane.m_vertex[0],
                                                         plane.m_vertex[2],
                                                         plane.m_vertex[3],
                                                         closestPoint[1]);

            if(Math::isPointInSphere(closestPoint[1], matrix)) {

                bool ok = true;

                /*double theta = qAcos(closestPoint[0].x());
                double phi = qAsin(closestPoint[0].y());

                if(theta >= initThetaRad && theta <= endThetaRad) {
                    if(phi >= initPhiRad && phi <= endPhiRad)
                        ok = true;
                }*/

                if(ok) {
                    addIntersectionPoint(closestPoint[1]);
                    CALL_SELECT_OPERATION;
                    return true;
                }
            }
        }
    }

    return false;
}

bool PolygonObjectsOfItemPicker::checkPoint(const double &x, const double &y, const double &z) const
{
    bool ok = false;
    int j, k;
    int nTriangles = m_triangles.size();

    for(j=0; !ok && (j != nTriangles); ++j) {

        const TrianglePlane &tr = m_triangles[j];

        ok = true;

        for(k=0; ok && k<3; ++k) {
            if(tr.planes[k].distance(x, y, z) <= 0.0)
                ok = false;
        }
    }

    return ok;
}

bool PolygonObjectsOfItemPicker::checkLine(const double &x1, const double &y1, const double &z1,
                                     const double &x2, const double &y2, const double &z2) const
{
    return checkLine(Eigen::Vector3d(x1, y1, z1), Eigen::Vector3d(x2, y2, z2));
}

bool PolygonObjectsOfItemPicker::checkLine(const Eigen::Vector3d &p1,
                                     const Eigen::Vector3d &p2) const
{
    size_t size = m_triangles.size();

    // one point is in the polygon ?
    bool ok = IntersectionChecker::staticIsAPointInsideAPolygon(p1, m_triangles, size)
                || IntersectionChecker::staticIsAPointInsideAPolygon(p2, m_triangles, size);

    // no ?
    if(!ok) {

        Eigen::Vector3d tmp;
        Eigen::Vector3d* intersectionPoint = nullptr;

        if(getIntersectionInterceptor() != nullptr)
            intersectionPoint = &tmp;

        // check if the segment intersect with one of the plane of the polygon
        size = m_polygonPlanes.size();

        for(size_t j=0; !ok && (j != size); ++j) {

            const Plane &plane = m_polygonPlanes[(int)j];

            ok = IntersectionChecker::staticDoesASegmentIntersectWithAPlane(p1, p2,
                                                                            plane.m_vertex[0],
                                                                            plane.m_vertex[1],
                                                                            plane.m_vertex[2],
                                                                            plane.m_vertex[3],
                                                                            intersectionPoint);
        }

        if(ok && (intersectionPoint != nullptr))
            addIntersectionPoint(*intersectionPoint);
    }

    return ok;
}

bool PolygonObjectsOfItemPicker::checkTriangle(const Eigen::Vector3d &p1,
                                         const Eigen::Vector3d &p2,
                                         const Eigen::Vector3d &p3,
                                         const bool &filled) const
{
    size_t size = m_triangles.size();

    // one point is in the polygon ?
    bool ok = IntersectionChecker::staticIsAPointInsideAPolygon(p1, m_triangles, size)
                || IntersectionChecker::staticIsAPointInsideAPolygon(p2, m_triangles, size)
                || IntersectionChecker::staticIsAPointInsideAPolygon(p3, m_triangles, size);

    // no ? so we check if a line of the triangle intersect with the polygon
    if(!ok) {

        Eigen::Vector3d tmp;
        Eigen::Vector3d* intersectionPoint = nullptr;

        if(getIntersectionInterceptor() != nullptr)
            intersectionPoint = &tmp;

        size = m_polygonPlanes.size();

        if(filled) {
            // exit loop if one ray of the polygon intersect with the triangle
            for(size_t j=0; !ok && (j != size); ++j) {

                const Plane &plane = m_polygonPlanes[(int)j];

                ok = IntersectionChecker::staticDoesASegmentIntersectWithATriangle(plane.m_vertex[0],
                                                                                   plane.m_vertex[1],
                                                                                   p1,
                                                                                   p2,
                                                                                   p3,
                                                                                   intersectionPoint);

                if(!ok) {
                    ok = IntersectionChecker::staticDoesATriangleIntersectWithAPlane(p1, p2, p3,
                                                                                     plane.m_vertex[0],
                                                                                     plane.m_vertex[1],
                                                                                     plane.m_vertex[2],
                                                                                     plane.m_vertex[3],
                                                                                     intersectionPoint);
                }
            }
        } else {
            // check if one edge of the triangle intersect with one plane of the polygon
            for(size_t j=0; !ok && (j != size); ++j) {

                const Plane &plane = m_polygonPlanes[(int)j];

                ok = IntersectionChecker::staticDoesATriangleIntersectWithAPlane(p1, p2, p3,
                                                                                 plane.m_vertex[0],
                                                                                 plane.m_vertex[1],
                                                                                 plane.m_vertex[2],
                                                                                 plane.m_vertex[3],
                                                                                 intersectionPoint);
            }
        }

        if(ok && (intersectionPoint != nullptr))
            addIntersectionPoint(*intersectionPoint);
    }

    return ok;
}

bool PolygonObjectsOfItemPicker::checkBox(const double &cx, const double &cy, const double &cz,
                                    const float &width, const float &height, const float &length,
                                    const Eigen::Vector3f &widthAxisDirection, const Eigen::Vector3f &lengthAxisDirection,
                                    bool filled,
                                    Eigen::Vector3d* intersectionPoint) const
{
    Eigen::Vector4d min, max;

    staticPrepareMinMaxForBox(cx, cy, cz, width, height, length, widthAxisDirection, lengthAxisDirection, min, max);

    // TODO : find a way to check only point that must be (per example if width or height or length == 0)
    Eigen::Vector3d points[8];
    points[0] = Eigen::Vector3d(min[0], min[1], min[2]);
    points[1] = Eigen::Vector3d(min[0], max[1], min[2]);
    points[2] = Eigen::Vector3d(max[0], max[1], min[2]);
    points[3] = Eigen::Vector3d(max[0], min[1], min[2]);

    int i, j, n;

    for(i=4; i<8; ++i) {
        points[i][0] = points[i-4][0];
        points[i][1] = points[i-4][1];
        points[i][2] = max[2];
    }

    const size_t nTriangles = m_triangles.size();

    bool ok = false;

    // one point in the polygon ?
    for(i=0; !ok && i<8; ++i)
        ok = IntersectionChecker::staticIsAPointInsideAPolygon(points[i], m_triangles, nTriangles);

    // no ? check if one segment of the box intersect with the polygon
    if(!ok) {
        int nPlanes = m_polygonPlanes.size();

        // if box is filled
        if(filled) {

            n = (int)ObjectsOfItemPicker::BOX_PLANE_INDICES.size();

            // check if at least one ray of the polygon intersect a face of the box (maximum of 6 faces to test)
            for(i=0; !ok && (i < n); i+=4) {

                // face of the box
                const Eigen::Vector3d& boxFaceP1 = points[ObjectsOfItemPicker::BOX_PLANE_INDICES[i]];
                const Eigen::Vector3d& boxFaceP2 = points[ObjectsOfItemPicker::BOX_PLANE_INDICES[i+1]];
                const Eigen::Vector3d& boxFaceP3 = points[ObjectsOfItemPicker::BOX_PLANE_INDICES[i+2]];
                const Eigen::Vector3d& boxFaceP4 = points[ObjectsOfItemPicker::BOX_PLANE_INDICES[i+3]];

                // exit loop if one ray of the polygon intersect with this face of the box
                for(int j=0; !ok && (j != nPlanes); ++j) {

                    const Plane &plane = m_polygonPlanes[j];

                    // ray of the polygon
                    const Eigen::Vector3d& polygonRayP1 = plane.m_vertex[0];
                    const Eigen::Vector3d& polygonRayP2 = plane.m_vertex[1];

                    ok = IntersectionChecker::staticDoesASegmentIntersectWithAPlane(polygonRayP1,
                                                                                    polygonRayP2,
                                                                                    boxFaceP1,
                                                                                    boxFaceP2,
                                                                                    boxFaceP3,
                                                                                    boxFaceP4,
                                                                                    intersectionPoint);
                }
            }
        } else {

            n = (int)ObjectsOfItemPicker::BOX_SEGMENT_INDICES.size();

            // check if at least one edge of the box intersect with one of the plane of the polygon
            for(i=0; !ok && (i != nPlanes); ++i) {

                const Plane &plane = m_polygonPlanes[i];

                // plane of the poygon
                const Eigen::Vector3d& polygonPlaneP1 = plane.m_vertex[0];
                const Eigen::Vector3d& polygonPlaneP2 = plane.m_vertex[1];
                const Eigen::Vector3d& polygonPlaneP3 = plane.m_vertex[2];
                const Eigen::Vector3d& polygonPlaneP4 = plane.m_vertex[3];

                // (maximum of 12 edges to test)
                for(j=0; !ok && j<n; j+=2) {

                    // edge of the box
                    const Eigen::Vector3d& boxEdgeP1 = points[ObjectsOfItemPicker::BOX_SEGMENT_INDICES[j]];
                    const Eigen::Vector3d& boxEdgeP2 = points[ObjectsOfItemPicker::BOX_SEGMENT_INDICES[j+1]];

                    ok = IntersectionChecker::staticDoesASegmentIntersectWithAPlane(boxEdgeP1,
                                                                                    boxEdgeP2,
                                                                                    polygonPlaneP1,
                                                                                    polygonPlaneP2,
                                                                                    polygonPlaneP3,
                                                                                    polygonPlaneP4,
                                                                                    intersectionPoint);
                }
            }
        }
    }

    if(ok && (intersectionPoint != nullptr))
        addIntersectionPoint(*intersectionPoint);

    return ok;
}

bool PolygonObjectsOfItemPicker::checkEllipse(const double &cx, const double &cy, const double &cz,
                                        const float &majorAxisRadius, const float &minorAxisRadius,
                                        const Eigen::Vector3f &majorAxisDirection,
                                        const Eigen::Vector3f &normal,
                                        bool filled,
                                        double initThetaInRad,
                                        double endThetaInRad)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(initThetaInRad)
    Q_UNUSED(endThetaInRad)
#endif
    // TODO : use initThetaInRad and endThetaInRad !

    if(majorAxisRadius == 0) {
        if(minorAxisRadius == 0)
            return checkPoint(cx, cy, cz);

        Eigen::Vector3d major = majorAxisDirection.normalized().cast<double>();
        const Eigen::Vector3d normalN = normal.normalized().cast<double>();
        Eigen::Vector3d minorAxisDirection = major.cross(normalN);

        return checkLine(cx-minorAxisDirection.x()*minorAxisRadius,
                         cy-minorAxisDirection.y()*minorAxisRadius,
                         cz-minorAxisDirection.z()*minorAxisRadius,
                         cx+minorAxisDirection.x()*minorAxisRadius,
                         cy+minorAxisDirection.y()*minorAxisRadius,
                         cz+minorAxisDirection.z()*minorAxisRadius);

    } else if(minorAxisRadius == 0) {

        Eigen::Vector3d major = majorAxisDirection.normalized().cast<double>();

        return checkLine(cx-major.x()*majorAxisRadius,
                         cy-major.y()*majorAxisRadius,
                         cz-major.z()*majorAxisRadius,
                         cx+major.x()*majorAxisRadius,
                         cy+major.y()*majorAxisRadius,
                         cz+major.z()*majorAxisRadius);
    }

    if(filled) {
        if(checkPoint(cx, cy, cz)) {
            return true;
        }
    }

    const Eigen::Vector3d center(cx, cy, cz);
    Eigen::Vector3d major = majorAxisDirection.normalized().cast<double>();
    const Eigen::Vector3d normalN = normal.normalized().cast<double>();
    Eigen::Vector3d minor = major.cross(normalN);

    major *= ((double)majorAxisRadius);
    minor *= ((double)minorAxisRadius);

    Eigen::Matrix4d  matrix = Eigen::Matrix4d::Identity();
    matrix.block(0,3,3,1) = center;
    matrix(0,0) = major[0];     // Xx
    matrix(1,0) = major[1];     // Xy
    matrix(2,0) = major[2];     // Xz
    matrix(0,1) = normalN[0];    // Yx
    matrix(1,1) = normalN[1];    // Yy
    matrix(2,1) = normalN[2];    // Yz
    matrix(0,2) = minor[0];     // Zx
    matrix(1,2) = minor[1];     // Zy
    matrix(2,2) = minor[2];     // Zz

    Eigen::Vector3d ellipseP1 = (matrix * Eigen::Vector4d(-1, 0, 1, 1)).block(0,0,3,1);
    Eigen::Vector3d ellipseP2 = (matrix * Eigen::Vector4d(-1, 0, -1, 1)).block(0,0,3,1);
    Eigen::Vector3d ellipseP3 = (matrix * Eigen::Vector4d(1, 0, -1, 1)).block(0,0,3,1);
    Eigen::Vector3d ellipseP4 = (matrix * Eigen::Vector4d(1, 0, 1, 1)).block(0,0,3,1);

    matrix = AffineInverse(matrix);

    Eigen::Vector3d intersectionLine[2];
    Eigen::Vector3d closestP[2];

    const size_t nPlanes = m_polygonPlanes.size();
    bool ok = false;
    bool lastIntersectionOk = false;

    int nInside = 0;
    int nOutside = 0;

    for(size_t i = 0; !ok && i < nPlanes; ++i) {

        const Plane &p = m_polygonPlanes[(int)i];

        if(lastIntersectionOk) {
            intersectionLine[0] = intersectionLine[1];
            closestP[0] = closestP[1];
            ok = true;
        } else {
            ok = IntersectionChecker::staticDoesASegmentIntersectWithAPlane(p.m_vertex[0], p.m_vertex[1], ellipseP1, ellipseP2, ellipseP3, ellipseP4, &intersectionLine[0], true, true);
            if(ok) {

                //addIntersectionPoint(intersectionLine[0]);
                if(Math::isPointInEllipse(intersectionLine[0], matrix, &closestP[0])) {

                    if(filled || (nOutside > 0)) {
                        addIntersectionPoint(intersectionLine[0]);
                        return true;
                    }

                    ++nInside;
                } else {
                    ++nOutside;
                }
                //addIntersectionPoint(closestP[0], Eigen::Vector3f(1,1,0));
            }
        }

        if(ok) {

            ok = IntersectionChecker::staticDoesASegmentIntersectWithAPlane(p.m_vertex[3], p.m_vertex[2], ellipseP1, ellipseP2, ellipseP3, ellipseP4, &intersectionLine[1], true, true);
            lastIntersectionOk = ok;

            if(ok) {

                //addIntersectionPoint(intersectionLine[1]);
                if(Math::isPointInEllipse(intersectionLine[1], matrix, &closestP[1])) {

                    if(filled || (nOutside > 0)) {
                        addIntersectionPoint(intersectionLine[1]);
                        return true;
                    }

                    ++nInside;
                } else {
                    ++nOutside;
                }
                //addIntersectionPoint(closestP[1], Eigen::Vector3f(1,1,0));

                //addIntersectionSegment(closestP[0], closestP[1], Eigen::Vector3f(1,1,0));
                ok = false;
                if(IntersectionChecker::staticDoestASegmentIntersectAUnitCircle(filled, closestP[0], closestP[1], &closestP[0])) {
                    addIntersectionPoint(closestP[0]/*, Eigen::Vector3f(1,0,0)*/);
                    return true;
                }

                //addIntersectionPoint(closestP[0], Eigen::Vector3f(1,0,0));
            }
        }
    }

    if(!filled && (nInside != static_cast<int>(nPlanes+1))) {
        if(checkPoint(cx, cy, cz)) {
            return true;
        }
    }

    return false;
}

DoublePoint PolygonObjectsOfItemPicker::getPointAtGlobalIndex(const size_t &globalIndex) const
{
    return m_pointAccessor->getDoublePointAtGlobalIndex(globalIndex);
}

void PolygonObjectsOfItemPicker::getPointAtGlobalIndex(const size_t &globalIndex, DoublePoint &p) const
{
    m_pointAccessor->getDoublePointAtGlobalIndex(globalIndex, p);
}
