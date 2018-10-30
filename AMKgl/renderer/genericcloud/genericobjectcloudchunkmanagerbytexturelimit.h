#ifndef GENERICOBJECTCLOUDCHUNKMANAGERBYTEXTURELIMIT_H
#define GENERICOBJECTCLOUDCHUNKMANAGERBYTEXTURELIMIT_H

#include "genericcloudchunkmanagerbymemorylimit.h"
#include "tools/opengl/openglinfo.h"

#define GenericObjectCloudChunkManagerByTextureLimitSuperClass GenericCloudChunkManagerByMemoryLimit< ChunkT, ObjectSizeInBytes, InheritFromT >

/**
 * @brief When you want to draw an object composed of multiple vertex and you want to colorize/select the
 *        object you have multiple clouds :
 *          - One cloud is the global vertex cloud
 *          - Other clouds was the global color object cloud or the global normal cloud, etc....
 *
 *        Per example :
 *
 *        Vertex cloud [V3f;V3f;V3f;V3f;V3f;V3f;V3f;V3f;V3f....] (cloud of vertex composed of 3 float)
 *        Face cloud [F;F;F;F;F;....] (cloud of face)
 *        Face Color cloud [C3ub;C3ub;C3ub;C3ub;C3ub;....] (cloud of colors for each face so it was the same size that the cloud of faces)
 *
 *        A chunk will create a texture for colors, another for normals, etc.... and pass it to a shader to
 *        be able to draw each object with attributes.
 *
 *        The chunk must have a limit of size that was the texture limit.
 */
template<typename ChunkT,
         size_t ObjectSizeInBytes,
         class InheritFromT = GenericChunkManagerDefaultSuperClass<typename ChunkT::RendererContextType> >
class GenericObjectCloudChunkManagerByTextureLimit : public GenericObjectCloudChunkManagerByTextureLimitSuperClass
{
public:
    GenericObjectCloudChunkManagerByTextureLimit(GLenum glMode,
                                                 const quint8& nVertexPerObject,
                                                 const size_t& beginGlobalIndex,
                                                 const size_t& cloudSize) : GenericObjectCloudChunkManagerByTextureLimitSuperClass(glMode,
                                                                                                                                   nVertexPerObject,
                                                                                                                                   beginGlobalIndex,
                                                                                                                                   cloudSize,
                                                                                                                                   /*qMax((OPENGL_INFO->getMaxTextureSizeInByte()/ObjectSizeInBytes), (1024/ObjectSizeInBytes))*/cloudSize)
    {
    }
};

/**
 * @brief Generic chunk manager with default traits
 */
template<typename ChunkT,
         size_t ObjectSizeInBytes,
         class InheritFromT = GenericChunkManagerDefaultSuperClass<typename ChunkT::RendererContextType> >
class GenericObjectCloudChunkManagerByTextureLimitWithDefaultTraits : public GenericCloudChunkManagerDefaultAllChunkTraits<GenericObjectCloudChunkManagerByTextureLimit<ChunkT, ObjectSizeInBytes, InheritFromT > >
{
public:
    GenericObjectCloudChunkManagerByTextureLimitWithDefaultTraits(GLenum glMode,
                                                                  const quint8& nVertexPerObject,
                                                                  const size_t& beginGlobalIndex,
                                                                  const size_t& cloudSize) : GenericCloudChunkManagerDefaultAllChunkTraits<GenericObjectCloudChunkManagerByTextureLimit<ChunkT, ObjectSizeInBytes, InheritFromT > >(glMode, nVertexPerObject, beginGlobalIndex, cloudSize)
    {
    }
};

#endif // GENERICOBJECTCLOUDCHUNKMANAGERBYTEXTURELIMIT_H
