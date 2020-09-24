#ifndef INSTANCEDPERMANENTRENDERER_HPP
#define INSTANCEDPERMANENTRENDERER_HPP

#include "instancedpermanentrenderer.h"

template<typename Chunk>
InstancedPermanentRenderer<Chunk>::InstancedPermanentRenderer() : InstancedPermanentRenderer<Chunk>(Chunk::ShaderParamsType::staticGetGLMode())
{
}

template<typename Chunk>
InstancedPermanentRenderer<Chunk>::InstancedPermanentRenderer(GLenum glMode) : PermanentRenderer< InstancedShapeRendererContext >()
{
    m_glMode = glMode;
    m_drawMode = DM_VAO;

    m_mutex = new QMutex(QMutex::Recursive);
    m_vertexArray = nullptr;
    m_fastVertexArray = nullptr;
    m_normalIndicesArray = nullptr;
    m_fastIndicesArray = nullptr;

    m_lastCsKeyUsed = std::numeric_limits<uint>::max();
}

template<typename Chunk>
InstancedPermanentRenderer<Chunk>::~InstancedPermanentRenderer()
{
    clearShapes();
    delete m_mutex;
}

template<typename Chunk>
void InstancedPermanentRenderer<Chunk>::setVertexArray(const std::vector<Eigen::Vector3f> *vertexArray)
{
    m_vertexArray = (std::vector<Eigen::Vector3f>*)vertexArray;
}

template<typename Chunk>
void InstancedPermanentRenderer<Chunk>::setFastVertexArray(const std::vector<Eigen::Vector3f> *vertexArray)
{
    m_fastVertexArray = (std::vector<Eigen::Vector3f>*)vertexArray;
}

template<typename Chunk>
void InstancedPermanentRenderer<Chunk>::setCustomVertexAttribute(const QString& name,
                                                                 GLint tupleSize,
                                                                 GLenum type,
                                                                 GLboolean normalized,
                                                                 GLsizei stride,
                                                                 size_t sizeOfOneValueInByte,
                                                                 size_t arraySize,
                                                                 GLvoid* pointer)
{

    VertexAttribute attribute;
    attribute.name = name.toStdString();
    attribute.tupleSize = tupleSize;
    attribute.type = type;
    attribute.normalized = normalized;
    attribute.stride = stride;
    attribute.sizeOfOneValueInByte = sizeOfOneValueInByte;
    attribute.arraySize = arraySize;
    attribute.pointer = pointer;

    m_customVertexAttributeCollection.insert(name, attribute);
}

template<typename Chunk>
void InstancedPermanentRenderer<Chunk>::setNormalIndicesArray(const std::vector<GLuint> *normalIndicesArray)
{
    m_normalIndicesArray = (std::vector<GLuint>*)normalIndicesArray;
}

template<typename Chunk>
void InstancedPermanentRenderer<Chunk>::setFastIndicesArray(const std::vector<GLuint> *fastIndicesArray)
{
    m_fastIndicesArray = (std::vector<GLuint>*)fastIndicesArray;
}

template<typename Chunk>
void InstancedPermanentRenderer<Chunk>::updateInAllContext()
{
    QMutexLocker locker(m_mutex);

    typename IContextAccessor< typename Chunk::RendererContextType >::ContextCollectionIterator it(this->getContexts());

    while(it.hasNext()) {
        it.next();

        InstancedShapeRendererContext *c = it.value();

        c->makeCurrent();
        c->getShaders().init(c->getOpenglContext());

        ChunkCollectionQtIterator itC(m_chunks);

        while(itC.hasNext()) {
            itC.next();
            itC.value()->setCurrentContext(c);
            itC.value()->update();
            itC.value()->setCurrentContext(nullptr);
        }
    }
}

template<typename Chunk>
void InstancedPermanentRenderer<Chunk>::updateInfoInAllContext()
{
    QMutexLocker locker(m_mutex);

    typename IContextAccessor< typename Chunk::RendererContextType >::ContextCollectionIterator it(this->getContexts());

    while(it.hasNext()) {
        it.next();

        InstancedShapeRendererContext *c = it.value();

        c->makeCurrent();
        c->getShaders().init(c->getOpenglContext());

        ChunkCollectionQtIterator itC(m_chunks);

        while(itC.hasNext()) {
            itC.next();
            itC.value()->setCurrentContext(c);
            itC.value()->updateInfoBO();
            itC.value()->setCurrentContext(nullptr);
        }
    }
}

template<typename Chunk>
void InstancedPermanentRenderer<Chunk>::updateColorInAllContext()
{
    QMutexLocker locker(m_mutex);

    typename IContextAccessor< typename Chunk::RendererContextType >::ContextCollectionIterator it(this->getContexts());

    while(it.hasNext()) {
        it.next();

        InstancedShapeRendererContext *c = it.value();

        c->makeCurrent();
        c->getShaders().init(c->getOpenglContext());

        ChunkCollectionQtIterator itC(m_chunks);

        while(itC.hasNext()) {
            itC.next();
            itC.value()->setCurrentContext(c);
            itC.value()->updateColorBO();
            itC.value()->setCurrentContext(nullptr);
        }
    }
}

template<typename Chunk>
void InstancedPermanentRenderer<Chunk>::updateNormalInAllContext()
{
    // Instanced elements don't have normals !
}

template<typename Chunk>
void InstancedPermanentRenderer<Chunk>::updateOnlyVAOInAllContext()
{
    QMutexLocker locker(m_mutex);

    typename IContextAccessor< typename Chunk::RendererContextType >::ContextCollectionIterator it(this->getContexts());

    while(it.hasNext()) {
        it.next();

        InstancedShapeRendererContext *c = it.value();

        c->makeCurrent();
        c->getShaders().init(c->getOpenglContext());

        ChunkCollectionQtIterator itC(m_chunks);

        while(itC.hasNext()) {
            itC.next();
            itC.value()->setCurrentContext(c);
            itC.value()->updateOnlyVAO();
            itC.value()->setCurrentContext(nullptr);
        }
    }
}

template<typename Chunk>
void InstancedPermanentRenderer<Chunk>::setDrawModeToUse(DrawMode mode)
{
    QMutexLocker locker(m_mutex);

    if(m_drawMode != mode) {
        m_drawMode = mode;

        ChunkCollectionQtIterator itC(m_chunks);

        while(itC.hasNext()) {
            itC.next();
            itC.value()->setDrawModeToUse(mode);
        }
    }
}

template<typename Chunk>
void InstancedPermanentRenderer<Chunk>::destroyGL(const QOpenGLContext *context)
{
    QMutexLocker locker(m_mutex);

    InstancedShapeRendererContext *c = this->getContexts().value(const_cast<QOpenGLContext*>(context), nullptr);

    if(c != nullptr)
        c->destroyGL();
}

template<typename Chunk>
const Chunk& InstancedPermanentRenderer<Chunk>::createOrGetChunkForShape(const Eigen::AlignedBox3d &bbox,
                                                                                                                                                                           uint *chunkUniqueKey)
{
    QMutexLocker locker(m_mutex);

    // TODO : use other information than just the center of the bbox because if the bbox overlap two or more coordinate system it will be a problem
    Eigen::Vector3d center = bbox.center();
    Eigen::Vector3d offset;
    uint uniqueKey;

    AMKglEA->getCoordinateSystemManager()->getCoordinateSystemInfosForCoordinates(center[0],
            center[1],
            center[2],
            uniqueKey,
            offset);

    if(chunkUniqueKey != nullptr)
        *chunkUniqueKey = uniqueKey;

    if(uniqueKey == m_lastCsKeyUsed) {
        offset = m_lastOffsetUsed;
    } else {
        m_lastCsKeyUsed = uniqueKey;
        m_lastOffsetUsed = offset;
    }

    Chunk *ch = m_chunks.value(uniqueKey, nullptr);

    if(ch == nullptr) {
        Q_ASSERT(m_vertexArray != nullptr);

        ch = new Chunk(uniqueKey,
                       this->getTypeOfObjectDrawn(),
                       m_glMode,
                       *m_vertexArray,
                       offset,
                       &m_customVertexAttributeCollection,
                       m_fastVertexArray,
                       m_normalIndicesArray,
                       m_fastIndicesArray);
        ch->setContextAccessor(this);
        ch->setAttributesAccessor(this->getAttributesAccessor());
        ch->setCurrentColor(this->getCurrentColor());
        ch->setDrawModeToUse(m_drawMode);

        m_chunks.insert(uniqueKey, ch);
    }

    return *ch;
}

template<typename Chunk>
void InstancedPermanentRenderer<Chunk>::addShape(const Chunk& chunk,
                                                                             const ShaderParams &params,
                                                                             const ElementInfo &info)
{
    QMutexLocker locker(m_mutex);

    Chunk& ch = const_cast<Chunk&>(chunk);

    ch.addShape(params, info);
}

template<typename Chunk>
void InstancedPermanentRenderer<Chunk>::removeShapes(const uint& chunkUniqueKey, const size_t &index, const size_t &count)
{
    QMutexLocker locker(m_mutex);

    Chunk* c = m_chunks.value(chunkUniqueKey, nullptr);

    if(c == nullptr)
       return;

    c->removeShapes(index, count);
}
template<typename Chunk>
template<typename iterator>
void InstancedPermanentRenderer<Chunk>::removeShapes(const uint& chunkUniqueKey, const iterator& begin, const iterator& end)
{
    QMutexLocker locker(m_mutex);

    if(begin == end)
       return;

    Chunk* c = m_chunks.value(chunkUniqueKey, nullptr);

    if(c == nullptr)
       return;

    c->removeShapes(begin, end);
}

template<typename Chunk>
void InstancedPermanentRenderer<Chunk>::setShapesVisible(const uint& chunkUniqueKey, const size_t &index, const size_t &count, bool visible)
{
    QMutexLocker locker(m_mutex);

    Chunk* c = m_chunks.value(chunkUniqueKey, nullptr);

    if(c == nullptr)
        return;

    c->setShapesVisible(index, count, visible);
}

template<typename Chunk>
void InstancedPermanentRenderer<Chunk>::clearShapes()
{
    QMutexLocker locker(m_mutex);

    qDeleteAll(m_chunks.begin(), m_chunks.end());
    m_chunks.clear();
}

template<typename Chunk>
size_t InstancedPermanentRenderer<Chunk>::countShapes() const
{
    QMutexLocker locker(m_mutex);

    size_t count = 0;

    ChunkCollectionQtIterator it(m_chunks);

    while(it.hasNext())
        count += it.next().value()->countShape();

    return count;
}

template<typename Chunk>
size_t InstancedPermanentRenderer<Chunk>::countShapesInChunk(const uint& chunkUniqueKey) const
{
    QMutexLocker locker(m_mutex);

    Chunk* c = m_chunks.value(chunkUniqueKey, nullptr);

    if(c == nullptr)
        return 0;

    return c->countShape();
}

template<typename Chunk>
void InstancedPermanentRenderer<Chunk>::visitObjects(IObjectsVisitor &visitor)
{
    QMutexLocker locker(m_mutex);

    ChunkCollectionQtIterator it(m_chunks);

    while(it.hasNext() && visitor.mustContinueVisit())
        visitor.visitChunk(it.next().value());
}

template<typename Chunk>
void InstancedPermanentRenderer<Chunk>::draw(DrawInfo &info)
{
    QMutexLocker locker(m_mutex);

    InstancedShapeRendererContext* c = this->getCurrentContext();

    ChunkCollectionQtIterator itC(m_chunks);

    while(itC.hasNext()) {
        itC.next();
        itC.value()->setCurrentContext(c);
        itC.value()->draw(info);
        itC.value()->setCurrentContext(nullptr);
    }
}

template<typename Chunk>
void InstancedPermanentRenderer<Chunk>::fastDraw(DrawInfo &info)
{
    QMutexLocker locker(m_mutex);

    InstancedShapeRendererContext *c = this->getCurrentContext();

    ChunkCollectionQtIterator itC(m_chunks);

    while(itC.hasNext()) {
        itC.next();
        itC.value()->setCurrentContext(c);
        itC.value()->fastDraw(info);
    }
}

template<typename Chunk>
void InstancedPermanentRenderer<Chunk>::postDraw(DrawInfo &info)
{
    QMutexLocker locker(m_mutex);

    InstancedShapeRendererContext *c = this->getCurrentContext();

    ChunkCollectionQtIterator itC(m_chunks);

    while(itC.hasNext()) {
        itC.next();
        itC.value()->setCurrentContext(c);
        itC.value()->postDraw(info);
        itC.value()->setCurrentContext(nullptr);
    }
}

template<typename Chunk>
void InstancedPermanentRenderer<Chunk>::currentColorChanged()
{
    QMutexLocker locker(m_mutex);

    ChunkCollectionQtIterator it(m_chunks);

    while(it.hasNext()) {
        it.next().value()->setCurrentColor(this->getCurrentColor());
    }
}

template<typename Chunk>
typename InstancedPermanentRenderer<Chunk>::ChunkCollection& InstancedPermanentRenderer<Chunk>::getChunks()
{
    return m_chunks;
}

#endif // INSTANCEDPERMANENTRENDERER_HPP
