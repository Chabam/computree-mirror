#ifndef GENERICCHUNKMANAGERDEFAULTINITCHUNKTRAITS_H
#define GENERICCHUNKMANAGERDEFAULTINITCHUNKTRAITS_H

#include <QtOpenGL>

/**
 * @brief Default method "initChunk"
 */
template<typename SuperClassT>
class GenericChunkManagerDefaultInitChunkTraits : public SuperClassT
{
public:
    typedef typename SuperClassT::ChunkType ChunkType;

    GenericChunkManagerDefaultInitChunkTraits(GLenum glMode,
                                              const quint8& nVertexPerObject,
                                              const size_t& beginGlobalIndex,
                                              const size_t& cloudSize) : SuperClassT(glMode,
                                                                                    nVertexPerObject,
                                                                                    beginGlobalIndex,
                                                                                    cloudSize) {}

    virtual ~GenericChunkManagerDefaultInitChunkTraits() {}

protected:

    /**
     * @brief Init the chunk (you can redefine this method if you want to init it differently)
     */
    virtual void initChunk(ChunkType* chunk)
    {
        chunk->setContextAccessor(this->getContextAccessor());
        chunk->setAttributesAccessor(this->getAttributesAccessor());
        chunk->setDrawModeToUse(this->getDrawModeToUse());

        chunk->init();
    }
};

#endif // GENERICCHUNKMANAGERDEFAULTINITCHUNKTRAITS_H
