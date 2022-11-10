#include "abstractchunkgenericcloudobject.h"

#include "tools/pointcloudattributesprovider.h"
#include "tools/opengl/genericglrenderer.h"
#include "scene/permanentscene.h"

#define SLOW_DISPLAY_LIST           0
#define SLOW_NORMALS_DISPLAY_LIST   1

AbstractChunkGenericCloudObject::AbstractChunkGenericCloudObject(const uint &uniqueKey,
                                                                 Scene::ObjectType objectType,
                                                                 GLenum glMode,
                                                                 const quint8 &nVertexPerObject,
                                                                 const Eigen::Vector3d &coordinateSystemOffset) : SuperClass(uniqueKey,
                                                                                                                             objectType,
                                                                                                                             coordinateSystemOffset)
{
    m_cloudOfToUse = AMKgl::GCM_Object;

    m_nVertexPerObject = nVertexPerObject;

    m_glMode = glMode;
    m_nPoints = 0;
    m_nObjects = 0;

    m_objectColorCloud = nullptr;
    m_objectNormalCloud = nullptr;
    m_objectInfoCloud = nullptr;

    m_pointCloudProvider = nullptr;
    m_objectCloudProvider = nullptr;
}

AbstractChunkGenericCloudObject::~AbstractChunkGenericCloudObject()
{
    delete m_objectColorCloud;
    delete m_objectNormalCloud;
    delete m_objectInfoCloud;
}

void AbstractChunkGenericCloudObject::setAttributesAccessor(const IAttributesAccessor *accessor)
{
    SuperClass::setAttributesAccessor(accessor);

    m_pointCloudProvider = nullptr;
    m_objectCloudProvider = nullptr;

    if(accessor != nullptr) {
        m_pointCloudProvider = accessor->getPermanentScene()->getCloudAttributesProvider(Scene::PointGlobalCloud);
        m_objectCloudProvider = accessor->getPermanentScene()->getCloudAttributesProvider(getTypeOfObjectDrawn());
    }
}

void AbstractChunkGenericCloudObject::setWhichCloudToUseForObjects(GenericCloudMode cloudOf)
{
    if(cloudOf != m_cloudOfToUse) {
        lock();
        setUpdated(false);
        m_cloudOfToUse = cloudOf;
        unlock();
    }
}

GenericCloudMode AbstractChunkGenericCloudObject::getWhichCloudToUseForObjects() const
{
    return m_cloudOfToUse;
}

void AbstractChunkGenericCloudObject::setObjectsVisible(const size_t &from, const size_t &count, bool visible)
{
    if(count == 0)
        return;

    size_t end = from + count;

    Info* fi = createOrGetFirstObjectInfo();

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

bool AbstractChunkGenericCloudObject::isAtLeastOneObjectVisible(const size_t &from, const size_t &count) const
{
    if(count == 0)
        return false;

    size_t end = from + count;

    Info* fi = getFirstObjectInfo();

    if(fi == nullptr)
        return false;

    do {
        if(ObjectsFlagsTool::staticIsObjectVisible(fi[--end]))
            return true;
    } while(end != from);

    return false;
}

void AbstractChunkGenericCloudObject::clearObjects()
{
    setUpdated(false);
    setNumberOfObjects(0);
}

void AbstractChunkGenericCloudObject::resetObjects()
{
    setUpdated(false);
    internalSetNumberOfObjects(0, false);
}

size_t AbstractChunkGenericCloudObject::countObjects() const
{
    return m_nObjects;
}

quint8 AbstractChunkGenericCloudObject::getNumberOfElementPerObject() const
{
    return m_nVertexPerObject;
}

void AbstractChunkGenericCloudObject::draw(DrawInfo &info)
{
    if((m_nPoints == 0) || !isUpdated())
        return;

    DrawMode type = getNormalDrawModeUsed();

    if(type != DM_NONE) {

        const bool showNormals = (getCurrentDocument() != nullptr) && getCurrentDocument()->mustShowNormals();

        pushCoordinateSystemMatrix(info);

        if(type == DM_DISPLAY_LIST) {
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
            drawBasic(info);

            if(showNormals)
                drawBasicNormals(info);
        }

        // TODO : enabled other draw mode ;)
        /*else {
            shaders.bindAndConfigureByDefaultShader(getCurrentDocument());

            if(type == DM_RAW)
                drawRaw(info);
            else if(type == DM_VBO)
                drawVBO(info);
            else if(type == DM_VAO)
                drawVAO(info);

            shaders.releaseShader();

            if(showNormals) {

                if(shaders.bindAndConfigureByDefaultNormalsShader(getCurrentDocument())) {
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
        }*/

        popCoordinateSystemMatrix();
    }
}

void AbstractChunkGenericCloudObject::fastDraw(DrawInfo &info)
{
    // no fast draw available here
    draw(info);
}

void AbstractChunkGenericCloudObject::postDraw(DrawInfo &info)
{
    // nothing to do
    Q_UNUSED(info)
}

Basic::AbstractColorCloud* AbstractChunkGenericCloudObject::getObjectColorCloud() const
{
    return m_objectColorCloud;
}

Basic::AbstractNormalCloud* AbstractChunkGenericCloudObject::getObjectNormalCloud() const
{
    return m_objectNormalCloud;
}

Basic::AbstractInfoCloud* AbstractChunkGenericCloudObject::getObjectInfoCloud() const
{
    return m_objectInfoCloud;
}

Basic::AbstractColorCloud* AbstractChunkGenericCloudObject::createOrGetObjectColorCloud()
{
    createObjectColorCloudMemberIfNot();
    return m_objectColorCloud;
}

Basic::AbstractNormalCloud* AbstractChunkGenericCloudObject::createOrGetObjectNormalCloud()
{
    createObjectNormalCloudMemberIfNot();
    return m_objectNormalCloud;
}

Basic::AbstractInfoCloud* AbstractChunkGenericCloudObject::createOrGetObjectInfoCloud()
{
    createObjectInfoCloudMemberIfNot();
    return m_objectInfoCloud;
}

void AbstractChunkGenericCloudObject::updateInfoInCurrentContext()
{
    updateColorInCurrentContext();
}

void AbstractChunkGenericCloudObject::updateColorInCurrentContext()
{
    if((getNormalDrawModeUsed() == DM_DISPLAY_LIST)
                || (getFastDrawModeUsed() == DM_DISPLAY_LIST)) {

        GenericCloudRendererContext* context = getCurrentContext();

        if(context == nullptr)
            return;

        context->destroyAllDisplayList(this);
    }
}

void AbstractChunkGenericCloudObject::updateOnlyVAO()
{
    if((getNormalDrawModeUsed() == DM_VAO) && mustUseVAO()) {

        // TODO
    }

    if((getNormalDrawModeUsed() == DM_DISPLAY_LIST)
                || (getFastDrawModeUsed() == DM_DISPLAY_LIST)) {

        GenericCloudRendererContext* context = getCurrentContext();

        if(context == nullptr)
            return;

        context->destroyAllDisplayList(this);
    }
}

void AbstractChunkGenericCloudObject::setNumberOfObjects(const size_t &n)
{
    internalSetNumberOfObjects(n, true);
}

AbstractChunkGenericCloudObject::GlobalColorCloud* AbstractChunkGenericCloudObject::getVertexGlobalColorCloud() const
{
    Q_ASSERT(m_pointCloudProvider != nullptr);
    if (m_pointCloudProvider == nullptr) {qDebug() << "AbstractChunkGenericCloudObject::getVertexGlobalColorCloud" << ", " <<  "m_pointCloudProvider == nullptr"; return nullptr;}

    return m_pointCloudProvider->getColorCloud();
}

AbstractChunkGenericCloudObject::GlobalNormalCloud* AbstractChunkGenericCloudObject::getVertexGlobalNormalCloud() const
{
    Q_ASSERT(m_pointCloudProvider != nullptr);
    if (m_pointCloudProvider == nullptr) {qDebug() << "AbstractChunkGenericCloudObject::getVertexGlobalNormalCloud" << ", " <<  "m_pointCloudProvider == nullptr"; return nullptr;}

    return m_pointCloudProvider->getNormalCloud();
}

AbstractChunkGenericCloudObject::GlobalInfoCloud* AbstractChunkGenericCloudObject::getVertexGlobalInfoCloud() const
{
    Q_ASSERT(m_pointCloudProvider != nullptr);
    if (m_pointCloudProvider == nullptr) {qDebug() << "AbstractChunkGenericCloudObject::getVertexGlobalInfoCloud" << ", " <<  "m_pointCloudProvider == nullptr"; return nullptr;}

    return m_pointCloudProvider->getInfoCloud();
}

AbstractChunkGenericCloudObject::GlobalColorCloud *AbstractChunkGenericCloudObject::createOrGetVertexGlobalColorCloud()
{
    Q_ASSERT(m_pointCloudProvider != nullptr);
    if (m_pointCloudProvider == nullptr) {qDebug() << "AbstractChunkGenericCloudObject::createOrGetVertexGlobalColorCloud" << ", " <<  "m_pointCloudProvider == nullptr"; return nullptr;}

    return m_pointCloudProvider->createOrGetColorCloud();
}

AbstractChunkGenericCloudObject::GlobalNormalCloud *AbstractChunkGenericCloudObject::createOrGetVertexGlobalNormalCloud()
{
    Q_ASSERT(m_pointCloudProvider != nullptr);
    if (m_pointCloudProvider == nullptr) {qDebug() << "AbstractChunkGenericCloudObject::createOrGetVertexGlobalNormalCloud" << ", " <<  "m_pointCloudProvider == nullptr"; return nullptr;}

    return m_pointCloudProvider->createOrGetNormalCloud();
}

AbstractChunkGenericCloudObject::GlobalInfoCloud *AbstractChunkGenericCloudObject::createOrGetVertexGlobalInfoCloud()
{
    Q_ASSERT(m_pointCloudProvider != nullptr);
    if (m_pointCloudProvider == nullptr) {qDebug() << "AbstractChunkGenericCloudObject::createOrGetVertexGlobalInfoCloud" << ", " <<  "m_pointCloudProvider == nullptr"; return nullptr;}

    return m_pointCloudProvider->createOrGetInfoCloud();
}

FloatPoint* AbstractChunkGenericCloudObject::getFirstVertex() const
{
    return &AMKglEA->getGlobalPointCloudManager()->getFloatPointAtGlobalIndex(getBeginningOfVertexCloud());
}

AbstractChunkGenericCloudObject::Color* AbstractChunkGenericCloudObject::getFirstVertexColor() const
{
    GlobalColorCloud* cc = getVertexGlobalColorCloud();

    if(cc == nullptr)
        return nullptr;

    return &(*cc)[getBeginningOfVertexCloud()];
}

AbstractChunkGenericCloudObject::Normal* AbstractChunkGenericCloudObject::getFirstVertexNormal() const
{
    GlobalNormalCloud* nc = getVertexGlobalNormalCloud();

    if(nc == nullptr)
        return nullptr;

    return &(*nc)[getBeginningOfVertexCloud()];
}

AbstractChunkGenericCloudObject::Info* AbstractChunkGenericCloudObject::getFirstVertexInfo() const
{
    GlobalInfoCloud* ic = getVertexGlobalInfoCloud();

    if(ic == nullptr)
        return nullptr;

    return &(*ic)[getBeginningOfVertexCloud()];
}

AbstractChunkGenericCloudObject::GlobalColorCloud* AbstractChunkGenericCloudObject::getObjectGlobalColorCloud() const
{
    Q_ASSERT(m_objectCloudProvider != nullptr);
    if (m_objectCloudProvider == nullptr) {qDebug() << "AbstractChunkGenericCloudObject::getObjectGlobalColorCloud" << ", " <<  "m_objectCloudProvider == nullptr"; return nullptr;}

    return m_objectCloudProvider->getColorCloud();
}

AbstractChunkGenericCloudObject::GlobalNormalCloud* AbstractChunkGenericCloudObject::getObjectGlobalNormalCloud() const
{
    Q_ASSERT(m_objectCloudProvider != nullptr);
    if (m_objectCloudProvider == nullptr) {qDebug() << "AbstractChunkGenericCloudObject::getObjectGlobalNormalCloud" << ", " <<  "m_objectCloudProvider == nullptr"; return nullptr;}

    return m_objectCloudProvider->getNormalCloud();
}

AbstractChunkGenericCloudObject::GlobalInfoCloud* AbstractChunkGenericCloudObject::getObjectGlobalInfoCloud() const
{
    Q_ASSERT(m_objectCloudProvider != nullptr);
    if (m_objectCloudProvider == nullptr) {qDebug() << "AbstractChunkGenericCloudObject::getObjectGlobalInfoCloud" << ", " <<  "m_objectCloudProvider == nullptr"; return nullptr;}

    return m_objectCloudProvider->getInfoCloud();
}

AbstractChunkGenericCloudObject::GlobalColorCloud* AbstractChunkGenericCloudObject::createOrGetObjectGlobalColorCloud()
{
    Q_ASSERT(m_objectCloudProvider != nullptr);
    if (m_objectCloudProvider == nullptr) {qDebug() << "AbstractChunkGenericCloudObject::createOrGetObjectGlobalColorCloud" << ", " <<  "m_objectCloudProvider == nullptr"; return nullptr;}

    GlobalColorCloud* gcc = m_objectCloudProvider->createOrGetColorCloud();
    createObjectColorCloudMemberIfNot();
    return gcc;
}

AbstractChunkGenericCloudObject::GlobalNormalCloud* AbstractChunkGenericCloudObject::createOrGetObjectGlobalNormalCloud()
{
    Q_ASSERT(m_objectCloudProvider != nullptr);
    if (m_objectCloudProvider == nullptr) {qDebug() << "AbstractChunkGenericCloudObject::createOrGetObjectGlobalNormalCloud" << ", " <<  "m_objectCloudProvider == nullptr"; return nullptr;}

    GlobalNormalCloud* gnc = m_objectCloudProvider->createOrGetNormalCloud();
    createObjectNormalCloudMemberIfNot();
    return gnc;
}

AbstractChunkGenericCloudObject::GlobalInfoCloud* AbstractChunkGenericCloudObject::createOrGetObjectGlobalInfoCloud()
{
    Q_ASSERT(m_objectCloudProvider != nullptr);
    if (m_objectCloudProvider == nullptr) {qDebug() << "AbstractChunkGenericCloudObject::createOrGetObjectGlobalInfoCloud" << ", " <<  "m_objectCloudProvider == nullptr"; return nullptr;}

    GlobalInfoCloud* gic = m_objectCloudProvider->createOrGetInfoCloud();
    createObjectInfoCloudMemberIfNot();
    return gic;
}

AbstractChunkGenericCloudObject::Color* AbstractChunkGenericCloudObject::getFirstObjectColor() const
{
    GlobalColorCloud* gcc = getObjectGlobalColorCloud();

    if(gcc == nullptr)
        return nullptr;

    return &(*gcc)[getBeginningOfObjectCloud()];
}

AbstractChunkGenericCloudObject::Normal* AbstractChunkGenericCloudObject::getFirstObjectNormal() const
{
    GlobalNormalCloud* gnc = getObjectGlobalNormalCloud();

    if(gnc == nullptr)
        return nullptr;

    return &(*gnc)[getBeginningOfObjectCloud()];
}

AbstractChunkGenericCloudObject::Info *AbstractChunkGenericCloudObject::createOrGetFirstObjectInfo() const
{
    const_cast<AbstractChunkGenericCloudObject*>(this)->createObjectInfoCloudMemberIfNot();
    GlobalInfoCloud *gic = m_objectCloudProvider->createOrGetInfoCloud();
    return &(*gic)[getBeginningOfObjectCloud()];
}

void AbstractChunkGenericCloudObject::drawBasic(DrawInfo &info)
{
    Q_UNUSED(info)

    float selectionColor[4];
    getSelectionColor(selectionColor);

    float currentColor[4];
    getCurrentColor(currentColor);

    GenericGLRendererNM::IndicesFromPointer vertexIndexes(getFirstVertexIndex());
    GenericGLRendererNM::IndicesFromPointer objectIndexes(getFirstObjectIndex());

    if(getWhichCloudToUseForObjects() == AMKgl::GCM_Vertex) {
        GenericGLRenderer::staticBasicObjectVertexRender(m_glMode,
                                                         getFirstVertex(),
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
        GenericGLRenderer::staticBasicObjectRender(m_glMode,
                                                   getFirstVertex(),
                                                   createOrGetFirstObjectInfo(),
                                                   getFirstObjectColor(),
                                                   getFirstObjectNormal(),
                                                   *getFlagsPropertyManager(),
                                                   vertexIndexes,
                                                   objectIndexes,
                                                   m_nObjects,
                                                   selectionColor,
                                                   currentColor);
    }
}

void AbstractChunkGenericCloudObject::drawBasicNormals(DrawInfo &info)
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

    GenericGLRendererNM::IndicesFromPointer vertexIndexes(getFirstVertexIndex());
    GenericGLRendererNM::IndicesFromPointer objectIndexes(getFirstObjectIndex());

    if(getWhichCloudToUseForObjects() == AMKgl::GCM_Vertex) {

        GenericGLRenderer::staticBasicObjectVertexNormalRender(getFirstVertex(),
                                                               createOrGetFirstObjectInfo(),
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
        GenericGLRenderer::staticBasicObjectNormalRender(getFirstVertex(),
                                                         createOrGetFirstObjectInfo(),
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
    }
}

AbstractChunkGenericCloudObject::Info* AbstractChunkGenericCloudObject::getFirstObjectInfo() const
{
    GlobalInfoCloud* gic = getObjectGlobalInfoCloud();

    if(gic == nullptr)
        return nullptr;

    return &(*gic)[getBeginningOfObjectCloud()];
}
