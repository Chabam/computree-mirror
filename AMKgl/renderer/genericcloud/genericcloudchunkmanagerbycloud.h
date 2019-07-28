#ifndef GENERICCLOUDCHUNKMANAGERBYCLOUD_H
#define GENERICCLOUDCHUNKMANAGERBYCLOUD_H

#include "amkglapp.h"
#include "drawinfo.h"

#include "renderer/iglobalcloudlistener.h"
#include "renderer/genericcloud/genericcloudchunkmanager.h"

#define GenericCloudChunkManagerByCloudSuperClass GenericCloudChunkManager< ChunkT, std::vector<ChunkT*>, InheritFromT >

/**
 * @brief This class use an entire vector of X (points, edges, faces, etc...) and create
 *        a chunk for each cloud.
 *
 *        The global cloud manager has multiple cloud in the vector, per
 *        example 3 clouds :
 *
 *              C1         C2        C3
 *        --------------////////XXXXXXXXXXXXX       (global vector of X)
 *
 *        This class will create a Chunk (template) for each cloud :
 *
 *        For C1 : chunk1 = new Chunk(beginIndexC1, sizeC1);
 *        For C2 : chunk2 = new Chunk(beginIndexC2, sizeC2);
 *        For C3 : chunk3 = new Chunk(beginIndexC3, sizeC3);
 *
 *        This class will be registered to the global cloud manager to receive information when a
 *        cloud is added or deleted.
 *
 *        If a cloud is added a new Chunk is created with the begin of this cloud and its size :
 *
 *          void cloudAdded(const size_t &size) { .... new Chunk(beginIndex, size); ..... }
 *
 *        If a cloud is deleted all chunks after it will have it's beginning changed :
 *
 *          void cloudWillBeDeleted(const size_t &beginIndex, const size_t &size) {
 *                  .......
 *                  chunkAfter->changeBeginningOfCloud(newBegin);
 *                  .......
 *          }
 */
template<typename ICloudManagerT,
         typename ChunkT,
         class InheritFromT = GenericChunkManagerDefaultSuperClass<typename ChunkT::RendererContextType> >
class GenericCloudChunkManagerByCloud : public GenericCloudChunkManagerByCloudSuperClass, public IGlobalCloudListener
{
private:
    typedef GenericCloudChunkManagerByCloud<ICloudManagerT, ChunkT, InheritFromT> ThisClass;

public:
    typedef ICloudManagerT                                                     CloudManager;

    GenericCloudChunkManagerByCloud(GLenum glMode,
                                    const quint8& nVertexPerObject,
                                    const size_t& beginGlobalIndex,
                                    const size_t& cloudSize) : GenericCloudChunkManagerByCloudSuperClass(glMode,
                                                                                                         nVertexPerObject,
                                                                                                         beginGlobalIndex,
                                                                                                         cloudSize)
    {
        m_globalCloudManager = AMKglEA->getGlobalCloudManager<CloudManager>();
        m_globalCloudManager->addCloudListener(this);

        m_lastChunkUsed = nullptr;
        m_lastChunkIndexUsed = 0;
    }

    ~GenericCloudChunkManagerByCloud()
    {
        m_globalCloudManager->removeCloudListener(this);
    }

    /**
     * @brief Call it after you have set the attribute accessor and all necessary elements
     */
    void init()
    {
        QMutexLocker locker(this->m_mutex);

        size_t n = m_globalCloudManager->countCloud();

        for(size_t i=0; i<n; ++i) {

            const size_t globalIndex = m_globalCloudManager->cloudBeginIndexAt(i);
            const size_t size = m_globalCloudManager->cloudSizeAt(i);

            this->createAndAddNewChunk(globalIndex,
                                 size);
        }
    }

protected:
    /**
     * @brief The global cloud manager
     */
    CloudManager*       m_globalCloudManager;

    /**
     * @brief The last chunk index used
     */
    size_t              m_lastChunkIndexUsed;

    /**
     * @brief The last chunk used
     */
    ChunkT*             m_lastChunkUsed;

    /**
     * @brief Called when the global cloud will be imputed of elements between beginIndex and endIndex (endIndex = beginIndex+size).
     */
    void cloudWillBeDeleted(const size_t &beginIndex, const size_t &size)
    {
        // lock the mutex here but unlock it only when the cloud is really deleted to avoid
        // drawing or update between this two event
        this->m_mutex->lock();

        typename GenericCloudChunkManagerByCloudSuperClass::ChunkCollection::iterator it = this->m_chunks.begin();
        typename GenericCloudChunkManagerByCloudSuperClass::ChunkCollection::iterator end = this->m_chunks.end();

        const size_t beginOfNextCloud = beginIndex + size;

        while(it != end) {
            ChunkT *r = (*it);

            if(r->getBeginningOfCloud() >= beginIndex) {
                if(r->getBeginningOfCloud() < beginOfNextCloud) {
                    chunkAboutToBeDeleted(r);
                    it = this->m_chunks.erase(it);
                    end = this->m_chunks.end();

                    if(qApp->thread() != QThread::currentThread())
                        this->m_chunksToDeleteInGuiThread.push_back(r);
                    else
                        delete r;
                } else {
                    r->changeBeginningOfCloud(r->getBeginningOfCloud() - size);
                    ++it;
                }
            } else {
                ++it;
            }
        }
    }

    /**
     * @brief Called when the global cloud is imputed of elements between beginIndex and endIndex (endIndex = beginIndex+size).
     */
    void cloudDeleted(const size_t &beginIndex, const size_t &size)
    {
        Q_UNUSED(beginIndex)
        Q_UNUSED(size)

        // unlock the mutex that was locked in the "cloudWillBeDeleted" event
        this->m_mutex->unlock();
    }

    /**
     * @brief Called when the size of the global cloud has increased
     */
    void cloudAdded(const size_t &size)
    {
        QMutexLocker locker(this->m_mutex);

        this->createAndAddNewChunk(m_globalCloudManager->size() - size,
                                   size);
    }

    /**
     * @brief Called when a chunk must be added to the collection
     */
    void addChunkToCollection(ChunkT* chunk)
    {
        this->m_chunks.push_back(chunk);
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

        typename GenericCloudChunkManagerByCloudSuperClass::ChunkCollection::const_iterator it = this->m_chunks.begin();
        typename GenericCloudChunkManagerByCloudSuperClass::ChunkCollection::const_iterator end = this->m_chunks.end();

        Q_ASSERT(it != end);

        ChunkT* r = nullptr;

        size_t i = 0;

        do
        {
            r = (*it);
            ++it;
            ++i;
        }while((it != end) && (r->getEndOfCloud() < globalObjectIndex));

        const_cast<ThisClass*>(this)->m_lastChunkUsed = r;
        const_cast<ThisClass*>(this)->m_lastChunkIndexUsed = i;

        return i;
    }

    /**
     * @brief We have previously called the method "getChunkUniqueIndexForVertexAt" and now we want
     *        to create a chunk or get it if it already exist. You must returns the chunk !
     * @param uniqueIndex : unique index to create a chunk
     */
    ChunkT& createOrGetChunkFromUniqueIndex(const size_t& uniqueIndex)
    {
        return (*this->m_chunks[uniqueIndex-1]);
    }

    /**
     * @brief Returns the chunk to use to add object at index "globalIndex"
     * @param globalIndex : the index of the object in the global cloud to add
     * @param currentIndexInCollection : index in the ChunkCollection when we have found the chunk.
     * @warning * If you want the index in the collection of chunks you must do the operation : m_chunks[(*currentIndexInCollection)-1]
     * @warning * If you want to use the method "createOrGetChunkFromUniqueIndex" just call it like this "createOrGetChunkFromUniqueIndex(*currentIndexInCollection)"
     */
    ChunkT* getChunkForObjectAt(const size_t &globalIndex,
                                size_t* currentIndexInCollection = nullptr) const
    {
        const size_t ui = getChunkUniqueIndexForObjectAt(globalIndex);

        if(currentIndexInCollection != nullptr)
            *currentIndexInCollection = ui;

        return &const_cast<GenericCloudChunkManagerByCloud<ICloudManagerT, ChunkT, InheritFromT>*>(this)->createOrGetChunkFromUniqueIndex(ui);
    }

    void chunkAboutToBeDeleted(ChunkT* chunk)
    {
        m_lastChunkUsed = nullptr;

        GenericCloudChunkManagerByCloudSuperClass::chunkAboutToBeDeleted(chunk);
    }
};

/**
 * @brief Generic chunk manager with default traits
 */
template<typename ICloudManagerT,
         typename ChunkT,
         class InheritFromT = GenericChunkManagerDefaultSuperClass<typename ChunkT::RendererContextType> >
class GenericCloudChunkManagerByCloudWithDefaultTraits : public GenericCloudChunkManagerDefaultAllChunkTraits< GenericCloudChunkManagerByCloud< ICloudManagerT, ChunkT, InheritFromT > >
{
public:
    GenericCloudChunkManagerByCloudWithDefaultTraits(GLenum glMode,
                                              const quint8& nVertexPerObject,
                                              const size_t& beginGlobalIndex,
                                              const size_t& cloudSize) : GenericCloudChunkManagerDefaultAllChunkTraits< GenericCloudChunkManagerByCloud< ICloudManagerT, ChunkT, InheritFromT > >(glMode, nVertexPerObject, beginGlobalIndex, cloudSize)
    {
    }
};

#endif // GENERICCLOUDCHUNKMANAGERBYCLOUD_H
