#include "permanentscene.h"

#include "amkglapp.h"

#include "renderer/tools/bufferobjectmanager.h"

#include "visitor/applycustomfunctiontoobjectvisitor.h"

#include "tools/pointcloudattributesprovider.h"
#include "tools/facecloudattributesprovider.h"
#include "tools/edgecloudattributesprovider.h"

#include "scene/tools/objectsflagspropertymanager.h"

#include <QThread>
#include <QApplication>

PermanentScene::PermanentScene()
{
    m_visible = true;
    m_mustReduceNumberOfPointsInFastDraw = true;
    m_mustReduceNumberOfPointsInNormalDraw = false;

    m_currentColor = Qt::white;

    // add all objects renderer
    m_allRenderers.append(&m_localPointCloudRenderer);
    m_allRenderers.append(&m_ellipseRenderer);
    m_allRenderers.append(&m_boxRenderer);
    m_allRenderers.append(&m_quadsRenderer);
    m_allRenderers.append(&m_linesRenderer);
    m_allRenderers.append(&m_trianglesRenderer);
    m_allRenderers.append(&m_cylindersRenderer);
    m_allRenderers.append(&m_pyramidRenderer);
    m_allRenderers.append(&m_sphereRenderer);
    m_allRenderers.append(&m_globalPointCloudRenderer);
    m_allRenderers.append(&m_meshEdgeRenderer);
    m_allRenderers.append(&m_meshFaceRenderer);

    foreach (AbstractPermanentRenderer *r, m_allRenderers) {
        m_objectsRenderer.insert(r->getTypeOfObjectDrawn(), r);
    }

    m_objectFlagsPropertyManager = new ObjectsFlagsPropertyManager(*this);

    connect(this, SIGNAL(updateNecessary()), this, SLOT(updateInAllContext()), Qt::QueuedConnection);

    connect(this, SIGNAL(updatePointsInfoNecessary()), this, SLOT(updatePointsInfoInAllContext()), Qt::QueuedConnection);
    connect(this, SIGNAL(updatePointsColorNecessary()), this, SLOT(updatePointsColorInAllContext()), Qt::QueuedConnection);
    connect(this, SIGNAL(updatePointsNormalNecessary()), this, SLOT(updatePointsNormalInAllContext()), Qt::QueuedConnection);

    connect(this, SIGNAL(updateObjectsInfoNecessary()), this, SLOT(updateObjectsInfoInAllContext()), Qt::QueuedConnection);
    connect(this, SIGNAL(updateObjectsColorNecessary()), this, SLOT(updateObjectsColorInAllContext()), Qt::QueuedConnection);
    connect(this, SIGNAL(updateObjectsNormalNecessary()), this, SLOT(updateObjectsNormalInAllContext()), Qt::QueuedConnection);
}

PermanentScene::~PermanentScene()
{
    delete m_objectFlagsPropertyManager;
}

void PermanentScene::setAttributesAccessor(const IAttributesAccessor* accessor)
{
    foreach (AbstractPermanentRenderer *r, m_allRenderers) {
        r->setAttributesAccessor(accessor);
    }
}

AnyElementAttributesProvider* PermanentScene::getAnyElementsAttributesProvider() const
{
    return m_allRenderers.first()->getAnyElementsAttributesProvider();
}

void PermanentScene::init()
{
    foreach (AbstractPermanentRenderer *r, m_allRenderers) {
        r->init();
    }
}

void PermanentScene::addedTo(const IGraphicsDocument* doc, const QOpenGLContext* newContext)
{
    foreach (AbstractPermanentRenderer *r, m_allRenderers) {
        r->addedTo(doc, newContext);
    }
}

void PermanentScene::removedFrom(const IGraphicsDocument* doc, const QOpenGLContext* newContext)
{
    foreach (AbstractPermanentRenderer *r, m_allRenderers) {
        r->removedFrom(doc, newContext);
    }
}

void PermanentScene::updateInAllContext()
{
    if((QThread::currentThread() != qApp->thread())) {
        emit updateNecessary();
        return;
    }

    m_globalPointCloudRenderer.setMaxNumberOfPointsToDrawInFastDraw(m_globalPointCloudRenderer.getMaxNumberOfPointsToDrawInFastDraw());

    foreach (AbstractPermanentRenderer *r, m_allRenderers) {
        r->updateInAllContext();
    }

    emit changesHaveOccured();
}

void PermanentScene::updatePointsInfoInAllContext()
{
    if((QThread::currentThread() != qApp->thread())) {
        emit updatePointsInfoNecessary();
        return;
    }

    m_globalPointCloudRenderer.updateInfoInAllContext();

    emit changesHaveOccured();
}

void PermanentScene::updatePointsColorInAllContext()
{
    if((QThread::currentThread() != qApp->thread())) {
        emit updatePointsColorNecessary();
        return;
    }

    m_globalPointCloudRenderer.updateColorInAllContext();

    emit changesHaveOccured();
}

void PermanentScene::updatePointsNormalInAllContext()
{
    if((QThread::currentThread() != qApp->thread())) {
        emit updatePointsNormalNecessary();
        return;
    }

    m_globalPointCloudRenderer.updateNormalInAllContext();

    emit changesHaveOccured();
}

void PermanentScene::updateObjectsInfoInAllContext()
{
    if((QThread::currentThread() != qApp->thread())) {
        emit updateObjectsInfoNecessary();
        return;
    }

    foreach (AbstractPermanentRenderer *r, m_allRenderers) {
        r->updateInfoInAllContext();
    }

    emit changesHaveOccured();
}

void PermanentScene::updateObjectsColorInAllContext()
{
    if((QThread::currentThread() != qApp->thread())) {
        emit updateObjectsColorNecessary();
        return;
    }

    foreach (AbstractPermanentRenderer *r, m_allRenderers) {
        r->updateColorInAllContext();
    }

    emit changesHaveOccured();
}

void PermanentScene::updateObjectsNormalInAllContext()
{
    if((QThread::currentThread() != qApp->thread())) {
        emit updateObjectsNormalNecessary();
        return;
    }

    foreach (AbstractPermanentRenderer *r, m_allRenderers) {
        r->updateNormalInAllContext();
    }

    emit changesHaveOccured();
}

void PermanentScene::setVisible(bool visible)
{
    m_visible = visible;
}

void PermanentScene::setDrawModeToUse(DrawMode mode)
{
    foreach (AbstractPermanentRenderer *r, m_allRenderers) {
        r->setDrawModeToUse(mode);
    }

    emit changesHaveOccured();
}

void PermanentScene::setDrawModeToUse(Scene::ObjectType type, DrawMode mode)
{
    AbstractPermanentRenderer* r = m_objectsRenderer.value(type, NULL);

    if(r != NULL)
        r->setDrawModeToUse(mode);
    else
        Q_ASSERT_X(false, "Contact developper !", QString("Object Type (%1) not implement in this method").arg(type).toUtf8());

    emit changesHaveOccured();
}

void PermanentScene::setMustReduceNumberOfPointsInFastDraw(bool enabled)
{
    m_mustReduceNumberOfPointsInFastDraw = enabled;
}

void PermanentScene::setMustReduceNumberOfPointsInNormalDraw(bool enabled)
{
    m_mustReduceNumberOfPointsInNormalDraw = enabled;
}

bool PermanentScene::mustReduceNumberOfPointsInFastDraw() const
{
    return m_mustReduceNumberOfPointsInFastDraw;
}

bool PermanentScene::mustReduceNumberOfPointsInNormalDraw() const
{
    return m_mustReduceNumberOfPointsInNormalDraw;
}

void PermanentScene::destroyGL(const QOpenGLContext* context)
{
    foreach (AbstractPermanentRenderer *r, m_allRenderers) {
        r->destroyGL(context);
    }

    emit changesHaveOccured();
}

bool PermanentScene::isVisible() const
{
    return m_visible;
}

void PermanentScene::visitPoints(IGlobalPointsVisitor &visitor)
{
    m_globalPointCloudRenderer.visitPoints(visitor);
}

void PermanentScene::visitObjects(IObjectsVisitor &visitor)
{
    AllRenderersCollectionIterator it(m_allRenderers);

    while(it.hasNext() && visitor.mustContinueVisit())
        it.next()->visitObjects(visitor);
}

void PermanentScene::visitObjectsOfTypes(const QList<Scene::ObjectType>& types, IObjectsVisitor &visitor)
{
    foreach (const Scene::ObjectType& type, types) {
        AbstractPermanentRenderer* r = m_objectsRenderer.value(type, NULL);

        if(r != NULL) {
            r->visitObjects(visitor);

            if(!visitor.mustContinueVisit())
                return;
        }
    }
}

void PermanentScene::clearObjectsOfType(Scene::ObjectType el)
{
    AbstractPermanentRenderer* r = m_objectsRenderer.value(el, NULL);

    if(r != NULL)
        r->clearObjects();
    else
        Q_ASSERT_X(false, "Contact developper !", "Type not implement in this method");

    emit changesHaveOccured();
}

void PermanentScene::clearAllObjects()
{
    AllRenderersCollectionIterator it(m_allRenderers);

    while(it.hasNext())
        it.next()->clearObjects();

    emit changesHaveOccured();
}

size_t PermanentScene::countObject(Scene::ObjectType el) const
{
    AbstractPermanentRenderer* r = m_objectsRenderer.value(el, NULL);

    if(r != NULL)
        return r->countObjects();
    else
        Q_ASSERT_X(false, "Contact developper !", "Type not implement in this method");

    return 0;
}

void PermanentScene::setCurrentColor(const QColor &color)
{
    if(m_currentColor != color) {
        AllRenderersCollectionIterator it(m_allRenderers);

        while(it.hasNext())
            it.next()->setCurrentColor(color);

        m_currentColor = color;
    }
}

QColor PermanentScene::getCurrentColor() const
{
    return m_currentColor;
}

void PermanentScene::addPoints(const CloudIndex* indexes, PermanentItemInformation* dispatchInfos)
{
    m_globalPointCloudRenderer.addPoints(indexes, dispatchInfos);

    emit changesHaveOccured();
}

void PermanentScene::addPoint(const size_t& globalIndex, PermanentItemInformation* dispatchInfos)
{
    m_globalPointCloudRenderer.addPoint(globalIndex, dispatchInfos);
}

size_t PermanentScene::countPoints() const
{
    return m_globalPointCloudRenderer.countPoints();
}

void PermanentScene::setMaxNumberOfPointsToDrawInFastDraw(const size_t &maxNumber)
{
    m_globalPointCloudRenderer.setMaxNumberOfPointsToDrawInFastDraw(maxNumber);

    emit changesHaveOccured();
}

size_t PermanentScene::getMaxNumberOfPointsToDrawInFastDraw() const
{
    return m_globalPointCloudRenderer.getMaxNumberOfPointsToDrawInFastDraw();
}

void PermanentScene::addLocalPoint(const Eigen::Vector3d &p,
                                   IChunk **chunk,
                                   const Basic::LocalColor &color)
{
    m_localPointCloudRenderer.addPoint(p, 0, chunk, color);

    emit changesHaveOccured();
}

void PermanentScene::addEllipse(const double& cx, const double& cy, const double &cz,
                                const float& majorAxisRadius, const float& minorAxisRadius,
                                const Eigen::Vector3f& majorAxisDirection,
                                const Eigen::Vector3f& normal,
                                IChunk** chunk)
{
    m_ellipseRenderer.addEllipse(cx, cy, cz,
                                  majorAxisRadius, minorAxisRadius,
                                  majorAxisDirection, normal,
                                  0,
                                  chunk);

    emit changesHaveOccured();
}

void PermanentScene::addBox(const double &cx, const double &cy, const double &cz,
                            const float &width, const float &height, const float &length,
                            const Eigen::Vector3f &widthAxisDirection, const Eigen::Vector3f &heightAxisDirection,
                            IChunk **chunk)
{
    m_boxRenderer.addBox(cx, cy, cz,
                          width, height, length,
                          widthAxisDirection,
                          heightAxisDirection,
                          0,
                          chunk);
    emit changesHaveOccured();
}

void PermanentScene::addQuads(const double &cx, const double &cy, const double &cz,
                              const float &width, const float &length,
                              const Eigen::Vector3f &widthAxisDirection, const Eigen::Vector3f &lengthAxisDirection,
                              IChunk** chunk)
{
    m_quadsRenderer.addQuads(cx, cy, cz,
                              width, length,
                              widthAxisDirection,
                              lengthAxisDirection,
                              0,
                              chunk);
    emit changesHaveOccured();
}

void PermanentScene::addLine(const Eigen::Vector3d &p1,
                             const Eigen::Vector3d &p2,
                             IChunk** chunk,
                             const Basic::LocalColor &c1,
                             const Basic::LocalColor &c2)
{
    m_linesRenderer.addLine(p1, p2, 0, chunk, c1, c2);
}

void PermanentScene::addTriangle(const Eigen::Vector3d &p1,
                                 const Eigen::Vector3d &p2,
                                 const Eigen::Vector3d &p3,
                                 IChunk** chunk,
                                 const Basic::LocalColor &c1,
                                 const Basic::LocalColor &c2,
                                 const Basic::LocalColor &c3)
{
    m_trianglesRenderer.addTriangle(p1, p2, p3, 0, chunk, c1, c2, c3);
}

void PermanentScene::addCylinder(const double &cx, const double &cy, const double &cz,
                                 const float &width, const float &height, const float &length,
                                 const Eigen::Vector3f &widthAxisDirection, const Eigen::Vector3f &heightAxisDirection,
                                 IChunk** chunk)
{
    m_cylindersRenderer.addCylinder(cx, cy, cz,
                                    width, height, length,
                                    widthAxisDirection, heightAxisDirection,
                                    0,
                                    chunk);
}

void PermanentScene::addPyramid(const double &cx, const double &cy, const double &cz,
                                const float &width, const float &height, const float &length,
                                const Eigen::Vector3f &widthAxisDirection, const Eigen::Vector3f &heightAxisDirection,
                                IChunk **chunk)
{
    m_pyramidRenderer.addPyramid(cx, cy, cz,
                                 width, height, length,
                                 widthAxisDirection, heightAxisDirection,
                                 0,
                                 chunk);
}

void PermanentScene::addSphere(const double &cx, const double &cy, const double &cz,
                               const double &radius,
                               const double &initTheta, const double &endTheta,
                               const double &initPhi, const double &endPhi,
                               bool phiThetaInRadians,
                               IChunk **chunk)
{
    m_sphereRenderer.addSphere(cx, cy, cz,
                               radius,
                               initTheta, endTheta,
                               initPhi, endPhi,
                               phiThetaInRadians,
                               0,
                               chunk);
}

void PermanentScene::addMeshFace(const size_t &fi,
                                 const size_t &i1,
                                 const size_t &i2,
                                 const size_t &i3,
                                 IChunk **chunk)
{
    m_meshFaceRenderer.addFace(fi,
                               i1,
                               i2,
                               i3,
                               chunk);
}

void PermanentScene::addMeshEdge(const size_t &fi,
                                 const size_t &i1,
                                 const size_t &i2,
                                 IChunk **chunk)
{
    m_meshEdgeRenderer.addEdge(fi,
                               i1,
                               i2,
                               chunk);
}

void PermanentScene::preDraw(DrawInfo &info)
{
    Q_UNUSED(info)

    // If at least one buffer object has been updated
    if(AMKglEA->createOrGetBufferObjectManager(QOpenGLContext::currentContext()).processBOThatMustBeUpdated()) {
        // We update VAO
        foreach (AbstractPermanentRenderer *r, m_allRenderers) {
            r->updateOnlyVAOInAllContext();
        }
    }
}

void PermanentScene::draw(DrawInfo &info)
{
    foreach (AbstractPermanentRenderer *r, m_allRenderers) {
        r->draw(info);
    }
}

void PermanentScene::fastDraw(DrawInfo &info)
{
    foreach (AbstractPermanentRenderer *r, m_allRenderers) {
        r->fastDraw(info);
    }
}

void PermanentScene::postDraw(DrawInfo &info)
{
    foreach (AbstractPermanentRenderer *r, m_allRenderers) {
        r->postDraw(info);
    }
}

AbstractPermanentRenderer* PermanentScene::getRendererForType(Scene::ObjectType type) const
{
    return m_objectsRenderer.value(type, NULL);
}

PointCloudAttributesProvider* PermanentScene::getPointCloudAttributesProvider() const
{
    return m_globalPointCloudRenderer.getPointCloudAttributesProvider();
}

EdgeCloudAttributesProvider *PermanentScene::getEdgeCloudAttributesProvider() const
{
    return m_meshEdgeRenderer.getEdgeCloudAttributesProvider();
}

FaceCloudAttributesProvider* PermanentScene::getFaceCloudAttributesProvider() const
{
    return m_meshFaceRenderer.getFaceCloudAttributesProvider();
}

ICloudAttributesProvider* PermanentScene::getCloudAttributesProvider(Scene::ObjectType type) const
{
    if(type == Scene::PointGlobalCloud)
        return getPointCloudAttributesProvider();
    else if(type == Scene::LineGlobalCloud)
        return getEdgeCloudAttributesProvider();
    else if(type == Scene::TriangleGlobalCloud)
        return getFaceCloudAttributesProvider();

    Q_ASSERT_X(false, "PermanentScene::getCloudAttributesProvider", "Not implemented yet ! contact the developper or AMKgl");

    return NULL;
}

ObjectsFlagsPropertyManager* PermanentScene::getObjectsFlagsPropertyManager() const
{
    return m_objectFlagsPropertyManager;
}

bool PermanentScene::mustBeUpdated() const
{
    bool updated = true;

    ApplyCustomFunctionToObjectVisitor visitor;
    visitor.setContinueFunction([&updated]() {
        return (updated == true);
    });

    visitor.setVisitFunction([&updated](const IChunk* chunk) {
        if(!chunk->isUpdated())
            updated = false;
    });

    const_cast<PermanentScene*>(this)->visitObjects(visitor);

    return !updated;
}
