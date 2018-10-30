#ifndef EDGECLOUDATTRIBUTESPROVIDER_H
#define EDGECLOUDATTRIBUTESPROVIDER_H

#include "genericcloudattributesprovider.h"
#include "interfaces/iglobaledgecloudmanager.h"

/**
 * @brief Provider for attributes of edges
 */
class EdgeCloudAttributesProvider : public GenericCloudAttributesProvider<IGlobalEdgeCloudManager>
{
};

#endif // EDGECLOUDATTRIBUTESPROVIDER_H
