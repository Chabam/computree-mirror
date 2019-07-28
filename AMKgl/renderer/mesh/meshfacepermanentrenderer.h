#ifndef MESHFACEPERMANENTRENDERER_H
#define MESHFACEPERMANENTRENDERER_H

#include "interfaces/iglobalpointcloudmanager.h"
#include "interfaces/iglobalfacecloudmanager.h"

#include "renderer/permanentrenderer.h"
#include "renderer/genericcloud/genericcloudrenderercontext.h"
#include "renderer/genericcloud/genericcloudchunkmanagerbymemorylimit.h"
#include "renderer/genericcloud/genericobjectcloudchunkmanagerbytexturelimit.h"

#include "meshobjectchunkmanagerbycoordinate.h"
#include "meshobjectchunkmanagerbyobjectcloud.h"
#include "traits/meshobjectchunkmanagerlaststagetraits.h"

class FaceCloudAttributesProvider;

namespace MeshFacePermanentRendererNM {

    typedef PermanentRenderer< GenericCloudRendererContext >        MFSuperClass;

    /**
     * @brief For a face cloud manager :
     */
    typedef MeshObjectChunkManagerLastStageTraits< GenericObjectCloudChunkManagerByTextureLimit< MeshObjectNM::FinalChunkType, 12 >, Scene::TriangleGlobalCloud >           ByTextureLimitManager;
    typedef MeshObjectChunkManagerByObjectCloud< IGlobalFaceCloudManager, ByTextureLimitManager >                                                                           ByFaceCloudManager;
    typedef MeshObjectChunkManagerVertexStageTraits< MeshObjectChunkManagerByCoordinate< ByFaceCloudManager > >                                                             ByCoordinateManager;
    typedef MeshObjectChunkManagerVertexStageTraits< GenericCloudChunkManagerByMemoryLimitWithDefaultTraits< ByCoordinateManager, sizeof(FloatPoint) > >                    ByMemoryLimitManager;
    typedef MeshObjectChunkManagerVertexStageTraits< GenericCloudChunkManagerByCloudWithDefaultTraits< IGlobalPointCloudManager, ByMemoryLimitManager, MFSuperClass > >     ByVertexCloudManager;
}

/**
 * @brief Use this class to render in opengl face from global cloud that don't change its position. It is a renderer
 *        optimized for static face (triangle)
 */
class MeshFacePermanentRenderer : public MeshFacePermanentRendererNM::ByVertexCloudManager,
                                  public IMeshObjectChunkForOutliersCreator
{
private:
    typedef MeshFacePermanentRendererNM::ByVertexCloudManager   SuperClass;

public:
    MeshFacePermanentRenderer();
    ~MeshFacePermanentRenderer();

    /**
     * @brief Inherited from IObjectsRendererAttributesAccessor
     */
    int getNumberOfJumpForOneObject() const { return 1; }

    /**
     * @brief Inherited from IObjectsRendererAttributesAccessor
     */
    Scene::ObjectType getTypeOfObjectDrawn() const { return Scene::TriangleGlobalCloud; }

    /**
     * @brief Add a face
     * @param faceIndex : face index in the global cloud
     * @param globalV1Index : global index of the first vertex
     * @param globalV2Index : global index of the second vertex
     * @param globalV3Index : global index of the third vertex
     * @param chunk : pointer that will contains the chunk used to store this object
     */
    void addFace(const size_t& faceIndex,
                 const size_t& globalV1Index,
                 const size_t& globalV2Index,
                 const size_t& globalV3Index,
                 IChunk** chunk = nullptr);

    /**
     * @brief Returns the face cloud attributes provider
     */
    FaceCloudAttributesProvider* getFaceCloudAttributesProvider() const;

    /**
     * @brief Draw all elements normally (all elements are drawn with a good precision)
     */
    void draw(DrawInfo &info);

    /**
     * @brief Draw all elements fast (all elements are drawn with a minimum number of points)
     */
    void fastDraw(DrawInfo &info);

private:
    size_t m_vertexIndices[3];

    MeshObjectNM::OutlierChunkType* m_chunkForOutliers;

    /**
     * @brief The face coud attributes provider
     */
    FaceCloudAttributesProvider*    m_attributesCloudProvider;

protected:
    /**
     * @brief Returns others chunk that we must used to draw it or visit it
     */
    OtherChunkCollection getOthersChunk() const;

    /**
     * @brief Returns the chunk to use to add objects that was considered like outliers
     */
    MeshObjectNM::OutlierChunkType& createOrGetChunkForOutliers();
};

#endif // MESHFACEPERMANENTRENDERER_H
