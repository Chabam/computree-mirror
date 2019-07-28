#include "pointcloudrenderer.h"

#include "visitor/applycustomfunctiontoobjectvisitor.h"
#include "visitor/iglobalpointsvisitor.h"

#include "tools/pointcloudattributesprovider.h"
#include "includecloudindex.h"

PointCloudRenderer::PointCloudRenderer() : PointCloudRendererNM::ByVertexCloudManager(GL_POINTS, 1, 0, 0)
{
    m_maxNumberOfPointsInFastDrawMode = 200000;
    m_attributesCloudProvider = new PointCloudAttributesProvider();

    setContextAccessor(this);
}

PointCloudRenderer::~PointCloudRenderer()
{
    delete m_attributesCloudProvider;
}

Scene::ObjectType PointCloudRenderer::getTypeOfObjectDrawn() const
{
    return Scene::PointGlobalCloud;
}

void PointCloudRenderer::addPoints(const CloudIndex *indexes,
                                   PermanentItemInformation *dispatchInfos)
{
    if(indexes == nullptr)
        return;

    QMutexLocker locker(m_mutex);

    size_t size = indexes->size();

    if(size > 0) {
        if(indexes->doIndexesFollowEachOther()) {

            size_t begin = indexes->first();
            size_t i = 0;

            ChunkType* r = getChunkForObjectAt(begin, &i);

            const size_t n = m_chunks.size();

            do
            {
                size_t count = qMin(size, (r->getEndOfCloud() - begin)+1);

                r->addPointsFrom(begin, count, dispatchInfos);
                begin += count;
                size -= count;

                if(i < n) {
                    r = m_chunks.at(i);
                    ++i;
                }

            }while(size != 0);

        } else {
            ChunkType* r = getChunkForObjectAt((*indexes)[--size]);

            r->beginAddMultiplePoints();

            size_t begin = r->getBeginningOfCloud();
            size_t end = r->getEndOfCloud();

            while(size != 0) {
                const size_t &index = (*indexes)[--size];

                if((index < begin) || (index > end)) {
                    r->endAddMultiplePoints();

                    r = getChunkForObjectAt(index);
                    r->beginAddMultiplePoints();

                    begin = r->getBeginningOfCloud();
                    end = r->getEndOfCloud();
                }

                r->addPoint(index, dispatchInfos);
            }

            r->endAddMultiplePoints();
        }
    }
}

void PointCloudRenderer::addPoint(const size_t &globalIndex,
                                  PermanentItemInformation *dispatchInfos)
{
    ChunkType* r = getChunkForObjectAt(globalIndex);

    r->addPoint(globalIndex, dispatchInfos);
}

void PointCloudRenderer::visitPoints(IGlobalPointsVisitor &visitor)
{
    ApplyCustomFunctionToObjectVisitor objectVisitor;

    objectVisitor.setContinueFunction([&visitor]() -> bool {
        return visitor.mustContinueVisit();
    });

    objectVisitor.setVisitFunction([&visitor](const IChunk* chunk) {
        static_cast<PointCloudChunk*>((IChunk*)chunk)->visitPoints(visitor);
    });

    visitObjects(objectVisitor);
}

size_t PointCloudRenderer::countPoints() const
{
    QMutexLocker locker(m_mutex);

    size_t total = 0;

    ApplyCustomFunctionToObjectVisitor visitorCount;
    visitorCount.setVisitFunction([&total](const IChunk* chunk) {
        const size_t n = static_cast<const PointCloudChunk*>(chunk)->countPoints();
        total += n;
    });

    return total;
}

void PointCloudRenderer::setMaxNumberOfPointsToDrawInFastDraw(const size_t &maxNumber)
{
    QMutexLocker locker(m_mutex);

    m_maxNumberOfPointsInFastDrawMode = qMax((size_t)1, maxNumber);

    size_t total = countPoints();

    ApplyCustomFunctionToObjectVisitor visitorSet;
    visitorSet.setVisitFunction([&total, maxNumber](const IChunk* chunk) {
        PointCloudChunk* pcChunk = static_cast<PointCloudChunk*>((IChunk*)chunk);
        const size_t n = pcChunk->countPoints();

        double ratio = 1;

        if(total > 0)
            ratio = double(n)/double(total);

        pcChunk->setMaxNumberOfPointsToDrawInFastDraw(qMax(double(maxNumber)*ratio, 1.0));
    });

    visitObjects(visitorSet);
}

size_t PointCloudRenderer::getMaxNumberOfPointsToDrawInFastDraw() const
{
    return m_maxNumberOfPointsInFastDrawMode;
}

int PointCloudRenderer::getNumberOfJumpForOneObject() const
{
    return 1;
}

PointCloudAttributesProvider* PointCloudRenderer::getPointCloudAttributesProvider() const
{
    return m_attributesCloudProvider;
}
