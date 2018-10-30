#ifndef IINTERSECTIONINTERCEPTOR_H
#define IINTERSECTIONINTERCEPTOR_H

#include "Eigen/Core"

/**
 * @brief Inherit this class if you want to intercept all intersection point discovered
 */
class IIntersectionInterceptor {
public:
    virtual ~IIntersectionInterceptor() {}

    /**
     * @brief Called when a intersection point is detected
     */
    virtual void addIntersectionPoint(const Eigen::Vector3d& p, const Eigen::Vector3f& color = Eigen::Vector3f(1,1,1)) = 0;

    /**
     * @brief Called when a intersection line is detected
     */
    virtual void addIntersectionLine(const Eigen::Vector3d& origin, const Eigen::Vector3d& direction, const Eigen::Vector3f& color = Eigen::Vector3f(1,1,1)) = 0;

    /**
     * @brief Called when a intersection segment is detected
     */
    virtual void addIntersectionSegment(const Eigen::Vector3d& p1, const Eigen::Vector3d& p2, const Eigen::Vector3f& color = Eigen::Vector3f(1,1,1)) = 0;
};

#endif // IINTERSECTIONINTERCEPTOR_H
