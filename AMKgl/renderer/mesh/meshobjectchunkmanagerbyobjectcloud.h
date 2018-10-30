#ifndef MESHOBJECTCHUNKMANAGERBYOBJECTCLOUD_H
#define MESHOBJECTCHUNKMANAGERBYOBJECTCLOUD_H

#include "renderer/genericcloud/genericcloudchunkmanagerbycloud.h"
#include "traits/meshobjectchunkmanageraddobjecttraits.h"
#include "traits/meshobjectchunkmanagervertexstagetraits.h"

#define MeshObjectChunkManagerByObjectCloudSuperClass MeshObjectChunkManagerAddObjectTraits<MeshObjectChunkManagerVertexStageTraits< GenericCloudChunkManagerByCloudWithDefaultTraits<ICloudManagerT, ChunkT > > >

template<typename ICloudManagerT,
         typename ChunkT>
class MeshObjectChunkManagerByObjectCloud : public MeshObjectChunkManagerByObjectCloudSuperClass
{
public:
    MeshObjectChunkManagerByObjectCloud(GLenum glMode,
                                        const quint8& nVertexPerObject,
                                        const size_t& beginGlobalIndex,
                                        const size_t& cloudSize) : MeshObjectChunkManagerByObjectCloudSuperClass(glMode,
                                                                                                                 nVertexPerObject,
                                                                                                                 beginGlobalIndex,
                                                                                                                 cloudSize)
    {
    }

    void setOffset(const Eigen::Vector3d& offset)
    {
        QMutexLocker locker(this->m_mutex);

        m_offset = offset;
    }

    Eigen::Vector3d getOffset() const
    {
        return m_offset;
    }

private:
    Eigen::Vector3d m_offset;

protected:

    /**
     * @brief Called when the beginning of the (vertex) cloud has changed
     */
    void beginningOfCloudChanged(const size_t& newBegin)
    {
        typename MeshObjectChunkManagerByObjectCloudSuperClass::ChunkCollection::iterator it = this->m_chunks.begin();
        typename MeshObjectChunkManagerByObjectCloudSuperClass::ChunkCollection::iterator end = this->m_chunks.end();

        while(it != end) {
            (*it)->changeBeginningOfVertexCloud(newBegin);
            ++it;
        }
    }

    /**
     * @brief Init the chunk (you can redefine this method if you want to init it differently)
     */
    void initChunk(typename MeshObjectChunkManagerByObjectCloudSuperClass::ChunkType* chunk)
    {
        chunk->changeBeginningOfVertexCloud(this->getBeginningOfCloud());
        chunk->setVertexCloudSize(this->getCloudSize());
        chunk->setOffset(getOffset());

        MeshObjectChunkManagerByObjectCloudSuperClass::initChunk(chunk);
    }
};

#endif // MESHOBJECTCHUNKMANAGERBYOBJECTCLOUD_H
