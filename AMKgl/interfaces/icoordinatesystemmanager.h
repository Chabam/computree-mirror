#ifndef ICOORDINATESYSTEMMANAGER_H
#define ICOORDINATESYSTEMMANAGER_H

#include <QtOpenGL>

#include <Eigen/Core>

/**
 * @brief A coordinate system manager must be able to shift points with "double"
 *        coordinates to "float" coordinates (and vice/versa) by using an offset
 *        mechanism. The 3D space is divide in cube with a unique key for each. The
 *        center of each cube is the offset. This class manage a collection of
 *        coordinate system and we can access it by it's index in the collection. We
 *        can retrieve the unique key and the offset for each coordinate sytem.
 *
 *        A collection of index must be synced with the global point cloud. When a point
 *        is inserted in the global point cloud the index of the coordinate system used
 *        to shift this point in float/double coordinate must be set in the synced cloud.
 *
 *        So for each point inserted in the global point cloud the index to the coordinate
 *        system used to shift it can be easily founded.
 *
 *        Used by "IEasyAccess" class.
 */
class ICoordinateSystemManager
{
public:
    virtual ~ICoordinateSystemManager() {}

    /**
     * @brief Get the unique key (unique key is not the index !)
     *        and the offset of the specified coordinate system (by its index)
     */
    virtual void getCoordinateSystemInfos(const GLuint &coordinateSystemIndex,
                                          uint& uniqueKey,
                                          Eigen::Vector3d& offset) = 0;

    /**
     * @brief Returns the coordinate system index in the collection for the global point at specified index
     */
    virtual const GLuint& getCoordinateSystemIndexOfPointAt(const size_t &globalIndex) = 0;

    /**
     * @brief Get the unique key (unique key is not the index !)
     *        and the offset for a point at specified coordinates.
     */
    virtual void getCoordinateSystemInfosForCoordinates(const double& x,
                                                        const double& y,
                                                        const double& z,
                                                        uint& uniqueKey,
                                                        Eigen::Vector3d& offset) = 0;
};

#endif // ICOORDINATESYSTEMMANAGER_H
