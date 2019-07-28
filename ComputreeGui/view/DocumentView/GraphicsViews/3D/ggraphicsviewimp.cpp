#include "ggraphicsviewimp.h"

#include "scene/permanentitemscenebymodel.h"
#include "scene/tools/objectsflagspropertymanager.h"
#include "tools/pointcloudattributesprovider.h"
#include "tools/edgecloudattributesprovider.h"
#include "tools/facecloudattributesprovider.h"
#include "picker/tools/pointspickingdefaultfunction.h"
#include "picker/tools/objectsflagstool.h"
#include "actions/picking/actionpickanyelements.h"
#include "tools/qglviewer/qglviewertools.h"
#include "includepoint.h"

#include "dm_guimanager.h"
#include "Camera/g3dcameracontroller.h"
#include "visitor/globalpointssetinfovisitor.h"
#include "visitor/itemsselectedvisitor.h"
#include "visitor/meshobjectstestinfovisitor.h"
#include "visitor/meshobjectssetinfovisitor.h"
#include "visitor/meshobjectsvisitor.h"
#include "visitor/selectitemsvisitor.h"
#include "visitor/toggleitemsselectionvisitor.h"
#include "visitor/applycustomfunctiontoitemvisitor.h"
#include "visitor/applycustomfunctiontoobjectvisitor.h"
#include "visitor/applycustomfunctiontoglobalpointsvisitor.h"
#include "tools/amkgl/dm_objectsvisitorfollowerbymodel.h"
#include "tools/amkgl/dm_objectsvisitorfollowerforaction.h"
#include "tools/amkgl/dm_globalpointsmodifier.h"
#include "tools/amkgl/dm_meshobjectsmodifier.h"
#include "tools/amkgl/dm_multipleobjectsmodifier.h"
#include "tools/amkgl/dm_amkglpermanentpainter.h"
#include "tools/itemcolorizer.h"

#include "ct_cloudindex/ct_cloudindexstdvectort.h"
#include "ct_colorcloud/registered/ct_standardcolorcloudregistered.h"
#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"
#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include "ct_accessor/ct_pointaccessor.h"

#include "view/DocumentView/gdocumentviewforgraphics.h"
#include "view/Tools/graphicsviewdebugmode.h"

#include "constraint.h"
#include "manipulatedCameraFrame.h"

#include "Eigen/Core"

QString GGraphicsViewImp::DEFAULT_STATE_FILENAME = "3DViewer.config";
uint GGraphicsViewImp::NUMBER_OF_VIEWS = 0;

GGraphicsViewImp::GGraphicsViewImp(const GDocumentViewForGraphics* parentDocument, QWidget* parent) : AMKglViewer(parentDocument, parent), GGraphicsView()
{
    m_document = (GDocumentViewForGraphics*)parentDocument;

    m_camController = new G3DCameraController();
    m_camController->setRealCameraManipulator(amkglCamera());
    m_camController->setView(this);

    m_backupPointsSelected = CT_SPCIR(nullptr);
    m_backupEdgesSelected = CT_SECIR(nullptr);
    m_backupFacesSelected = CT_SFCIR(nullptr);

    m_uniqueIndex = NUMBER_OF_VIEWS++;

    connect(actionsHandler(), SIGNAL(currentActionChanged(CT_AbstractAction*)), this, SLOT(currentActionChanged(CT_AbstractAction*)));
    connect(this, SIGNAL(debugModeChanged(bool)), this, SLOT(amkglDebugModeChanged(bool)), Qt::DirectConnection);
}

GGraphicsViewImp::~GGraphicsViewImp()
{
    saveStateToFile();

    m_sceneForAction.removedFrom(getDocument(), getNewOpenGlContext());

    delete m_camController;
}

void GGraphicsViewImp::active2DView(bool e)
{
    if(e) {
        qglviewer::LocalConstraint* c = new qglviewer::LocalConstraint();
        c->setTranslationConstraintType(qglviewer::AxisPlaneConstraint::FREE);
        c->setRotationConstraintType(qglviewer::AxisPlaneConstraint::FORBIDDEN);

        amkglCamera()->frame()->setConstraint(c);

        m_camController->setType(CameraInterface::ORTHOGRAPHIC);

    } else {
        amkglCamera()->frame()->setConstraint(nullptr);
    }
}

void GGraphicsViewImp::disablePointsSelectionBackup()
{
    m_backupPointsSelected = CT_SPCIR(nullptr);
}

void GGraphicsViewImp::disableEdgesSelectionBackup()
{
    m_backupEdgesSelected = CT_SECIR(nullptr);
}

void GGraphicsViewImp::disableFacesSelectionBackup()
{
    m_backupFacesSelected = CT_SFCIR(nullptr);
}

GraphicsViewInterface* GGraphicsViewImp::graphicsView() const
{
    return const_cast<GGraphicsViewImp*>(this);
}

QOpenGLWidget* GGraphicsViewImp::getOpenGLWidget() const
{
    return const_cast<GGraphicsViewImp*>(this);
}

bool GGraphicsViewImp::is2DView() const
{
    return (amkglCamera()->frame()->constraint() != nullptr);
}

void GGraphicsViewImp::setOptions(const DM_GraphicsViewOptions &newOptions)
{
    if(newOptions != constGetOptionsInternal())
    {
        GGraphicsView::setOptions(newOptions);

        m_camController->setType(newOptions.getCameraType());

        setAxisIsDrawn(newOptions.drawAxis());
        setGridIsDrawn(newOptions.drawGrid());
        setFPSIsDisplayed(newOptions.getCameraInformationDisplayed().testFlag(DM_GraphicsViewOptions::FpsInformation));

        if(getPermanentSceneToRender() != nullptr) {
            bool update = false;

            // set all draw mode to use
            if((*getPermanentSceneToRender()) != newOptions) {
                update = true;
                (*getPermanentSceneToRender()) = newOptions;
            }

            if(getPermanentSceneToRender()->getMaxNumberOfPointsToDrawInFastDraw() != newOptions.getMaxNbrPointsInFastDraw()) {
                update = true;
                getPermanentSceneToRender()->setMaxNumberOfPointsToDrawInFastDraw(newOptions.getMaxNbrPointsInFastDraw());
            }

            if(getPermanentSceneToRender()->mustReduceNumberOfPointsInFastDraw() != newOptions.mustReduceNbrPointsInFastDraw()) {
                update = true;
                getPermanentSceneToRender()->setMustReduceNumberOfPointsInFastDraw(newOptions.mustReduceNbrPointsInFastDraw());
            }

            if(getPermanentSceneToRender()->mustReduceNumberOfPointsInNormalDraw() != newOptions.mustReduceNbrPointsInNormalDraw()) {
                update = true;
                getPermanentSceneToRender()->setMustReduceNumberOfPointsInNormalDraw(newOptions.mustReduceNbrPointsInNormalDraw());
            }

            if(update)
                getPermanentSceneToRender()->updateInAllContext();
        }

        m_document->setOptions(newOptions);

        update();
    }
}

void GGraphicsViewImp::lockPaint()
{
}

void GGraphicsViewImp::unlockPaint()
{
}

void GGraphicsViewImp::redraw()
{
    m_sceneForAction.checkAndUpdateIfItNeedsToBe();

    AMKglViewer::redraw(AMKglViewer::RedrawAll);
}

DM_GraphicsViewCamera* GGraphicsViewImp::getCamera() const
{
    return m_camController;
}

void GGraphicsViewImp::takeAndSaveScreenshot()
{
    saveSnapshot(false, false);
}

void GGraphicsViewImp::updateDrawing3DOfItemDrawables(const QList<CT_AbstractItemDrawable *> &list)
{
    // TODO : use the correct model
    DM_DocumentView* doc = getDocumentView();

    PermanentItemScene* scene = getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr);

    foreach (CT_AbstractItemDrawable* item, list) {
        QColor color = doc->getColor(item);
        color.setAlpha(255);
        scene->updateItem(item, *this, color);
    }

    scene->updateInAllContext();
}

void GGraphicsViewImp::updateItemDrawablesThatColorWasModified()
{
    ItemColorizer colorizer;
    colorizer.setSceneToVisit(getPermanentSceneToRender());

    m_document->visitItemsThatColorWasModified([&colorizer](const CT_AbstractItemDrawable* item,
                                                            const QColor& color) {
        QColor mColor = color;
        mColor.setAlpha(255);

        colorizer.setItem(item);
        colorizer.setColor(mColor);
        colorizer.apply();
    });

    getPermanentSceneToRender()->updateColorInAllContext();
}

void GGraphicsViewImp::updateColorOfPointsOfItemDrawable(CT_AbstractItemDrawable *item, const QColor &color)
{
    // TODO
}

void GGraphicsViewImp::getBoundingBoxOfAllItemDrawablePresentInView(Eigen::Vector3d &min, Eigen::Vector3d &max) const
{
    ApplyCustomFunctionToItemVisitor visitor;

    Eigen::Array3d minArray = Eigen::Array3d(1,1,1);
    Eigen::Array3d maxArray = Eigen::Array3d(1,1,1);

    minArray *= std::numeric_limits<double>::max();
    maxArray *= -std::numeric_limits<double>::max();

    visitor.setFunction([&minArray, &maxArray](const AMKgl::Item* item,
                            const AMKgl::ItemModel* model,
                            const PermanentItemScene* scene) {
        Q_UNUSED(model)
        Q_UNUSED(scene)

        if(item->doesBoundingBoxAppearToBeValid()) {

            Eigen::Vector3d tmpMin, tmpMax;

            item->boundingBox(tmpMin, tmpMax);

            if(tmpMin[0] <= tmpMax[0]) {
                minArray = minArray.min(tmpMin.array());
                maxArray = maxArray.max(tmpMax.array());
            }
        }
    } );

    getPermanentSceneToRender()->visitItems(visitor);

    if(minArray[0] > maxArray[0]
            || minArray[1] > maxArray[1]
            || minArray[2] > maxArray[2]) {
        min = Eigen::Vector3d(0,0,0);
        max = Eigen::Vector3d(0,0,0);
    } else {
        min = minArray;
        max = maxArray;
    }
}

void GGraphicsViewImp::getBoundingBoxOfAllItemDrawableSelectedInView(Eigen::Vector3d &min, Eigen::Vector3d &max) const
{
    ApplyCustomFunctionToItemVisitor visitor;

    Eigen::Array3d minArray = Eigen::Array3d(1,1,1);
    Eigen::Array3d maxArray = Eigen::Array3d(1,1,1);

    minArray *= std::numeric_limits<double>::max();
    maxArray *= -std::numeric_limits<double>::max();

    visitor.setFunction([&minArray, &maxArray](const AMKgl::Item* item,
                            const AMKgl::ItemModel* model,
                            const PermanentItemScene* scene) {
        Q_UNUSED(model)
        Q_UNUSED(scene)

        if(item->isSelected() && item->doesBoundingBoxAppearToBeValid()) {
            Eigen::Vector3d tmpMin, tmpMax;

            item->boundingBox(tmpMin, tmpMax);

            if(tmpMin[0] <= tmpMax[0]) {
                minArray = minArray.min(tmpMin.array());
                maxArray = maxArray.max(tmpMax.array());
            }
        }
    } );

    getPermanentSceneToRender()->visitItems(visitor);

    min = minArray;
    max = maxArray;
}

void GGraphicsViewImp::getBoundingBoxOfAllPointsSelectedInView(Eigen::Vector3d &min, Eigen::Vector3d &max) const
{
    min.setConstant(std::numeric_limits<double>::max());
    max = -min;

    PermanentItemScene* scene = getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr);

    if(scene != nullptr) {

        GlobalInfoCloud* infos = scene->getPointCloudAttributesProvider()->getInfoCloud();

        if(infos != nullptr) {

            ApplyCustomFunctionToGlobalPointsVisitor visitor;
            visitor.setFunction([&min, &max, &infos](IChunk* chunk,
                                                     const Eigen::Vector3d &offset,
                                                     std::vector<GLuint> &indices,
                                                     const size_t& firstPointGlobalIndex) {
                Q_UNUSED(chunk)
                Q_UNUSED(offset)

                CT_PointAccessor pAccessor;
                CT_Point point;
                ElementInfo* firstInfo = &(*infos)[firstPointGlobalIndex];

                std::vector<GLuint>::const_iterator it = indices.begin();
                std::vector<GLuint>::const_iterator end = indices.end();

                quint8 j;

                while(it != end) {
                    if(ObjectsFlagsTool::staticIsObjectSelected(firstInfo[*it])) {
                        pAccessor.pointAt(firstPointGlobalIndex+(*it), point);

                        for(j=0; j<3; ++j) {
                            min[j] = qMin(min[j], point[j]);
                            max[j] = qMax(max[j], point[j]);
                        }
                    }

                    ++it;
                }
            });

            scene->visitPoints(visitor);
        }
    }
}

void GGraphicsViewImp::getBoundingBoxOfAllEdgesSelectedInView(Eigen::Vector3d &min, Eigen::Vector3d &max) const
{
    getBoundingBoxOfSelectedElementsOfMeshesInView(Scene::LineGlobalCloud, min, max);
}

void GGraphicsViewImp::getBoundingBoxOfAllFacesSelectedInView(Eigen::Vector3d &min, Eigen::Vector3d &max) const
{
    getBoundingBoxOfSelectedElementsOfMeshesInView(Scene::TriangleGlobalCloud, min, max);
}

void GGraphicsViewImp::setVisible(const CT_AbstractItemDrawable *item, bool e)
{
    ApplyCustomFunctionToItemVisitor visitor;
    visitor.setFunction([=](const AMKgl::Item* item,
                           const AMKgl::ItemModel* model,
                           const PermanentItemScene* scene) {
        Q_UNUSED(model)

        ((PermanentItemScene*)scene)->setItemVisible(item, e);
    });

    getPermanentSceneToRender()->visitOneItem(item, visitor);
}

bool GGraphicsViewImp::isVisible(const CT_AbstractItemDrawable *item) const
{
    bool visible = false;

    ApplyCustomFunctionToItemVisitor visitor;
    visitor.setContinueVisitFunction([&visible]() -> bool {
        return !visible;
    });

    visitor.setFunction([&visible](const AMKgl::Item* item,
                           const AMKgl::ItemModel* model,
                           const PermanentItemScene* scene) {
        Q_UNUSED(model)

        if(scene->isItemVisible(item))
            visible = true;
    });

    getPermanentSceneToRender()->visitOneItem(item, visitor);

    return visible;
}

DocumentInterface* GGraphicsViewImp::document() const
{
    return m_document;
}

GraphicsViewSignalEmitterInterface* GGraphicsViewImp::signalEmitter() const
{
    return &const_cast<GGraphicsViewImpSignalEmitter&>(m_signalEmitter);
}

GraphicsViewOptionsInterface& GGraphicsViewImp::getOptions()
{
    return m_options;
}

QSharedPointer<CT_StandardColorCloudRegistered> GGraphicsViewImp::colorCloudOf(GraphicsViewInterface::ColorCloudType type) const
{
    // TODO : problem here because we have multiple points attributes provider !
    if(type == GraphicsViewInterface::CPointCloud)
        return getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getPointCloudAttributesProvider()->createOrGetColorCloudRegistered();

    return QSharedPointer<CT_StandardColorCloudRegistered>();
}

QSharedPointer<CT_StandardNormalCloudRegistered> GGraphicsViewImp::normalCloudOf(GraphicsViewInterface::NormalCloudType type) const
{
    // TODO : problem here because we have multiple points attributes provider !
    if(type == GraphicsViewInterface::NPointCloud)
        return getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getPointCloudAttributesProvider()->createOrGetNormalCloudRegistered();

    return QSharedPointer<CT_StandardNormalCloudRegistered>();
}

void GGraphicsViewImp::setColorOfPoint(const size_t &globalIndexOfPoint, const CT_Color &color)
{
    // TODO : problem here because we have multiple points attributes provider !
    (*getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getPointCloudAttributesProvider()->createOrGetColorCloud())[globalIndexOfPoint] = color;
}

CT_Color &GGraphicsViewImp::colorOfPoint(const size_t &globalIndexOfPoint)
{
    // TODO : problem here because we have multiple points attributes provider !
    return (*getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getPointCloudAttributesProvider()->createOrGetColorCloud())[globalIndexOfPoint];
}

const CT_Color &GGraphicsViewImp::constColorOfPoint(const size_t &globalIndexOfPoint) const
{
    // TODO : problem here because we have multiple points attributes provider !
    return (*getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getPointCloudAttributesProvider()->createOrGetColorCloud())[globalIndexOfPoint];
}

void GGraphicsViewImp::setNormalOfPoint(const size_t &globalIndexOfPoint, const CT_Normal &normal)
{
    // TODO : problem here because we have multiple points attributes provider !
    (*getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getPointCloudAttributesProvider()->createOrGetNormalCloud())[globalIndexOfPoint] = normal;
}

CT_Normal &GGraphicsViewImp::normalOfPoint(const size_t &globalIndexOfPoint)
{
    // TODO : problem here because we have multiple points attributes provider !
    return (*getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getPointCloudAttributesProvider()->createOrGetNormalCloud())[globalIndexOfPoint];
}

const CT_Normal &GGraphicsViewImp::constNormalOfPoint(const size_t &globalIndexOfPoint) const
{
    // TODO : problem here because we have multiple points attributes provider !
    return (*getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getPointCloudAttributesProvider()->createOrGetNormalCloud())[globalIndexOfPoint];
}

void GGraphicsViewImp::dirtyColorsOfItemDrawablesWithPoints()
{
    getPermanentSceneToRender()->updateInAllContext();
}

void GGraphicsViewImp::dirtyNormalsOfItemDrawablesWithPoints()
{
    getPermanentSceneToRender()->updateInAllContext();
}

ObjectsFlagProperty toAmkglFlagProperty(const GraphicsObjectFlagProperty& property) {
    return ObjectsFlagProperty(property.onColor);
}

GraphicsObjectFlagProperty toComputreeFlagProperty(const ObjectsFlagProperty& property) {
    GraphicsObjectFlagProperty p;
    p.onColor = property.objectColorWhenFlagIsON;

    return p;
}

void GGraphicsViewImp::setFlagProperty(const GraphicsObjectFlag &flag, const GraphicsObjectFlagProperty &property)
{
    getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getPermanentScene()->getObjectsFlagsPropertyManager()->setProperty(ObjectsFlagsTool::Flag(flag), toAmkglFlagProperty(property));
}

void GGraphicsViewImp::setFlagProperty(const GraphicsObjectFlag &flag, const GraphicsObjectType &typeOfObject, const GraphicsObjectFlagProperty &property)
{
    getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getPermanentScene()->getObjectsFlagsPropertyManager()->setProperty(ObjectsFlagsTool::Flag(flag), Scene::ObjectType(typeOfObject), toAmkglFlagProperty(property));
}

GraphicsObjectFlagProperty GGraphicsViewImp::getFlagProperty(const GraphicsObjectFlag &flag, const GraphicsObjectType &typeOfObject) const
{
    return toComputreeFlagProperty(getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getPermanentScene()->getObjectsFlagsPropertyManager()->getProperty(ObjectsFlagsTool::Flag(flag), Scene::ObjectType(typeOfObject)));
}

GraphicsObjectFlagProperty GGraphicsViewImp::getFlagProperty(const GraphicsObjectFlag &flag, bool &ok) const
{
    return toComputreeFlagProperty(getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getPermanentScene()->getObjectsFlagsPropertyManager()->getProperty(ObjectsFlagsTool::Flag(flag), ok));
}

CameraInterface *GGraphicsViewImp::camera() const
{
    return m_camController;
}

GraphicsViewInterface::GraphicsViewType GGraphicsViewImp::type() const
{
    return GraphicsViewInterface::GV3D;
}

void GGraphicsViewImp::setSelectionMode(GraphicsViewInterface::SelectionMode mode)
{
    m_pickingConfiguration.mode = mode;
}

GraphicsViewInterface::SelectionMode GGraphicsViewImp::selectionMode() const
{
    return m_pickingConfiguration.mode;
}

bool GGraphicsViewImp::mustSelectPoints() const
{
    return (selectionMode() >= GraphicsViewInterface::SELECT_POINTS) && (selectionMode() <= GraphicsViewInterface::REMOVE_ONE_POINT);
}

bool GGraphicsViewImp::mustSelectEdges() const
{
    return (selectionMode() >= GraphicsViewInterface::SELECT_EDGES) && (selectionMode() <= GraphicsViewInterface::REMOVE_ONE_EDGE);
}

bool GGraphicsViewImp::mustSelectFaces() const
{
    return (selectionMode() >= GraphicsViewInterface::SELECT_FACES) && (selectionMode() <= GraphicsViewInterface::REMOVE_ONE_FACE);
}

bool GGraphicsViewImp::mustSelectItems() const
{
    return (selectionMode() >= GraphicsViewInterface::SELECT) && (selectionMode() <= GraphicsViewInterface::REMOVE_ONE);
}

void GGraphicsViewImp::setSelectRegionWidth(int width)
{
    m_pickingConfiguration.rect.setWidth(width);
}

void GGraphicsViewImp::setSelectRegionHeight(int height)
{
    m_pickingConfiguration.rect.setHeight(height);
}

void GGraphicsViewImp::select(const QPoint &point)
{
    m_pickingConfiguration.rect.moveCenter(point);
    select(m_pickingConfiguration.rect);
}

void GGraphicsViewImp::select(const QPolygon& polygon)
{
    GraphicsViewInterface::SelectionMode basicMode = staticToBasicSelectionMode(selectionMode());

    getPickingAction()->setMaxNumberOfElementsToPick((basicMode == GraphicsViewInterface::SELECT_ONE) ? 1 : 0);
    getPickingAction()->setSelectionType(ActionPickAnyElements::ST_Select);

    if((basicMode == GraphicsViewInterface::SELECT)
           || (basicMode == GraphicsViewInterface::SELECT_ONE)) {
        setAllItemsSelected(false);
    }

    if((basicMode == GraphicsViewInterface::REMOVE)
           || (basicMode == GraphicsViewInterface::REMOVE_ONE)) {
        getPickingAction()->setSelectionType(ActionPickAnyElements::ST_Unselect);
    }

    if(mustSelectPoints()) {
        disablePointsSelectionBackup();
        getPickingAction()->setElementToPickType(ActionPickAnyElements::ETP_Points);
    } else if(mustSelectEdges()) {
        disableEdgesSelectionBackup();
        getPickingAction()->setElementToPickType(ActionPickAnyElements::ETP_Edges);
    } else if(mustSelectFaces()) {
        disableFacesSelectionBackup();
        getPickingAction()->setElementToPickType(ActionPickAnyElements::ETP_Faces);
    } else
        getPickingAction()->setElementToPickType(ActionPickAnyElements::ETP_Items);

    getPickingAction()->setPolygon(polygon);
    getPickingAction()->pick();

    getPickingAction()->setPickType(ActionPickAnyElements::PT_None);
}

CT_SPCIR GGraphicsViewImp::getSelectedPoints() const
{
    if(m_backupPointsSelected.isNull()) {
        GlobalPointsTestInfoVisitor< CT_CloudIndexStdVectorT<CT_PointData> > vis;
        vis.setFunction(ObjectsFlagsTool::staticIsObjectSelected<ElementInfo>);
        vis.getFoundedPoints()->setSortType(CT_AbstractCloudIndex::NotSorted);
        getPermanentSceneToRender()->visitPoints(vis);

        const_cast<GGraphicsViewImp*>(this)->m_backupPointsSelected = PS_REPOSITORY->registerPointCloudIndex(vis.takeFoundedPoints());
    }

    return m_backupPointsSelected;
}

CT_SFCIR GGraphicsViewImp::getSelectedFaces() const
{
    if(m_backupFacesSelected.isNull()) {
        MeshObjectsTestInfoVisitor< CT_CloudIndexStdVectorT<CT_Face> > vis(Scene::TriangleGlobalCloud);
        vis.setFunction(ObjectsFlagsTool::staticIsObjectSelected<ElementInfo>);
        vis.getFoundedObjects()->setSortType(CT_AbstractCloudIndex::NotSorted);

        vis.compute(getPermanentSceneToRender());

        const_cast<GGraphicsViewImp*>(this)->m_backupFacesSelected = PS_REPOSITORY->registerFaceCloudIndex(vis.takeFoundedObjects());
    }

    return m_backupFacesSelected;
}

CT_SECIR GGraphicsViewImp::getSelectedEdges() const
{
    if(m_backupEdgesSelected.isNull()) {
        MeshObjectsTestInfoVisitor< CT_CloudIndexStdVectorT<CT_Edge> > vis(Scene::LineGlobalCloud);
        vis.setFunction(ObjectsFlagsTool::staticIsObjectSelected<ElementInfo>);
        vis.getFoundedObjects()->setSortType(CT_AbstractCloudIndex::NotSorted);

        vis.compute(getPermanentSceneToRender());

        const_cast<GGraphicsViewImp*>(this)->m_backupEdgesSelected = PS_REPOSITORY->registerEdgeCloudIndex(vis.takeFoundedObjects());
    }

    return m_backupEdgesSelected;
}

QList<CT_AbstractItemDrawable *> GGraphicsViewImp::getSelectedItems() const
{
    ItemsSelectedVisitor vis;
    vis.setSelectionValue(true);
    getPermanentSceneToRender()->visitItems(vis);

    return vis.getFoundedItems();
}

void GGraphicsViewImp::addPointsIDToSelection(const size_t &id)
{
    disablePointsSelectionBackup();

    ObjectsFlagsTool::staticSelectObject((*getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getPointCloudAttributesProvider()->createOrGetInfoCloud())[id]);
}

void GGraphicsViewImp::addMultiplePointsIDToSelection(const std::vector<size_t> &idList)
{
    disablePointsSelectionBackup();

    ElementInfo* firstInfo = &(*getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getPointCloudAttributesProvider()->createOrGetInfoCloud())[0];

    std::vector<size_t>::const_iterator it = idList.begin();
    std::vector<size_t>::const_iterator end = idList.end();

    while(it != end) {
        ObjectsFlagsTool::staticSelectObject(firstInfo[*it]);
        ++it;
    }
}

void GGraphicsViewImp::addFacesIDToSelection(const size_t &id)
{
    disableFacesSelectionBackup();

    ObjectsFlagsTool::staticSelectObject((*getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getFaceCloudAttributesProvider()->createOrGetInfoCloud())[id]);
}

void GGraphicsViewImp::addMultipleFacesIDToSelection(const std::vector<size_t> &idList)
{
    disableFacesSelectionBackup();

    ElementInfo* firstInfo = &(*getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getFaceCloudAttributesProvider()->createOrGetInfoCloud())[0];

    std::vector<size_t>::const_iterator it = idList.begin();
    std::vector<size_t>::const_iterator end = idList.end();

    while(it != end) {
        ObjectsFlagsTool::staticSelectObject(firstInfo[*it]);
        ++it;
    }
}

void GGraphicsViewImp::addEdgesIDToSelection(const size_t &id)
{
    disableEdgesSelectionBackup();

    ObjectsFlagsTool::staticSelectObject((*getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getEdgeCloudAttributesProvider()->createOrGetInfoCloud())[id]);
}

void GGraphicsViewImp::addMultipleEdgesIDToSelection(const std::vector<size_t> &idList)
{
    disableEdgesSelectionBackup();

    ElementInfo* firstInfo = &(*getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getEdgeCloudAttributesProvider()->createOrGetInfoCloud())[0];

    std::vector<size_t>::const_iterator it = idList.begin();
    std::vector<size_t>::const_iterator end = idList.end();

    while(it != end) {
        ObjectsFlagsTool::staticSelectObject(firstInfo[*it]);
        ++it;
    }
}

void GGraphicsViewImp::removePointsIDFromSelection(const size_t &id)
{
    disablePointsSelectionBackup();

    AMKgl::GlobalInfoCloud* infoCloud = getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getPointCloudAttributesProvider()->getInfoCloud();

    if(infoCloud != nullptr)
        ObjectsFlagsTool::staticUnselectObject((*infoCloud)[id]);
}

void GGraphicsViewImp::removeMultiplePointsIDFromSelection(const std::vector<size_t> &idList)
{
    disablePointsSelectionBackup();

    AMKgl::GlobalInfoCloud* infoCloud = getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getPointCloudAttributesProvider()->getInfoCloud();

    if(infoCloud != nullptr) {
        ElementInfo* firstInfo = &((*infoCloud)[0]);

        std::vector<size_t>::const_iterator it = idList.begin();
        std::vector<size_t>::const_iterator end = idList.end();

        while(it != end) {
            ObjectsFlagsTool::staticUnselectObject(firstInfo[*it]);
            ++it;
        }
    }
}

void GGraphicsViewImp::removeFacesIDFromSelection(const size_t &id)
{
    disableFacesSelectionBackup();

    AMKgl::GlobalInfoCloud* infoCloud = getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getFaceCloudAttributesProvider()->getInfoCloud();

    if(infoCloud != nullptr)
        ObjectsFlagsTool::staticUnselectObject((*infoCloud)[id]);
}

void GGraphicsViewImp::removeMultipleFacesIDFromSelection(const std::vector<size_t> &idList)
{
    disableFacesSelectionBackup();

    AMKgl::GlobalInfoCloud* infoCloud = getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getFaceCloudAttributesProvider()->getInfoCloud();

    if(infoCloud != nullptr) {
        ElementInfo* firstInfo = &((*infoCloud)[0]);

        std::vector<size_t>::const_iterator it = idList.begin();
        std::vector<size_t>::const_iterator end = idList.end();

        while(it != end) {
            ObjectsFlagsTool::staticUnselectObject(firstInfo[*it]);
            ++it;
        }
    }
}

void GGraphicsViewImp::removeEdgesIDFromSelection(const size_t &id)
{
    disableEdgesSelectionBackup();

    AMKgl::GlobalInfoCloud* infoCloud = getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getEdgeCloudAttributesProvider()->getInfoCloud();

    if(infoCloud != nullptr)
        ObjectsFlagsTool::staticUnselectObject((*infoCloud)[id]);
}

void GGraphicsViewImp::removeMultipleEdgesIDFromSelection(const std::vector<size_t> &idList)
{
    disableEdgesSelectionBackup();

    AMKgl::GlobalInfoCloud* infoCloud = getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->getEdgeCloudAttributesProvider()->getInfoCloud();

    if(infoCloud != nullptr) {
        ElementInfo* firstInfo = &((*infoCloud)[0]);

        std::vector<size_t>::const_iterator it = idList.begin();
        std::vector<size_t>::const_iterator end = idList.end();

        while(it != end) {
            ObjectsFlagsTool::staticUnselectObject(firstInfo[*it]);
            ++it;
        }
    }
}

void GGraphicsViewImp::setAllPointsSelected(bool select)
{
    disablePointsSelectionBackup();

    GlobalPointsSetInfoVisitor vis;

    if(select)
        vis.setFunction(ObjectsFlagsTool::staticSelectObject<ElementInfo>);
    else
        vis.setFunction(ObjectsFlagsTool::staticUnselectObject<ElementInfo>);

    getPermanentSceneToRender()->visitPoints(vis);

    getPermanentSceneToRender()->updateInfoInAllContext();
}

void GGraphicsViewImp::setAllFacesSelected(bool select)
{
    disableFacesSelectionBackup();

    MeshObjectsSetInfoVisitor vis(Scene::TriangleGlobalCloud);

    if(select)
        vis.setFunction(ObjectsFlagsTool::staticSelectObject<ElementInfo>);
    else {
        vis.setCreateInfoItNotCreated(false);
        vis.setFunction(ObjectsFlagsTool::staticUnselectObject<ElementInfo>);
    }

    vis.compute(getPermanentSceneToRender());

    getPermanentSceneToRender()->updateInfoInAllContext();
}

void GGraphicsViewImp::setAllEdgesSelected(bool select)
{
    disableEdgesSelectionBackup();

    MeshObjectsSetInfoVisitor vis(Scene::LineGlobalCloud);

    if(select)
        vis.setFunction(ObjectsFlagsTool::staticSelectObject<ElementInfo>);
    else {
        vis.setCreateInfoItNotCreated(false);
        vis.setFunction(ObjectsFlagsTool::staticUnselectObject<ElementInfo>);
    }

    vis.compute(getPermanentSceneToRender());

    getPermanentSceneToRender()->updateInfoInAllContext();
}

void GGraphicsViewImp::setAllItemsSelected(bool select)
{
    SelectItemsVisitor vis;
    vis.setSelectionValue(select);

    getPermanentSceneToRender()->visitItems(vis);

    setAllPointsSelected(false);
    setAllEdgesSelected(false);
    setAllFacesSelected(false);

    getPermanentSceneToRender()->updateInfoInAllContext();
}

void GGraphicsViewImp::toggleItemsSelected()
{
    disablePointsSelectionBackup();

    ToggleItemsSelectionVisitor vis;
    getPermanentSceneToRender()->visitItems(vis);

    getPermanentSceneToRender()->updateInfoInAllContext();
}

void GGraphicsViewImp::togglePointsSelected()
{
    disablePointsSelectionBackup();

    GlobalPointsSetInfoVisitor vis;
    vis.setFunction(ObjectsFlagsTool::staticToggleObjectSelection<ElementInfo>);

    getPermanentSceneToRender()->visitPoints(vis);

    getPermanentSceneToRender()->updateInfoInAllContext();
}

void GGraphicsViewImp::toggleFacesSelected()
{
    disableFacesSelectionBackup();

    MeshObjectsSetInfoVisitor vis(Scene::TriangleGlobalCloud);
    vis.setFunction(ObjectsFlagsTool::staticToggleObjectSelection<ElementInfo>);

    vis.compute(getPermanentSceneToRender());

    getPermanentSceneToRender()->updateInfoInAllContext();
}

void GGraphicsViewImp::toggleEdgesSelected()
{
    disableFacesSelectionBackup();

    MeshObjectsSetInfoVisitor vis(Scene::LineGlobalCloud);
    vis.setFunction(ObjectsFlagsTool::staticToggleObjectSelection<ElementInfo>);

    vis.compute(getPermanentSceneToRender());

    getPermanentSceneToRender()->updateInfoInAllContext();
}

size_t GGraphicsViewImp::countPoints()
{
    return getPermanentSceneToRender()->countPoints();
}

size_t GGraphicsViewImp::countEdges()
{
    size_t count;

    ApplyCustomFunctionToObjectVisitor visitor;

    visitor.setVisitFunction([&count](const IChunk* chunk) {
        count += chunk->countObjects();
    });

    getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->visitObjectsOfTypes(QList<Scene::ObjectType>() << Scene::LineGlobalCloud, visitor);

    return count;
}

size_t GGraphicsViewImp::countFaces()
{
    size_t count;

    ApplyCustomFunctionToObjectVisitor visitor;

    visitor.setVisitFunction([&count](const IChunk* chunk) {
        count += chunk->countObjects();
    });

    getPermanentSceneToRender()->getPermanentItemSceneForModel(nullptr)->visitObjectsOfTypes(QList<Scene::ObjectType>() << Scene::TriangleGlobalCloud, visitor);

    return count;
}

size_t GGraphicsViewImp::countItems()
{
    return getPermanentSceneToRender()->countAllItems();
}

void GGraphicsViewImp::visitObjects(ObjectsVisitorInterface &visitor)
{
    disablePointsSelectionBackup();

    DM_ObjectsVisitorFollowerByModel f(visitor);

    getPermanentSceneToRender()->visitObjects(f);
}

void GGraphicsViewImp::visitObjects(const QList<GraphicsObjectType> &types,
                                    ObjectsVisitorInterface &visitor)
{
    disablePointsSelectionBackup();

    DM_ObjectsVisitorFollowerByModel f(visitor);

    QList<Scene::ObjectType> amkglTypes;

    foreach (GraphicsObjectType type, types) {
        if(((int)type) < ((int)Scene::NumberOfElements))
            amkglTypes.append((Scene::ObjectType)type);
    }

    getPermanentSceneToRender()->visitObjectsOfTypes(amkglTypes, f);
}

QSharedPointer<ObjectsModifierInterface> GGraphicsViewImp::createObjectsModifier(GraphicsObjectType type)
{
    if(type == GOT_PointGlobalCloud) {
        disablePointsSelectionBackup();
        return QSharedPointer<ObjectsModifierInterface>(new DM_GlobalPointsModifier(getPermanentSceneToRender(), nullptr));
    } else if(type == GOT_LineGlobalCloud) {
        disableEdgesSelectionBackup();
        return QSharedPointer<ObjectsModifierInterface>(new DM_MeshObjectsModifier(getPermanentSceneToRender(), Scene::LineGlobalCloud, nullptr));
    } else if(type == GOT_TriangleGlobalCloud) {
        disableFacesSelectionBackup();
        return QSharedPointer<ObjectsModifierInterface>(new DM_MeshObjectsModifier(getPermanentSceneToRender(), Scene::TriangleGlobalCloud, nullptr));
    }

    return QSharedPointer<ObjectsModifierInterface>(new DM_MultipleObjectsModifier(getPermanentSceneToRender(), (Scene::ObjectType)type, nullptr));
}

QSharedPointer<ObjectStatusQueryiorInterface> GGraphicsViewImp::createObjectsStatusQueryior(GraphicsObjectType type)
{
    if(type == GOT_PointGlobalCloud) {
        return QSharedPointer<ObjectStatusQueryiorInterface>(new DM_GlobalPointsModifier(getPermanentSceneToRender(), nullptr, false));
    } else if(type == GOT_LineGlobalCloud) {
        return QSharedPointer<ObjectStatusQueryiorInterface>(new DM_MeshObjectsModifier(getPermanentSceneToRender(), Scene::LineGlobalCloud, nullptr, false));
    } else if(type == GOT_TriangleGlobalCloud) {
        return QSharedPointer<ObjectStatusQueryiorInterface>(new DM_MeshObjectsModifier(getPermanentSceneToRender(), Scene::TriangleGlobalCloud, nullptr, false));
    }

    return QSharedPointer<ObjectStatusQueryiorInterface>(new DM_MultipleObjectsModifier(getPermanentSceneToRender(), (Scene::ObjectType)type, nullptr, false));
}

void GGraphicsViewImp::visitObjectsOfItem(const CT_AbstractItemDrawable *item,
                                          ObjectsVisitorInterface &visitor)
{
    disablePointsSelectionBackup();

    DM_ObjectsVisitorFollowerByModel f(visitor);

    getPermanentSceneToRender()->visitObjectsOfOneItem(item, f);
}

void GGraphicsViewImp::visitObjectsOfItem(const CT_AbstractItemDrawable *item,
                                          const QList<GraphicsObjectType> &types,
                                          ObjectsVisitorInterface &visitor)
{
    disablePointsSelectionBackup();

    DM_ObjectsVisitorFollowerByModel f(visitor);

    QList<Scene::ObjectType> amkglTypes;

    foreach (GraphicsObjectType type, types) {
        if(((int)type) < ((int)Scene::NumberOfElements))
            amkglTypes.append((Scene::ObjectType)type);
    }

    getPermanentSceneToRender()->visitObjectsOfTypesOfOneItem(item, amkglTypes, f);
}

QSharedPointer<ObjectsModifierInterface> GGraphicsViewImp::createObjectsModifierForItem(const CT_AbstractItemDrawable *item, GraphicsObjectType type)
{
    if(type == GOT_PointGlobalCloud) {
        disablePointsSelectionBackup();
        return QSharedPointer<ObjectsModifierInterface>(new DM_GlobalPointsModifier(getPermanentSceneToRender(), item));
    } else if(type == GOT_LineGlobalCloud) {
        disableEdgesSelectionBackup();
        return QSharedPointer<ObjectsModifierInterface>(new DM_MeshObjectsModifier(getPermanentSceneToRender(), Scene::LineGlobalCloud, item));
    } else if(type == GOT_TriangleGlobalCloud) {
        disableFacesSelectionBackup();
        return QSharedPointer<ObjectsModifierInterface>(new DM_MeshObjectsModifier(getPermanentSceneToRender(), Scene::TriangleGlobalCloud, item));
    }

    return QSharedPointer<ObjectsModifierInterface>(new DM_MultipleObjectsModifier(getPermanentSceneToRender(), (Scene::ObjectType)type, item));
}

void GGraphicsViewImp::visitObjectsOfCurrentAction(const int &uniqueIndex, ObjectsVisitorInterface &visitor)
{
    DM_ObjectsVisitorFollowerForAction f(visitor);

    if(uniqueIndex != -1)
        f.setCurrentInformation(m_sceneForAction.getElementInformation(uniqueIndex));

    m_sceneForAction.getPermanentScene()->visitObjects(f);
}

void GGraphicsViewImp::visitObjectsOfCurrentAction(const int &uniqueIndex, const QList<GraphicsObjectType> &types, ObjectsVisitorInterface &visitor)
{
    DM_ObjectsVisitorFollowerForAction f(visitor);

    if(uniqueIndex != -1)
        f.setCurrentInformation(m_sceneForAction.getElementInformation(uniqueIndex));

    QList<Scene::ObjectType> amkglTypes;

    foreach (GraphicsObjectType type, types) {
        if(((int)type) < ((int)Scene::NumberOfElements))
            amkglTypes.append((Scene::ObjectType)type);
    }

    m_sceneForAction.getPermanentScene()->visitObjectsOfTypes(amkglTypes, f);
}

Eigen::Vector3d GGraphicsViewImp::pointUnderPixel(const QPoint &pixel,
                                                  bool &found) const
{
    return QGLViewerTools::vecToEigen(amkglCamera()->pointUnderPixel(pixel, found));
}

void GGraphicsViewImp::convertClickToLine(const QPoint &pixel,
                                          Eigen::Vector3d &orig,
                                          Eigen::Vector3d &dir) const
{
    AMKglViewer::VecType vecOrig;
    AMKglViewer::VecType vecDir;

    amkglCamera()->convertClickToLine(pixel,
                                      vecOrig,
                                      vecDir);

    orig = QGLViewerTools::vecToEigen(vecOrig);
    dir = QGLViewerTools::vecToEigen(vecDir);
}

void GGraphicsViewImp::convert3DPositionToPixel(const Eigen::Vector3d &position,
                                                QPoint &pixel) const
{
    qreal res[3];
    amkglCamera()->getProjectedCoordinatesOf(position.data(), res);
    pixel.setX(res[0]);
    pixel.setY(res[1]);
}

int GGraphicsViewImp::width() const
{
    return AMKglViewer::width();
}

int GGraphicsViewImp::height() const
{
    return AMKglViewer::height();
}

GraphicsView* GGraphicsViewImp::getGraphicsView() const
{
    return const_cast<GGraphicsViewImp*>(this);
}

void GGraphicsViewImp::drawCameraInformations()
{
    const DM_GraphicsViewOptions& options = constGetOptionsInternal();

    if(options.getCameraInformationDisplayed().testFlag(DM_GraphicsViewOptions::CameraPosition))
        getDrawInfo()->drawText(tr("Position : %1 | %2 | %3").arg(amkglCamera()->position().x)
                                                             .arg(amkglCamera()->position().y)
                                                             .arg(amkglCamera()->position().z));

    if(options.getCameraInformationDisplayed().testFlag(DM_GraphicsViewOptions::CameraSceneCenter))
        getDrawInfo()->drawText(tr("Centre de la scene : %1 | %2 | %3").arg(amkglCamera()->sceneCenter().x)
                                                             .arg(amkglCamera()->sceneCenter().y)
                                                             .arg(amkglCamera()->sceneCenter().z));

    if(options.getCameraInformationDisplayed().testFlag(DM_GraphicsViewOptions::CameraViewDirection))
        getDrawInfo()->drawText(tr("Direction : %1 | %2 | %3 | %4").arg(amkglCamera()->orientation()[0])
                                                                   .arg(amkglCamera()->orientation()[1])
                                                                   .arg(amkglCamera()->orientation()[2])
                                                                   .arg(amkglCamera()->orientation()[3]));
}

void GGraphicsViewImp::computeAndSetTextPosition()
{
    const DM_GraphicsViewOptions& options = constGetOptionsInternal();
    DrawInfo* df = getDrawInfo();
    const int fontH = getPainter()->fontMetrics().height();

    if(options.getCameraInformationPosition() == DM_GraphicsViewOptions::InfoInUpperLeftCorner)
        df->setPositionWhereDrawText(fontH, fontH);
    else if(options.getCameraInformationPosition() == DM_GraphicsViewOptions::InfoInLowerLeftCorner)
        df->setPositionWhereDrawText(fontH, height() - df->getHeightOfAllTextSaved() - fontH);
    else if(options.getCameraInformationPosition() == DM_GraphicsViewOptions::InfoInUpperRightCorner)
        df->setPositionWhereDrawText(width() - df->getWidthOfAllTextSaved() - fontH, fontH);
    else if(options.getCameraInformationPosition() == DM_GraphicsViewOptions::InfoInLowerRightCorner)
        df->setPositionWhereDrawText(width() - df->getWidthOfAllTextSaved() - fontH, height() - df->getHeightOfAllTextSaved() - fontH);
}

void GGraphicsViewImp::getBoundingBoxOfSelectedElementsOfMeshesInView(Scene::ObjectType globalType, Eigen::Vector3d &min, Eigen::Vector3d &max) const
{
    min.setConstant(std::numeric_limits<double>::max());
    max = -min;

    MeshObjectsVisitor visitor(globalType);

    visitor.setFunction([&min, &max](AbstractChunkGenericCloudObject* chunk) {

        const GLuint* fvIndex = chunk->getFirstVertexIndex();

        if(fvIndex == nullptr)
            return;

        const CT_PointData* fVertex = chunk->getFirstVertex();

        if(fVertex == nullptr)
            return;

        const ElementInfo* foInfo = chunk->getFirstObjectInfo();

        if(foInfo == nullptr)
            return;

        const GLuint* foIndex = chunk->getFirstObjectIndex();

        if(foIndex == nullptr)
            return;

        const Eigen::Vector3d& offset = chunk->getOffset();
        const quint8 nVertexPerObject = chunk->getNumberOfElementPerObject();
        const size_t nObjects = chunk->countObjects();

        size_t localVertexIndex = 0;
        quint8 j, k;

        for(size_t i=0; i<nObjects; ++i) {
            if(ObjectsFlagsTool::staticIsObjectSelected(foInfo[foIndex[i]])) {

                for(j=0; j<nVertexPerObject; ++j) {
                    const CT_PointData& point = fVertex[fvIndex[localVertexIndex++]];

                    for(k=0; k<3; ++k) {
                        const double value = point[k]+offset[k];
                        min[k] = qMin(min[k], value);
                        max[k] = qMax(max[k], value);
                    }
                }
            } else {
                localVertexIndex += nVertexPerObject;
            }
        }
    });

    visitor.compute(getPermanentSceneToRender());
}

void GGraphicsViewImp::init()
{
    AMKglViewer::init();

    if(getDocument() != nullptr)
        m_sceneForAction.addedTo(getDocument(), getNewOpenGlContext());

    restoreStateFromFile();
}

void GGraphicsViewImp::preDraw()
{
    makeCurrent();

    setBackgroundColor(constGetOptionsInternal().getBackgroundColor());

    AMKglViewer::preDraw();

    glPointSize(constGetOptionsInternal().getPointSize());

    m_signalEmitter.emitDrawingStarted();
}

void GGraphicsViewImp::draw()
{
    AMKglViewer::draw();

    if(!isFastDrawModeCurrentlyUsed()) {
        m_sceneForAction.draw(*getDrawInfo());
    } else {
        m_sceneForAction.fastDraw(*getDrawInfo());
    }
}

void GGraphicsViewImp::fastDraw()
{
    AMKglViewer::fastDraw();

    if(!isFastDrawModeCurrentlyUsed()) {
        m_sceneForAction.draw(*getDrawInfo());
    } else {
        m_sceneForAction.fastDraw(*getDrawInfo());
    }
}

void GGraphicsViewImp::postDraw()
{
    m_signalEmitter.emitDrawingFinished();

    AMKglViewer::postDraw();
}

void GGraphicsViewImp::subPostDraw(DrawInfo &info)
{
    /*glBegin(GL_LINES);

    Eigen::Vector3d uV = camera()->upVector();
    uV *= 2.0;

    glColor3f(1, 0, 0);
    glVertex3d(0,  0, 0);
    glVertex3d(uV[0], uV[1], uV[2]);

    uV = camera()->rightVector();
    uV *= 2.0;

    glColor3f(0, 1, 0);
    glVertex3d(0,  0, 0);
    glVertex3d(uV[0], uV[1], uV[2]);

    glEnd();*/

    drawCameraInformations();

    AMKglViewer::subPostDraw(info);

    // draw pivot point (scene center)
    /*glPushMatrix();
    glTranslatef(amkglCamera()->pivotPoint().x,
                 amkglCamera()->pivotPoint().y,
                 amkglCamera()->pivotPoint().z);
    drawAxis();
    glPopMatrix();*/
}

void GGraphicsViewImp::drawOverlay(QPainter &painter, DrawInfo &info)
{
    AMKglViewer::drawOverlay(painter, info);

    m_sceneForAction.drawOverlay(painter, info);

    computeAndSetTextPosition();
}

bool GGraphicsViewImp::event(QEvent *e)
{
    if(actionsHandler()->eventToAction((QEvent*)e))
        return true;

    return AMKglViewer::event(e);
}

void GGraphicsViewImp::enterEvent(QEvent *e)
{
    if(actionsHandler()->enterEvent(e))
        return;

    AMKglViewer::enterEvent(e);
}

void GGraphicsViewImp::leaveEvent(QEvent *e)
{
    if(actionsHandler()->leaveEvent(e))
        return;

    AMKglViewer::leaveEvent(e);
}

void GGraphicsViewImp::mousePressEvent(QMouseEvent *e)
{
    if(actionsHandler()->mousePressEvent(e))
        return;

    AMKglViewer::mousePressEvent(e);
}

void GGraphicsViewImp::mouseMoveEvent(QMouseEvent *e)
{
    if(actionsHandler()->mouseMoveEvent(e))
        return;

    AMKglViewer::mouseMoveEvent(e);
}

void GGraphicsViewImp::mouseReleaseEvent(QMouseEvent *e)
{
    if(actionsHandler()->mouseReleaseEvent(e))
        return;

    AMKglViewer::mouseReleaseEvent(e);
}

void GGraphicsViewImp::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(actionsHandler()->mouseDoubleClickEvent(e))
        return;

    AMKglViewer::mouseDoubleClickEvent(e);
}

void GGraphicsViewImp::wheelEvent(QWheelEvent *e)
{
    if(actionsHandler()->wheelEvent(e))
        return;

    AMKglViewer::wheelEvent(e);
}

void GGraphicsViewImp::keyPressEvent(QKeyEvent *e)
{
    if(actionsHandler()->keyPressEvent(e))
        return;

    if((e->key() == Qt::Key_D)
            && e->modifiers().testFlag(Qt::ControlModifier)
            && e->modifiers().testFlag(Qt::AltModifier)) {
        setDebugModeEnabled(!isDebugModeEnabled());
    }
    
    
    if(e->key() == Qt::Key_Delete) {
        int ret = QMessageBox::question(nullptr, tr("Suppression"), tr("Voulez-vous supprimer les items sélectionnés de la vue ?"), QMessageBox::Yes | QMessageBox::Cancel);

        if(ret == QMessageBox::Yes)
            document()->removeAllSelectedItemDrawable();
    }

    AMKglViewer::keyPressEvent(e);

    DM_GraphicsViewOptions newOptions;
    newOptions.updateFromOtherOptions(constGetOptionsInternal());
    newOptions.setDrawGrid(gridIsDrawn());
    newOptions.setDrawAxis(axisIsDrawn());
    newOptions.setCameraInformationDisplayed(FPSIsDisplayed() ?
                                                 newOptions.getCameraInformationDisplayed() | DM_GraphicsViewOptions::FpsInformation
                                               : newOptions.getCameraInformationDisplayed() & ~DM_GraphicsViewOptions::FpsInformation);

    setOptions(newOptions);
}

void GGraphicsViewImp::keyReleaseEvent(QKeyEvent *e)
{
    if(actionsHandler()->keyReleaseEvent(e))
        return;

    AMKglViewer::keyReleaseEvent(e);
}

QString GGraphicsViewImp::stateFileName() const
{
    QString name = DEFAULT_STATE_FILENAME;

    if (!name.isEmpty() && m_uniqueIndex > 0)
    {
        QFileInfo fi(name);
        if (fi.suffix().isEmpty())
            name += QString::number(m_uniqueIndex);
        else
            name = fi.absolutePath() + '/' + fi.completeBaseName() + QString::number(m_uniqueIndex) + "." + fi.suffix();
    }

    return name;
}

void GGraphicsViewImp::saveStateToFile()
{
    QString name = stateFileName();

    if (name.isEmpty())
        return;

    QFileInfo fileInfo(name);

    if (fileInfo.isDir())
    {
        GUI_LOG->addWarningMessage(LogInterface::gui, tr("Save to file error : State file name (%1) references a directory instead of a file.").arg(name));
        return;
    }

    const QString dirName = fileInfo.absolutePath();
    if (!QFileInfo(dirName).exists())
    {
        QDir dir;
        if (!(dir.mkdir(dirName)))
        {
            GUI_LOG->addWarningMessage(LogInterface::gui, tr("Save to file error : Unable to create directory %1").arg(dirName));
            return;
        }
    }

    // Write the DOM tree to file
    QFile f(name);
    if (f.open(QIODevice::WriteOnly))
    {
        QTextStream out(&f);
        QDomDocument doc("G3DVIEWER");

        QDomElement root = doc.createElement("Configuration");

        QDomElement nodeCustom = doc.createElement("GraphicsOptions");
        constGetOptionsInternal().saveToXml(nodeCustom, doc);
        root.appendChild(nodeCustom);

        root.appendChild(domElement("AMKglViewer", doc));

        doc.appendChild(root);

        doc.save(out, QDomNode::EncodingFromTextStream);

        f.flush();
        f.close();
    }
    else
        GUI_LOG->addWarningMessage(LogInterface::gui, tr("Save to file error : Unable to save to file %1").arg(name) + ":\n" + f.errorString());
}

bool GGraphicsViewImp::restoreStateFromFile()
{
    QString name = stateFileName();

    if (name.isEmpty())
        return false;

    QFileInfo fileInfo(name);

    if (!fileInfo.isFile())
        // No warning since it would be displayed at first start.
        return false;

    if (!fileInfo.isReadable())
    {
        GUI_LOG->addWarningMessage(LogInterface::gui, tr("Problem in state restoration : File %1 is not readable.").arg(name));
        return false;
    }

    // Read the DOM tree from file
    QFile f(name);
    if (f.open(QIODevice::ReadOnly))
    {
        QDomDocument doc;

        QString errMsg;
        int errLine, errColumn;

        if(!doc.setContent(&f, &errMsg, &errLine, &errColumn)) {
            GUI_LOG->addWarningMessage(LogInterface::gui, tr("Problem in state restoration when set xml content : %1 at line %2 column %3").arg(errMsg).arg(errLine).arg(errColumn));

            f.close();
            return false;
        }

        f.close();

        DM_GraphicsViewOptions opt;
        opt.updateFromOtherOptions(constGetOptionsInternal());

        QDomNodeList roots = doc.childNodes();

        if(!roots.isEmpty()) {
            QDomNode root = roots.at(0);

            if(root.nodeName() == "Configuration") {
                QDomElement el = root.firstChildElement("GraphicsOptions");

                if(!el.isNull())
                    opt.loadFromXml(el);

                el = root.firstChildElement("AMKglViewer");

                if(!el.isNull())
                    initFromDOMElement(el);

                setOptions(opt);

                if(qIsNaN(amkglCamera()->position().x)
                        || qIsNaN(amkglCamera()->position().y)
                        || qIsNaN(amkglCamera()->position().z)
                        || qIsNaN(amkglCamera()->orientation()[0])
                        || qIsNaN(amkglCamera()->orientation()[1])
                        || qIsNaN(amkglCamera()->orientation()[2])
                        || qIsNaN(amkglCamera()->orientation()[3])) {

                    m_camController->homePosition();
                }
                return true;
            }
        }

        return false;
    }
    else
    {
        GUI_LOG->addWarningMessage(LogInterface::gui, tr("Open file error : Unable to open file %1").arg(name) + ":\n" + f.errorString());
        return false;
    }

    return true;
}

AMKglViewer::CameraType* GGraphicsViewImp::amkglCamera() const
{
    return AMKglViewer::camera();
}

GraphicsViewInterface::SelectionMode GGraphicsViewImp::staticToBasicSelectionMode(GraphicsViewInterface::SelectionMode mode)
{
    while(mode >= GraphicsViewInterface::SELECT_POINTS)
        mode = (GraphicsViewInterface::SelectionMode)(((int)mode) - ((int)GraphicsViewInterface::REMOVE_ONE));

    return mode;
}

void GGraphicsViewImp::currentActionChanged(CT_AbstractAction *action)
{
    m_sceneForAction.setAction(dynamic_cast<CT_AbstractActionForGraphicsView*>(action), *this);
}

void GGraphicsViewImp::amkglDebugModeChanged(bool enabled)
{
    if(enabled) {
        GraphicsViewDebugMode dm;
        m_document->setCurrentInternalAction(&dm, false);

        connect(actionsHandler(), SIGNAL(currentActionChanged(CT_AbstractAction*)), this, SLOT(disableAmkglDebugMode()));
    } else {
        GUI_MANAGER->getActionsManager()->removeInternalAction(GraphicsViewDebugMode::staticMetaObject.className());
    }
}

void GGraphicsViewImp::disableAmkglDebugMode()
{
    disconnect(actionsHandler(), SIGNAL(currentActionChanged(CT_AbstractAction*)), this, SLOT(disableAmkglDebugMode()));
    setDebugModeEnabled(false);
}

GraphicsViewInterface* GGraphicsViewImpSignalEmitter::graphicsView() const
{
    return m_graphicsView;
}
