#ifndef GENERICCLOUDCHUNKMANAGER_H
#define GENERICCLOUDCHUNKMANAGER_H

#include "renderer/genericcloud/genericchunkmanager.h"
#include "traits/genericcloudchunkmanagerdefaultallchunktraits.h"

/**
 * @brief Class tools that made generic process of typical chunk manager
 */
template<typename ChunkT,
         typename ChunkCollectionT,
         class InheritFromT = GenericChunkManagerDefaultSuperClass<typename ChunkT::RendererContextType> >
class GenericCloudChunkManager : public GenericChunkManager<ChunkT, ChunkCollectionT, InheritFromT >
{
public:
    GenericCloudChunkManager(GLenum glMode,
                             const quint8& nVertexPerObject,
                             const size_t& beginGlobalIndex,
                             const size_t& cloudSize) : GenericChunkManager<ChunkT, ChunkCollectionT, InheritFromT >(glMode, nVertexPerObject)
    {
        m_cloudSize = cloudSize;
        changeBeginningOfCloud(beginGlobalIndex);
    }

    /**
     * @brief Change the beginning of the cloud. You must call this method if a cloud before the cloud managed by
     *        this object was deleted from memory. (Show GlobalCloudManager to know of it works).
     * @param beginGlobalIndex : the new index of the first point.
     */
    virtual void changeBeginningOfCloud(const size_t& beginGlobalIndex)
    {
        QMutexLocker locker(this->m_mutex);

        m_cloudBeginIndex = beginGlobalIndex;
        m_cloudEndIndex = beginGlobalIndex;

        if(getCloudSize() > 0)
            m_cloudEndIndex = beginGlobalIndex + getCloudSize() - 1;

        beginningOfCloudChanged(m_cloudBeginIndex);
    }

    /**
     * @brief Returns the global index of the first point in the cloud used by this manager
     */
    inline size_t getBeginningOfCloud() const
    {
        return m_cloudBeginIndex;
    }

    /**
     * @brief Returns the global index of the last point that can be added to this chunk manager
     */
    inline size_t getEndOfCloud() const
    {
        return m_cloudEndIndex;
    }

    /**
     * @brief Returns the global index of the last point that can be added to this chunk manager
     */
    inline size_t getCloudSize() const
    {
        return m_cloudSize;
    }

protected:

    /**
     * @brief Global index of the first point in the global vector
     */
    size_t                              m_cloudBeginIndex;

    /**
     * @brief Global index of the last point in the global vector that can be added to this manager
     */
    size_t                              m_cloudEndIndex;

    /**
     * @brief Size of the cloud managed
     */
    size_t                              m_cloudSize;

    /**
     * @brief Add a new chunk to the collection
     * @param globalBeginIndex : begin index of the new cloud
     * @param size : size of the new cloud
     */
    virtual ChunkT& createAndAddNewChunk(const size_t& globalBeginIndex,
                                        const size_t& size)
    {
        QMutexLocker locker(this->m_mutex);

        ChunkT* r = createNewChunk(globalBeginIndex,
                                  size);
        this->initAndAddChunk(r);

        return (*r);
    }

    /**
     * @brief Create a new chunk and return it (you can redefine this method if you want to create a chunk differently)
     * @param globalBeginIndex : begin index of the new cloud
     * @param size : size of the new cloud
     */
    virtual ChunkT* createNewChunk(const size_t& globalBeginIndex,
                                   const size_t& size) = 0;

    /**
     * @brief Called when the beginning of the cloud has changed
     */
    virtual void beginningOfCloudChanged(const size_t& newBegin) { Q_UNUSED(newBegin) }
};

/**
 * @brief Generic chunk manager with default traits
 */
template<typename ChunkT,
         typename ChunkCollectionT,
         class InheritFromT = GenericChunkManagerDefaultSuperClass<typename ChunkT::RendererContextType> >
class GenericCloudChunkManagerWithDefaultTraits : public GenericCloudChunkManagerDefaultAllChunkTraits<GenericCloudChunkManager<ChunkT, ChunkCollectionT, InheritFromT > >
{
public:
    GenericCloudChunkManagerWithDefaultTraits(GLenum glMode,
                                              const quint8& nVertexPerObject,
                                              const size_t& beginGlobalIndex,
                                              const size_t& cloudSize) : GenericCloudChunkManagerDefaultAllChunkTraits<GenericCloudChunkManager<ChunkT, ChunkCollectionT, InheritFromT > >(glMode, nVertexPerObject, beginGlobalIndex, cloudSize)
    {
    }
};

#endif // GENERICCLOUDCHUNKMANAGER_H
