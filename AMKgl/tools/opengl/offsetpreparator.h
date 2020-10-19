#ifndef OFFSETPREPARATOR_H
#define OFFSETPREPARATOR_H

#include <Eigen/Dense>

class DrawInfo;

/**
 * @brief Tool class to push/pop the matrix of opengl to map with the current offset
 */
class OffsetPreparator
{
public:
    OffsetPreparator();

    /**
     * @brief Call to initialize the current matrix to use this offset (before draw your object !)
     */
    static void staticPushMatrix(const Eigen::Vector3d &offset, DrawInfo &info);

    /**
     * @brief Call to disable the current matrix (after draw your object !)
     */
    static void staticPopMatrix();
};

#endif // OFFSETPREPARATOR_H
