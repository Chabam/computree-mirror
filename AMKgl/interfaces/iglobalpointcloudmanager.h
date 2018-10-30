#ifndef IGLOBALPOINTCLOUDMANAGER_H
#define IGLOBALPOINTCLOUDMANAGER_H

#include "interfaces/igenericcloudmanager.h"
#include "definepoint.h"

#include <QList>

using namespace AMKgl;

/**
 * @brief Inherit from this class to let the engine create new cloud of
 *        colors/normals/infos. Cloud must be synced with the global point cloud !
 *
 *        This class let the engine access to the global points cloud too.
 *
 *        And finally this class let the engine register some listener to know
 *        when cloud is added/deleted from the global points cloud.
 *
 *        Used by "IEasyAccess" class.
 */
class IGlobalPointCloudManager : public IGenericCloudManager {
public:
    /**
     * @brief Returns the precise point at global index
     */
    virtual DoublePoint getDoublePointAtGlobalIndex(const size_t &globalIndex) const = 0;

    /**
     * @brief Get the coordinate of the precise point at global index by reference
     */
    virtual void getDoublePointAtGlobalIndex(const size_t &globalIndex, DoublePoint &p) const = 0;

    /**
     * @brief Returns the opengl point at global index
     */
    virtual FloatPoint& getFloatPointAtGlobalIndex(const size_t &globalIndex) = 0;
};

#endif // IGLOBALPOINTCLOUDMANAGER_H
