#include "pointcloudchunkmanagerbymemorylimit.h"

PointCloudChunkManagerByMemoryLimit::PointCloudChunkManagerByMemoryLimit(GLenum glMode,
                                                                         const quint8& nVertexPerObject,
                                                                         const size_t& beginGlobalIndex,
                                                                         const size_t& cloudSize) : PointCloudChunkManagerByMemoryLimitSuperClass(glMode,
                                                                                                                                                  nVertexPerObject,
                                                                                                                                                  beginGlobalIndex,
                                                                                                                                                  cloudSize)
{
    m_multipleAddActivated = false;
}

void PointCloudChunkManagerByMemoryLimit::beginAddMultiplePoints()
{
    m_multipleAddActivated = true;
}

void PointCloudChunkManagerByMemoryLimit::endAddMultiplePoints()
{
    if(m_multipleAddActivated && (m_lastChunkUsed != nullptr))
        m_lastChunkUsed->endAddMultiplePoints();

    m_multipleAddActivated = false;
}

void PointCloudChunkManagerByMemoryLimit::addPoint(const size_t &globalIndex,
                                                   PermanentItemInformation *dispatchInfos)
{
    if((m_lastChunkUsed != nullptr)
            && (globalIndex >= m_lastChunkUsed->getBeginningOfCloud())
            && (globalIndex <= m_lastChunkUsed->getEndOfCloud())) {
        m_lastChunkUsed->addPoint(globalIndex - m_lastChunkUsed->getBeginningOfCloud(), globalIndex, dispatchInfos);
    } else {
        if(m_multipleAddActivated && (m_lastChunkUsed != nullptr))
            m_lastChunkUsed->endAddMultiplePoints();

        size_t localIndex = globalIndex - getBeginningOfCloud();
        const size_t chunkIndex = localIndex/getChunkMaxSize();

        if(chunkIndex != 0)
            localIndex = localIndex - (chunkIndex*getChunkMaxSize());

        m_lastChunkUsed = m_chunks[chunkIndex];

        if(m_multipleAddActivated)
            m_lastChunkUsed->beginAddMultiplePoints();

        m_lastChunkUsed->addPoint(localIndex, globalIndex, dispatchInfos);
    }
}

void PointCloudChunkManagerByMemoryLimit::addPoint(const size_t &globalIndex,
                                                   const size_t &localIndex,
                                                   const size_t &chunkIndex,
                                                   PermanentItemInformation *dispatchInfos)
{
    m_chunks[chunkIndex]->addPoint(localIndex, globalIndex, dispatchInfos);
}

void PointCloudChunkManagerByMemoryLimit::addPointsFrom(size_t globalIndexBegin,
                                                        size_t count,
                                                        PermanentItemInformation* dispatchInfos)
{
    if(count == 0)
        return;

    size_t localIndex = globalIndexBegin - getBeginningOfCloud();
    size_t chunkIndex = localIndex/getChunkMaxSize();

    localIndex = localIndex - (chunkIndex*getChunkMaxSize());

    do
    {
        ChunkType *r = m_chunks[chunkIndex];
        r->beginAddMultiplePoints();

        size_t end = r->getEndOfCloud()+1;

        while((count != 0) && (globalIndexBegin != end)) {
            r->addPoint(localIndex++, globalIndexBegin++, dispatchInfos);
            --count;
        }

        r->endAddMultiplePoints();

        ++chunkIndex;
        localIndex = 0;

    }while(count != 0);
}
