#ifndef MESHEDGEPERMANENTRENDERER_H
#define MESHEDGEPERMANENTRENDERER_H

#include "interfaces/iglobalpointcloudmanager.h"
#include "interfaces/iglobaledgecloudmanager.h"

#include "renderer/permanentrenderer.h"
#include "renderer/genericcloud/genericcloudrenderercontext.h"
#include "renderer/genericcloud/genericcloudchunkmanagerbymemorylimit.h"
#include "renderer/genericcloud/genericobjectcloudchunkmanagerbytexturelimit.h"

#include "meshobjectchunkmanagerbycoordinate.h"
#include "meshobjectchunkmanagerbyobjectcloud.h"
#include "traits/meshobjectchunkmanagerlaststagetraits.h"

class EdgeCloudAttributesProvider;

namespace MeshEdgePermanentRendererNM {

    typedef PermanentRenderer< GenericCloudRendererContext >        MFSuperClass;

    /**
     * @brief For a edge cloud manager :
     */
    typedef MeshObjectChunkManagerLastStageTraits< GenericObjectCloudChunkManagerByTextureLimit< MeshObjectNM::FinalChunkType, 8 >, Scene::LineGlobalCloud >                ByTextureLimitManager;
    typedef MeshObjectChunkManagerByObjectCloud< IGlobalEdgeCloudManager, ByTextureLimitManager >                                                                           ByEdgeCloudManager;
    typedef MeshObjectChunkManagerVertexStageTraits< MeshObjectChunkManagerByCoordinate< ByEdgeCloudManager > >                                                             ByCoordinateManager;
    typedef MeshObjectChunkManagerVertexStageTraits< GenericCloudChunkManagerByMemoryLimitWithDefaultTraits< ByCoordinateManager, sizeof(FloatPoint) > >                    ByMemoryLimitManager;
    typedef MeshObjectChunkManagerVertexStageTraits< GenericCloudChunkManagerByCloudWithDefaultTraits< IGlobalPointCloudManager, ByMemoryLimitManager, MFSuperClass > >     ByVertexCloudManager;
}

/**
 * @brief Use this class to render in opengl edge from global cloud that don't change its position. It is a renderer
 *        optimized for static edge (line)
 */
class MeshEdgePermanentRenderer : public MeshEdgePermanentRendererNM::ByVertexCloudManager,
                                  public IMeshObjectChunkForOutliersCreator
{
private:
    typedef MeshEdgePermanentRendererNM::ByVertexCloudManager   SuperClass;

public:
    MeshEdgePermanentRenderer();
    ~MeshEdgePermanentRenderer();

    /**
     * @brief Inherited from IObjectsRendererAttributesAccessor
     */
    int getNumberOfJumpForOneObject() const { return 1; }

    /**
     * @brief Inherited from IObjectsRendererAttributesAccessor
     */
    Scene::ObjectType getTypeOfObjectDrawn() const { return Scene::LineGlobalCloud; }

    /**
     * @brief Add an edge
     * @param edgeIndex : edge index in the global cloud
     * @param globalV1Index : global index of the first vertex
     * @param globalV2Index : global index of the second vertex
     * @param chunk : pointer that will contains the chunk used to store this object
     */
    void addEdge(const size_t& edgeIndex,
                 const size_t& globalV1Index,
                 const size_t& globalV2Index,
                 IChunk** chunk = NULL);

    /**
     * @brief Returns the edge cloud attributes provider
     */
    EdgeCloudAttributesProvider* getEdgeCloudAttributesProvider() const;

private:
    size_t m_vertexIndices[2];

    MeshObjectNM::OutlierChunkType* m_chunkForOutliers;

    /**
     * @brief The edge coud attributes provider
     */
    EdgeCloudAttributesProvider*    m_attributesCloudProvider;

protected:
    /**
     * @brief Returns others chunk that we must used to draw it or visit it
     */
    OtherChunkCollection getOthersChunk() const;

    /**
     * @brief Returns the chunk to use to add objects that was considered like outliers
     */
    MeshObjectNM::OutlierChunkType& createOrGetChunkForOutliers();

private:
    /**
     * @brief Called when we must update VAO of the chunk for outliers
     */
    void updateChunkForOutliersVAO(RendererContext* currentContext);
};

#endif // MESHEDGEPERMANENTRENDERER_H
