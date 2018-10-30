#ifndef ICLOUDATTRIBUTESPROVIDER_H
#define ICLOUDATTRIBUTESPROVIDER_H

#include "defineglobalcloud.h"

using namespace AMKgl;

/**
 * @brief Provider for attributes of points, faces or edges
 */
class ICloudAttributesProvider {
public:
    virtual ~ICloudAttributesProvider() {}

    /**
     * @brief Returns the cloud that store colors of points. If it wasn't created it will be before returned.
     */
    virtual GlobalColorCloud* createOrGetColorCloud() = 0;

    /**
     * @brief Returns the cloud that store colors of normals. If it wasn't created it will be before returned.
     */
    virtual GlobalNormalCloud* createOrGetNormalCloud() = 0;

    /**
     * @brief Returns the cloud that store informations of points (visibility, etc...). If it wasn't created it will be before returned.
     */
    virtual GlobalInfoCloud* createOrGetInfoCloud() = 0;

    /**
     * @brief Returns the cloud that store colors of points
     */
    virtual GlobalColorCloud* getColorCloud() const = 0;

    /**
     * @brief Returns the cloud that store colors of normals
     */
    virtual GlobalNormalCloud* getNormalCloud() const = 0;

    /**
     * @brief Returns the cloud that store informations of points (visibility, etc...)
     */
    virtual GlobalInfoCloud* getInfoCloud() const = 0;

    /**
     * @brief Returns the cloud that store colors of points
     */
    virtual GlobalColorCloudRegistered createOrGetColorCloudRegistered() = 0;

    /**
     * @brief Returns the cloud that store colors of normals
     */
    virtual GlobalNormalCloudRegistered createOrGetNormalCloudRegistered() = 0;
    /**
     * @brief Returns the cloud that store informations of points (visibility, etc...)
     */
    virtual GlobalInfoCloudRegistered createOrGetInfoCloudRegistered() = 0;
};

#endif // ICLOUDATTRIBUTESPROVIDER_H
