#ifndef POLYGONOBJECTSOFITEMPICKER_H
#define POLYGONOBJECTSOFITEMPICKER_H

#include "geometry/triangleplane.h"

#include "definepoint.h"

#include "objectsofitempicker.h"

#define _USE_MATH_DEFINES
#include <math.h>

class IGlobalPointCloudManager;

using namespace AMKgl;

/**
 * @brief Picker that use a polygone to pick objects
 */
class PolygonObjectsOfItemPicker : public ObjectsOfItemPicker
{
public:
    PolygonObjectsOfItemPicker(const QVector<Plane> &polygonPlanes, const QVector<TrianglePlane> &triangles);

    /**
     * @brief Returns true if all elements was set properly and is at least one triangle plane
     */
    virtual bool isValid() const;

    /**
     * @brief Call this method to check if the point is in the selection
     * @return true if at least one point is in the selection
     */
    bool computePoint(const double &x, const double &y, const double &z);

    /**
     * @brief Call this method to check if the point is in the selection
     * @return true if at least one point is in the selection
     */
    bool computePoint(const Eigen::Vector3d &p);

    /**
     * @brief Call this method to check if the point is in the selection
     * @return true if at least one point is in the selection
     */
    bool computePoint(const size_t &globalIndex);

    /**
     * @brief Call this method to check if a point in the vector of index of points is in the selection
     * @param cloud : collection of index of points to compute
     * @return true if at least one point is in the selection
     */
    bool computePoints(const CloudIndex* cloud);

    /**
     * @brief Call this method to check if the ellipse is in the selection
     * @param cx : X center position of the ellipse
     * @param cy : Y center position of the ellipse
     * @param cz : Z center position of the ellipse
     * @param majorAxisRadius : radius of the major axis
     * @param minorAxisRadius : radius of the minor axis
     * @param majorAxisDirection : a vector that describe the direction of the major axis
     * @param normal : the normal of the ellipse
     * @return true if the ellipse is in the selection
     */
    bool computeEllipse(const double& cx, const double& cy, const double &cz,
                        const float& majorAxisRadius, const float& minorAxisRadius,
                        const Eigen::Vector3f& majorAxisDirection = Eigen::Vector3f(1, 0, 0),
                        const Eigen::Vector3f& normal = Eigen::Vector3f(0, 1, 0),
                        bool filled = false);

    /**
     * @brief Call this method to check if the box is in the selection
     * @param cx : X center position of the box
     * @param cy : Y center position of the box
     * @param cz : Z center position of the box
     * @param width : the width of the box (X)
     * @param height : the height of the box (Y)
     * @param length : the length of the box (Z)
     * @param widthAxisDirection : a vector that describe the direction of the width (X)
     * @param lengthAxisDirection : a vector that describe the direction of the length (Z)
     * @return true if the box is in the selection
     */
    bool computeBox(const double& cx, const double& cy, const double &cz,
                    const float& width, const float& height, const float& length,
                    const Eigen::Vector3f& widthAxisDirection = Eigen::Vector3f(1, 0, 0),
                    const Eigen::Vector3f& lengthAxisDirection = Eigen::Vector3f(0, 0, 1),
                    bool filled = true);

    /**
     * @brief Call this method to check if the quads is in the selection
     * @param cx : X center position of the quads
     * @param cy : Y center position of the quads
     * @param cz : Z center position of the quads
     * @param width : the width of the quads (X)
     * @param length : the length of the quads (Z)
     * @param widthAxisDirection : a vector that describe the direction of the width (X)
     * @param lengthAxisDirection : a vector that describe the direction of the length (Z)
     * @return true if the quads is in the selection
     */
    bool computeQuads(const double& cx, const double& cy, const double &cz,
                      const float& width, const float& length,
                      const Eigen::Vector3f& widthAxisDirection = Eigen::Vector3f(1, 0, 0),
                      const Eigen::Vector3f& lengthAxisDirection = Eigen::Vector3f(0, 0, 1),
                      bool filled = true);

    /**
     * @brief Call this method to check if the line is in the selection
     * @param p1 : first point
     * @param p2 : second point
     * @return true if the line is in the selection
     */
    bool computeLine(const Eigen::Vector3d& p1,
                     const Eigen::Vector3d& p2);


    /**
     * @brief Call this method to check if the line is in the selection
     * @param p1GlobalIndex : first point global index
     * @param p2GlobalIndex : second point global index
     * @return true if the line is in the selection
     */
    bool computeLine(const size_t& p1GlobalIndex,
                     const size_t& p2GlobalIndex);

    /**
     * @brief Call this method to check if the triangle is in the selection
     * @param p1GlobalIndex : first point global index
     * @param p2GlobalIndex : second point global index
     * @param p3GlobalIndex : third point global index
     * @return true if the triangle is in the selection
     */
    bool computeTriangle(const size_t& p1GlobalIndex,
                         const size_t& p2GlobalIndex,
                         const size_t& p3GlobalIndex,
                         bool filled = true);

    /**
     * @brief Call this method to check if the triangle is in the selection
     * @param p1 : first point
     * @param p2 : second point
     * @param p3 : third point
     * @return true if the triangle is in the selection
     */
    bool computeTriangle(const Eigen::Vector3d& p1,
                         const Eigen::Vector3d& p2,
                         const Eigen::Vector3d& p3,
                         bool filled = true);

    /**
     * @brief Call this method to check if the cylinder is in the selection
     * @param cx : X center position of the cylinder
     * @param cy : Y center position of the cylinder
     * @param cz : Z center position of the cylinder
     * @param width : the width of the cylinder (X)
     * @param height : the width of the cylinder (Y)
     * @param length : the width of the cylinder (Z)
     * @param widthAxisDirection : a vector that describe the direction of the width
     * @param heightAxisDirection : a vector that describe the direction of the height
     * @return true if the cylinder is in the selection
     */
    bool computeCylinder(const double& cx, const double& cy, const double &cz,
                         const float& width, const float& height, const float& length,
                         const Eigen::Vector3f& widthAxisDirection = Eigen::Vector3f(1, 0, 0),
                         const Eigen::Vector3f& heightAxisDirection = Eigen::Vector3f(0, 1, 0));


    /**
     * @brief Call this method to check if the part of sphere is in the selection
     * @param cx : X center position of the sphere
     * @param cy : Y center position of the sphere
     * @param cz : Z center position of the sphere
     * @param radius : radius of the sphere
     * @param initTheta : start angle (X) of the part of sphere
     * @param endTheta : end angle (X) of the part of sphere
     * @param initPhi : start angle (Y) of the part of sphere
     * @param endPhi : end angle (Y) of the part of sphere
     * @param radians : true if angle has in radians, false if in degrees
     * @return true if the part of sphere is in the selection
     */
    bool computePartOfSphere(const double &cx, const double &cy, const double &cz,
                             const double &radius,
                             const double &initTheta, const double &endTheta,
                             const double &initPhi, const double &endPhi,
                             bool radians = true);

    /**
     * @brief Just check if the point is inside the 3d polygon (triangulated polygon). Don't call the select or check operation !
     * @param x : x coordinate of the point
     * @param y : y coordinate of the point
     * @param z : z coordinate of the point
     * @return true if the point is inside the polygon
     */
    bool checkPoint(const double &x, const double &y, const double &z) const;

    /**
     * @brief Just check if the line is inside or intersect the 3d polygon (triangulated polygon). Don't call the select or check operation !
     * @param x1 : x coordinate of the first point
     * @param y1 : y coordinate of the first point
     * @param z1 : z coordinate of the first point
     * @param x2 : x coordinate of the second point
     * @param y2 : y coordinate of the second point
     * @param z2 : z coordinate of the second point
     * @return true if the line is inside the polygon or intersect with it
     */
    bool checkLine(const double &x1, const double &y1, const double &z1,
                   const double &x2, const double &y2, const double &z2) const;

    /**
     * @brief Just check if the line is inside or intersect the 3d polygon (triangulated polygon). Don't call the select or check operation !
     * @param p1 : coordinates of the first point
     * @param p2 : coordinates of the second point
     * @return true if the line is inside the polygon or intersect with it
     */
    bool checkLine(const Eigen::Vector3d &p1,
                   const Eigen::Vector3d &p2) const;

    /**
     * @brief Just check if the triangle is inside the 3d polygon (triangulated polygon) or intersect with it. Don't call the select or check operation !
     * @param p1 : coordinates of the first point
     * @param p2 : coordinates of the second point
     * @param p3 : coordinates of the third point
     * @param filled : true if the triangle is filled, false otherwise
     * @return true if the triangle is inside the polygon or intersect with it
     */
    bool checkTriangle(const Eigen::Vector3d &p1,
                       const Eigen::Vector3d &p2,
                       const Eigen::Vector3d &p3,
                       const bool& filled) const;

    /**
     * @brief Just check if the box is inside the 3d polygon (triangulated polygon) or intersect with it. Don't call the select or check operation !
     * @param cx : X center position of the box
     * @param cy : Y center position of the box
     * @param cz : Z center position of the box
     * @param width : the width of the box (X)
     * @param height : the height of the box (Y)
     * @param length : the length of the box (Z)
     * @param widthAxisDirection : a vector that describe the direction of the width (X)
     * @param lengthAxisDirection : a vector that describe the direction of the length (Z)
     * @return true if the box is inside the polygon or intersect with it
     */
    bool checkBox(const double& cx, const double& cy, const double &cz,
                  const float& width, const float& height, const float& length,
                  const Eigen::Vector3f& widthAxisDirection = Eigen::Vector3f(1, 0, 0),
                  const Eigen::Vector3f& lengthAxisDirection = Eigen::Vector3f(0, 0, 1),
                  bool filled = true,
                  Eigen::Vector3d *intersectionPoint = nullptr) const;

    /**
     * @brief Call this method to check if the ellipse is in the selection or intersect with it. Don't call the select or check operation !
     * @param cx : X center position of the ellipse
     * @param cy : Y center position of the ellipse
     * @param cz : Z center position of the ellipse
     * @param majorAxisRadius : radius of the major axis
     * @param minorAxisRadius : radius of the minor axis
     * @param majorAxisDirection : a vector that describe the direction of the major axis
     * @param normal : the normal of the ellipse
     * @param filled : true if the ellipse is filled
     * @return true if the ellipse is in the selection
     */
    bool checkEllipse(const double& cx, const double& cy, const double &cz,
                        const float& majorAxisRadius, const float& minorAxisRadius,
                        const Eigen::Vector3f& majorAxisDirection = Eigen::Vector3f(1, 0, 0),
                        const Eigen::Vector3f& normal = Eigen::Vector3f(0, 1, 0),
                        bool filled = false,
                        double initThetaInRad = 0,
                        double endThetaInRad = M_PI*2);

private:
    IGlobalPointCloudManager*       m_pointAccessor;
    const QVector<Plane>&           m_polygonPlanes;
    const QVector<TrianglePlane>&   m_triangles;
    bool                            m_hasEnoughTriangle;

    /**
     * @brief Returns the point at specified index
     */
    DoublePoint getPointAtGlobalIndex(const size_t& globalIndex) const;
    void getPointAtGlobalIndex(const size_t& globalIndex, DoublePoint& p) const;
};

#endif // POLYGONOBJECTSOFITEMPICKER_H
