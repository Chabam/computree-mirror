#include "genericpermanentrenderer.h"

#include "amkglapp.h"
#include "interfaces/icoordinatesystemmanager.h"
#include "interfaces/igraphicsdocument.h"

#include "tools/anyelementattributesprovider.h"
#include "visitor/iobjectsvisitor.h"

GenericPermanentRenderer::GenericPermanentRenderer(GLenum glMode) : PermanentRenderer< GenericRendererContext >()
{
    m_mutex = new QMutex(QMutex::Recursive);
    m_lastChunkUsed = NULL;
    m_lastCsKeyUsed = std::numeric_limits<uint>::max();
    m_glMode = glMode;
    m_attributesAccessor = NULL;
    m_drawMode = DM_VAO;
    m_coordinateSystemManager = AMKglEA->getCoordinateSystemManager();
}

GenericPermanentRenderer::~GenericPermanentRenderer()
{
    clearVertex();
    delete m_mutex;
}

void GenericPermanentRenderer::updateInAllContext()
{
    QMutexLocker locker(m_mutex);

    SuperClass::ContextCollectionIterator it(getContexts());

    while(it.hasNext()) {
        it.next();

        GenericRendererContext *c = it.value();

        c->makeCurrent();
        c->getShaders().init(c->getOpenglContext());

        ChunkCollectionQtIterator itC(m_chunks);

        while(itC.hasNext()) {
            itC.next();
            itC.value()->setCurrentContext(c);
            itC.value()->update();
            itC.value()->setCurrentContext(NULL);
        }
    }
}

void GenericPermanentRenderer::updateInfoInAllContext()
{    
    QMutexLocker locker(m_mutex);

    SuperClass::ContextCollectionIterator it(getContexts());

    while(it.hasNext()) {
        it.next();

        GenericRendererContext *c = it.value();

        c->makeCurrent();
        c->getShaders().init(c->getOpenglContext());

        ChunkCollectionQtIterator itC(m_chunks);

        while(itC.hasNext()) {
            itC.next();
            itC.value()->setCurrentContext(c);
            itC.value()->updateInfoBO();
            itC.value()->setCurrentContext(NULL);
        }
    }
}

void GenericPermanentRenderer::updateColorInAllContext()
{
    QMutexLocker locker(m_mutex);

    SuperClass::ContextCollectionIterator it(getContexts());

    while(it.hasNext()) {
        it.next();

        GenericRendererContext *c = it.value();

        c->makeCurrent();
        c->getShaders().init(c->getOpenglContext());

        ChunkCollectionQtIterator itC(m_chunks);

        while(itC.hasNext()) {
            itC.next();
            itC.value()->setCurrentContext(c);
            itC.value()->updateColorBO();
            itC.value()->setCurrentContext(NULL);
        }
    }
}

void GenericPermanentRenderer::updateNormalInAllContext()
{
    QMutexLocker locker(m_mutex);

    SuperClass::ContextCollectionIterator it(getContexts());

    while(it.hasNext()) {
        it.next();

        GenericRendererContext *c = it.value();

        c->makeCurrent();
        c->getShaders().init(c->getOpenglContext());

        ChunkCollectionQtIterator itC(m_chunks);

        while(itC.hasNext()) {
            itC.next();
            itC.value()->setCurrentContext(c);
            itC.value()->updateNormalBO();
            itC.value()->setCurrentContext(NULL);
        }
    }
}

void GenericPermanentRenderer::updateOnlyVAOInAllContext()
{
    QMutexLocker locker(m_mutex);

    SuperClass::ContextCollectionIterator it(getContexts());

    while(it.hasNext()) {
        it.next();

        GenericRendererContext *c = it.value();

        c->makeCurrent();
        c->getShaders().init(c->getOpenglContext());

        ChunkCollectionQtIterator itC(m_chunks);

        while(itC.hasNext()) {
            itC.next();
            itC.value()->setCurrentContext(c);
            itC.value()->updateOnlyVAO();
            itC.value()->setCurrentContext(NULL);
        }
    }
}

void GenericPermanentRenderer::setDrawModeToUse(DrawMode mode)
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

void GenericPermanentRenderer::destroyGL(const QOpenGLContext *context)
{
    QMutexLocker locker(m_mutex);

    GenericRendererContext *c = getContexts().value(const_cast<QOpenGLContext*>(context), NULL);

    if(c != NULL)
        c->destroyGL();
}

const GenericPermanentRenderer::Chunk& GenericPermanentRenderer::createOrGetChunkForObject(const Eigen::Vector3d &p,
                                                                                           uint* chunkUniqueKey)
{
    QMutexLocker locker(m_mutex);

    // TODO : use other information than just the center of the bbox because if the bbox overlap two or more coordinate system it will be a problem
    Eigen::Vector3d offset;
    uint uniqueKey;

    m_coordinateSystemManager->getCoordinateSystemInfosForCoordinates(p[0],
            p[1],
            p[2],
            uniqueKey,
            offset);

    if(chunkUniqueKey != NULL)
        *chunkUniqueKey = uniqueKey;

    if(m_lastCsKeyUsed == uniqueKey)
        return *m_lastChunkUsed;

    Chunk* ch = m_chunks.value(uniqueKey, NULL);

    if(ch == NULL) {
        ch = new Chunk(uniqueKey,
                       getTypeOfObjectDrawn(),
                       m_glMode,
                       getNumberOfJumpForOneObject(),
                       offset);
        ch->setContextAccessor(this);
        ch->setAttributesAccessor(getAttributesAccessor());
        ch->setCurrentColor(getCurrentColor());
        ch->setDrawModeToUse(m_drawMode);

        m_chunks.insert(uniqueKey, ch);
    }

    m_lastCsKeyUsed = uniqueKey;
    m_lastChunkUsed = ch;

    return *ch;
}

void GenericPermanentRenderer::addObjectVertex(const Chunk& chunk,
                                               const Eigen::Vector3d &p,
                                               const ElementInfo &info,
                                               const Color &color)
{
    QMutexLocker locker(m_mutex);

    Chunk& ch = const_cast<Chunk&>(chunk);

    Eigen::Vector3d newP = p - chunk.getOffset();
    ch.addPoint(newP.cast<float>(), info, color);
}

void GenericPermanentRenderer::clearVertex()
{
    QMutexLocker locker(m_mutex);

    qDeleteAll(m_chunks.begin(), m_chunks.end());
    m_chunks.clear();

    m_lastCsKeyUsed = std::numeric_limits<uint>::max();
    m_lastChunkUsed = NULL;
}

size_t GenericPermanentRenderer::countVertex() const
{
    QMutexLocker locker(m_mutex);

    size_t count = 0;

    ChunkCollectionQtIterator it(m_chunks);

    while(it.hasNext())
        count += it.next().value()->countPoints();

    return count;
}

size_t GenericPermanentRenderer::countObjects() const
{
    return countVertex()/getNumberOfJumpForOneObject();
}

void GenericPermanentRenderer::clearObjects()
{
    clearVertex();
}

void GenericPermanentRenderer::visitObjects(IObjectsVisitor &visitor)
{
    QMutexLocker locker(m_mutex);

    ChunkCollectionQtIterator it(m_chunks);

    while(it.hasNext() && visitor.mustContinueVisit())
        visitor.visitChunk(it.next().value());
}

void GenericPermanentRenderer::draw(DrawInfo &info)
{
    QMutexLocker locker(m_mutex);

    GenericRendererContext *c = getCurrentContext();

    ChunkCollectionQtIterator itC(m_chunks);

    while(itC.hasNext()) {
        itC.next();
        itC.value()->setCurrentContext(c);
        itC.value()->draw(info);
        itC.value()->setCurrentContext(NULL);
    }
}

void GenericPermanentRenderer::fastDraw(DrawInfo &info)
{
    QMutexLocker locker(m_mutex);

    GenericRendererContext *c = getCurrentContext();

    ChunkCollectionQtIterator itC(m_chunks);

    while(itC.hasNext()) {
        itC.next();
        itC.value()->setCurrentContext(c);
        itC.value()->fastDraw(info);
        itC.value()->setCurrentContext(NULL);
    }
}

void GenericPermanentRenderer::postDraw(DrawInfo &info)
{
    QMutexLocker locker(m_mutex);

    GenericRendererContext *c = getCurrentContext();

    ChunkCollectionQtIterator itC(m_chunks);

    while(itC.hasNext()) {
        itC.next();
        itC.value()->setCurrentContext(c);
        itC.value()->postDraw(info);
        itC.value()->setCurrentContext(NULL);
    }
}

void GenericPermanentRenderer::currentColorChanged()
{
    QMutexLocker locker(m_mutex);

    ChunkCollectionQtIterator it(m_chunks);

    while(it.hasNext()) {
        it.next().value()->setCurrentColor(getCurrentColor());
    }
}
