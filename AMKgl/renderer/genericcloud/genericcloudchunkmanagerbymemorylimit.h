#ifndef GENERICCLOUDCHUNKMANAGERBYMEMORYLIMIT_H
#define GENERICCLOUDCHUNKMANAGERBYMEMORYLIMIT_H

#include "renderer/genericcloud/genericcloudchunkmanager.h"
#include "tools/opengl/openglinfo.h"

#define GenericCloudChunkManagerByMemoryLimitSuperClass GenericCloudChunkManager<ChunkT, std::vector<ChunkT*>, InheritFromT >

/**
 * @brief This class divide the cloud in X parts. Each parts have a maximum of Y points (parameter chunkMaxSize).
 *
 *        Per example if the cloud have a size of 14 elements :
 *
 *        -------------- (cloud of X (14 elements))
 *
 *        This class must be created like this (the last parameter was your choice) : new GenericCloudChunkManagerByMemoryLimit(0, 14, 2);
 *
 *        For the example we have set a max size for chunk to 2 elements. So the cloud will be
 *        partitionned into 7 chunks of 2 elements each.
 *
 *        If a cloud before it was deleted you must change the beginning of this manager :
 *
 *        manager->changeBeginningOfCloud(14);
 *
 *        This will shift all indexes stored in it.
 */
template<typename ChunkT,
         size_t ObjectSizeInBytes,
         class InheritFromT = GenericChunkManagerDefaultSuperClass<typename ChunkT::RendererContextType> >
class GenericCloudChunkManagerByMemoryLimit : public GenericCloudChunkManagerByMemoryLimitSuperClass
{
public:

    /**
     * @brief Create an object that manage the cloud that begin at "beginGlobalIndex".
     * @param beginGlobalIndex : index of the first point of the cloud to manage
     * @param cloudSize : size of the cloud
     * @param chunkMaxSize : the maximum size of one chunk. Typically it must be fixed
     *                       to the total available memory size of the graphics card minus
     *                       a margin. By default we define it to use the chunkMaxSize fixed by the developper
     *                       but if it was equal to 0 we use a maximum of 200Mo.
     */
    GenericCloudChunkManagerByMemoryLimit(GLenum glMode,
                                          const quint8& nVertexPerObject,
                                          const size_t& beginGlobalIndex,
                                          const size_t& cloudSize,
                                          const size_t& chunkMaxSize = 0) : GenericCloudChunkManagerByMemoryLimitSuperClass(glMode,
                                                                                       nVertexPerObject,
                                                                                       beginGlobalIndex,
                                                                                       cloudSize)
    {
        m_chunkMaxSize = (chunkMaxSize == 0 ? 209715198/ObjectSizeInBytes : chunkMaxSize);
        m_lastChunkUsed = nullptr;
        m_lastChunkIndexUsed = 0;
    }

    /**
     * @brief Create all chunks.
     */
    void init()
    {
        QMutexLocker locker(this->m_mutex);

        Q_ASSERT(getChunkMaxSize() != 0);

        this->deleteChunksFromMemory();

        size_t n = qCeil(((double)this->getCloudSize())/((double)this->getChunkMaxSize()));

        this->m_chunks.resize(n);

        size_t begin = this->getBeginningOfCloud();
        size_t end = begin+this->getCloudSize();

        for(size_t i=0; i<n; ++i) {
            this->m_chunks[i] = &this->createAndAddNewChunk(begin, qMin(end-begin, getChunkMaxSize()));
            begin += getChunkMaxSize();
        }
    }

    /**
     * @brief Returns a unique index that can be used after to create a chunk. If we call
     *        this method multiple times with a different object index but all object goes
     *        to the same chunk you must return the same unique index.
     * @param globalObjectIndex : global index of the object that we want to add to a chunk
     */
    size_t getChunkUniqueIndexForObjectAt(const size_t& globalObjectIndex) const
    {
        if((m_lastChunkUsed != nullptr)
                && (globalObjectIndex >= m_lastChunkUsed->getBeginningOfCloud())
                && (globalObjectIndex <= m_lastChunkUsed->getEndOfCloud())) {
            return m_lastChunkIndexUsed;
        }

        Q_ASSERT(globalObjectIndex >= this->getBeginningOfCloud());

        size_t localIndex = globalObjectIndex - this->getBeginningOfCloud();
        size_t chunkIndex = localIndex/getChunkMaxSize();

        if(chunkIndex != 0)
            localIndex = localIndex - (chunkIndex*this->getChunkMaxSize());

        const_cast<GenericCloudChunkManagerByMemoryLimit<ChunkT,ObjectSizeInBytes,InheritFromT>*>(this)->m_lastChunkIndexUsed = chunkIndex;
        const_cast<GenericCloudChunkManagerByMemoryLimit<ChunkT,ObjectSizeInBytes,InheritFromT>*>(this)->m_lastChunkUsed = this->m_chunks[chunkIndex];

        return chunkIndex;
    }

    /**
     * @brief We have previously called the method "getChunkUniqueIndexForVertexAt" and now we want
     *        to create a chunk or get it if it already exist. You must returns the chunk !
     * @param uniqueIndex : unique index to create a chunk
     */
    virtual ChunkT& createOrGetChunkFromUniqueIndex(const size_t& uniqueIndex)
    {
        return (*this->m_chunks[uniqueIndex]);
    }

    /**
     * @brief Returns the max size for one chunk
     */
    inline size_t getChunkMaxSize() const
    {
        return m_chunkMaxSize;
    }

private:

    /**
     * @brief Max size for one chunk
     */
    size_t          m_chunkMaxSize;

protected:

    /**
     * @brief The last chunk index used
     */
    size_t          m_lastChunkIndexUsed;

    /**
     * @brief The last chunk used
     */
    ChunkT*      m_lastChunkUsed;

    /**
     * @brief Called when a chunk must be added to the collection
     */
    void addChunkToCollection(ChunkT* chunk)
    {
        Q_UNUSED(chunk)
    }

    /**
     * @brief Called when the beginning of the cloud was changed
     */
    void beginningOfCloudChanged(const size_t& newBegin)
    {
        QMutexLocker locker(this->m_mutex);

        const size_t size = this->m_chunks.size();

        if(size > 0) {
            size_t begin = newBegin;
            size_t n = 0;

            do {
                changeBeginningOfCloudOfChunk(this->m_chunks[n++], begin);
                begin += m_chunkMaxSize;
            }while(n != size);
        }
    }

    /**
     * @brief Called when the begin of the cloud for a chunk must be changed
     */
    virtual void changeBeginningOfCloudOfChunk(ChunkT* chunk, const size_t& newBegin)
    {
        chunk->changeBeginningOfCloud(newBegin);
    }

    /**
     * @brief Called when a chunk is about to be deleted
     */
    void chunkAboutToBeDeleted(ChunkT* chunk)
    {
        m_lastChunkUsed = nullptr;

        GenericCloudChunkManagerByMemoryLimitSuperClass::chunkAboutToBeDeleted(chunk);
    }
};

/**
 * @brief Generic chunk manager with default traits
 */
template<typename ChunkT,
         size_t ObjectSizeInBytes,
         class InheritFromT = GenericChunkManagerDefaultSuperClass<typename ChunkT::RendererContextType> >
class GenericCloudChunkManagerByMemoryLimitWithDefaultTraits : public GenericCloudChunkManagerDefaultAllChunkTraits<GenericCloudChunkManagerByMemoryLimit<ChunkT, ObjectSizeInBytes, InheritFromT > >
{
public:
    GenericCloudChunkManagerByMemoryLimitWithDefaultTraits(GLenum glMode,
                                                           const quint8& nVertexPerObject,
                                                           const size_t& beginGlobalIndex,
                                                           const size_t& cloudSize) : GenericCloudChunkManagerDefaultAllChunkTraits<GenericCloudChunkManagerByMemoryLimit<ChunkT, ObjectSizeInBytes, InheritFromT > >(glMode, nVertexPerObject, beginGlobalIndex, cloudSize)
    {
    }
};

#endif // GENERICCLOUDCHUNKMANAGERBYMEMORYLIMIT_H
