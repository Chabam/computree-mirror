#ifndef INTERSECTIONCHECKER_H
#define INTERSECTIONCHECKER_H

#if defined(_WIN32) && defined(_MSC_VER) // Microsoft Visual Studio Compiler
#elif (defined(__linux__) || defined(_WIN32)) && defined(__GNUC__) // GNU Compiler (gcc,g++) for Linux, Unix, and MinGW (Windows)
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#if __GNUC__ > 7
#pragma GCC diagnostic ignored "-Wdeprecated-copy"
#else
#pragma GCC diagnostic ignored "-Wattributes" // See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=89325
#endif
#pragma GCC diagnostic ignored "-Wint-in-bool-context"
#elif defined(__APPLE__) // Clang Compiler (Apple)
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wint-in-bool-context"
#endif
#include "Eigen/Dense"
#if defined(_WIN32) && defined(_MSC_VER)
#elif (defined(__linux__) || defined(_WIN32)) && defined(__GNUC__)
#pragma GCC diagnostic warning "-Wall"
#pragma GCC diagnostic warning "-Wextra"
#if __GNUC__ > 7
#pragma GCC diagnostic warning "-Wdeprecated-copy"
#else
#pragma GCC diagnostic warning "-Wattributes" // See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=89325
#endif
#pragma GCC diagnostic warning "-Wint-in-bool-context"
#elif defined(__APPLE__)
#pragma GCC diagnostic warning "-Wall"
#pragma GCC diagnostic warning "-Wextra"
#pragma GCC diagnostic warning "-Wint-in-bool-context"
#endif

#include "picker/geometry/triangleplane.h"

/**
 * @brief Class that make multiple intersection test
 */
class IntersectionChecker
{
public:
    IntersectionChecker();

    /**
     * @brief Returns true if a segment intersect the triangle (filled)
     * @param segmentP1 : first point of the segment
     * @param segmentP2 : second point of the segment
     * @param v0 : first point of the triangle
     * @param v1 : second point of the triangle
     * @param v2 : third point of the triangle
     * @param intersectionPoint : if != nullptr contains the intersection point if founded
     * @param segmentAsARay : true if you want that the segment is infinite, otherwise false
     * @return true if it was an instersection, false otherwise.
     */
    template<typename VecType>
    static bool staticDoesASegmentIntersectWithATriangle(const VecType& segmentP1, const VecType& segmentP2,
                                                         const VecType& v0, const VecType& v1, const VecType& v2,
                                                         VecType* intersectionPoint = nullptr,
                                                         bool segmentAsARay = false)
    {
        VecType    u, v, n;              // triangle vectors
        VecType    dir, w;           // ray vectors
        double     b;              // params to calc ray-plane intersect

        // get triangle edge vectors and plane normal
        u = v1 - v0;
        v = v2 - v0;
        n = u.cross(v);              // cross product

        dir = segmentP2 - segmentP1;              // ray direction vector

        b = n.dot(dir);

        if (fabs(b) < 1e-6)     // ray is  parallel to triangle plane
            return false;

        VecType w0 = segmentP1 - v0;
        double a = -n.dot(w0);

        // get intersect point of ray with triangle plane
        double r = a / b;
        if (r < 0.0)                    // ray goes away from triangle
            return false;                   // => no intersect
        // for a segment, also test if (r > 1.0) => no intersect

        VecType I = segmentP1 + r * dir;            // intersect point of ray and plane

        if(!segmentAsARay
                && /*(Math::distancePoints(segmentP1, I) > Math::distancePoints(segmentP1,segmentP2))*/r > 1.0)
            return false;

        if(intersectionPoint != nullptr)
            *intersectionPoint = I;

        // is I inside T?
        double    uu, uv, vv, wu, wv, D;
        uu = u.dot(u);
        uv = u.dot(v);
        vv = v.dot(v);
        w = I - v0;
        wu = w.dot(u);
        wv = w.dot(v);
        D = uv * uv - uu * vv;

        // get and test parametric coords
        double s, t;
        s = (uv * wv - vv * wu) / D;
        if (s < 0.0 || s > 1.0)         // I is outside T
            return false;
        t = (uv * wu - uu * wv) / D;
        if (t < 0.0 || (s + t) > 1.0)  // I is outside T
            return false;

        return true;                       // I is in T
    }

    /**
     * @brief Returns true if a segment intersect the plane (filled)
     * @param segmentP1 : first point of the segment
     * @param segmentP2 : second point of the segment
     * @param v0 : first point of the plane
     * @param v1 : second point of the plane
     * @param v2 : third point of the plane
     * @param v3 : fourth point of the plane
     * @param intersectionPoint : if != nullptr contains the intersection point if founded
     * @param segmentAsARay : true if the segment is a infinite line, false if we must consider it at a segment
     * @param infinitePlane : true if the plane is infinite, false if we must consider the plane as two filled triangles (v0/v1/v2, v0/v2/v3)
     * @return true if it was an instersection, false otherwise.
     */
    template<typename VecType>
    static bool staticDoesASegmentIntersectWithAPlane(const VecType& segmentP1, const VecType& segmentP2,
                                                      const VecType& v0, const VecType& v1, const VecType& v2, const VecType& v3,
                                                      VecType* intersectionPoint = nullptr,
                                                      bool segmentAsARay = false,
                                                      bool infinitePlane = false)
    {
        if(infinitePlane) {
            VecType    u = segmentP2 - segmentP1;
            VecType    w = segmentP1 - v0;

            VecType up = v1-v0;
            VecType vp = v3-v0;
            VecType np = up.cross(vp);

            double     D = np.dot(u);
            double     N = -np.dot(w);

            if (fabs(D) < 1e-6) {           // segment is parallel to plane
                if (N == 0)  {                    // segment lies in plane

                    if(intersectionPoint != nullptr)
                        *intersectionPoint = v0;

                    return true;
                } else
                    return false;                    // no intersection
            }
            // they are not parallel
            // compute intersect param
            double sI = N / D;
            if (!segmentAsARay && (sI < 0 || sI > 1))
                return false;                        // no intersection

            if(intersectionPoint != nullptr) {
                VecType I = segmentP1 + sI * u;                  // compute segment intersect point
                *intersectionPoint = I;
            }

            return true;
        }

        return staticDoesASegmentIntersectWithATriangle(segmentP1, segmentP2, v0, v1, v2, intersectionPoint, segmentAsARay)
                || staticDoesASegmentIntersectWithATriangle(segmentP1, segmentP2, v0, v2, v3, intersectionPoint, segmentAsARay);
    }


    /**
     * @brief Returns true if a plane intersect another plane (planes are infinite !)
     * @param plane1 : equation of the first plane
     * @param plane2 : equation of the second plane
     * @param intersectionLine : if != nullptr contains the intersection line if founded (first point is the start point, second point is the direction)
     * @return true if it was an instersection, false otherwise.
     */
    static bool staticDoesAPlaneIntersectWithAPlane(const Eigen::Vector4d& plane1,
                                                    const Eigen::Vector4d& plane2,
                                                    Eigen::Vector3d intersectionLine[2] = nullptr);


    /**
     * @brief Returns true if a triangle intersect the plane
     * @param p1 : first point of the triangle
     * @param p2 : second point of the triangle
     * @param p3 : third point of the triangle
     * @param v0 : first point of the plane
     * @param v1 : second point of the plane
     * @param v2 : third point of the plane
     * @param v3 : fourth point of the plane
     * @param intersectionPoint : if != nullptr contains the intersection point if founded
     * @param infinitePlane : true if the plane is infinite, false if we must consider the plane as two filled triangles (v0/v1/v2, v0/v2/v3)
     * @return true if it was an instersection, false otherwise.
     */
    template<typename VecType>
    static bool staticDoesATriangleIntersectWithAPlane(const VecType& p1, const VecType& p2, const VecType& p3,
                                                       const VecType& v0, const VecType& v1, const VecType& v2, const VecType& v3,
                                                       VecType* intersectionPoint = nullptr,
                                                       bool infinitePlane = false)
    {
        /*if(infinitePlane && (intersectionPoint == nullptr)) {
            const Eigen::Vector3d normal = planeEquation.block(0,0,3,1);
            double a = normal.dot(p1) - planeEquation(3);
            double b = normal.dot(p2) - planeEquation(3);
            double c = normal.dot(p3) - planeEquation(3);

            // how to get the intersection point ?

            return (a*b <= 0.f || a*c <= 0.f);
        }*/

        return staticDoesASegmentIntersectWithAPlane(p1, p2, v0, v1, v2, v3, intersectionPoint, false, infinitePlane)
                || staticDoesASegmentIntersectWithAPlane(p2, p3, v0, v1, v2, v3, intersectionPoint, false, infinitePlane)
                || staticDoesASegmentIntersectWithAPlane(p3, p1, v0, v1, v2, v3, intersectionPoint, false, infinitePlane);


    }

    /**
     * @brief Returns true if a filled triangle intersect the plane
     * @param p1 : first point of the triangle
     * @param p2 : second point of the triangle
     * @param p3 : third point of the triangle
     * @param v0 : first point of the plane
     * @param v1 : second point of the plane
     * @param v2 : third point of the plane
     * @param v3 : fourth point of the plane
     * @param intersectionLine : if != nullptr contains the intersection line if founded (first point is the start point, second point is the direction)
     * @param infinitePlane : true if the plane is infinite, false if we must consider the plane as two filled triangles (v0/v1/v2, v0/v2/v3)
     * @return true if it was an instersection, false otherwise.
     */
    static bool staticDoesAFilledTriangleIntersectWithAPlane(const Eigen::Vector3d& p1, const Eigen::Vector3d& p2, const Eigen::Vector3d& p3,
                                                             const Eigen::Vector3d& v0, const Eigen::Vector3d& v1, const Eigen::Vector3d& v2, const Eigen::Vector3d& v3,
                                                             Eigen::Vector3d intersectionLine[2] = nullptr,
                                                             bool infinitePlane = false);

    static bool staticDoesALineIntersectWithACylinder(const Eigen::Vector3d &p1,
                                                      const Eigen::Vector3d &p2,
                                                      Eigen::Vector3d intersectionPoint[2] = nullptr);

    static bool staticDoestASegmentIntersectAUnitCircle(bool filled, const Eigen::Vector3d &p1, const Eigen::Vector3d &p2, Eigen::Vector3d *closestP = nullptr);

    /**
     * @brief Just check if the point is inside the 3d polygon (triangulated polygon)
     * @param x : x coordinate of the point
     * @param y : y coordinate of the point
     * @param z : z coordinate of the point
     * @param triangles : collection of planes (triangles of planes) that represent the polygon (std::vector<TrianglePlane> per example)
     * @param nTriangles : number of triangle in the list (just to accelerate the process if you have multiple points) => triangles.size()
     * @return true if the point is inside the polygon
     */
    template<typename Collection, typename ScalarType>
    static bool staticIsAPointInsideAPolygon(const ScalarType& x, const ScalarType& y, const ScalarType &z,
                                             const Collection& triangles,
                                             const size_t& nTriangles) {
        bool ok = false;
        int j, k;

        for(j=0; !ok && (j != static_cast<int>(nTriangles)); ++j) {

            const auto &tr = triangles[j];

            ok = true;

            for(k=0; ok && k<3; ++k) {
                if(tr.planes[k].distance(x, y, z) <= 0.0)
                    ok = false;
            }
        }

        return ok;
    }

    template<typename Collection, typename VecType>
    inline static bool staticIsAPointInsideAPolygon(const VecType& p,
                                                    const Collection& triangles,
                                                    const size_t& nTriangles) {
        return staticIsAPointInsideAPolygon(p(0), p(1), p(2), triangles, nTriangles);
    }
};

#endif // INTERSECTIONCHECKER_H
