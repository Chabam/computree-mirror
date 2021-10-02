#include "actionpickanyelements.h"

#include "amkglapp.h"
#include "elementinfo.h"

#include "interfaces/igraphicsdocument.h"
#include "interfaces/iglobalpointcloudmanager.h"

#include "scene/permanentitemscenebymodel.h"

#include "picker/polygonpointspickerbymodel.h"
#include "picker/polygonitemspickerbymodel.h"
#include "picker/polygonmeshobjectspickerbymodel.h"
#include "picker/tools/pointspickingdefaultfunction.h"
#include "picker/tools/itemspickingdefaultfunction.h"
#include "picker/tools/objectsflagstool.h"

#include "tools/opengl/shapetovolume.h"
#include "tools/polygonforpicking.h"
#include "tools/rectangleforpicking.h"
#include "tools/pointcloudattributesprovider.h"


ActionPickAnyElements::ActionPickAnyElements()
{
    m_pickType = PT_None;
    m_selectionType = ST_Select;
    m_maxElementsToPick = 0;
    m_glViewer = nullptr;
    m_polygonPickingTools = new PolygonForPicking();
    m_rectanglePickingTools = new RectangleForPicking();

    connect(m_polygonPickingTools, SIGNAL(mustBeRedraw()), this, SLOT(redrawOverlay()), Qt::DirectConnection);
    connect(m_polygonPickingTools, SIGNAL(polygonChanged()), this, SIGNAL(valuesOfShapeChanged()), Qt::DirectConnection);

    connect(m_rectanglePickingTools, SIGNAL(mustBeRedraw()), this, SLOT(redrawOverlay()), Qt::DirectConnection);
    connect(m_rectanglePickingTools, SIGNAL(polygonChanged()), this, SIGNAL(valuesOfShapeChanged()), Qt::DirectConnection);
}

ActionPickAnyElements::~ActionPickAnyElements()
{
    delete m_polygonPickingTools;
    delete m_rectanglePickingTools;
}

void ActionPickAnyElements::setGlViewer(const Viewer *w)
{
    m_glViewer = (Viewer*)w;
}

void ActionPickAnyElements::setPickType(ActionPickAnyElements::PickType t)
{
    if(m_pickType == t)
        return;

    m_pickType = t;

    emit pickingTypeChanged();
    emit pickingActivated(t != PT_None);

    m_polygonPickingTools->clear();
    m_rectanglePickingTools->clear();
}

ActionPickAnyElements::PickType ActionPickAnyElements::getPickType() const
{
    return m_pickType;
}

void ActionPickAnyElements::setElementToPickType(ActionPickAnyElements::ElementToPick t)
{
    if(m_elementToPickType == t)
        return;

    m_elementToPickType = t;
    emit elementToPickTypeChanged();
}

ActionPickAnyElements::ElementToPick ActionPickAnyElements::getElementToPickType() const
{
    return m_elementToPickType;
}

void ActionPickAnyElements::setMaxNumberOfElementsToPick(const size_t &max)
{
    m_maxElementsToPick = max;
}

size_t ActionPickAnyElements::getMaxNumberOfElementsToPick() const
{
    return m_maxElementsToPick;
}

void ActionPickAnyElements::setSelectionType(ActionPickAnyElements::SelectionType t)
{
    m_selectionType = t;
}

ActionPickAnyElements::SelectionType ActionPickAnyElements::getSelectionType() const
{
    return m_selectionType;
}

void ActionPickAnyElements::setPolygon(const QPolygon &p)
{
    setPickType(PT_Polygone);
    m_polygonPickingTools->setPolygon(p);
}

void ActionPickAnyElements::setRectangle(const QRect &r)
{
    setPickType(PT_Rectangle);
    m_rectanglePickingTools->setRectangle(r);
}

QPolygon ActionPickAnyElements::getPolygon() const
{
    if(getPickType() == PT_Polygone)
        return m_polygonPickingTools->getPolygon();
    else if(getPickType() == PT_Rectangle)
        return m_rectanglePickingTools->getPolygon();

    return QPolygon();
}

QRect ActionPickAnyElements::getRectangle() const
{
    if(getPickType() == PT_Polygone)
        return m_polygonPickingTools->getPolygon().boundingRect();
    else if(getPickType() == PT_Rectangle)
        return m_rectanglePickingTools->getPolygon().boundingRect();

    return QRect();
}

QPolygon ActionPickAnyElements::getLastPolygon() const
{
    return m_lastPolygon;
}

bool ActionPickAnyElements::mousePressEvent(QMouseEvent *e)
{
    if(getPickType() == PT_Polygone)
        return m_polygonPickingTools->mousePressEvent(e);
    else if(getPickType() == PT_Rectangle)
        return m_rectanglePickingTools->mousePressEvent(e);

    return false;
}

bool ActionPickAnyElements::mouseMoveEvent(QMouseEvent *e)
{
    Q_UNUSED(e)

    if(getPickType() == PT_Polygone)
        return m_polygonPickingTools->mouseMoveEvent(e);
    else if(getPickType() == PT_Rectangle)
        return m_rectanglePickingTools->mouseMoveEvent(e);

    return false;
}

bool ActionPickAnyElements::mouseReleaseEvent(QMouseEvent *e)
{
    if(getPickType() == PT_Polygone)
        return m_polygonPickingTools->mouseReleaseEvent(e);
    else if(getPickType() == PT_Rectangle)
        return m_rectanglePickingTools->mouseReleaseEvent(e);

    return false;
}

bool ActionPickAnyElements::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(getPickType() == PT_Polygone)
        return m_polygonPickingTools->mouseDoubleClickEvent(e);
    else if(getPickType() == PT_Rectangle)
        return m_rectanglePickingTools->mouseDoubleClickEvent(e);

    return false;
}

bool ActionPickAnyElements::wheelEvent(QWheelEvent *e)
{
    if(getPickType() == PT_Polygone)
        return m_polygonPickingTools->wheelEvent(e);
    else if(getPickType() == PT_Rectangle)
        return m_rectanglePickingTools->wheelEvent(e);

    return false;
}

bool ActionPickAnyElements::keyPressEvent(QKeyEvent *e)
{
    if(getPickType() == PT_Polygone)
        return m_polygonPickingTools->keyPressEvent(e);
    else if(getPickType() == PT_Rectangle)
        return m_rectanglePickingTools->keyPressEvent(e);

    return false;
}

bool ActionPickAnyElements::keyReleaseEvent(QKeyEvent *e)
{
    if(getPickType() == PT_Polygone) {

        if(e->key() == Qt::Key_Delete) {

            m_polygonPickingTools->removeLastPoint();
            return true;

        } else if((e->key() == Qt::Key_Enter)
                  || (e->key() == Qt::Key_Return)) {

            if(!m_polygonPickingTools->isPolygonClosed()) {
                m_polygonPickingTools->closePolygon();
                return true;
            }

        }
    }

    if((getPickType() == PT_Rectangle)
            || (getPickType() == PT_Polygone)) {

        if(e->key() == Qt::Key_Escape) {
            setPickType(PT_None);
            return true;
        } else if((e->key() == Qt::Key_Enter)
                || (e->key() == Qt::Key_Return)) {

            pick();

            setPickType(PT_None);
            return true;
        }
    }

    return false;
}

void ActionPickAnyElements::drawOverlay(QPainter &painter, DrawInfo &info)
{
    if(getPickType() == PT_Polygone)
        m_polygonPickingTools->drawOverlay(painter, info);
    else if(getPickType() == PT_Rectangle)
        m_rectanglePickingTools->drawOverlay(painter, info);
}

bool ActionPickAnyElements::pick()
{
    bool ok = false;

    if(m_glViewer->getDocument() != nullptr) {
        if(getPickType() == PT_Polygone)
            ok = m_polygonPickingTools->closePolygon();
        else if(getPickType() == PT_Rectangle)
            ok = (getRectangle().width() > 0) && (getRectangle().height() > 0);

        if(ok) {
            m_lastPolygon = getPolygon();

            if(getElementToPickType() == ETP_Points) {

                ok = false;

                if(!AMKglEA->getGlobalPointCloudManager()->isEmpty()) {
                    Eigen::Matrix4d modelViewMatrix;
                    m_glViewer->camera()->getModelViewMatrix(modelViewMatrix);

                    PolygonPointsPickerByModel picker(ShapeToVolume::staticTriangulatePolygonAndConvertItToPlanes(getPolygon(),
                                                                                                                  *m_glViewer->camera()));

                    PointsPicker::selectionOperation selectOperation = &PointsPickingDefaultFunction::staticSelectPoint;
                    PointsPicker::checkOperation checkOperation = &PointsPickingDefaultFunction::staticCheckSelectPoint;

                    if(m_selectionType == ST_Unselect) {
                        selectOperation = &PointsPickingDefaultFunction::staticUnselectPoint;
                        checkOperation = &PointsPickingDefaultFunction::staticCheckUnselectPoint;
                    }

                    size_t nSelected = 0;
                    QMutex mutex;

                    if(m_maxElementsToPick > 0) {
                        picker.setSelectionOperation([&](const FloatPoint &p,
                                                         ElementInfo* firstInfo,
                                                         const size_t &indexContainedInSelection) {
                            mutex.lock();
                            ++nSelected;

                            if(nSelected > this->m_maxElementsToPick) {
                                mutex.unlock();
                                return;
                            }

                            mutex.unlock();

                            selectOperation(p, firstInfo, indexContainedInSelection);
                        });

                        picker.setCheckOperation([&](const FloatPoint &p,
                                                     ElementInfo* firstInfo,
                                                     const size_t &indexContainedInSelection) -> bool {
                            if(nSelected >= m_maxElementsToPick)
                                return false;

                            return checkOperation(p, firstInfo, indexContainedInSelection);
                        });
                    } else {
                        picker.setSelectionOperation(selectOperation);
                        picker.setCheckOperation(checkOperation);
                    }

                    picker.setCameraPosition(m_glViewer->camera()->position());
                    picker.setCameraModelViewMatrix(modelViewMatrix);

                    m_glViewer->getPermanentSceneToRender()->visitPoints(picker);

                    picker.compute();

                    m_glViewer->getPermanentSceneToRender()->updatePointsInfoInAllContext();

                    ok = true;
                }

            } else if(getElementToPickType() == ETP_Items) {
                if(ok) {
                    Eigen::Matrix4d modelViewMatrix;
                    m_glViewer->camera()->getModelViewMatrix(modelViewMatrix);

                    PolygonItemsPickerByModel picker(ShapeToVolume::staticConvertPolygonToPlanes(getPolygon(), *m_glViewer->camera()),
                                                     ShapeToVolume::staticTriangulatePolygonAndConvertItToPlanes(getPolygon(), *m_glViewer->camera()));

                    picker.setGraphicsView(m_glViewer->getGraphicsView());

                    ItemsPicker::selectionOperation selectOperation = &ItemsPickingDefaultFunction::staticSelectItem;
                    ItemsPicker::checkOperation checkOperation = &ItemsPickingDefaultFunction::staticCheckSelectItem;

                    if(m_selectionType == ST_Unselect) {
                        selectOperation = &ItemsPickingDefaultFunction::staticUnselectItem;
                        checkOperation = &ItemsPickingDefaultFunction::staticCheckUnselectItem;
                    }

                    size_t nSelected = 0;
                    QMutex mutex;

                    if(m_maxElementsToPick > 0) {
                        picker.setSelectionOperation([&](Item* item,
                                                         PermanentItemInformation *info,
                                                         PermanentItemScene* scene,
                                                         AnyElementAttributesProvider* provider) {
                            mutex.lock();
                            ++nSelected;

                            if(nSelected > this->m_maxElementsToPick) {
                                mutex.unlock();
                                return;
                            }

                            mutex.unlock();

                            selectOperation(item, info, scene, provider);
                        });

                        picker.setCheckOperation([&](const Item* item,
                                                     const PermanentItemInformation *info,
                                                     const PermanentItemScene* scene,
                                                     const AnyElementAttributesProvider* provider) -> bool {
                            if(nSelected >= this->m_maxElementsToPick)
                                return false;

                            return checkOperation(item, info, scene, provider);
                        });
                    } else {
                        picker.setSelectionOperation(selectOperation);
                        picker.setCheckOperation(checkOperation);
                    }

                    picker.setCameraPosition(m_glViewer->camera()->position());
                    picker.setCameraModelViewMatrix(modelViewMatrix);

                    m_glViewer->getPermanentSceneToRender()->visitItems(picker);

                    picker.compute();

                    m_glViewer->getPermanentSceneToRender()->updateItemsInfoInAllContext();
                }
            } else if((getElementToPickType() == ETP_Faces) || (getElementToPickType() == ETP_Edges)) {

                Eigen::Matrix4d modelViewMatrix;
                m_glViewer->camera()->getModelViewMatrix(modelViewMatrix);

                PolygonMeshObjectsPickerByModel* picker = nullptr;
                QList<Scene::ObjectType> objectToVisit;

                QVector<Plane> polygonPlanes = ShapeToVolume::staticConvertPolygonToPlanes(getPolygon(), *m_glViewer->camera());
                QVector<TrianglePlane> triangulatedPlanes = ShapeToVolume::staticTriangulatePolygonAndConvertItToPlanes(getPolygon(), *m_glViewer->camera());

                if(getElementToPickType() == ETP_Edges) {
                    picker = new PolygonMeshObjectsPickerByModel(polygonPlanes,
                                                                 triangulatedPlanes,
                                                                 2);

                    objectToVisit.append(Scene::LineGlobalCloud);
                } else {
                    picker = new PolygonMeshObjectsPickerByModel(polygonPlanes,
                                                                 triangulatedPlanes,
                                                                 3);
                    objectToVisit.append(Scene::TriangleGlobalCloud);
                }

                MeshObjectsPicker::selectionOperation selectOperation = &ObjectsFlagsTool::staticSelectObject<ElementInfo>;
                MeshObjectsPicker::checkOperation checkOperation = &ObjectsFlagsTool::staticCheckSelectObject<ElementInfo>;

                if(m_selectionType == ST_Unselect) {
                    selectOperation = &ObjectsFlagsTool::staticUnselectObject<ElementInfo>;
                    checkOperation = &ObjectsFlagsTool::staticCheckUnselectObject<ElementInfo>;
                }

                size_t nSelected = 0;
                QMutex mutex;

                if(m_maxElementsToPick > 0) {
                    picker->setSelectionOperation([&](ElementInfo& objectInfo) {
                        mutex.lock();
                        ++nSelected;

                        if(nSelected > this->m_maxElementsToPick) {
                            mutex.unlock();
                            return;
                        }

                        mutex.unlock();

                        selectOperation(objectInfo);
                    });

                    picker->setCheckOperation([&](ElementInfo& objectInfo) -> bool {
                        if(nSelected >= m_maxElementsToPick)
                            return false;

                        return checkOperation(objectInfo);
                    });
                } else {
                    picker->setSelectionOperation(selectOperation);
                    picker->setCheckOperation(checkOperation);
                }

                picker->setCameraPosition(m_glViewer->camera()->position());
                picker->setCameraModelViewMatrix(modelViewMatrix);

                m_glViewer->getPermanentSceneToRender()->visitObjectsOfTypes(objectToVisit, *picker);

                picker->compute();

                delete picker;

                m_glViewer->getPermanentSceneToRender()->updatePointsInfoInAllContext();

                ok = true;
            }

            if(ok)
                redrawAll();
        }
    }

    return ok;
}

void ActionPickAnyElements::redrawOverlay()
{
    if(m_glViewer != nullptr)
        m_glViewer->redraw(Viewer::RedrawOnlyOverlay);
}

void ActionPickAnyElements::redrawAll()
{
    if(m_glViewer != nullptr)
        m_glViewer->redraw(Viewer::RedrawAll);
}
