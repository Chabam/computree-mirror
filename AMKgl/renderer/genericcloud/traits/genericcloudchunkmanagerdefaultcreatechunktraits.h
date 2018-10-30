#ifndef GENERICCLOUDCHUNKMANAGERDEFAULTCREATECHUNKTRAITS_H
#define GENERICCLOUDCHUNKMANAGERDEFAULTCREATECHUNKTRAITS_H

#include <QtOpenGL>

/**
 * @brief Default method "createNewChunk"
 */
template<typename SuperClassT>
class GenericCloudChunkManagerDefaultCreateChunkTraits : public SuperClassT
{
public:
    typedef typename SuperClassT::ChunkType ChunkType;

    GenericCloudChunkManagerDefaultCreateChunkTraits(GLenum glMode,
                                                     const quint8& nVertexPerObject,
                                                     const size_t& beginGlobalIndex,
                                                     const size_t& cloudSize) : SuperClassT(glMode,
                                                                                           nVertexPerObject,
                                                                                           beginGlobalIndex,
                                                                                           cloudSize) {}

    virtual ~GenericCloudChunkManagerDefaultCreateChunkTraits() {}

protected:
    /**
     * @brief Create a new chunk and return it (you can redefine this method if you want to create a chunk differently)
     * @param globalBeginIndex : begin index of the new cloud
     * @param size : size of the new cloud
     */
    virtual ChunkType* createNewChunk(const size_t& globalBeginIndex,
                                   const size_t& size)
    {
        return new ChunkType(this->getGlMode(),
                             this->getNVertexPerObject(),
                             globalBeginIndex,
                             size);
    }
};

#endif // GENERICCLOUDCHUNKMANAGERDEFAULTCREATECHUNKTRAITS_H
