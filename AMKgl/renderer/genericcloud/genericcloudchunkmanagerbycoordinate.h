#ifndef GENERICCLOUDCHUNKMANAGERBYCOORDINATE_H
#define GENERICCLOUDCHUNKMANAGERBYCOORDINATE_H

#include "amkglapp.h"
#include "renderer/genericcloud/genericcloudchunkmanager.h"
#include "interfaces/icoordinatesystemmanager.h"

#define GenericCloudChunkManagerByCoordinateSuperClass GenericCloudChunkManager< ChunkT, QList<ChunkT*>, InheritFromT >

/**
 * @brief When you want to draw a vertex from the global cloud you must use an offset. This class can
 *        create a chunk for each coordinate system and you must add all vertex with the same coordinate
 *        system to the created chunk.
 */
template<typename ChunkT,
         class InheritFromT = GenericChunkManagerDefaultSuperClass<typename ChunkT::RendererContextType> >
class GenericCloudChunkManagerByCoordinate : public GenericCloudChunkManagerByCoordinateSuperClass
{
public:
    /**
     * @brief Create a manager that begin at point at "beginGlobalIndex" and has a size of "size"
     * @param beginGlobalIndex : index of the first point in the vector
     * @param size : size of this chunk.
     * @warning (beginGlobalIndex + size) must not exceed the end of the cloud managed !
     */
    GenericCloudChunkManagerByCoordinate(GLenum glMode,
                                         const quint8& nVertexPerObject,
                                         const size_t& beginGlobalIndex,
                                         const size_t& cloudSize) : GenericCloudChunkManagerByCoordinateSuperClass(glMode,
                                                                               nVertexPerObject,
                                                                               beginGlobalIndex,
                                                                               cloudSize)
    {
        m_csm = AMKglEA->getCoordinateSystemManager();

        m_lastChunkUsed = nullptr;
        m_lastCoordinateSystemIndexUsed = std::numeric_limits<uint>::max();
    }

    /**
     * @brief Returns a unique index that can be used after to create a chunk. If we call
     *        this method multiple times with a different object index but all object goes
     *        to the same chunk you must return the same unique index.
     * @param globalObjectIndex : global index of the object that we want to add to a chunk
     */
    size_t getChunkUniqueIndexForObjectAt(const size_t& globalObjectIndex) const
    {
        return m_csm->getCoordinateSystemIndexOfPointAt(globalObjectIndex);
    }

    /**
     * @brief We have previously called the method "getChunkUniqueIndexForVertexAt" and now we want
     *        to create a chunk or get it if it already exist. You must returns the chunk !
     * @param uniqueIndex : unique index to create a chunk
     */
    ChunkT& createOrGetChunkFromUniqueIndex(const size_t& uniqueIndex)
    {
        if((m_lastChunkUsed == nullptr) || (uniqueIndex != m_lastCoordinateSystemIndexUsed)) {
            m_lastCoordinateSystemIndexUsed = (uint)uniqueIndex;
            m_lastChunkUsed = m_chunksByCoordinateSystem.value((uint)uniqueIndex, nullptr);

            if(m_lastChunkUsed == nullptr)
                m_lastChunkUsed = &this->createAndAddNewChunk(this->getBeginningOfCloud(), this->getCloudSize());
        }

        return *m_lastChunkUsed;
    }

    /**
     * @brief Returns a unique key that must be used to create the chunk for the specified vertex. One unique
     *        key for one coordinate system !
     * @param globalIndex : global index of the vertex
     */
    GLuint getChunkCoordinateSystemKeyForVertex(const size_t& globalIndex) const
    {
        return getChunkUniqueIndexForObjectAt(globalIndex);
    }

    /**
     * @brief Get (if already exist) or create and returns the chunk for the specified key (get it with method "getChunkCoordinateSystemKeyForVertex")
     * @param globalIndex : global index of the vertex
     */
    ChunkT& createOrGetChunkWithCoordinateSystemKey(const GLuint& csKey)
    {
        Q_UNUSED(csKey);
        return createOrGetChunkFromUniqueIndex((const GLuint)this->uniqueIndex);
    }

    /**
     * @brief Delete all chunks from memory
     */
    void deleteChunksFromMemory()
    {
        m_chunksByCoordinateSystem.clear();

        m_lastChunkUsed = nullptr;
        m_lastCoordinateSystemIndexUsed = std::numeric_limits<uint>::max();

        GenericCloudChunkManagerByCoordinateSuperClass::deleteChunksFromMemory();
    }

protected:
    typedef QHash<uint, ChunkT*>                ChunkByCsCollection;
    typedef QMutableHashIterator<uint, ChunkT*> ChunkByCsCollectionMutableIterator;

    /**
     * @brief One chunk for each coordinate system. Key = index of coordinate system, Value = ChunkT
     */
    ChunkByCsCollection             m_chunksByCoordinateSystem;

    /**
     * @brief Last index of the coordinate system used (to optimize access)
     */
    uint                            m_lastCoordinateSystemIndexUsed;

    /**
     * @brief Last chunk used (it was the chunk that match with the last coordinate system used)
     */
    ChunkT*                          m_lastChunkUsed;

    /**
     * @brief Coordinate system manager (just for access it faster)
     */
    ICoordinateSystemManager*       m_csm;

    /**
     * @brief Returns the informations of the current coordinate system used
     */
    void getCurrentCoordinateSystemInformations(Eigen::Vector3d& offset, uint& uniqueKey) const
    {
        m_csm->getCoordinateSystemInfos(m_lastCoordinateSystemIndexUsed, uniqueKey, offset);
    }

    /**
     * @brief Called when a chunk must be added to the collection
     */
    void addChunkToCollection(ChunkT* chunk)
    {
        m_lastChunkUsed = chunk;

        this->m_chunks.append(chunk);
        m_chunksByCoordinateSystem.insert(m_lastCoordinateSystemIndexUsed, m_lastChunkUsed);
    }

    /**
     * @brief Called when the beginning of the cloud has changed
     */
    void beginningOfCloudChanged(const size_t& newBegin)
    {
         typename GenericCloudChunkManagerByCoordinateSuperClass::ChunkCollection::iterator it = this->m_chunks.begin();
         typename GenericCloudChunkManagerByCoordinateSuperClass::ChunkCollection::iterator end = this->m_chunks.end();

        while(it != end) {
            (*it)->changeBeginningOfCloud(newBegin);
            ++it;
        }
    }

    /**
     * @brief Called when a chunk is about to be deleted
     */
    void chunkAboutToBeDeleted(ChunkT* chunk)
    {
        m_lastChunkUsed = nullptr;
        m_lastCoordinateSystemIndexUsed = std::numeric_limits<uint>::max();

        if(!m_chunksByCoordinateSystem.isEmpty()) {
            ChunkByCsCollectionMutableIterator it(m_chunksByCoordinateSystem);

            while(it.hasNext()) {
                if(it.next().value() == chunk)
                    it.remove();
            }
        }

        GenericCloudChunkManagerByCoordinateSuperClass::chunkAboutToBeDeleted(chunk);
    }
};

/**
 * @brief Generic chunk manager with default traits
 */
template<typename ChunkT,
         class InheritFromT = GenericChunkManagerDefaultSuperClass<typename ChunkT::RendererContextType> >
class GenericCloudChunkManagerByCoordinateWithDefaultTraits : public GenericCloudChunkManagerDefaultAllChunkTraits<GenericCloudChunkManagerByCoordinate<ChunkT, InheritFromT > >
{
public:
    GenericCloudChunkManagerByCoordinateWithDefaultTraits(GLenum glMode,
                                              const quint8& nVertexPerObject,
                                              const size_t& beginGlobalIndex,
                                              const size_t& cloudSize) : GenericCloudChunkManagerDefaultAllChunkTraits<GenericCloudChunkManagerByCoordinate<ChunkT, InheritFromT > >(glMode, nVertexPerObject, beginGlobalIndex, cloudSize)
    {
    }
};

#endif // GENERICCLOUDCHUNKMANAGERBYMEMORYLIMIT_H
