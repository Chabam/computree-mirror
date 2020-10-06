#include "chunkedgeneric.h"

#include "amkglapp.h"
#include "drawinfo.h"
#include "interfaces/igraphicsdocument.h"
#include "tools/opengl/openglinfo.h"
#include "tools/opengl/genericglrenderer.h"
#include "genericobjectarrayobjectmanager.h"

#include <QOpenGLFunctions_2_0>

#define RESIZE_VECTOR_ADD 10
#define SLOW_DISPLAY_LIST           0
#define FAST_DISPLAY_LIST           1

ChunkedGeneric::ChunkedGeneric(const uint& uniqueKey,
                               Scene::ObjectType objectType,
                               GLenum glMode,
                               const quint8 &nElementPerObject,
                               const Eigen::Vector3d &offset) : Chunk<GenericRendererContext>(uniqueKey, objectType, offset)
{
    m_colors = new ColorArray();
    m_normals = nullptr;
    m_glMode = glMode;
    m_nPoints = 0;
    m_fastStride = 1;
    m_maxNumberOfPointsInFastDrawMode = 0;

    m_infosAccess = new InfoCloud(m_infos, nElementPerObject);
    m_colorsAccess = nullptr;

    if(m_colors != nullptr)
        m_colorsAccess = new ColorCloud(*m_colors, nElementPerObject);

    if(m_normals != nullptr)
        m_normalsAccess = new NormalCloud(*m_normals, nElementPerObject);

    m_mutex = new QMutex(QMutex::Recursive);
}

ChunkedGeneric::~ChunkedGeneric()
{
    delete m_infosAccess;
    delete m_colorsAccess;

    delete m_colors;
    delete m_normals;

    delete m_mutex;
}

void ChunkedGeneric::addPoint(const Eigen::Vector3f& point,
                              const ElementInfo& info,
                              const Color &color,
                              const Normal& normal)
{
    setUpdated(false);

    if(m_vertexes.size() == m_nPoints) {
        const size_t newSize = m_nPoints + RESIZE_VECTOR_ADD;

        m_vertexes.resize(newSize, point);
        m_infos.resize(newSize, info);

        if(m_colors != nullptr)
            m_colors->resize(newSize, color);

        if(m_normals != nullptr)
            m_normals->resize(newSize, normal);
    } else {
        m_vertexes[m_nPoints] = point;
        m_infos[m_nPoints] = info;

        if(m_colors != nullptr)
            (*m_colors)[m_nPoints] = color;

        if(m_normals != nullptr)
            (*m_normals)[m_nPoints] = normal;
    }

    ++m_nPoints;
}

void ChunkedGeneric::setPointsVisible(const size_t& from, const size_t& count, bool visible)
{
    if(count == 0)
        return;

    size_t end = from + count;

    Info* fi = getFirstInfo();

    if(visible) {
        do {
            ObjectsFlagsTool::staticShowObject(fi[--end]);
        } while(end != from);
    } else {
        do {
            ObjectsFlagsTool::staticHideObjectAndUnselectIt(fi[--end]);
        } while(end != from);
    }

    setUpdated(false);
}

void ChunkedGeneric::transformObjects(const Eigen::Matrix4f &trMatrix, const size_t& from, const size_t& count)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(trMatrix)
    Q_UNUSED(from)
    Q_UNUSED(count)
#endif
    /*if(count == 0)
        return;

    const size_t firstPoint = from*getNumberOfElementPerObject();
    const size_t n = count*getNumberOfElementPerObject();

    Vertex* fv = getFirstVertex();

    Eigen::Vector4d v;

    size_t end = firstPoint + n;

    do {
        v = fv[--end];
        v = v * trMatrix;
        fv[end] = v;
    } while(end != firstPoint);

    setUpdated(false);*/

    // TODO
}

void ChunkedGeneric::translateObjects(const Eigen::Vector3f &translation, const size_t& from, const size_t& count)
{
    if(count == 0)
        return;

    const size_t firstPoint = from*getNumberOfElementPerObject();
    const size_t n = count*getNumberOfElementPerObject();

    Vertex* fv = getFirstVertex();

    size_t end = firstPoint + n;

    do {
        Vertex& v = fv[--end];
        v[0] += translation[0];
        v[1] += translation[1];
        v[2] += translation[2];
    } while(end != firstPoint);

    setUpdated(false);
}

void ChunkedGeneric::rotateObjects(const Eigen::AngleAxis<float> &rotation, const size_t& from, const size_t& count)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(rotation)
    Q_UNUSED(from)
    Q_UNUSED(count)
#endif
    /*if(count == 0)
        return;

    const size_t firstPoint = from*getNumberOfElementPerObject();
    const size_t n = count*getNumberOfElementPerObject();

    Vertex* fv = getFirstVertex();

    size_t end = firstPoint + n;

    do {
        rotation.rotate(fv[--end]);
    } while(end != firstPoint);

    setUpdated(false);*/

    // TODO
}

void ChunkedGeneric::rotateObjects(const Eigen::Quaternion<float> &rotation, const size_t& from, const size_t& count)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(rotation)
    Q_UNUSED(from)
    Q_UNUSED(count)
#endif
    /*if(count == 0)
        return;

    const size_t firstPoint = from*getNumberOfElementPerObject();
    const size_t n = count*getNumberOfElementPerObject();

    Vertex* fv = getFirstVertex();

    size_t end = firstPoint + n;

    do {
        rotation.rotate(fv[--end]);
    } while(end != firstPoint);

    setUpdated(false);*/

    // TODO
}

void ChunkedGeneric::scaleObjects(const Eigen::Vector3f &scaling, const size_t& from, const size_t& count)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(scaling)
    Q_UNUSED(from)
    Q_UNUSED(count)
#endif
    /*if(count == 0)
        return;

    const size_t firstPoint = from*getNumberOfElementPerObject();
    const size_t n = count*getNumberOfElementPerObject();

    Vertex* fv = getFirstVertex();

    size_t end = firstPoint + n;

    do {
        fv[--end].scale(scaling);
    } while(end != firstPoint);

    setUpdated(false);*/

    // TODO
}

void ChunkedGeneric::setObjectsVisible(const size_t &from, const size_t &count, bool visible)
{
    if(count == 0)
        return;

    const size_t firstPoint = from*getNumberOfElementPerObject();
    const size_t n = count*getNumberOfElementPerObject();

    setPointsVisible(firstPoint, n, visible);
}

bool ChunkedGeneric::isAtLeastOneObjectVisible(const size_t &from, const size_t &count) const
{
    if(count == 0)
        return false;

    const size_t firstPoint = from*getNumberOfElementPerObject();
    const size_t n = count*getNumberOfElementPerObject();

    size_t end = firstPoint + n;

    Info* fi = getFirstInfo();

    do {
        if(ObjectsFlagsTool::staticIsObjectVisible(fi[--end]))
            return true;
    } while(end != firstPoint);

    return false;
}

void ChunkedGeneric::clearPoints()
{
    QMutexLocker locker(m_mutex);

    setUpdated(false);
    setNumberOfObjects(0);
}

void ChunkedGeneric::resetObjects()
{
    QMutexLocker locker(m_mutex);

    setUpdated(false);
    internalSetNumberOfObjects(0, false);
}

size_t ChunkedGeneric::countPoints() const
{
    return m_nPoints;
}

void ChunkedGeneric::setMaxNumberOfPointsToDrawInFastDraw(const size_t &maxNumber)
{
    if(maxNumber != m_maxNumberOfPointsInFastDrawMode)
        setUpdated(false);

    m_maxNumberOfPointsInFastDrawMode = maxNumber;
}

/*void ChunkedGeneric::changeBeginningOfCloud(const size_t &beginGlobalIndex)
{
    Q_UNUSED(beginGlobalIndex)
}*/

ChunkedGeneric::ColorCloud* ChunkedGeneric::getColorCloud() const
{
    return m_colorsAccess;
}

ChunkedGeneric::NormalCloud* ChunkedGeneric::getNormalCloud() const
{
    return m_normalsAccess;
}

ChunkedGeneric::InfoCloud* ChunkedGeneric::getInfoCloud() const
{
    return m_infosAccess;
}

void ChunkedGeneric::draw(DrawInfo &info)
{
    QMutexLocker locker(m_mutex);

    if((m_nPoints == 0) || !isUpdated())
        return;

    DrawMode type = getNormalDrawModeUsed();

    if(type != DM_NONE) {

        GenericRendererShaders& shaders = getCurrentContext()->getShaders();

        pushCoordinateSystemMatrix(info);

        if(type == DM_BASIC) {
            drawBasic(info);
        } else if(type == DM_DISPLAY_LIST) {
            if(!getCurrentContext()->executeOrCreateAndCompileAndExecuteDisplayList(this, SLOW_DISPLAY_LIST)) {
                drawBasic(info);
                getCurrentContext()->endDisplayList(this, SLOW_DISPLAY_LIST);
            }
        } else {
            shaders.bindAndConfigureByDefaultShader(getCurrentDocument(),
                                                    *getFlagsPropertyManager());

            if(type == DM_RAW)
                drawRaw(info);
            else if(type == DM_VBO)
                drawVBO(info);
            else if(type == DM_VAO)
                drawVAO(info);

            shaders.releaseShader();
        }

        popCoordinateSystemMatrix();
    }
}

void ChunkedGeneric::fastDraw(DrawInfo &info)
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

        pushCoordinateSystemMatrix(info);

        if(type == DM_DISPLAY_LIST) {
            if(!getCurrentContext()->executeOrCreateAndCompileAndExecuteDisplayList(this, FAST_DISPLAY_LIST)) {
                fastDrawBasic(info);
                getCurrentContext()->endDisplayList(this, FAST_DISPLAY_LIST);
            }
        } else {
            fastDrawBasic(info);
        }

        popCoordinateSystemMatrix();
    }
}

void ChunkedGeneric::postDraw(DrawInfo &info)
{
    Q_UNUSED(info)
}

void ChunkedGeneric::lock()
{
    m_mutex->lock();
}

void ChunkedGeneric::unlock()
{
    m_mutex->unlock();
}

bool ChunkedGeneric::internalUpdate()
{
    GenericRendererContext* context = getCurrentContext();

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

    AMKglLOG->addInfoMessage(QString("Update generic chunk that begin at index %1 and contains %2 points").arg(0).arg(nPoints));

    if((m_glMode == GL_POINTS) && (m_maxNumberOfPointsInFastDrawMode != 0)) {
        m_fastStride = (int)(nPoints/m_maxNumberOfPointsInFastDrawMode);
        m_fastStride = qMax(m_fastStride, 1);
    } else {
        m_fastStride = 1;
    }

    GenericRendererShaders& shaders = context->getShaders();

    if(shaders.getShaderProgram() == nullptr) {
        setNormalDrawModeUsed(getDrawModeToUse() == DM_BASIC ? DM_BASIC : DM_DISPLAY_LIST);
        setFastDrawModeUsed(getNormalDrawModeUsed());
        setUpdated(true);
        return true;
    }

    // NORMAL DRAW : for the moment we will use the draw mode set by user
    setNormalDrawModeUsed(getDrawModeToUse());

    // To use DRAW mode RAW or VBO or VAO with must have at least opengl function 2.0
    QOpenGLFunctions_2_0* func = context->getOpenglContext()->versionFunctions<QOpenGLFunctions_2_0>();

    // if the graphics card don't have this feature we must use the draw mode BASIC
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

        ok = shaders.bindShader();

        if(ok) {
            ok = bufferObjects.createVertexBO<Vertex>(this, GENERIC_OBJECT_VERTEX_BO_UI, [](void* object) -> void* { return ((ChunkedGeneric*)object)->getFirstVertex(); }, m_nPoints);

            if(ok) {
                ok = bufferObjects.createInfosBO<Info>(this, GENERIC_OBJECT_INFOS_BO_UI, [](void* object) -> void* { return ((ChunkedGeneric*)object)->getFirstInfo(); }, m_nPoints);

                if(ok) {
                    bufferObjects.createColorsBO<Color>(this, GENERIC_OBJECT_COLORS_BO_UI, [](void* object) -> void* { return ((ChunkedGeneric*)object)->getFirstColor(); }, m_nPoints);
                    bufferObjects.createNormalsBO<Normal>(this, GENERIC_OBJECT_NORMALS_BO_UI, [](void* object) -> void* { return ((ChunkedGeneric*)object)->getFirstNormal(); }, m_nPoints);
                }
            }

            if(!ok)
                setNormalDrawModeUsed(DM_RAW);
        } else {
            setNormalDrawModeUsed(DM_DISPLAY_LIST);
        }

        // Create Vertex Array Object
        if(ok && mustUseVAO())
            ok = context->createOrGetArrayObjectManager(this).createVertexAO(this, *context);

        //bufferObjects.releaseAll();

        shaders.releaseShader();
    }

    setUpdated(true);

    return true;
}

void ChunkedGeneric::fastDrawBasic(DrawInfo &info)
{
    Q_UNUSED(info)

    float selectionColor[4];
    getSelectionColor(selectionColor);

    float currentColor[4];
    getCurrentColor(currentColor);

    GenericGLRendererNM::DirectIndices d;

    GenericGLRenderer::staticBasicFastRender(m_glMode,
                                             getFirstVertex(),
                                             getFirstInfo(),
                                             getFirstColor(),
                                             getFirstNormal(),
                                             *getFlagsPropertyManager(),
                                             d,
                                             m_nPoints,
                                             m_fastStride,
                                             selectionColor,
                                             currentColor);
}

void ChunkedGeneric::drawBasic(DrawInfo &info)
{
    Q_UNUSED(info)

    float selectionColor[4];
    getSelectionColor(selectionColor);

    float currentColor[4];
    getCurrentColor(currentColor);

    GenericGLRendererNM::DirectIndices d;

    GenericGLRenderer::staticBasicRender(m_glMode,
                                         getFirstVertex(),
                                         getFirstInfo(),
                                         getFirstColor(),
                                         getFirstNormal(),
                                         *getFlagsPropertyManager(),
                                         d,
                                         m_nPoints,
                                         selectionColor,
                                         currentColor);
}

void ChunkedGeneric::drawRaw(DrawInfo &info)
{
    Q_UNUSED(info)

    size_t size = m_nPoints;

    Vertex* fv = getFirstVertex();
    Color* fc = getFirstColor();
    Info* fi = getFirstInfo();
    Normal* fn = getFirstNormal();

    GenericRendererContext* context = getCurrentContext();

    context->getShaders().setVertexAttributeToShader(fv);

    if(fc != nullptr)
        context->getShaders().setColorAttributeToShader(fc);
    else
        context->getShaders().setUseColorAttribute(false);

    if(fn != nullptr)
        context->getShaders().setNormalAttributeToShader(fn);
    else
        context->getShaders().setUseNormalAttribute(false);

    context->getShaders().setInfoAttributeToShader(fi, context->getOpenglContext());

    callDrawElements(size);

    context->getShaders().disableAttributeOfShader();
}

void ChunkedGeneric::drawVBO(DrawInfo &info)
{
    Q_UNUSED(info)

    GenericRendererContext* context = getCurrentContext();

    GenericRendererShaders& shaders = context->getShaders();
    BufferObjectManager& buffers = context->getBufferObjectManager();

    bool ok = false;

    size_t size = m_nPoints;

    if(shaders.bindAndSetVertexAttributeToShader(buffers.getVertexBO(this, GENERIC_OBJECT_VERTEX_BO_UI))) {
        ok = shaders.bindAndSetInfoAttributeToShader(buffers.getInfosBO(this, GENERIC_OBJECT_INFOS_BO_UI), context->getOpenglContext());

        if(ok) {
            shaders.bindAndSetColorAttributeToShader(buffers.getColorsBO(this, GENERIC_OBJECT_COLORS_BO_UI));
            shaders.bindAndSetNormalAttributeToShader(buffers.getNormalsBO(this, GENERIC_OBJECT_NORMALS_BO_UI));

            callDrawElements(size);
        }

        shaders.disableAttributeOfShader();
    }

    if(!ok) {
        context->getShaders().releaseShader();
        drawBasic(info);
    }
}

void ChunkedGeneric::drawVAO(DrawInfo &info)
{
    Q_UNUSED(info)

    GenericObjectArrayObjectManager& aoManager = getCurrentContext()->createOrGetArrayObjectManager(this);

    bool ok = false;

    size_t size = m_nPoints;

    if(aoManager.canUseVertexAO()) {

        if(aoManager.bindVertexAO()) {

            ok = true;

            callDrawElements(size);

            aoManager.releaseVertexAO();
        }
    }

    if(!ok)
        drawVBO(info);
}

void ChunkedGeneric::callDrawElements(const size_t &size)
{
    glDrawArrays(m_glMode, 0, (GLsizei)size);
}

void ChunkedGeneric::setUpdated(bool status)
{
    if(mustChangeUpdateStatus(status)) {
        QMutexLocker locker(m_mutex);

        Chunk<GenericRendererContext>::setUpdated(status);
    }
}

void ChunkedGeneric::moveObjectsFromTo(const size_t &from, const size_t &to, const size_t &size)
{
    size_t fromM = from*getNumberOfElementPerObject();
    size_t sizeM = size*getNumberOfElementPerObject();
    size_t toM = to*getNumberOfElementPerObject();

    std::memcpy(&((*(m_vertexes.begin()+toM))(0)), &((*(m_vertexes.begin()+fromM))(0)), sizeof(Vertex)*sizeM);
    std::memcpy(&(*(m_infos.begin()+toM)), &(*(m_infos.begin()+fromM)), sizeof(Info)*sizeM);

    if(m_colors != nullptr)
        std::memcpy(&((*(m_colors->begin()+toM))(0)), &((*(m_colors->begin()+fromM))(0)), sizeof(Color)*sizeM);

    if(m_normals != nullptr)
        std::memcpy(&((*(m_normals->begin()+toM))(0)), &((*(m_normals->begin()+fromM))(0)), sizeof(Normal)*sizeM);
}

void ChunkedGeneric::setNumberOfObjects(const size_t &n)
{
    internalSetNumberOfObjects(n, true);
}

void ChunkedGeneric::internalSetNumberOfObjects(const size_t &n, const bool &shrinkToFit)
{
    m_nPoints = n*getNumberOfElementPerObject();

    m_vertexes.resize(m_nPoints);

    if(shrinkToFit)
        m_vertexes.shrink_to_fit();

    m_infos.resize(m_nPoints);

    if(shrinkToFit)
        m_infos.shrink_to_fit();

    if(m_colors != nullptr) {
        m_colors->resize(m_nPoints);

        if(shrinkToFit)
            m_colors->shrink_to_fit();
    }

    if(m_normals != nullptr) {
        m_normals->resize(m_nPoints);

        if(shrinkToFit)
            m_normals->shrink_to_fit();
    }
}

bool ChunkedGeneric::isUpdated() const
{
    if(!Chunk<GenericRendererContext>::isUpdated())
        return false;

    GenericRendererContext* context = getCurrentContext();

    if(context == nullptr)
        return true;

    ChunkCustomUpdateValues* cuv = context->createOrGetChunkCustomUpdateValues(this);

    if(cuv->getUserValue("MaxNumberOfPointsInFastDrawMode", QVariant::fromValue(m_maxNumberOfPointsInFastDrawMode)).value<size_t>() != m_maxNumberOfPointsInFastDrawMode)
        return false;

    if(cuv->getUserValue("NPoints", QVariant::fromValue(m_nPoints)).value<size_t>() != m_nPoints)
        return false;

    return true;
}

void ChunkedGeneric::updateOnlyVAO()
{
    if((getNormalDrawModeUsed() == DM_VAO) && mustUseVAO()) {

        GenericRendererContext* context = getCurrentContext();

        if(context == nullptr)
            return;

        BufferObjectManager& bufferObjects = context->getBufferObjectManager();

        if(getColorCloud() != nullptr && !bufferObjects.getColorsBO(this, GENERIC_OBJECT_COLORS_BO_UI).isCreated())
            setUpdated(false);

        if(isUpdated()) {
            GenericRendererShaders& shaders = context->getShaders();
            GenericObjectArrayObjectManager& aoManager = getCurrentContext()->createOrGetArrayObjectManager(this);

            context->makeCurrent();

            aoManager.updateVertexAO(this, *context);

            //bufferObjects.releaseAll();

            shaders.releaseShader();
        } else {
            internalUpdate();
        }
    }

    if((getNormalDrawModeUsed() == DM_DISPLAY_LIST)
              || (getFastDrawModeUsed() == DM_DISPLAY_LIST)) {

        GenericRendererContext* context = getCurrentContext();

        if(context == nullptr)
          return;

        context->destroyAllDisplayList(this);
    }
}

void ChunkedGeneric::updateInfoBO()
{
    if(((getNormalDrawModeUsed() == DM_VBO)
            || (getNormalDrawModeUsed() == DM_VAO)) && mustUseVBO()) {

        if(getCurrentContext() == nullptr)
            return;

        BufferObjectManager& buffers = getCurrentContext()->getBufferObjectManager();
        buffers.updateInfosBO(getFirstInfo(), m_nPoints);
    }
}

void ChunkedGeneric::updateColorBO()
{
    if(((getNormalDrawModeUsed() == DM_VBO)
            || (getNormalDrawModeUsed() == DM_VAO)) && mustUseVBO()) {

        if(getCurrentContext() == nullptr)
            return;

        BufferObjectManager& buffers = getCurrentContext()->getBufferObjectManager();
        buffers.updateColorsBO(getFirstColor(), m_nPoints);
    }
}

void ChunkedGeneric::updateNormalBO()
{
    if(((getNormalDrawModeUsed() == DM_VBO)
            || (getNormalDrawModeUsed() == DM_VAO)) && mustUseVBO()) {

        if(getCurrentContext() == nullptr)
            return;

        BufferObjectManager& buffers = getCurrentContext()->getBufferObjectManager();
        buffers.updateNormalsBO(getFirstNormal(), m_nPoints);
    }
}

quint8 ChunkedGeneric::getNumberOfElementPerObject() const
{
    return m_infosAccess->getNumberOfElementPerObject();
}

ChunkedGeneric::VertexArray* ChunkedGeneric::getVertexArray() const
{
    return &const_cast<VertexArray&>(m_vertexes);
}

ChunkedGeneric::InfoArray* ChunkedGeneric::getInfoArray() const
{
    return &const_cast<InfoArray&>(m_infos);
}

ChunkedGeneric::ColorArray *ChunkedGeneric::getColorArray() const
{
    return m_colors;
}

ChunkedGeneric::NormalArray* ChunkedGeneric::getNormalArray() const
{
    return m_normals;
}

ChunkedGeneric::Vertex* ChunkedGeneric::getFirstVertex() const
{
    if(m_vertexes.empty())
        return nullptr;

    return &(const_cast<VertexArray&>(m_vertexes)[0]);
}

ChunkedGeneric::Info* ChunkedGeneric::getFirstInfo() const
{
    if(m_infos.empty())
        return nullptr;

    return &(const_cast<InfoArray&>(m_infos)[0]);
}

ChunkedGeneric::Color* ChunkedGeneric::getFirstColor() const
{
    if((m_colors == nullptr) || m_colors->empty())
        return nullptr;

    return &((*m_colors)[0]);
}

ChunkedGeneric::Normal* ChunkedGeneric::getFirstNormal() const
{
    if((m_normals == nullptr) || m_normals->empty())
        return nullptr;

    return &((*m_normals)[0]);
}

void ChunkedGeneric::setContextUpdated(GenericRendererContext *context, bool status)
{
    Chunk<GenericRendererContext>::setContextUpdated(context, status);

    ChunkCustomUpdateValues* cuv = context->createOrGetChunkCustomUpdateValues(this);
    cuv->setUserValue("MaxNumberOfPointsInFastDrawMode", QVariant::fromValue(m_maxNumberOfPointsInFastDrawMode));
    cuv->setUserValue("NPoints", QVariant::fromValue(m_nPoints));
}
