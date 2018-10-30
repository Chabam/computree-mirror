#ifndef OBJECTSPROCESSORFORPICKER_H
#define OBJECTSPROCESSORFORPICKER_H

#include "tools/iintersectioninterceptor.h"
#include "definecloudindexes.h"
#include "definegraphicsview.h"

#include "Eigen/Dense"

#include <vector>
#include <QObject>

using namespace AMKgl;

/**
 * @brief Interface of an element that process objects intersection with selection for a picker
 */
class ObjectsProcessorForPicker : public QObject
{
    Q_OBJECT

public:

    ObjectsProcessorForPicker();

    /**
     * @brief Returns true if all elements was set properly
     */
    virtual bool isValid() const  = 0;

    /**
     * @brief Returns the graphics view used when draw items
     */
    virtual GraphicsView* getGraphicsView() const = 0;

    /**
     * @brief Set the intersection interceptor
     */
    void setIntersectionInterceptor(const IIntersectionInterceptor* interceptor);

    /**
     * @brief Get the intersection interceptor
     */
    IIntersectionInterceptor* getIntersectionInterceptor() const;

public slots:
    /**
     * @brief Call this method to compute objects
     * @return true if at least one object is in the selection
     */
    virtual bool compute() = 0;

public:
    /**
     * @brief Call this method to check if the point is in the selection
     * @return true if at least one point is in the selection
     */
    virtual bool computePoint(const double &x, const double &y, const double &z) = 0;

    /**
     * @brief Call this method to check if the point is in the selection
     * @return true if at least one point is in the selection
     */
    virtual bool computePoint(const size_t &globalIndex) = 0;

    /**
     * @brief Call this method to check if a point in the vector of index of points is in the selection
     * @param cloud : collection of index of points to compute
     * @return true if at least one point is in the selection
     */
    virtual bool computePoints(const CloudIndex* cloud) = 0;

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
    virtual bool computeEllipse(const double& cx, const double& cy, const double &cz,
                                const float& majorAxisRadius, const float& minorAxisRadius,
                                const Eigen::Vector3f& majorAxisDirection = Eigen::Vector3f(1, 0, 0),
                                const Eigen::Vector3f& normal = Eigen::Vector3f(0, 1, 0),
                                bool filled = false) = 0;

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
    virtual bool computeBox(const double& cx, const double& cy, const double &cz,
                            const float& width, const float& height, const float& length,
                            const Eigen::Vector3f& widthAxisDirection = Eigen::Vector3f(1, 0, 0),
                            const Eigen::Vector3f& lengthAxisDirection = Eigen::Vector3f(0, 0, 1),
                            bool filled = true) = 0;

    /**
     * @brief Call this method to check if the quads is in the selection
     * @param cx : X center position of the quads
     * @param cy : Y center position of the quads
     * @param cz : Z center position of the quads
     * @param width : the width of the quads (X)
     * @param length : the width of the quads (Z)
     * @param widthAxisDirection : a vector that describe the direction of the width
     * @return true if the ellipse is in the selection
     * @param lengthAxisDirection : a vector that describe the direction of the length
     * @return true if the quads is in the selection
     */
    virtual bool computeQuads(const double& cx, const double& cy, const double &cz,
                              const float& width, const float& length,
                              const Eigen::Vector3f& widthAxisDirection = Eigen::Vector3f(1, 0, 0),
                              const Eigen::Vector3f& lengthAxisDirection = Eigen::Vector3f(0, 0, 1),
                              bool filled = true) = 0;

    /**
     * @brief Call this method to check if the line is in the selection
     * @param p1 : first point
     * @param p2 : second point
     * @return true if the line is in the selection
     */
    virtual bool computeLine(const Eigen::Vector3d& p1,
                             const Eigen::Vector3d& p2) = 0;


    /**
     * @brief Call this method to check if the line is in the selection
     * @param p1GlobalIndex : first point global index
     * @param p2GlobalIndex : second point global index
     * @return true if the line is in the selection
     */
    virtual bool computeLine(const size_t& p1GlobalIndex,
                             const size_t& p2GlobalIndex) = 0;

    /**
     * @brief Call this method to check if the triangle is in the selection
     * @param p1GlobalIndex : first point global index
     * @param p2GlobalIndex : second point global index
     * @param p3GlobalIndex : third point global index
     * @return true if the triangle is in the selection
     */
    virtual bool computeTriangle(const size_t& p1GlobalIndex,
                                 const size_t& p2GlobalIndex,
                                 const size_t& p3GlobalIndex,
                                 bool filled = true) = 0;

    /**
     * @brief Call this method to check if the triangle is in the selection
     * @param p1 : first point
     * @param p2 : second point
     * @param p3 : third point
     * @return true if the triangle is in the selection
     */
    virtual bool computeTriangle(const Eigen::Vector3d& p1,
                                 const Eigen::Vector3d& p2,
                                 const Eigen::Vector3d& p3,
                                 bool filled = true) = 0;

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
    virtual bool computeCylinder(const double& cx, const double& cy, const double &cz,
                                 const float& width, const float& height, const float& length,
                                 const Eigen::Vector3f& widthAxisDirection = Eigen::Vector3f(1, 0, 0),
                                 const Eigen::Vector3f& heightAxisDirection = Eigen::Vector3f(0, 1, 0)) = 0;

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
    virtual bool computePartOfSphere(const double &cx, const double &cy, const double &cz,
                                     const double &radius,
                                     const double &initTheta, const double &endTheta,
                                     const double &initPhi, const double &endPhi,
                                     bool radians = true) = 0;

    /**
     * @brief Prepare min and max (bouding box) of a box or a quads (pass 0 for "height" parameter)
     * @param cx : X center position of the box
     * @param cy : Y center position of the box
     * @param cz : Z center position of the box
     * @param width : the width of the box (X)
     * @param height : the height of the box (Y)
     * @param length : the length of the box (Z)
     * @param widthAxisDirection : a vector that describe the direction of the width (X)
     * @param lengthAxisDirection : a vector that describe the direction of the length (Z)
     * @param min : (OUT) min of the bounding box
     * @param max : (OUT) max of the bounding box
     */
    static void staticPrepareMinMaxForBox(const double &cx, const double &cy, const double &cz,
                                          const float &width, const float &height, const float &length,
                                          const Eigen::Vector3f &widthAxisDirection,
                                          const Eigen::Vector3f &lengthAxisDirection,
                                          Eigen::Vector4d& min,
                                          Eigen::Vector4d& max);

private:
    IIntersectionInterceptor*       m_interceptor;

protected:

    static const std::vector<quint8>      BOX_SEGMENT_INDICES;
    static const std::vector<quint8>      BOX_PLANE_INDICES;

    static std::vector<quint8> staticInitBoxSegmentIndices();
    static std::vector<quint8> staticInitBoxPlaneIndices();

    /**
     * @brief Add an intersection point (pass it to the interceptor if is not NULL)
     */
    void addIntersectionPoint(const Eigen::Vector3d& p, const Eigen::Vector3f& color = Eigen::Vector3f(1,1,1)) const;

    /**
     * @brief Add an intersection line (pass it to the interceptor if is not NULL)
     */
    void addIntersectionLine(const Eigen::Vector3d& origin, const Eigen::Vector3d& direction, const Eigen::Vector3f& color = Eigen::Vector3f(1,1,1)) const;

    /**
     * @brief Add an intersection segment (pass it to the interceptor if is not NULL)
     */
    void addIntersectionSegment(const Eigen::Vector3d &p1, const Eigen::Vector3d &p2, const Eigen::Vector3f& color = Eigen::Vector3f(1,1,1)) const;

signals:
    /**
     * @brief Emmited when picking has finished
     */
    void finished();
};

#endif // OBJECTSPROCESSORFORPICKER_H
