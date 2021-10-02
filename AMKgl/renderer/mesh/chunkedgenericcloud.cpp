#include "chunkedgenericcloud.h"

#include "amkglapp.h"
#include "interfaces/igraphicsdocument.h"
#include "tools/icloudattributesprovider.h"
#include "includenormal.h"

#include <QOpenGLFunctions_2_0>

#define RESIZE_VECTOR_ADD 10

#define GENERIC_CLOUD_VERTEX_UI     0
#define GENERIC_CLOUD_COLORS_UI     1
#define GENERIC_CLOUD_NORMALS_UI    2

ChunkedGenericCloud::ChunkedGenericCloud(const uint& uniqueKey,
                                         Scene::ObjectType objectType,
                                         GLenum glMode,
                                         const quint8& nVertexPerObject,
                                         const size_t& beginVertexGlobalIndex,
                                         const size_t& vertexArraySize,
                                         const size_t& beginElementGlobalIndex,
                                         const size_t& elementArraySize,
                                         const Eigen::Vector3d& coordinateSystemOffset) : SuperClass(uniqueKey,
                                                                                                     objectType,
                                                                                                     glMode,
                                                                                                     nVertexPerObject,
                                                                                                     coordinateSystemOffset)
{
    m_beginVertexGlobalIndex = beginVertexGlobalIndex;
    m_vertexArraySize = vertexArraySize;

    m_beginObjectGlobalIndex = beginElementGlobalIndex;
    m_objectArraySize = elementArraySize;
}

ChunkedGenericCloud::~ChunkedGenericCloud()
{
    deleteAllObjectXXXCloud();
}

void ChunkedGenericCloud::changeBeginningOfVertexCloud(const size_t &beginGlobalIndex)
{
    if(m_beginVertexGlobalIndex != beginGlobalIndex)
        m_beginVertexGlobalIndex = beginGlobalIndex;
}

void ChunkedGenericCloud::changeBeginningOfObjectCloud(const size_t &beginGlobalIndex)
{
    if(m_beginObjectGlobalIndex != beginGlobalIndex)
    {
        m_beginObjectGlobalIndex = beginGlobalIndex;

        deleteAllObjectXXXCloud();
    }
}

size_t ChunkedGenericCloud::getBeginningOfObjectCloud() const
{
    return m_beginObjectGlobalIndex;
}

size_t ChunkedGenericCloud::getEndOfObjectCloud() const
{
    if(m_objectArraySize == 0)
        return m_beginObjectGlobalIndex;

    return m_beginObjectGlobalIndex + (m_objectArraySize-1);
}

size_t ChunkedGenericCloud::getObjectCloudSize() const
{
    return m_objectArraySize;
}

size_t ChunkedGenericCloud::getBeginningOfVertexCloud() const
{
    return m_beginVertexGlobalIndex;
}

size_t ChunkedGenericCloud::getEndOfVertexCloud() const
{
    if(m_vertexArraySize == 0)
        return m_beginVertexGlobalIndex;

    return m_beginVertexGlobalIndex + (m_vertexArraySize-1);
}

size_t ChunkedGenericCloud::getVertexCloudSize() const
{
    return m_vertexArraySize;
}

void ChunkedGenericCloud::addObjectVertexes(const size_t& globalObjectIndex,
                                            const size_t* vertexGlobalIndexes,
                                            IChunk** chunk)
{
    Q_ASSERT(globalObjectIndex >= m_beginObjectGlobalIndex);

    setUpdated(false);

    const int n = getNumberOfElementPerObject();

    for(int i=0; i<n; ++i)
        addVertex(vertexGlobalIndexes[i] - m_beginVertexGlobalIndex);

    if(m_objectIndexes.size() == m_nObjects)
        m_objectIndexes.resize(m_nObjects + RESIZE_VECTOR_ADD, GLuint(globalObjectIndex - m_beginObjectGlobalIndex));
    else
        m_objectIndexes[m_nObjects] = GLuint(globalObjectIndex - m_beginObjectGlobalIndex);

    ++m_nObjects;

    if(chunk != nullptr)
        *chunk = this;
}

GLuint* ChunkedGenericCloud::getFirstObjectIndex() const
{
    if(m_objectIndexes.empty())
        return nullptr;

    return &const_cast<ChunkedGenericCloud*>(this)->m_objectIndexes[0];
}

GLuint* ChunkedGenericCloud::getFirstVertexIndex() const
{
    if(m_vertexIndexes.empty())
        return nullptr;

    return &const_cast<ChunkedGenericCloud*>(this)->m_vertexIndexes[0];
}

void ChunkedGenericCloud::addVertex(const size_t &localIndex)
{
    setUpdated(false);

    if(m_vertexIndexes.size() == m_nPoints)
        m_vertexIndexes.resize(m_nPoints + RESIZE_VECTOR_ADD, GLuint(localIndex));
    else
        m_vertexIndexes[m_nPoints] = GLuint(localIndex);

    ++m_nPoints;
}

void ChunkedGenericCloud::drawRaw(DrawInfo &info)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(info)
#endif
    // TODO
}

void ChunkedGenericCloud::drawVBO(DrawInfo &info)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(info)
#endif
    // TODO
}

void ChunkedGenericCloud::drawVAO(DrawInfo &info)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(info)
#endif
    // TODO
}

void ChunkedGenericCloud::drawRawNormals(DrawInfo &info)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(info)
#endif
    // TODO
}

void ChunkedGenericCloud::drawVBONormals(DrawInfo &info)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(info)
#endif
    // TODO
}

void ChunkedGenericCloud::drawVAONormals(DrawInfo &info)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(info)
#endif
    // TODO
}

bool ChunkedGenericCloud::internalUpdate()
{
    // TODO
    Context* context = getCurrentContext();

    if(context == nullptr)
        return false;

    if(isUpdated())
        return true;

    context->makeCurrent();

    destroyGLForCurrentContext();

    if(m_nPoints == 0) {
        setNormalDrawModeUsed(DM_NONE);
        setFastDrawModeUsed(DM_NONE);
        setUpdated(true);
        return true;
    }

    size_t nPoints = m_nPoints;
    bool ok = true;

    AMKglLOG->addInfoMessage(QString("Update generic global cloud chunk. "
                                     "vertexes : [%1;%2], objects [%3;%4]").arg(m_beginVertexGlobalIndex)
                                                                           .arg(nPoints)
                                                                           .arg(m_beginObjectGlobalIndex)
                                                                           .arg(countObjects()));


    Shaders& shaders = context->getShaders();
    shaders.setCurrentMode(getWhichCloudToUseForObjects());

    //if(shaders.getShaderProgram() == nullptr) {
        setNormalDrawModeUsed(getDrawModeToUse() == DM_BASIC ? DM_BASIC : DM_DISPLAY_LIST);
        setFastDrawModeUsed(getNormalDrawModeUsed());
        setUpdated(true);
        return true;
    //}

    TextureManager& textureManager = context->getTexturesManager();

    // NORMAL DRAW : for the moment we will use the draw mode set by user
    setNormalDrawModeUsed(getDrawModeToUse());

    // To use DRAW mode RAW or VBO or VAO with must have at least opengl function 2.0
    QOpenGLFunctions_2_0* func = context->getOpenglContext()->versionFunctions<QOpenGLFunctions_2_0>();

    // if the graphics card don't have this feature we must use the draw mode BASIC
    if(func == nullptr)
        setNormalDrawModeUsed(DM_DISPLAY_LIST);
    else
        func->initializeOpenGLFunctions();

    // FAST DRAW : same as normal draw mode because we don't have a fast draw mode here
    setFastDrawModeUsed(getNormalDrawModeUsed());

    if(mustUseVBO() && (func != nullptr)) {

        BufferObjectManager& bufferObjects = context->getBufferObjectManager();

        ok = shaders.bindShader();

        if(ok) {
            ok = bufferObjects.createVertexBO<FloatPoint>(this, GENERIC_CLOUD_VERTEX_UI, [](void* object) -> void* { return ((ChunkedGenericCloud*)object)->getFirstVertex(); }, m_nPoints);

            if(ok) {

                if(getWhichCloudToUseForObjects() == AMKgl::GCM_Object)
                    ok = textureManager.createNew1DTextureOrUseExisting(QString("CloudObjectIndex%1").arg(getNumberOfElementPerObject()),
                                                                        m_nObjects,
                                                                        QOpenGLTexture::DepthFormat);

                if(ok) {
                    ok = textureManager.createNew1DTextureOrUseExisting(QString("CloudObjectInfo%1").arg(getNumberOfElementPerObject()),
                                                                        m_objectArraySize,
                                                                        QOpenGLTexture::R8U);

                    if(ok) {

                        if(getWhichCloudToUseForObjects() == AMKgl::GCM_Vertex)
                            bufferObjects.createColorsBO<Color>(this, GENERIC_CLOUD_COLORS_UI, [](void* object) -> void* { return ((ChunkedGenericCloud*)object)->getFirstVertexColor(); }, m_nPoints);
                        else
                            ok = (getFirstObjectColor() == nullptr) || textureManager.createNew1DTextureOrUseExisting(QString("CloudObjectColor%1").arg(getNumberOfElementPerObject()),
                                                                                                                   m_objectArraySize,
                                                                                                                   QOpenGLTexture::RGBAFormat);

                        if(ok) {
                            if(getWhichCloudToUseForObjects() == AMKgl::GCM_Vertex)
                                bufferObjects.createNormalsBO<Normal>(this, GENERIC_CLOUD_NORMALS_UI, [](void* object) -> void* { return ((ChunkedGenericCloud*)object)->getFirstVertexNormal(); }, m_nPoints);
                            else
                                ok = (getFirstObjectNormal() == nullptr) || textureManager.createNew1DTextureOrUseExisting(QString("CloudObjectNormal%1").arg(getNumberOfElementPerObject()),
                                                                                                                        m_objectArraySize,
                                                                                                                        QOpenGLTexture::RGBAFormat);
                        }
                    }
                }
            }

            if(!ok) {
                if(getWhichCloudToUseForObjects() == AMKgl::GCM_Vertex)
                    setNormalDrawModeUsed(DM_RAW);
                else
                    setNormalDrawModeUsed(DM_DISPLAY_LIST); // object renderer must absolutely have texture to be draw in RAW mode
            }
        } else {
            setNormalDrawModeUsed(DM_DISPLAY_LIST);
        }

        // Create Vertex Array Object
        if(ok && mustUseVAO()) {
            // TODO :
            //bufferObjects.createVertexAO(shaders, context->getOpenglContext());
            //bufferObjects.createNormalsAO(shaders, context->getOpenglContext());
        }

        //bufferObjects.releaseAll();

        shaders.releaseShader();
    }

    setUpdated(true);

    return true;
}

void ChunkedGenericCloud::moveObjectsFromTo(const size_t &from, const size_t &to, const size_t &size)
{
    size_t fromM = from*getNumberOfElementPerObject();
    size_t sizeM = size*getNumberOfElementPerObject();
    size_t toM = to*getNumberOfElementPerObject();

    std::memcpy(&(*(m_objectIndexes.begin()+to)), &(*(m_objectIndexes.begin()+from)), sizeof(GLuint)*size);
    std::memcpy(&(*(m_vertexIndexes.begin()+toM)), &(*(m_vertexIndexes.begin()+fromM)), sizeof(GLuint)*sizeM);

    deleteAllObjectXXXCloud();
}

void ChunkedGenericCloud::internalSetNumberOfObjects(const size_t &n, const bool &shrinkToFit)
{
    m_nObjects = n;

    m_objectIndexes.resize(m_nObjects);

    if(shrinkToFit)
        m_objectIndexes.shrink_to_fit();

    m_nPoints = n*getNumberOfElementPerObject();
    m_vertexIndexes.resize(m_nPoints);

    if(shrinkToFit)
        m_vertexIndexes.shrink_to_fit();

    deleteAllObjectXXXCloud();
}

void ChunkedGenericCloud::createObjectColorCloudMemberIfNot()
{
    if(m_objectColorCloud == nullptr) {
        Q_ASSERT(m_objectCloudProvider != nullptr);
        GlobalColorCloud *gcc = m_objectCloudProvider->createOrGetColorCloud();
        Q_ASSERT(gcc != nullptr);
        Q_UNUSED(gcc)
        m_objectColorCloud = new ObjectFuncPointerCloudIndexed<Color, IndicesCollection>([](void* object) -> Color* { return ((ChunkedGenericCloud*)object)->getFirstObjectColor(); },
                                                                                         this,
                                                                                         1,
                                                                                         m_objectIndexes);
    }
}

void ChunkedGenericCloud::createObjectNormalCloudMemberIfNot()
{
    if(m_objectNormalCloud == nullptr) {
        Q_ASSERT(m_objectCloudProvider != nullptr);
        GlobalNormalCloud* gnc = m_objectCloudProvider->createOrGetNormalCloud();
        Q_ASSERT(gnc != nullptr);
        Q_UNUSED(gnc)
        m_objectNormalCloud = new ObjectFuncPointerCloudIndexed<Normal, IndicesCollection>([](void* object) -> Normal* { return ((ChunkedGenericCloud*)object)->getFirstObjectNormal(); },
                                                                                           this,
                                                                                           1,
                                                                                           m_objectIndexes);
    }
}

void ChunkedGenericCloud::createObjectInfoCloudMemberIfNot()
{
    if(m_objectInfoCloud == nullptr) {
        Q_ASSERT(m_objectCloudProvider != nullptr);
        GlobalInfoCloud *gic = m_objectCloudProvider->createOrGetInfoCloud();
        Q_ASSERT(gic != nullptr);
        Q_UNUSED(gic)
        m_objectInfoCloud = new ObjectFuncPointerCloudIndexed<Info, IndicesCollection>([](void* object) -> Info* { return ((ChunkedGenericCloud*)object)->createOrGetFirstObjectInfo(); },
                                                                                        this,
                                                                                        1,
                                                                                        m_objectIndexes);
    }
}

void ChunkedGenericCloud::deleteAllObjectXXXCloud()
{
    delete m_objectColorCloud;
    m_objectColorCloud = nullptr;

    delete m_objectNormalCloud;
    m_objectNormalCloud = nullptr;

    delete m_objectInfoCloud;
    m_objectInfoCloud = nullptr;
}
