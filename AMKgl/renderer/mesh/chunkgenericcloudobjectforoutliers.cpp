#include "chunkgenericcloudobjectforoutliers.h"

#include "amkglapp.h"
#include "scene/permanentscene.h"
#include "defineglobalcloud.h"
#include "tools/opengl/genericglrenderer.h"

#define RESIZE_VECTOR_ADD 10

ChunkGenericCloudObjectForOutliers::ChunkGenericCloudObjectForOutliers(Scene::ObjectType objectType,
                                                                       GLenum glMode,
                                                                       const quint8& nVertexPerObject,
                                                                       const IGenericCloudManager& vertexCloudManager,
                                                                       const IGenericCloudManager& objectCloudManager) : SuperClass(0,
                                                                                                                                    objectType,
                                                                                                                                    glMode,
                                                                                                                                    nVertexPerObject,
                                                                                                                                    Eigen::Vector3d(0,0,0))
{
    static_assert(sizeof(GlobalGlIndexCloud::IndexType) == sizeof(GLuint), "Index used in this chunk must be of type GLuint !");

    m_vertexIndexes = vertexCloudManager.createNewGlIndexCloudSyncedAndRegisterIt(m_vertexIndexesCloud);
    m_objectIndexes = objectCloudManager.createNewGlIndexCloudSyncedAndRegisterIt(m_objectIndexesCloud);

    m_globalPointCloudManager = AMKglEA->getGlobalPointCloudManager();
    m_csm = AMKglEA->getCoordinateSystemManager();

    m_pointsTmp.resize(nVertexPerObject);
}

ChunkGenericCloudObjectForOutliers::~ChunkGenericCloudObjectForOutliers()
{
    deleteAllObjectXXXCloud();
}

size_t ChunkGenericCloudObjectForOutliers::getBeginningOfVertexCloud() const
{
    return 0;
}

size_t ChunkGenericCloudObjectForOutliers::getBeginningOfObjectCloud() const
{
    return 0;
}

void ChunkGenericCloudObjectForOutliers::addObjectVertexes(const size_t& globalObjectIndex,
                                                           const size_t* vertexGlobalIndexes,
                                                           IChunk** chunk)
{
    setUpdated(false);

    computeAndAddOffsetAndVertex(vertexGlobalIndexes);

    if(m_objectIndexesCloud->size() == m_nObjects)
        m_objectIndexesCloud->resize(m_nObjects + RESIZE_VECTOR_ADD);

    (*m_objectIndexesCloud)[m_nObjects] = GLuint(globalObjectIndex);

    ++m_nObjects;

    if(chunk != NULL)
        *chunk = this;
}

GLuint* ChunkGenericCloudObjectForOutliers::getFirstObjectIndex() const
{
    if(m_objectIndexesCloud->size() == 0)
        return NULL;

    return (GLuint*)&(*const_cast<ChunkGenericCloudObjectForOutliers*>(this)->m_objectIndexesCloud)[0];
}

GLuint* ChunkGenericCloudObjectForOutliers::getFirstVertexIndex() const
{
    if(m_vertexIndexesCloud->size() == 0)
        return NULL;

    return (GLuint*)&(*const_cast<ChunkGenericCloudObjectForOutliers*>(this)->m_vertexIndexesCloud)[0];
}

const ChunkGenericCloudObjectForOutliers::VertexesAndOffsetCollection& ChunkGenericCloudObjectForOutliers::getVertexesWithOffset() const
{
    return m_vertexesAndOffset;
}

void ChunkGenericCloudObjectForOutliers::computeAndAddOffsetAndVertex(const size_t *vertexGlobalIndexes)
{
    Eigen::Vector3d middle(0,0,0);

    // for each vertex
    for(int i=0; i<m_nVertexPerObject; ++i) {
        const size_t& globalVertexIndex = vertexGlobalIndexes[i];

        // keep it and sum it
        m_globalPointCloudManager->getDoublePointAtGlobalIndex(globalVertexIndex, m_pointsTmp[i]);
        middle += m_pointsTmp[i];

        // add the global index to the collection of index
        addVertex(globalVertexIndex);
    }

    // center of vertexes
    middle /= double(m_nVertexPerObject);

    VertexesByOffset* vertexes = createOrGetVertexesForMiddle(middle);

    addVertexesTo(m_pointsTmp, vertexes);
}

void ChunkGenericCloudObjectForOutliers::addVertex(const size_t& globalIndex)
{
    setUpdated(false);

    if(m_vertexIndexesCloud->size() == m_nPoints)
        m_vertexIndexesCloud->resize(m_nPoints + RESIZE_VECTOR_ADD);

    (*m_vertexIndexesCloud)[m_nPoints] = GLuint(globalIndex);

    ++m_nPoints;
}

bool ChunkGenericCloudObjectForOutliers::internalUpdate()
{
    Context* context = getCurrentContext();

    if(context == NULL)
        return false;

    if(isUpdated())
        return true;

    if(m_nPoints == 0) {
        setNormalDrawModeUsed(DM_NONE);
        setFastDrawModeUsed(DM_NONE);
        setUpdated(true);
        return true;
    }

    AMKglLOG->addInfoMessage(QString("Update generic global cloud chunk for outliers."));

    setNormalDrawModeUsed(getDrawModeToUse() == DM_BASIC ? DM_BASIC : DM_DISPLAY_LIST);
    setFastDrawModeUsed(getNormalDrawModeUsed());
    setUpdated(true);
    return true;
}

void ChunkGenericCloudObjectForOutliers::drawBasic(DrawInfo &info)
{
    Q_UNUSED(info)

    float selectionColor[4];
    getSelectionColor(selectionColor);

    float currentColor[4];
    getCurrentColor(currentColor);

    GenericGLRendererNM::IndicesFromPointer vertexIndexes(getFirstVertexIndex());
    GenericGLRendererNM::IndicesFromPointer objectIndexes(getFirstObjectIndex());

    if(getWhichCloudToUseForObjects() == AMKgl::GCM_Vertex) {

        GenericGLRenderer::staticBasicObjectVertexOutliersRender(m_glMode,
                                                                 m_vertexesAndOffset,
                                                                 createOrGetFirstObjectInfo(),
                                                                 getFirstVertexColor(),
                                                                 getFirstVertexNormal(),
                                                                 *getFlagsPropertyManager(),
                                                                 vertexIndexes,
                                                                 objectIndexes,
                                                                 m_nObjects,
                                                                 selectionColor,
                                                                 currentColor);
    } else {
        GenericGLRenderer::staticBasicObjectOutliersRender(m_glMode,
                                                           m_vertexesAndOffset,
                                                           createOrGetFirstObjectInfo(),
                                                           getFirstObjectColor(),
                                                           getFirstObjectNormal(),
                                                           *getFlagsPropertyManager(),
                                                           objectIndexes,
                                                           m_nObjects,
                                                           selectionColor,
                                                           currentColor);
    }
}

void ChunkGenericCloudObjectForOutliers::drawBasicNormals(DrawInfo &info)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(info)
#endif
    // TODO
    /*Q_UNUSED(info)

    QColor color = getCurrentDocument()->getNormalsColor();

    float selectionColor[4];
    getSelectionColor(selectionColor);

    float currentColor[4];
    getCurrentColor(currentColor);

    float normalColor[4];
    normalColor[0] = color.redF();
    normalColor[1] = color.greenF();
    normalColor[2] = color.blueF();
    normalColor[3] = color.alphaF();

    GenericGLRendererNM::IndicesFromPointer vertexIndexes(getFirstVertexIndex());
    GenericGLRendererNM::IndicesFromPointer objectIndexes(getFirstObjectIndex());

    if(getWhichCloudToUseForObjects() == AMKgl::GCM_Vertex) {

        GenericGLRenderer::staticBasicObjectVertexNormalOutliersRender(getFirstVertex(),
                                                               createOrGetFirstObjectInfo(),
                                                               m_objectCoordinateOffsets,
                                                               getFirstVertexNormal(),
                                                               *getFlagsPropertyManager(),
                                                               vertexIndexes,
                                                               objectIndexes,
                                                               m_nObjects,
                                                               getCurrentDocument()->getNormalsLength(),
                                                               m_nVertexPerObject,
                                                               normalColor,
                                                               selectionColor,
                                                               currentColor);
    } else {
        GenericGLRenderer::staticBasicObjectNormalOutliersRender(getFirstVertex(),
                                                         createOrGetFirstObjectInfo(),
                                                         m_objectCoordinateOffsets,
                                                         getFirstObjectNormal(),
                                                         *getFlagsPropertyManager(),
                                                         vertexIndexes,
                                                         objectIndexes,
                                                         m_nObjects,
                                                         getCurrentDocument()->getNormalsLength(),
                                                         m_nVertexPerObject,
                                                         normalColor,
                                                         selectionColor,
                                                         currentColor);
    }*/
}

void ChunkGenericCloudObjectForOutliers::moveObjectsFromTo(const size_t &from, const size_t &to, const size_t &size)
{
    size_t fromM = from*getNumberOfElementPerObject();
    size_t sizeM = size*getNumberOfElementPerObject();
    size_t toM = to*getNumberOfElementPerObject();

    std::memcpy(&(*m_objectIndexesCloud)[to], &(*m_objectIndexesCloud)[from], sizeof(GLuint)*size);
    std::memcpy(&m_vertexesAndOffset[to], &m_vertexesAndOffset[from], sizeof(VertexesByOffset)*size);
    std::memcpy(&(*m_vertexIndexesCloud)[toM], &(*m_vertexIndexesCloud)[fromM], sizeof(GLuint)*sizeM);

    deleteAllObjectXXXCloud();
}

void ChunkGenericCloudObjectForOutliers::internalSetNumberOfObjects(const size_t &n, const bool &shrinkToFit)
{
    m_nObjects = n;
    m_objectIndexesCloud->resize(m_nObjects);

    m_nObjects = n;
    m_vertexesAndOffset.resize(m_nObjects);

    if(shrinkToFit)
        m_vertexesAndOffset.shrink_to_fit();

    m_nPoints = n*getNumberOfElementPerObject();
    m_vertexIndexesCloud->resize(m_nPoints);

    deleteAllObjectXXXCloud();
}

void ChunkGenericCloudObjectForOutliers::createObjectColorCloudMemberIfNot()
{
    if(m_objectColorCloud == NULL) {
        Q_ASSERT(m_objectCloudProvider != NULL);
        GlobalColorCloud *gcc = m_objectCloudProvider->createOrGetColorCloud();
        Q_ASSERT(gcc != NULL);
        Q_UNUSED(gcc)
        m_objectColorCloud = new ObjectFuncPointerCloudIndexed<GlobalColor, GlobalGlIndexCloud>([](void* object) -> GlobalColor* { return ((ChunkGenericCloudObjectForOutliers*)object)->getFirstObjectColor(); },
                                                                                                this,
                                                                                                1,
                                                                                                *m_objectIndexesCloud);
    }
}

void ChunkGenericCloudObjectForOutliers::createObjectNormalCloudMemberIfNot()
{
    if(m_objectNormalCloud == NULL) {
        Q_ASSERT(m_objectCloudProvider != NULL);
        GlobalNormalCloud* gnc = m_objectCloudProvider->createOrGetNormalCloud();
        Q_ASSERT(gnc != NULL);
        Q_UNUSED(gnc)
        m_objectNormalCloud = new ObjectFuncPointerCloudIndexed<GlobalNormal, GlobalGlIndexCloud>([](void* object) -> GlobalNormal* { return ((ChunkGenericCloudObjectForOutliers*)object)->getFirstObjectNormal(); },
                                                                                                  this,
                                                                                                  1,
                                                                                                  *m_objectIndexesCloud);
    }
}

void ChunkGenericCloudObjectForOutliers::createObjectInfoCloudMemberIfNot()
{
    if(m_objectInfoCloud == NULL) {
        Q_ASSERT(m_objectCloudProvider != NULL);
        GlobalInfoCloud *gic = m_objectCloudProvider->createOrGetInfoCloud();
        Q_ASSERT(gic != NULL);
        Q_UNUSED(gic)
        m_objectInfoCloud = new ObjectFuncPointerCloudIndexed<ElementInfo, GlobalGlIndexCloud>([](void* object) -> ElementInfo* { return ((ChunkGenericCloudObjectForOutliers*)object)->createOrGetFirstObjectInfo(); },
                                                                                               this,
                                                                                               1,
                                                                                               *m_objectIndexesCloud);
    }
}

void ChunkGenericCloudObjectForOutliers::deleteAllObjectXXXCloud()
{
    delete m_objectColorCloud;
    m_objectColorCloud = NULL;

    delete m_objectNormalCloud;
    m_objectNormalCloud = NULL;

    delete m_objectInfoCloud;
    m_objectInfoCloud = NULL;
}

ChunkGenericCloudObjectForOutliers::VertexesByOffset* ChunkGenericCloudObjectForOutliers::createOrGetVertexesForMiddle(const Eigen::Vector3d &middle)
{
    // get the chunk for this center
    /*Eigen::Vector3d chunkOffset;
    uint uniqueKey;

    m_csm->getCoordinateSystemInfosForCoordinates(middle[0], middle[1], middle[2], uniqueKey, chunkOffset);

    VertexesByOffset* vertexes = m_vertexesByNewOffset.value(uniqueKey, NULL);

    if(vertexes == NULL) {
        vertexes = new VertexesByOffset();
        vertexes->offset = chunkOffset;
        vertexes->nPoints = 0;
        m_vertexesByNewOffset.insert(uniqueKey, vertexes);
    }

    return vertexes;*/

    if(m_vertexesAndOffset.size() == m_nObjects)
        m_vertexesAndOffset.resize(m_nObjects + RESIZE_VECTOR_ADD);

    VertexesByOffset& vertexes = m_vertexesAndOffset[m_nObjects];
    vertexes.offset = middle;
    vertexes.vertexes.resize(m_nVertexPerObject);

    return &vertexes;
}

void ChunkGenericCloudObjectForOutliers::addVertexesTo(const std::vector<DoublePoint>& points,
                                                       ChunkGenericCloudObjectForOutliers::VertexesByOffset* vertexes)
{
    /*if(vertexes->vertexes.size() < (vertexes->nPoints + m_nVertexPerObject))
        vertexes->vertexes.resize(vertexes->nPoints + m_nVertexPerObject + RESIZE_VECTOR_ADD);

    for(int i=0; i<m_nVertexPerObject; ++i) {
        vertexes->vertexes[vertexes->nPoints] = (points[i] - vertexes->offset).cast<float>();
        ++vertexes->nPoints;
    }*/

    for(quint8 i=0; i<m_nVertexPerObject; ++i)
        vertexes->vertexes[i] = (points[i] - vertexes->offset).cast<float>();
}
