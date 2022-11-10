#include "pointcloudchunkmanagerbycoordinate.h"

#include "scene/tools/dispatchinformation.h"
#include "tools/pointcloudattributesprovider.h"
#include "scene/permanentscene.h"

PointCloudChunkManagerByCoordinate::PointCloudChunkManagerByCoordinate(GLenum glMode,
                                                                       const quint8 &nVertexPerObject,
                                                                       const size_t &beginGlobalIndex,
                                                                       const size_t &cloudSize) : PointCloudChunkManagerByCoordinateSuperClass(glMode,
                                                                                                             nVertexPerObject,
                                                                                                             beginGlobalIndex,
                                                                                                             cloudSize)
{
    m_lastDispatchInfos = nullptr;
    m_multiplePointsActivated = false;
    m_pointCloudProvider = nullptr;
}

void PointCloudChunkManagerByCoordinate::beginAddMultiplePoints()
{
    m_lastDispatchInfos = nullptr;
    m_multiplePointsActivated = true;
}

void PointCloudChunkManagerByCoordinate::endAddMultiplePoints()
{
    m_multiplePointsActivated = false;
}

void PointCloudChunkManagerByCoordinate::addPoint(const size_t &globalIndex,
                                                  PermanentItemInformation* dispatchInfos)
{
    addPoint(globalIndex - getBeginningOfCloud(),
             globalIndex,
             dispatchInfos);
}

void PointCloudChunkManagerByCoordinate::addPoint(const size_t &localIndex,
                                                  const size_t &globalIndex,
                                                  PermanentItemInformation* dispatchInfos)
{
    GLuint csI = GLuint(getChunkUniqueIndexForObjectAt(globalIndex));

    const ChunkType* currentChunk = m_lastChunkUsed;

    ChunkType& chunk = createOrGetChunkFromUniqueIndex(csI);

    if(dispatchInfos != nullptr)
    {
        if(m_multiplePointsActivated
                && (currentChunk == m_lastChunkUsed)
                && (m_lastDispatchInfos != nullptr)) {
            m_lastDispatchInfos->addObjects(1);
        } else {
            m_lastDispatchInfos = &dispatchInfos->createOrGetObjectForChunk(m_lastChunkUsed);

            if(m_lastDispatchInfos->begin() == DispatchInformation::INVALID_BEGIN)
                m_lastDispatchInfos->setBegin(m_lastChunkUsed->countPoints());

            m_lastDispatchInfos->addObjects(1);
        }
    }

    chunk.addPoint(localIndex);
}

PointCloudChunkManagerByCoordinate::PointCloudChunk* PointCloudChunkManagerByCoordinate::createNewChunk(const size_t &globalBeginIndex,
                                                                                                        const size_t &size)
{
    m_lastDispatchInfos = nullptr;

    Eigen::Vector3d offset;
    uint uniqueKey;

    getCurrentCoordinateSystemInformations(offset, uniqueKey);

    return new PointCloudChunk(uniqueKey,
                               globalBeginIndex,
                               size,
                               offset);
}

void PointCloudChunkManagerByCoordinate::initChunk(PointCloudChunk *chunk)
{
    chunk->setContextAccessor(getContextAccessor());
    chunk->setAttributesAccessor(getAttributesAccessor());
    chunk->setDrawModeToUse(getDrawModeToUse());

    chunk->init();
}

void PointCloudChunkManagerByCoordinate::mustUpdateInfoBO(GenericChunkManager::RendererContext *currentContext)
{
    Q_ASSERT(m_pointCloudProvider != nullptr);
    if (m_pointCloudProvider == nullptr) {qDebug() << "PointCloudChunkManagerByCoordinate::mustUpdateInfoBO" << ", " <<  "m_pointCloudProvider == nullptr"; return;}

    ElementInfo* info = &(*m_pointCloudProvider->createOrGetInfoCloud())[getBeginningOfCloud()];

    BufferObjectManager& buffer = currentContext->getBufferObjectManager();
    buffer.updateInfosBO(info, getCloudSize());
}

void PointCloudChunkManagerByCoordinate::mustUpdateColorBO(GenericChunkManager::RendererContext *currentContext)
{
    Q_ASSERT(m_pointCloudProvider != nullptr);
    if (m_pointCloudProvider == nullptr) {qDebug() << "PointCloudChunkManagerByCoordinate::mustUpdateColorBO" << ", " <<  "m_pointCloudProvider == nullptr"; return;}

    Basic::LocalColor* color = nullptr;

    if(m_pointCloudProvider->getColorCloud() != nullptr)
        color = &(*m_pointCloudProvider->getColorCloud())[getBeginningOfCloud()];

    BufferObjectManager& buffer = currentContext->getBufferObjectManager();
    currentContext->getShaders().bindPointShader();
    currentContext->getShaders().setUseColorAttribute(buffer.updateColorsBO(color, getCloudSize()));
    currentContext->getShaders().releasePointsShader();
}

void PointCloudChunkManagerByCoordinate::mustUpdateNormalBO(GenericChunkManager::RendererContext *currentContext)
{
    Q_ASSERT(m_pointCloudProvider != nullptr);
    if (m_pointCloudProvider == nullptr) {qDebug() << "PointCloudChunkManagerByCoordinate::mustUpdateNormalBO" << ", " <<  "m_pointCloudProvider == nullptr"; return;}

    Basic::LocalNormal* normal = nullptr;

    if(m_pointCloudProvider->getNormalCloud() != nullptr)
        normal = &(*m_pointCloudProvider->getNormalCloud())[getBeginningOfCloud()];

    BufferObjectManager& buffer = currentContext->getBufferObjectManager();
    buffer.updateNormalsBO(normal, getCloudSize());
}

void PointCloudChunkManagerByCoordinate::attributesAccessorChanged(const GenericChunkManager::AttributesAccessor *aa)
{
    m_pointCloudProvider = nullptr;

    if(aa != nullptr)
        m_pointCloudProvider = aa->getPermanentScene()->getPointCloudAttributesProvider();
}
