#ifndef POINTCLOUDATTRIBUTESPROVIDER_H
#define POINTCLOUDATTRIBUTESPROVIDER_H

#include "genericcloudattributesprovider.h"
#include "interfaces/iglobalpointcloudmanager.h"

/**
 * @brief Provider for attributes of points
 */
class PointCloudAttributesProvider : public GenericCloudAttributesProvider<IGlobalPointCloudManager>
{
public:
};

#endif // POINTCLOUDATTRIBUTESPROVIDER_H
