#ifndef FACECLOUDATTRIBUTESPROVIDER_H
#define FACECLOUDATTRIBUTESPROVIDER_H

#include "genericcloudattributesprovider.h"
#include "interfaces/iglobalfacecloudmanager.h"

/**
 * @brief Provider for attributes of faces
 */
class FaceCloudAttributesProvider : public GenericCloudAttributesProvider<IGlobalFaceCloudManager>
{
};

#endif // FACECLOUDATTRIBUTESPROVIDER_H
