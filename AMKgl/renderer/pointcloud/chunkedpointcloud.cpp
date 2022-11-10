#include "chunkedpointcloud.h"

#include "amkglapp.h"
#include "drawinfo.h"
#include "pointrenderercontext.h"
#include "pointcloudarrayobjectmanager.h"
#include "interfaces/igraphicsdocument.h"
#include "interfaces/iglobalpointcloudmanager.h"
#include "includeglobalcloud.h"
#include "tools/opengl/openglinfo.h"
#include "tools/pointcloudattributesprovider.h"
#include "visitor/iglobalpointsvisitor.h"
#include "tools/objectattributesprovider.h"
#include "tools/opengl/genericglrenderer.h"
#include "scene/permanentscene.h"

#include <QOpenGLFunctions_2_0>

#define RESIZE_VECTOR_ADD           10
#define SLOW_DISPLAY_LIST           0
#define SLOW_NORMALS_DISPLAY_LIST   1
#define FAST_DISPLAY_LIST           2
#define FAST_NORMALS_DISPLAY_LIST   3

ChunkedPointCloud::ChunkedPointCloud(const uint& uniqueKey,
                                     const size_t &beginGlobalIndex,
                                     const size_t &vertexArraySize,
                                     const Eigen::Vector3d &coordinateSystemOffset) : Chunk<PointRendererContext>(uniqueKey,
                                                                                                                  Scene::PointGlobalCloud,
                                                                                                                  coordinateSystemOffset)
{
    m_mutex = new QMutex(QMutex::Recursive);

    m_objectColorCloud = nullptr;
    m_objectNormalCloud = nullptr;
    m_objectInfoCloud = nullptr;
    m_globalInfoCloud = nullptr;

    m_vertexArraySize = vertexArraySize;
    m_beginGlobalIndex = beginGlobalIndex;
    m_maxNumberOfPointsInFastDrawMode = 0;

    changeBeginningOfCloud(beginGlobalIndex);

    setMaxNumberOfPointsToDrawInFastDraw(vertexArraySize);

    m_nPoints = 0;
    m_pointCloudProvider = nullptr;
}

ChunkedPointCloud::~ChunkedPointCloud()
{
    deleteAllObjectXXXCloud();
    delete m_mutex;
}

void ChunkedPointCloud::setAttributesAccessor(const IAttributesAccessor *accessor)
{
    Chunk<PointRendererContext>::setAttributesAccessor(accessor);

    m_pointCloudProvider = nullptr;

    if(accessor != nullptr)
        m_pointCloudProvider = accessor->getPermanentScene()->getPointCloudAttributesProvider();
}

void ChunkedPointCloud::addPoint(const size_t &localIndex)
{
    setUpdated(false);

    if(m_indexes.size() == m_nPoints)
        m_indexes.resize(m_nPoints + RESIZE_VECTOR_ADD, GLuint(localIndex));
    else
        m_indexes[m_nPoints] = GLuint(localIndex);

    if(m_globalInfoCloud != nullptr)
        (*m_globalInfoCloud)[m_beginGlobalIndex+localIndex] = 0; // reset the visibility/selection/etc... state

    ++m_nPoints;
}

void ChunkedPointCloud::removePoints(const size_t &from, const size_t &count)
{
    removeObjects(from, count);
}

void ChunkedPointCloud::setPointsVisible(const size_t &from, const size_t &count, bool visible)
{
    if(count == 0)
        return;

    size_t end = from + count;

    ElementInfo* fi = createOrGetFirstInfo();

    if(fi == nullptr)
        return;

    if(visible) {
        do {
            ObjectsFlagsTool::staticShowObject(fi[m_indexes[--end]]);
        } while(end != from);
    } else {
        do {
            ObjectsFlagsTool::staticHideObjectAndUnselectIt(fi[m_indexes[--end]]);
        } while(end != from);
    }

    setUpdated(false);
}

bool ChunkedPointCloud::isAtLeastOneObjectVisible(const size_t &from, const size_t &count) const
{
    if(count == 0)
        return false;

    size_t end = from + count;

    ElementInfo* fi = createOrGetFirstInfo();

    if(fi == nullptr)
        return false;

    do {
        if(ObjectsFlagsTool::staticIsObjectVisible(fi[m_indexes[--end]]))
            return true;
    } while(end != from);

    return false;
}

void ChunkedPointCloud::hidePointsSelected()
{
    size_t end = countPoints();

    if(end == 0)
        return;

    ElementInfo* fi = createOrGetFirstInfo();

    if(fi == nullptr)
        return;

    do {
        if(ObjectsFlagsTool::staticIsObjectSelected(fi[m_indexes[--end]]))
            ObjectsFlagsTool::staticHideObjectAndUnselectIt(fi[m_indexes[end]]);
    } while(end != 0);

    setUpdated(false);
}

void ChunkedPointCloud::clearPoints()
{
    QMutexLocker locker(m_mutex);

    setUpdated(false);
    setNumberOfObjects(0);
}

void ChunkedPointCloud::resetObjects()
{
    QMutexLocker locker(m_mutex);

    setUpdated(false);
    internalSetNumberOfObjects(0, false);
}

size_t ChunkedPointCloud::countPoints() const
{
    return m_nPoints;
}

void ChunkedPointCloud::visitPoints(IGlobalPointsVisitor &visitor)
{
    if(m_indexes.size() != m_nPoints)
        m_indexes.resize(m_nPoints);

    visitor.setChunk(this);
    visitor.addPointsToCompute(getOffset(), m_indexes, m_beginGlobalIndex);
}

void ChunkedPointCloud::setMaxNumberOfPointsToDrawInFastDraw(const size_t &maxNumber)
{
    if(maxNumber != m_maxNumberOfPointsInFastDrawMode)
        setUpdated(false);

    m_maxNumberOfPointsInFastDrawMode = maxNumber;
}

void ChunkedPointCloud::changeBeginningOfCloud(const size_t &beginGlobalIndex)
{
    QMutexLocker locker(m_mutex);

    if(m_beginGlobalIndex != beginGlobalIndex) {

        m_beginGlobalIndex = beginGlobalIndex;

        deleteAllObjectXXXCloud();
    }
}

void ChunkedPointCloud::draw(DrawInfo &info)
{
    QMutexLocker locker(m_mutex);

    if((m_nPoints == 0) || !isUpdated())
        return;

    DrawMode type = getNormalDrawModeUsed();

    if(type != DM_NONE) {

        bool showNormals = (getCurrentDocument() != nullptr) && getCurrentDocument()->mustShowNormals();
        PointRendererShaders& shaders = getCurrentContext()->getShaders();

        pushCoordinateSystemMatrix(info);

        if(type == DM_BASIC) {
            drawBasic(info);

            if(showNormals)
                drawBasicNormals(info);
        } else if(type == DM_DISPLAY_LIST) {
            if(!getCurrentContext()->executeOrCreateAndCompileAndExecuteDisplayList(this, SLOW_DISPLAY_LIST)) {
                drawBasic(info);
                getCurrentContext()->endDisplayList(this, SLOW_DISPLAY_LIST);
            }

            if(showNormals) {
                if(!getCurrentContext()->executeOrCreateAndCompileAndExecuteDisplayList(this, SLOW_NORMALS_DISPLAY_LIST)) {
                    drawBasicNormals(info);
                    getCurrentContext()->endDisplayList(this, SLOW_NORMALS_DISPLAY_LIST);
                }
            }
        } else {
            shaders.bindAndConfigureByDefaultPointsShader(getCurrentDocument(), *getFlagsPropertyManager());

            if(type == DM_RAW)
                drawRaw(info);
            else if(type == DM_VBO)
                drawVBO(info);
            else if(type == DM_VAO)
                drawVAO(info);

            shaders.releasePointsShader();

            if(showNormals) {

                if(shaders.bindAndConfigureByDefaultNormalsShader(getCurrentDocument(), *getFlagsPropertyManager())) {
                    if(type == DM_RAW)
                        drawRawNormals(info);
                    else if(type == DM_VBO)
                        drawVBONormals(info);
                    else if(type == DM_VAO)
                        drawVAONormals(info);

                    shaders.releaseNormalsShader();
                } else {
                    drawBasicNormals(info);
                }
            }
        }

        popCoordinateSystemMatrix();
    }
}

void ChunkedPointCloud::fastDraw(DrawInfo &info)
{
    QMutexLocker locker(m_mutex);

    if((m_nPoints == 0) || !isUpdated())
        return;

    if(m_fastStride == 1) {
        draw(info);
        return ;
    }

    DrawMode type = getFastDrawModeUsed();

    if(type != DM_NONE) {

        bool showNormals = (getCurrentDocument() != nullptr) && getCurrentDocument()->mustShowNormals();

        pushCoordinateSystemMatrix(info);

        if(type == DM_DISPLAY_LIST) {
            if(!getCurrentContext()->executeOrCreateAndCompileAndExecuteDisplayList(this, FAST_DISPLAY_LIST)) {
                fastDrawBasic(info);
                getCurrentContext()->endDisplayList(this, FAST_DISPLAY_LIST);
            }

            if(showNormals) {
                if(!getCurrentContext()->executeOrCreateAndCompileAndExecuteDisplayList(this, FAST_NORMALS_DISPLAY_LIST)) {
                    fastDrawBasicNormals(info);
                    getCurrentContext()->endDisplayList(this, FAST_NORMALS_DISPLAY_LIST);
                }
            }
        } else {
            fastDrawBasic(info);

            if(showNormals)
                fastDrawBasicNormals(info);
        }

        popCoordinateSystemMatrix();
    }
}

void ChunkedPointCloud::postDraw(DrawInfo &info)
{
    QString drawMode = "?";

    if(info.inFastDrawMode()) {
        DrawMode type = getFastDrawModeUsed();

        if(m_fastStride != 1) {
            drawMode = "FAST ";

            if(type == DM_NONE)
                drawMode += "NONE";
            else if(type == DM_BASIC)
                drawMode += "BASIC";
            else if(type == DM_DISPLAY_LIST)
                drawMode += "DISPLAY_LIST";
            else if(type == DM_RAW)
                drawMode += "RAW";
            else if(type == DM_VAO)
                drawMode += "VBO + VAO";
            else if(type == DM_VBO)
                drawMode += "VBO";
        }
    }

    if(drawMode == "?") {
        DrawMode type = getNormalDrawModeUsed();

        drawMode = "NORMAL ";

        if(type == DM_NONE)
            drawMode += "NONE";
        else if(type == DM_BASIC)
            drawMode += "BASIC";
        else if(type == DM_DISPLAY_LIST)
            drawMode += "DISPLAY_LIST";
        else if(type == DM_RAW)
            drawMode += "RAW";
        else if(type == DM_VAO)
            drawMode += "VBO + VAO";
        else if(type == DM_VBO)
            drawMode += "VBO";
    }

    info.drawText(QString("%1 points (Draw mode for Points is %2 / Normals is %3)")
            .arg(m_nPoints)
            .arg(drawMode)
            .arg(((getCurrentContext() == nullptr) || (getCurrentContext()->getShaders().getNormalsShaderProgram() == nullptr)) ? "Manual" : drawMode));

    /*Eigen::Vector4d camTranslation = info.getCameraTranslation();

    QVector3D newOffset;

    newOffset.setX(m_coordinateSystemOffset[0] - camTranslation[0]);
    newOffset.setY(m_coordinateSystemOffset[1] - camTranslation[1]);
    newOffset.setZ(m_coordinateSystemOffset[2] - camTranslation[2]);

    info.drawText(QString("chunk [%1;%2;%3] [%4;%5;%6]").arg(m_coordinateSystemOffset[0])
            .arg(m_coordinateSystemOffset[1])
            .arg(m_coordinateSystemOffset[2])
            .arg(newOffset[0])
            .arg(newOffset[1])
            .arg(newOffset[2]));*/
}

void ChunkedPointCloud::lock()
{
    m_mutex->lock();
}

void ChunkedPointCloud::unlock()
{
    m_mutex->unlock();
}

bool ChunkedPointCloud::internalUpdate()
{
    PointRendererContext* context = getCurrentContext();

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

    PointRendererShaders& shaders = context->getShaders();

    size_t nPoints = m_nPoints;
    bool ok = true;

    AMKglLOG->addInfoMessage(QString("Update global cloud chunk (%3) that begin at index %1 and contains %2 points").arg(m_beginGlobalIndex).arg(nPoints).arg(getUniqueKey()));

    if(m_maxNumberOfPointsInFastDrawMode != 0) {
        m_fastStride = (int)(nPoints/m_maxNumberOfPointsInFastDrawMode);
        m_fastStride = qMax(m_fastStride, 1);
    } else {
        m_fastStride = 1;
    }

    if(shaders.getPointsShaderProgram() == nullptr) {
        setNormalDrawModeUsed(getDrawModeToUse() == DM_BASIC ? DM_BASIC : DM_DISPLAY_LIST);
        setFastDrawModeUsed(getNormalDrawModeUsed());
        setUpdated(true);
        return true;
    }

    // NORMAL DRAW : for the moment we will use the draw mode set by user
    setNormalDrawModeUsed(getDrawModeToUse());

    // To use DRAW mode RAW or VBO or VAO with must have at least opengl function 2.0
    QOpenGLFunctions_2_0* func = context->getOpenglContext()->versionFunctions<QOpenGLFunctions_2_0>();

    // if the graphics card don't have this feature we must use the draw mode BASIC or display list
    if(func == nullptr)
        setNormalDrawModeUsed(getDrawModeToUse() == DM_BASIC ? DM_BASIC : DM_DISPLAY_LIST);
    else
        func->initializeOpenGLFunctions();

    // FAST DRAW : can be only BASIC or DISPLAY_LIST
    // RAW or VBO or VAO not available in fast mode (or if you want to do it you must create
    // a second vector of indexes and store it indexes at 0, 0+m_fastStride, 0+m_fastride*2, etc...
    // But it was not necessary because in basic mode we are fast because we don't draw all points.

    setFastDrawModeUsed(getDrawModeToUse() == DM_BASIC ? DM_BASIC : DM_DISPLAY_LIST);

    if(mustUseVBO() && (func != nullptr)) {

        BufferObjectManager& bufferObjects = context->getBufferObjectManager();

        ok = shaders.bindPointShader();

        if(ok) {
            ok = bufferObjects.createVertexBO<FloatPoint>(this, POINTCLOUD_VERTEX_BO_UI, [](void* object) -> void* { return ((ChunkedPointCloud*)object)->getFirstVertex(); }, m_vertexArraySize);

            if(ok) {

                ok = bufferObjects.createIndexBO<GLuint>(this, POINTCLOUD_INDEX_BO_UI, [](void* object) -> void* { return ((ChunkedPointCloud*)object)->getFirstLocalIndex(); }, nPoints);

                if(ok) {

                    ok = bufferObjects.createInfosBO<ElementInfo>(this, POINTCLOUD_INFO_BO_UI, [](void* object) -> void* { return ((ChunkedPointCloud*)object)->createOrGetFirstInfo(); }, m_vertexArraySize);

                    if(ok) {
                        bufferObjects.createColorsBO<GlobalColor>(this, POINTCLOUD_COLOR_BO_UI, [](void* object) -> void* { return ((ChunkedPointCloud*)object)->getFirstColor(); }, m_vertexArraySize);

                        if(shaders.bindNormalsShader())
                            bufferObjects.createNormalsBO<GlobalNormal>(this, POINTCLOUD_NORMAL_BO_UI, [](void* object) -> void* { return ((ChunkedPointCloud*)object)->getFirstNormal(); }, m_vertexArraySize);
                    }
                }
            }

            if(!ok)
                setNormalDrawModeUsed(DM_RAW);
        } else {
            setNormalDrawModeUsed(DM_DISPLAY_LIST);
        }

        // Create Vertex Array Object
        if(ok && mustUseVAO()) {
            PointCloudArrayObjectManager& arrayObjects = context->createOrGetArrayObjectManager(this);

            arrayObjects.createVertexAO(this, *context);
            arrayObjects.createNormalsAO(this, *context);
        }

        //bufferObjects.releaseAll();

        shaders.releasePointsShader();
        shaders.releaseNormalsShader();
    }

    setUpdated(true);

    return true;
}

void ChunkedPointCloud::fastDrawBasic(DrawInfo &info)
{
    Q_UNUSED(info)

    float selectionColor[4];
    getSelectionColor(selectionColor);

    float currentColor[4];
    getCurrentColor(currentColor);

    GenericGLRenderer::staticBasicFastRender(GL_POINTS,
                                             getFirstVertex(),
                                             createOrGetFirstInfo(),
                                             getFirstColor(),
                                             (GlobalNormal*)nullptr,
                                             *getFlagsPropertyManager(),
                                             m_indexes,
                                             m_nPoints,
                                             m_fastStride,
                                             selectionColor,
                                             currentColor);
}

void ChunkedPointCloud::drawBasic(DrawInfo &info)
{
    Q_UNUSED(info)

    float selectionColor[4];
    getSelectionColor(selectionColor);

    float currentColor[4];
    getCurrentColor(currentColor);

    GenericGLRenderer::staticBasicRender(GL_POINTS,
                                         getFirstVertex(),
                                         createOrGetFirstInfo(),
                                         getFirstColor(),
                                         (GlobalNormal*)nullptr,
                                         *getFlagsPropertyManager(),
                                         m_indexes,
                                         m_nPoints,
                                         selectionColor,
                                         currentColor);
}

void ChunkedPointCloud::drawRaw(DrawInfo &info)
{
    Q_UNUSED(info)

    size_t size = m_nPoints;

    FloatPoint* fv = getFirstVertex();
    GlobalColor* fc = getFirstColor();
    ElementInfo* fi = createOrGetFirstInfo();

    PointRendererContext* context = getCurrentContext();

    context->getShaders().setVertexAttributeToPointsShader(fv);

    if(fc != nullptr)
        context->getShaders().setColorAttributeToPointsShader(fc);
    else
        context->getShaders().setUseColorAttribute(false);

    context->getShaders().setInfoAttributeToPointsShader(fi, context->getOpenglContext());

    callDrawElements(size);

    context->getShaders().disableAttributeOfPointsShader();
}

void ChunkedPointCloud::drawVBO(DrawInfo &info)
{
    Q_UNUSED(info)

    PointRendererContext* context = getCurrentContext();

    PointRendererShaders& shaders = context->getShaders();
    BufferObjectManager& buffers = context->getBufferObjectManager();

    bool ok = false;

    size_t size = m_nPoints;

    if(shaders.bindAndSetVertexAttributeToPointsShader(buffers.getVertexBO(this, POINTCLOUD_VERTEX_BO_UI))) {
        ok = shaders.bindAndSetInfoAttributeToPointsShader(buffers.getInfosBO(this, POINTCLOUD_INFO_BO_UI), context->getOpenglContext());

        if(ok) {
            shaders.bindAndSetColorAttributeToPointsShader(buffers.getColorsBO(this, POINTCLOUD_COLOR_BO_UI));

            callDrawElements(size);
        }

        shaders.disableAttributeOfPointsShader();
    }

    if(!ok) {
        context->getShaders().releasePointsShader();
        drawBasic(info);
    }
}

void ChunkedPointCloud::drawVAO(DrawInfo &info)
{
    Q_UNUSED(info)

    PointCloudArrayObjectManager& arrayObjects = getCurrentContext()->createOrGetArrayObjectManager(this);

    bool ok = false;

    size_t size = m_nPoints;

    if(arrayObjects.canUseVertexAO()) {

        if(arrayObjects.bindVertexAO()) {

            ok = true;

            callDrawElements(size);

            arrayObjects.releaseVertexAO();
        }
    }

    if(!ok)
        drawVBO(info);
}

void ChunkedPointCloud::fastDrawBasicNormals(DrawInfo &info)
{
    Q_UNUSED(info)

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

    GenericGLRenderer::staticBasicNormalFastRender(getFirstVertex(),
                                                   createOrGetFirstInfo(),
                                                   getFirstNormal(),
                                                   *getFlagsPropertyManager(),
                                                   m_indexes,
                                                   m_nPoints,
                                                   m_fastStride,
                                                   getCurrentDocument()->getNormalsLength(),
                                                   normalColor,
                                                   selectionColor,
                                                   currentColor);
}

void ChunkedPointCloud::drawBasicNormals(DrawInfo &info)
{
    Q_UNUSED(info)

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

    GenericGLRenderer::staticBasicNormalRender(getFirstVertex(),
                                               createOrGetFirstInfo(),
                                               getFirstNormal(),
                                               *getFlagsPropertyManager(),
                                               m_indexes,
                                               m_nPoints,
                                               getCurrentDocument()->getNormalsLength(),
                                               normalColor,
                                               selectionColor,
                                               currentColor);
}

void ChunkedPointCloud::drawRawNormals(DrawInfo &info)
{
    Q_UNUSED(info)

    GlobalNormal* fn = getFirstNormal();

    if(fn != nullptr) {
        size_t size = m_nPoints;

        FloatPoint* fv = getFirstVertex();
        ElementInfo* fi = createOrGetFirstInfo();

        PointRendererContext* context = getCurrentContext();

        context->getShaders().setVertexAttributeToNormalsShader(fv);
        context->getShaders().setNormalAttributeToNormalsShader(fn);
        context->getShaders().setInfoAttributeToNormalsShader(fi, context->getOpenglContext());

        callDrawElements(size);

        context->getShaders().disableAttributeOfNormalsShader();
    }
}

void ChunkedPointCloud::drawVBONormals(DrawInfo &info)
{
    Q_UNUSED(info)

    PointRendererContext* context = getCurrentContext();

    BufferObjectManager& bufferObjects = context->getBufferObjectManager();
    PointRendererShaders& shaders = context->getShaders();

    bool ok = false;

    if(bufferObjects.getNormalsBO(this, POINTCLOUD_NORMAL_BO_UI).isCreated()) {

        size_t size = m_nPoints;

        if(shaders.bindAndSetVertexAttributeToNormalsShader(bufferObjects.getVertexBO(this, POINTCLOUD_INDEX_BO_UI))
                && shaders.bindAndSetNormalAttributeToNormalsShader(bufferObjects.getNormalsBO(this, POINTCLOUD_NORMAL_BO_UI))
                && shaders.bindAndSetInfoAttributeToNormalsShader(bufferObjects.getInfosBO(this, POINTCLOUD_INFO_BO_UI), context->getOpenglContext())) {

            ok = true;
            callDrawElements(size);
        }

        shaders.disableAttributeOfNormalsShader();
    }

    if(!ok) {
        shaders.releaseNormalsShader();
        drawBasicNormals(info);
    }
}

void ChunkedPointCloud::drawVAONormals(DrawInfo &info)
{
    Q_UNUSED(info)

    PointRendererContext* context = getCurrentContext();

    PointCloudArrayObjectManager& arrayObjects = context->createOrGetArrayObjectManager(this);
    BufferObjectManager& bufferObjects = context->getBufferObjectManager();

    bool ok = false;

    if(bufferObjects.getNormalsBO(this, POINTCLOUD_NORMAL_BO_UI).isCreated()) {

        size_t size = m_nPoints;

        if(arrayObjects.canUseNormalsAO()) {

            if(arrayObjects.bindNormalsAO()) {
                ok = true;

                callDrawElements(size);

                arrayObjects.releaseNormalsAO();
            }
        }
    }

    if(!ok)
        drawVBONormals(info);
}

void ChunkedPointCloud::callDrawElements(const size_t &size)
{
    BufferObjectManager& bufferObjects = getCurrentContext()->getBufferObjectManager();

    QOpenGLBuffer& ibo = bufferObjects.getIndexBO(this, POINTCLOUD_INDEX_BO_UI);

    if(ibo.isCreated()
            && ibo.bind()) {
        glDrawElements(GL_POINTS, (GLsizei)size, GL_UNSIGNED_INT, 0);
        ibo.release();
        return;
    }

    glDrawElements(GL_POINTS, (GLsizei)size, GL_UNSIGNED_INT, &m_indexes[0]);
}

void ChunkedPointCloud::setUpdated(bool status)
{
    if(mustChangeUpdateStatus(status)) {
        QMutexLocker locker(m_mutex);

        Chunk<PointRendererContext>::setUpdated(status);
    }
}

void ChunkedPointCloud::moveObjectsFromTo(const size_t &from, const size_t &to, const size_t &size)
{
    std::memcpy(&(*(m_indexes.begin()+to)), &(*(m_indexes.begin()+from)), sizeof(GLuint)*size);

    deleteAllObjectXXXCloud();
}

void ChunkedPointCloud::setNumberOfObjects(const size_t &n)
{
    internalSetNumberOfObjects(n, true);
}

void ChunkedPointCloud::internalSetNumberOfObjects(const size_t &n, const bool &shrinkToFit)
{
    m_nPoints = n;
    m_indexes.resize(m_nPoints);

    if(shrinkToFit)
        m_indexes.shrink_to_fit();

    deleteAllObjectXXXCloud();
}

bool ChunkedPointCloud::isUpdated() const
{
    if(!Chunk<PointRendererContext>::isUpdated())
        return false;

    PointRendererContext* context = getCurrentContext();

    if(context == nullptr)
        return true;

    ChunkCustomUpdateValues* cuv = context->createOrGetChunkCustomUpdateValues(this);

    if(cuv->getUserValue("MaxNumberOfPointsInFastDrawMode", QVariant::fromValue(m_maxNumberOfPointsInFastDrawMode)).value<size_t>() != m_maxNumberOfPointsInFastDrawMode)
        return false;

    if(cuv->getUserValue("NPoints", QVariant::fromValue(m_nPoints)).value<size_t>() != m_nPoints)
        return false;

    return true;
}

void ChunkedPointCloud::updateOnlyVAO()
{
    if((getNormalDrawModeUsed() == DM_VAO) && mustUseVAO()) {

        PointRendererContext* context = getCurrentContext();

        if(context == nullptr)
            return;

        BufferObjectManager& bufferObjects = context->getBufferObjectManager();

        if(getGlobalColorCloud() != nullptr && !bufferObjects.getColorsBO(this, POINTCLOUD_COLOR_BO_UI).isCreated())
            setUpdated(false);

        if(getGlobalNormalCloud() != nullptr && !bufferObjects.getNormalsBO(this, POINTCLOUD_NORMAL_BO_UI).isCreated())
            setUpdated(false);

        if(isUpdated()) {
            PointRendererShaders& shaders = context->getShaders();
            PointCloudArrayObjectManager& arrayObjects = context->createOrGetArrayObjectManager(this);

            context->makeCurrent();

            arrayObjects.updateVertexAO(this, *context);
            arrayObjects.updateNormalsAO(this, *context);

            //bufferObjects.releaseAll();

            shaders.releasePointsShader();
            shaders.releaseNormalsShader();
        } else {
            internalUpdate();
        }
    }

    if((getNormalDrawModeUsed() == DM_DISPLAY_LIST)
                || (getFastDrawModeUsed() == DM_DISPLAY_LIST)) {

        PointRendererContext* context = getCurrentContext();

        if(context == nullptr)
            return;

        context->destroyAllDisplayList(this);
    }
}

GlobalColorCloud* ChunkedPointCloud::getGlobalColorCloud() const
{
    Q_ASSERT(m_pointCloudProvider != nullptr);
    if (m_pointCloudProvider == nullptr) {qDebug() << "ChunkedPointCloud::getGlobalColorCloud" << ", " <<  "m_pointCloudProvider == nullptr"; return nullptr;}

    return m_pointCloudProvider->getColorCloud();
}

GlobalNormalCloud* ChunkedPointCloud::getGlobalNormalCloud() const
{
    Q_ASSERT(m_pointCloudProvider != nullptr);
    if (m_pointCloudProvider == nullptr) {qDebug() << "ChunkedPointCloud::getGlobalNormalCloud" << ", " <<  "m_pointCloudProvider == nullptr"; return nullptr;}

    return m_pointCloudProvider->getNormalCloud();
}

GlobalInfoCloud* ChunkedPointCloud::getGlobalInfoCloud() const
{
    Q_ASSERT(m_pointCloudProvider != nullptr);
    if (m_pointCloudProvider == nullptr) {qDebug() << "ChunkedPointCloud::getGlobalInfoCloud" << ", " <<  "m_pointCloudProvider == nullptr"; return nullptr;}

    GlobalInfoCloud* gic = m_pointCloudProvider->getInfoCloud();

    if(m_globalInfoCloud == nullptr)
        const_cast<ChunkedPointCloud*>(this)->m_globalInfoCloud = gic;

    return gic;
}

GlobalColorCloud* ChunkedPointCloud::createOrGetColorCloud()
{
    QMutexLocker locker(m_mutex);

    Q_ASSERT(m_pointCloudProvider != nullptr);
    if (m_pointCloudProvider == nullptr) {qDebug() << "ChunkedPointCloud::createOrGetColorCloud" << ", " <<  "m_pointCloudProvider == nullptr"; return nullptr;}

    GlobalColorCloud* gcc = m_pointCloudProvider->createOrGetColorCloud();
    createObjectColorCloudMemberIfNot();
    return gcc;
}

GlobalNormalCloud* ChunkedPointCloud::createOrGetNormalCloud()
{
    QMutexLocker locker(m_mutex);

    Q_ASSERT(m_pointCloudProvider != nullptr);
    if (m_pointCloudProvider == nullptr) {qDebug() << "ChunkedPointCloud::createOrGetNormalCloud" << ", " <<  "m_pointCloudProvider == nullptr"; return nullptr;}

    GlobalNormalCloud* gnc = m_pointCloudProvider->createOrGetNormalCloud();
    createObjectNormalCloudMemberIfNot();
    return gnc;
}

GlobalInfoCloud* ChunkedPointCloud::createOrGetInfoCloud()
{
    QMutexLocker locker(m_mutex);

    Q_ASSERT(m_pointCloudProvider != nullptr);
    if (m_pointCloudProvider == nullptr) {qDebug() << "ChunkedPointCloud::createOrGetInfoCloud" << ", " <<  "m_pointCloudProvider == nullptr"; return nullptr;}

    GlobalInfoCloud* gic = m_pointCloudProvider->createOrGetInfoCloud();
    createObjectInfoCloudMemberIfNot();
    return gic;
}

Basic::AbstractColorCloud* ChunkedPointCloud::createOrGetObjectColorCloud()
{
    createObjectColorCloudMemberIfNot();

    return m_objectColorCloud;
}

Basic::AbstractNormalCloud* ChunkedPointCloud::createOrGetObjectNormalCloud()
{
    createObjectNormalCloudMemberIfNot();

    return m_objectNormalCloud;
}

Basic::AbstractInfoCloud* ChunkedPointCloud::createOrGetObjectInfoCloud()
{
    createObjectInfoCloudMemberIfNot();

    return m_objectInfoCloud;
}

FloatPoint* ChunkedPointCloud::getFirstVertex() const
{
    IGlobalPointCloudManager* gpcm = AMKglEA->getGlobalPointCloudManager();

    if(m_beginGlobalIndex >= gpcm->size())
        return nullptr;

    return &gpcm->getFloatPointAtGlobalIndex(m_beginGlobalIndex);
}

GLuint* ChunkedPointCloud::getFirstLocalIndex() const
{
    if(m_indexes.empty())
        return nullptr;

    return (GLuint*)&m_indexes[0];
}

GlobalColor* ChunkedPointCloud::getFirstColor() const
{
    GlobalColorCloud* cc = getGlobalColorCloud();

    if((cc == nullptr) || (m_beginGlobalIndex >= cc->size()))
        return nullptr;

    return &(*cc)[m_beginGlobalIndex];
}

GlobalNormal* ChunkedPointCloud::getFirstNormal() const
{
    GlobalNormalCloud* nc = getGlobalNormalCloud();

    if((nc == nullptr) || (m_beginGlobalIndex >= nc->size()))
        return nullptr;

    return &(*nc)[m_beginGlobalIndex];
}

ElementInfo *ChunkedPointCloud::createOrGetFirstInfo() const
{
    GlobalInfoCloud* ic = const_cast<ChunkedPointCloud*>(this)->createOrGetInfoCloud();

    if((ic == nullptr) || (m_beginGlobalIndex >= ic->size()))
        return nullptr;

    return &(*ic)[m_beginGlobalIndex];
}

ElementInfo* ChunkedPointCloud::getFirstInfo() const
{
    GlobalInfoCloud* ic = getGlobalInfoCloud();

    if((ic == nullptr) || (m_beginGlobalIndex >= ic->size()))
        return nullptr;

    return &(*ic)[m_beginGlobalIndex];
}

void ChunkedPointCloud::setContextUpdated(PointRendererContext *context, bool status)
{
    Chunk<PointRendererContext>::setContextUpdated(context, status);

    ChunkCustomUpdateValues* cuv = context->createOrGetChunkCustomUpdateValues(this);
    cuv->setUserValue("MaxNumberOfPointsInFastDrawMode", QVariant::fromValue(m_maxNumberOfPointsInFastDrawMode));
    cuv->setUserValue("NPoints", QVariant::fromValue(m_nPoints));
}

void ChunkedPointCloud::createObjectColorCloudMemberIfNot()
{
    QMutexLocker locker(m_mutex);

    if(m_objectColorCloud == nullptr) {
        Q_ASSERT(m_pointCloudProvider != nullptr);
        if (m_pointCloudProvider == nullptr) {qDebug() << "ChunkedPointCloud::createObjectColorCloudMemberIfNot" << ", " <<  "m_pointCloudProvider == nullptr"; return;}

        GlobalColorCloud *gcc = m_pointCloudProvider->createOrGetColorCloud();
        Q_ASSERT(gcc != nullptr);
        if (gcc == nullptr) {qDebug() << "ChunkedPointCloud::createObjectColorCloudMemberIfNot" << ", " <<  "gcc == nullptr"; return;}

        Q_UNUSED(gcc)
        m_objectColorCloud = new ObjectFuncPointerCloudIndexed<GlobalColor, IndicesCollection>([](void* object) -> GlobalColor* { return ((ChunkedPointCloud*)object)->getFirstColor(); },
                                                                                           this,
                                                                                           getNumberOfElementPerObject(),
                                                                                           m_indexes);
    }
}

void ChunkedPointCloud::createObjectNormalCloudMemberIfNot()
{
    QMutexLocker locker(m_mutex);

    if(m_objectNormalCloud == nullptr) {
        Q_ASSERT(m_pointCloudProvider != nullptr);
        if (m_pointCloudProvider == nullptr) {qDebug() << "ChunkedPointCloud::createObjectNormalCloudMemberIfNot" << ", " <<  "m_pointCloudProvider == nullptr"; return;}

        GlobalNormalCloud* gnc = m_pointCloudProvider->createOrGetNormalCloud();
        Q_ASSERT(gnc != nullptr);
        if (gnc == nullptr) {qDebug() << "ChunkedPointCloud::createObjectNormalCloudMemberIfNot" << ", " <<  "gnc == nullptr"; return;}

        Q_UNUSED(gnc)
        m_objectNormalCloud = new ObjectFuncPointerCloudIndexed<GlobalNormal, IndicesCollection>([](void* object) -> GlobalNormal* { return ((ChunkedPointCloud*)object)->getFirstNormal(); },
                                                                                             this,
                                                                                             getNumberOfElementPerObject(),
                                                                                             m_indexes);
    }
}

void ChunkedPointCloud::createObjectInfoCloudMemberIfNot()
{
    QMutexLocker locker(m_mutex);

    if(m_objectInfoCloud == nullptr) {
        Q_ASSERT(m_pointCloudProvider != nullptr);
        if (m_pointCloudProvider == nullptr) {qDebug() << "ChunkedPointCloud::createObjectInfoCloudMemberIfNot" << ", " <<  "m_pointCloudProvider == nullptr"; return;}

        GlobalInfoCloud *gic = m_pointCloudProvider->createOrGetInfoCloud();
        Q_ASSERT(gic != nullptr);
        if (gic == nullptr) {qDebug() << "ChunkedPointCloud::createObjectInfoCloudMemberIfNot" << ", " <<  "gic == nullptr"; return;}

        m_objectInfoCloud = new ObjectFuncPointerCloudIndexed<ElementInfo, IndicesCollection>([](void* object) -> ElementInfo* { return ((ChunkedPointCloud*)object)->createOrGetFirstInfo(); },
                                                                                          this,
                                                                                          getNumberOfElementPerObject(),
                                                                                          m_indexes);

        if(m_globalInfoCloud == nullptr)
            m_globalInfoCloud = gic;
    }
}

void ChunkedPointCloud::deleteAllObjectXXXCloud()
{
    QMutexLocker locker(m_mutex);

    delete m_objectColorCloud;
    m_objectColorCloud = nullptr;

    delete m_objectNormalCloud;
    m_objectNormalCloud = nullptr;

    delete m_objectInfoCloud;
    m_objectInfoCloud = nullptr;
}
