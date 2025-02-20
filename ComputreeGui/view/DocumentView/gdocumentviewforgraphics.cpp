#include "gdocumentviewforgraphics.h"

#include "dm_guimanager.h"

#include "view/DocumentView/GraphicsViews/PointsAttributes/gpointsattributesmanager.h"

#include "cdm_tools.h"

#include "ct_global/ct_context.h"
#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"
#include "ct_cloudindex/abstract/ct_abstractmodifiablecloudindex.h"

#include "ct_colorcloud/registered/ct_standardcolorcloudregistered.h"
#include "ct_normalcloud/registered/ct_standardnormalcloudregistered.h"

#include "ct_iterator/ct_singlemodeliteratorstdstyleforresultgroup.h"

#include "ct_itemdrawable/ct_standarditemgroup.h"
#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"
#include "ct_accessibility/ct_iaccesspointcloud.h"
#include "ct_accessibility/ct_iaccessedgecloud.h"
#include "ct_accessibility/ct_iaccessfacecloud.h"

#include "dm_iteminfoforgraphics.h"

#include "scene/permanentitemscenebymodel.h"
#include "view/drawmodeconfigurator.h"

#include "tools/itemdrawable/dm_documentmenugenerator.h"
#include "tools/attributes/dm_attributesbuildingcollectiont.h"
#include "tools/attributes/worker/dm_attributesmanagerupdater.h"

#include <QInputDialog>
#include <QLineEdit>
#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QProgressDialog>

GDocumentViewForGraphics::GDocumentViewForGraphics(GDocumentManagerView &manager, QString title, QString type) : GDocumentView(manager, title)
{
    m_scene = new PermanentSceneToRender();
    mResultGui.init();

    m_graphics = nullptr;
    _graphicsOptionsView = nullptr;
    _type = type;
    _pofManager.loadDefault();

    connect(this, SIGNAL(itemDrawableToBeRemoved(CT_AbstractItemDrawable&)), this, SLOT(removeItemDrawableFromScene(CT_AbstractItemDrawable&)), Qt::DirectConnection);

    m_timerUpdateColors.setSingleShot(true);
    m_timerUpdateColors.setInterval(50);

    m_timerDirtyColorsOfPoints.setSingleShot(true);
    m_timerDirtyColorsOfPoints.setInterval(50);

    m_timerDirtyNormalsOfPoints.setSingleShot(true);
    m_timerDirtyNormalsOfPoints.setInterval(50);

    connect(&m_timerUpdateColors, SIGNAL(timeout()), this, SLOT(mustUpdateItemDrawablesThatColorWasModified()), Qt::QueuedConnection);
    connect(&m_timerDirtyColorsOfPoints, SIGNAL(timeout()), this, SLOT(mustDirtyColorsOfItemDrawablesWithPoints()), Qt::QueuedConnection);
    connect(&m_timerDirtyNormalsOfPoints, SIGNAL(timeout()), this, SLOT(mustDirtyNormalsOfItemDrawablesWithPoints()), Qt::QueuedConnection);

    connect(this, SIGNAL(startUpdateColorsTimer()), &m_timerUpdateColors, SLOT(start()), Qt::QueuedConnection);
    connect(this, SIGNAL(startDirtyColorsOfPointTimer()), &m_timerDirtyColorsOfPoints, SLOT(start()), Qt::QueuedConnection);
    connect(this, SIGNAL(startDirtyNormalsOfPointTimer()), &m_timerDirtyNormalsOfPoints, SLOT(start()), Qt::QueuedConnection);

    _viewDetached = false;
}

GDocumentViewForGraphics::~GDocumentViewForGraphics()
{
    delete _buttonExport->menu();
    delete _graphicsOptionsView;
	delete m_graphics;
	delete m_scene;
}

IGraphicsDocument::PermanentSceneToRender *GDocumentViewForGraphics::getPermanentSceneToRender() const
{
    return m_scene;
}

QOpenGLWidget* GDocumentViewForGraphics::getOpengWidgetWithContext(const QOpenGLContext *context) const
{
    if((m_graphics == nullptr) || (m_graphics->getOpenGLWidget()->context() != context))
        return nullptr;

    return m_graphics->getOpenGLWidget();
}

QColor GDocumentViewForGraphics::getSelectionColor() const
{
    return _graphicsOptionsView->getOptions().getSelectedColor();
}

QColor GDocumentViewForGraphics::getNormalsColor() const
{
    return _graphicsOptionsView->getOptions().normalColor();
}

float GDocumentViewForGraphics::getNormalsLength() const
{
    return _graphicsOptionsView->getOptions().normalLength();
}

bool GDocumentViewForGraphics::mustShowNormals() const
{
    return _graphicsOptionsView->getOptions().mustShowNormals();
}

void GDocumentViewForGraphics::setMustShowNormals(bool enabled)
{
    DM_GraphicsViewOptions options = getOptions();
    options.setMustShowNormals(enabled);

    setOptions(options);
}

bool GDocumentViewForGraphics::mustReduceNumberOfPointsInFastDraw() const
{
    return _graphicsOptionsView->getOptions().mustReduceNbrPointsInFastDraw();
}

bool GDocumentViewForGraphics::mustReduceNumberOfPointsInNormalDraw() const
{
    return _graphicsOptionsView->getOptions().mustReduceNbrPointsInNormalDraw();
}

void GDocumentViewForGraphics::init()
{
    GDocumentView::init();

    // le widget avec la config des caméras et des graphiques....
    createAndAddCameraAndGraphicsOptions(getCentralWidget());

    // ...et le widget contenant les graphiques
    createAndAddGraphicsWidgetContainer(getCentralWidget());
}

void GDocumentViewForGraphics::addGraphics(GGraphicsView *graphics)
{
    if(m_graphics != nullptr) {
        delete graphics;
        return;
    }

    graphics->setDocumentView(this);
    graphics->setAttributesManager(&m_attributesManager);
    m_graphics = graphics;

    m_graphics->setOptions(getOptions());

    _layoutGraphics->addWidget(m_graphics->getOpenGLWidget());
    _cameraOptionsView->setCamera(m_graphics->getCamera());
}

QList<GGraphicsView *> GDocumentViewForGraphics::getGraphicsList() const
{
    QList<GGraphicsView *> l;

    if(m_graphics != nullptr)
        l << m_graphics;

    return l;
}

void GDocumentViewForGraphics::addItemDrawable(CT_AbstractItemDrawable &item)
{
    GDocumentView::addItemDrawable(item);
    mResultGui.addItemDrawable(item);
    // TODO : model !
    m_scene->addItem(&item, nullptr, *m_graphics);
}

void GDocumentViewForGraphics::removeItemDrawable(CT_AbstractItemDrawable &item)
{
    GDocumentView::removeItemDrawable(item);
    removeItemDrawableFromScene(item);
}

void GDocumentViewForGraphics::updateDrawing3DOfItemDrawablesInGraphicsView(const QList<CT_AbstractItemDrawable *> &items)
{
    if(m_graphics != nullptr)
        m_graphics->updateDrawing3DOfItemDrawables(items);
}

QList<InDocumentViewInterface *> GDocumentViewForGraphics::views() const
{
    QList<InDocumentViewInterface *> l;

    if(m_graphics != nullptr)
        l.append(m_graphics);

    return l;
}

void GDocumentViewForGraphics::redrawGraphics()
{
    getPermanentSceneToRender()->checkAndUpdateIfItNeedsToBe();

    if(m_graphics != nullptr)
        m_graphics->redraw();
}

void GDocumentViewForGraphics::updateItems(const QList<CT_AbstractItemDrawable *> &items)
{
    updateDrawing3DOfItemDrawablesInGraphicsView(items);
}

void GDocumentViewForGraphics::dirtyColorsOfPoints()
{
    emit startDirtyColorsOfPointTimer();
}

void GDocumentViewForGraphics::dirtyNormalsOfPoints()
{
    emit startDirtyNormalsOfPointTimer();
}

void GDocumentViewForGraphics::applyNormalsConfiguration(GDocumentViewForGraphics::NormalsConfiguration c)
{
    DM_GraphicsViewOptions opt = getOptions();
    opt.setNormalColor(c.normalColor);
    opt.setNormalLength(c.normalLength);

    setOptions(opt);
}

GDocumentViewForGraphics::NormalsConfiguration GDocumentViewForGraphics::getNormalsConfiguration() const
{
    GDocumentViewForGraphics::NormalsConfiguration c;

    if(m_graphics != nullptr) {
        const DM_GraphicsViewOptions &opt = m_graphics->constGetOptionsInternal();
        c.normalColor = opt.normalColor();
        c.normalLength = opt.normalLength();
    }

    return c;
}

bool GDocumentViewForGraphics::acceptAction(const CT_AbstractAction *action) const
{
    return (action == nullptr) || (dynamic_cast<const CT_AbstractActionForGraphicsView*>(action) != nullptr);
}

bool GDocumentViewForGraphics::setCurrentAction(CT_AbstractAction *action, bool deleteAction)
{
    return setCurrentAction(false, action, deleteAction);
}

bool GDocumentViewForGraphics::setDefaultAction(CT_AbstractAction *action, bool deleteAction)
{
    if(m_graphics == nullptr) {

        if(deleteAction && !GUI_MANAGER->getActionsManager()->existActionCompareAddress(action))
            delete action;

        return false;
    }

    if(!acceptAction(action))
    {
        if(deleteAction && !GUI_MANAGER->getActionsManager()->existActionCompareAddress(action))
            delete action;

        return false;
    }

    if(action == nullptr)
        m_graphics->setDefaultAction(nullptr);
    else
        m_graphics->setDefaultAction((CT_AbstractActionForGraphicsView*)action->createInstance());

    redrawGraphics();

    if(deleteAction && !GUI_MANAGER->getActionsManager()->existActionCompareAddress(action))
        delete action;

    emit defaultActionChanged(defaultAction());

    return true;
}

bool GDocumentViewForGraphics::setCurrentInternalAction(CT_AbstractAction *action, bool deleteAction)
{
    return setCurrentAction(true, action, deleteAction);
}

CT_AbstractAction* GDocumentViewForGraphics::currentAction() const
{
    if(m_graphics == nullptr)
        return nullptr;

    return m_graphics->actionsHandler()->currentAction();
}

CT_AbstractAction *GDocumentViewForGraphics::defaultAction() const
{
    if(m_graphics == nullptr)
        return nullptr;

    return m_graphics->actionsHandler()->defaultAction();
}

void GDocumentViewForGraphics::removeActions(const QString &uniqueName) const
{
    if(m_graphics == nullptr)
        return;

    m_graphics->actionsHandler()->removeActions(uniqueName);
}

QString GDocumentViewForGraphics::getType() const
{
    return _type;
}

bool GDocumentViewForGraphics::useItemColor() const
{
    return true;
}

void GDocumentViewForGraphics::setColor(const CT_AbstractItemDrawable *item, const QColor &color)
{
    if(item->result() == nullptr) {
        GUI_LOG->addErrorMessage(LogInterface::gui, tr("Impossible d'affecter une couleur à un item dont le résultat est nullptr"));
        return;
    }

    const QHash<CT_AbstractResult*, QHash<CT_AbstractItemDrawable*, DM_AbstractInfo*>* > &ii = getItemsInformations();
    QHash<CT_AbstractItemDrawable*, DM_AbstractInfo*> *hash = ii.value(item->result(), nullptr);

    if(hash == nullptr)
        hash = createItemInformationsForResult(item->result());

    DM_ItemInfoForGraphics *info = static_cast<DM_ItemInfoForGraphics*>(hash->value((CT_AbstractItemDrawable*)item, nullptr));

    if(info == nullptr)
    {
        info = static_cast<DM_ItemInfoForGraphics*>(createNewItemInformation(item));
        hash->insert((CT_AbstractItemDrawable*)item, info);
    }

    info->setColor(color);
    recursiveSetColor(dynamic_cast<CT_StandardItemGroup*>((CT_AbstractItemDrawable*)item), color);

    emit startUpdateColorsTimer();
}

bool GDocumentViewForGraphics::isColorModified(const CT_AbstractItemDrawable *item)
{
    QHash<CT_AbstractItemDrawable*, DM_AbstractInfo*> *hash = getItemsInformations().value(item->result(), nullptr);

    if(hash != nullptr)
    {
        DM_ItemInfoForGraphics *info = static_cast<DM_ItemInfoForGraphics*>(hash->value((CT_AbstractItemDrawable*)item, nullptr));

        if(info != nullptr)
            return info->isColorModified();
    }

    return false;
}

QColor GDocumentViewForGraphics::getColor(const CT_AbstractItemDrawable *item)
{
    QHash<CT_AbstractItemDrawable*, DM_AbstractInfo*> *hash = getItemsInformations().value(item->result(), nullptr);

    if(hash != nullptr)
    {
        DM_ItemInfoForGraphics *info = static_cast<DM_ItemInfoForGraphics*>(hash->value((CT_AbstractItemDrawable*)item, nullptr));

        if(info != nullptr)
            return info->color();
    }
    else
    {
        GUI_LOG->addErrorMessage(LogInterface::gui, tr("Impossible de récupérer une couleur d'un item dont le résultat est nullptr"));
    }

    return QColor();
}

void GDocumentViewForGraphics::visitItemsThatColorWasModified(GDocumentViewForGraphics::visitItemColorModifiedFunction f)
{
    QHash<CT_AbstractResult *, QHash<CT_AbstractItemDrawable *, DM_AbstractInfo *> *> infos = getItemsInformations();

    CT_AbstractResult* lastResult = nullptr;
    QHash<CT_AbstractItemDrawable *, DM_AbstractInfo *>* lastInfoByItem = nullptr;

    QListIterator<CT_AbstractItemDrawable*> itI = getItemDrawable();

    while(itI.hasNext()) {
        const CT_AbstractItemDrawable* item = itI.next();

        if(item->result() != lastResult) {
            lastResult = item->result();
            lastInfoByItem = infos.value(lastResult, nullptr);
        }

        DM_ItemInfoForGraphics* infoOfItem = static_cast<DM_ItemInfoForGraphics*>(lastInfoByItem->value((CT_AbstractItemDrawable*)item, nullptr));

        // bool modifie = infoOfItem->isColorModified();

        if((infoOfItem != nullptr) && (infoOfItem->isColorModified())) {
            f(item, infoOfItem->color());
        }
    }
}

bool GDocumentViewForGraphics::canChangeVisibility() const
{
    return true;
}

void GDocumentViewForGraphics::setVisible(const CT_AbstractItemDrawable *item, bool visible)
{
    if(m_graphics != nullptr)
        m_graphics->setVisible(item, visible);
}

bool GDocumentViewForGraphics::isVisible(const CT_AbstractItemDrawable *item) const
{
    bool isVisible = false;

    if(m_graphics != nullptr) {
        if(m_graphics->isVisible(item))
            isVisible = true;
    }

    return isVisible;
}

void GDocumentViewForGraphics::applyAttributes(DM_AbstractAttributes *dpa)
{
    QProgressDialog dialog(tr("Veuillez patienter pendant le traitement..."), "", 0, 100);
    dialog.setWindowModality(Qt::ApplicationModal);
    dialog.setCancelButton(nullptr);
    dialog.show();

    dpa->setDocument(this);
    dpa->checkAndSetNecessaryCloudToDoc();

    QEventLoop event;

    QThread *thread = dpa->thread();

    DM_AbstractWorker::staticConnectWorkerToThread(dpa, false, false, true);

    connect(dpa, SIGNAL(finished()), &event, SLOT(quit()), Qt::DirectConnection);
    connect(thread, SIGNAL(finished()), &event, SLOT(quit()), Qt::DirectConnection);
    connect(dpa, SIGNAL(progressChanged(int)), &dialog, SLOT(setValue(int)), Qt::QueuedConnection);

    thread->start();
    event.exec();

    dialog.close();

    thread->wait();

    disconnect(thread, nullptr, dpa, nullptr);
    disconnect(dpa, nullptr, thread, nullptr);
}

void GDocumentViewForGraphics::setOptions(const DM_GraphicsViewOptions &options)
{
    if(_graphicsOptionsView != nullptr) {
        if(getOptions() != options) {
            _graphicsOptionsView->setOptions(options);
            validateOptions();
        }
    }
}

DM_GraphicsViewOptions GDocumentViewForGraphics::getOptions() const
{
    if(_graphicsOptionsView != nullptr)
        return _graphicsOptionsView->getOptions();

    return DM_GraphicsViewOptions();
}

void GDocumentViewForGraphics::updateAttributesManager()
{
    DM_AttributesManagerUpdater::Update(m_attributesManager);
}

const DM_AttributesManager& GDocumentViewForGraphics::attributesManager() const
{
    return m_attributesManager;
}

void GDocumentViewForGraphics::showOptions()
{
    if(m_graphics != nullptr)
    {
        DM_GraphicsViewOptions opt;
        opt.updateFromOtherOptions(m_graphics->constGetOptionsInternal());

        _graphicsOptionsView->setOptions(opt);

        QScreen *screen = QGuiApplication::primaryScreen();
        QRect rec = screen->geometry();
        _graphicsOptionsView->move(rec.center() - _graphicsOptionsView->rect().center());

        if(_graphicsOptionsView->exec())
            validateOptions();
        else
            _graphicsOptionsView->setOptions(opt);
    }
}

void GDocumentViewForGraphics::validateOptions()
{
    const DM_GraphicsViewOptions &options = _graphicsOptionsView->getOptions();

    if(m_graphics != nullptr)
        m_graphics->setOptions(options);

    updateButtonFastDrawMode();
    updateButtonCameraType();
}

void GDocumentViewForGraphics::takeAndSaveScreenshot()
{
    if(m_graphics != nullptr)
        m_graphics->takeAndSaveScreenshot();
}

void GDocumentViewForGraphics::addActualPointOfView()
{
    QList<DM_PointOfView> listPof = _pofManager.getPointOfViewList(getKeyForPointOfViewManager());

    // on limite  20 le nombre de point de vue
    if(listPof.size() == 20)
        _pofManager.removePointOfView(getKeyForPointOfViewManager(), listPof.takeLast());

    QString name = QInputDialog::getText(getSubWindow(), tr("Point de vue"), tr("Veuillez entrer un nom pour le point de vue :"), QLineEdit::Normal, QString("P%1").arg(_pofManager.numberPointOfViewAddedFromBeginning(getKeyForPointOfViewManager())+1));

    if(!name.isEmpty())
    {
        DM_GraphicsViewCamera *cam = _cameraOptionsView->getCamera();

        double q0, q1, q2, q3;

        cam->getOrientation(q0, q1, q2 ,q3);

        _pofManager.addPointOfView(getKeyForPointOfViewManager(),
                                              DM_PointOfView(name,
                                                             cam->cx(),
                                                             cam->cy(),
                                                             cam->cz(),
                                                             cam->x(),
                                                             cam->y(),
                                                             cam->z(),
                                                             q0,
                                                             q1,
                                                             q2,
                                                             q3));
    }
}

void GDocumentViewForGraphics::setPointOfView(DM_PointOfView *pof)
{
    if(pof != nullptr)
    {
        DM_GraphicsViewCamera *cam = _cameraOptionsView->getCamera();

        cam->setPointOfView(pof->cx(),
                            pof->cy(),
                            pof->cz(),
                            pof->px(),
                            pof->py(),
                            pof->pz(),
                            pof->q0(),
                            pof->q1(),
                            pof->q2(),
                            pof->q3(), true);
    }
}

void GDocumentViewForGraphics::showAttributesOptions()
{
    updateAttributesManager();

    GPointsAttributesManager dialog;
    dialog.setManager(&m_attributesManager);
    dialog.setDocument(this);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect rec = screen->geometry();
    dialog.move(rec.center() - dialog.rect().center());

    dialog.exec();
}

void GDocumentViewForGraphics::changePixelSizeUp()
{
    DM_GraphicsViewOptions opt = getOptions();
    opt.setPointSize(opt.getPointSize() + 1);

    setOptions(opt);
}

void GDocumentViewForGraphics::changePixelSizeDown()
{
    DM_GraphicsViewOptions opt = getOptions();

    if(opt.getPointSize() > 1) {
        opt.setPointSize(opt.getPointSize() - 1);
        setOptions(opt);
    }
}

void GDocumentViewForGraphics::changeFastDrawMode()
{
    DM_GraphicsViewOptions opt = getOptions();

    // if Normal
    if (opt.mustReduceNbrPointsInFastDraw()
            && !opt.mustReduceNbrPointsInNormalDraw())
    {
        // Always
        opt.setMustReduceNbrPointsInFastDraw(true);
        opt.setMustReduceNbrPointsInNormalDraw(true);
    }
    // if Always
    else if (opt.mustReduceNbrPointsInFastDraw()
             && opt.mustReduceNbrPointsInNormalDraw())
    {
        // Never
        opt.setMustReduceNbrPointsInFastDraw(false);
        opt.setMustReduceNbrPointsInNormalDraw(false);
    }
    // if Never
    else
    {
        // Normal
        opt.setMustReduceNbrPointsInFastDraw(true);
        opt.setMustReduceNbrPointsInNormalDraw(false);
    }

    setOptions(opt);
}

void GDocumentViewForGraphics::changeCameraType()
{
    DM_GraphicsViewOptions opt = getOptions();

    if(opt.getCameraType() == CameraInterface::PERSPECTIVE)
        opt.setCameraType(CameraInterface::ORTHOGRAPHIC);
    else
        opt.setCameraType(CameraInterface::PERSPECTIVE);

    setOptions(opt);
}

void GDocumentViewForGraphics::setTransparencyActivated(bool activated)
{
    DM_GraphicsViewOptions opt = getOptions();
    opt.setUseTransparency(activated);

    setOptions(opt);
}

void GDocumentViewForGraphics::setCameraType(bool orthographic)
{
    DM_GraphicsViewOptions opt = getOptions();

    if (orthographic)
        opt.setCameraType(CameraInterface::ORTHOGRAPHIC);
    else
        opt.setCameraType(CameraInterface::PERSPECTIVE);

    setOptions(opt);
}

void GDocumentViewForGraphics::syncChanged(bool enable)
{
    if(enable)
        emit syncEnabled(this);
    else
        emit syncDisabled(this);
}

void GDocumentViewForGraphics::pluginExporterManagerReloaded()
{
    CDM_Tools tools(GUI_MANAGER->getPluginManager());

    _buttonExport->setMenu(nullptr);
    _buttonExport->setEnabled(false);

    QMenu *exportersMenu = tools.createMenuForAllExporters(this, SLOT(exporterActionTriggered()));

    if(exportersMenu != nullptr)
    {
        _buttonExport->setMenu(exportersMenu);
        _buttonExport->setEnabled(true);
    }
}

void GDocumentViewForGraphics::exporterActionTriggered()
{
    if(m_graphics == nullptr)
        return;

    CDM_Tools tools(GUI_MANAGER->getPluginManager());

    CT_AbstractExporter *exporter = dynamic_cast<CT_AbstractExporter*>(sender()->parent());

    CT_AbstractExporter *exCopy = exporter->copy();

    exCopy->declareInputModels();

    if(exCopy->inputsModelManager()->doesAtLeastOneInputModelsUseType(CT_AbstractAttributes::staticType()))
    {
        const QList<CT_VirtualAbstractStep*> steps = GUI_MANAGER->getStepManager()->getStepRootList();

        QList<CT_AbstractAttributes*> allAttributes;
        DM_AttributesBuildingCollectionT<CT_AbstractAttributes> builder;

        for(CT_VirtualAbstractStep* step : steps)
        {
            builder.buildFrom(step);
            allAttributes.append(builder.attributesCollection());
        }

        mResultGui.setAttributes(allAttributes);
    }

    mResultGui.setGraphicsView(m_graphics);
    mResultGui.finalizeBeforeUseIt();

    if(exCopy->findInputsInOutputsOfThisManager(*mResultGui.outModelStructureManager()))
    {
        if(exCopy->showInputsConfigurationDialog())
        {
            const QString filepath = QFileDialog::getSaveFileName(nullptr, tr("Exporter sous..."), exporter->filePath(), tools.createFileExtensionForExporter(exCopy));

            if(!filepath.isEmpty()
                    && exCopy->setFilePath(filepath))
            {
                exporter->setFilePath(filepath);

                GUI_MANAGER->asyncExport(exCopy, nullptr);
                return;
            }
        }
    }

    /*QList<CT_AbstractItemDrawable*> selectedItems = getSelectedItemDrawable();

    bool pointsAlreadyExported = false;
    bool edgesAlreadyExported = false;
    bool facesAlreadyExported = false;

    if(exCopy->canExportPoints() || exCopy->canExportEdges() || exCopy->canExportFaces()) {
        for(CT_AbstractItemDrawable* item : selectedItems) {
            if(dynamic_cast<CT_IAccessPointCloud*>(item))
                pointsAlreadyExported = true;
            else if(dynamic_cast<CT_IAccessEdgeCloud*>(item))
                edgesAlreadyExported = true;
            else if(dynamic_cast<CT_IAccessFaceCloud*>(item))
                facesAlreadyExported = true;
        }
    }

    if(exCopy->canExportPoints() && !pointsAlreadyExported)
    {
        QList<CT_AbstractCloudIndex*> points;

        CT_SPCIR selec = m_graphics->getSelectedPoints();

        if(!selec.isNull()
                && (selec->abstractCloudIndex() != nullptr))
            points.append(selec->abstractCloudIndex());

        exCopy->setPointsToExport(points);
    }

    if(exCopy->canExportFaces() && !facesAlreadyExported)
    {
        QList<CT_AbstractCloudIndex*> faces;

        CT_SFCIR selec = m_graphics->getSelectedFaces();

        if(!selec.isNull()
                && (selec->abstractCloudIndex() != nullptr))
            faces.append(selec->abstractCloudIndex());

        exCopy->setFacesToExport(faces);
    }

    if(exCopy->canExportEdges() && !edgesAlreadyExported)
    {
        QList<CT_AbstractCloudIndex*> edges;

        CT_SECIR selec = m_graphics->getSelectedEdges();

        if(!selec.isNull()
                && (selec->abstractCloudIndex() != nullptr))
            edges.append(selec->abstractCloudIndex());

        exCopy->setEdgesToExport(edges);
    }

    if(exCopy->hasSomethingToExport())
    {
        QString filepath = QFileDialog::getSaveFileName(nullptr, tr("Exporter sous..."), exporter->filePath(), tools.createFileExtensionForExporter(exCopy));

        if(!filepath.isEmpty()
                && exCopy->setFilePath(filepath))
        {
            exporter->setFilePath(filepath);

            GUI_MANAGER->asyncExport(exCopy, nullptr);
            return;
        }
    }
    else if(!exCopy->errorMessage().isEmpty())
    {
        QMessageBox::critical(nullptr, tr("Erreur"), exCopy->errorMessage(), QMessageBox::Ok);
    }*/

    delete exCopy;
}

void GDocumentViewForGraphics::mustUpdateItemDrawablesThatColorWasModified()
{
    if(m_graphics == nullptr)
        return;

    m_graphics->updateItemDrawablesThatColorWasModified();

    QHashIterator<CT_AbstractResult *, QHash<CT_AbstractItemDrawable *, DM_AbstractInfo *> *> it = getItemsInformations();

    while(it.hasNext())
    {
        it.next();
        QHashIterator<CT_AbstractItemDrawable *, DM_AbstractInfo *> it2(*it.value());

        while(it2.hasNext())
            static_cast<DM_ItemInfoForGraphics*>(it2.next().value())->setColorModified(false);
    }

    m_graphics->dirtyColorsOfItemDrawablesWithPoints();
}

void GDocumentViewForGraphics::mustDirtyColorsOfItemDrawablesWithPoints()
{
    if(m_graphics != nullptr)
        m_graphics->dirtyColorsOfItemDrawablesWithPoints();
}

void GDocumentViewForGraphics::mustDirtyNormalsOfItemDrawablesWithPoints()
{
    if(m_graphics != nullptr)
        m_graphics->dirtyNormalsOfItemDrawablesWithPoints();
}

void GDocumentViewForGraphics::removeItemDrawableFromScene(CT_AbstractItemDrawable &item)
{
    // TODO : model !
    mResultGui.removeItemDrawable(item);
    m_scene->removeItem(&item, nullptr);
}

void GDocumentViewForGraphics::closeEvent(QCloseEvent *closeEvent)
{
    if(canClose())
    {
        emit syncDisabled(this);

        delete m_graphics;
        m_graphics = nullptr;
    }

    GDocumentView::closeEvent(closeEvent);
}

///////////////// PRIVATE //////////////

void GDocumentViewForGraphics::createAndAddCameraAndGraphicsOptions(QWidget *parent)
{
    QWidget *widgetContainer = new QWidget(parent);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 3, 0, 0);

    delete widgetContainer->layout();
    widgetContainer->setLayout(layout);

    // widget avec les X, Y, Z, etc...
    _cameraOptionsView = new GCameraGraphicsOptions(widgetContainer);

    QPushButton *screenshotButton = new QPushButton(widgetContainer);
    screenshotButton->setToolTip(tr("Enregistrer une capture d'écran"));
    screenshotButton->setIcon(QIcon(":/Icones/Icones/screenshot.png"));

//    QPushButton *maximizeButton = new QPushButton(widgetContainer);
//    maximizeButton->setIcon(QIcon(":/Icones/Icones/maximize.png"));


    // bouton qui permet d'ouvrir/enregistrer un point de vue
    _pointOfViewButton= new QToolButton(widgetContainer);
    _pointOfViewButton->setMaximumWidth(38);
    _pointOfViewButton->setMinimumWidth(38);
    _pointOfViewButton->setMinimumHeight(24);
    _pointOfViewButton->setToolTip(tr("Point de vue"));
    _pointOfViewButton->setIcon(QIcon(":/Icones/Icones/eye.png"));
    _pointOfViewButton->setPopupMode(QToolButton::InstantPopup);

    // son menu
    _pointOfViewMenu = new GPointOfViewDocumentManager(_pofManager, getKeyForPointOfViewManager(), _pointOfViewButton);
    _pointOfViewButton->setMenu(_pointOfViewMenu);

    // bouton qui permet d'exporter les CT_AbstractItemDrawable selectionnes
    _buttonExport = new QPushButton(widgetContainer);
    _buttonExport->setMaximumWidth(33);
    _buttonExport->setMinimumWidth(33);
    _buttonExport->setToolTip(tr("Exporter les éléments sélectionnés"));
    _buttonExport->setIcon(QIcon(":/Icones/Icones/upload.png"));
    _buttonExport->setEnabled(false);

    // bouton qui permet d'ajouter les CT_AbstractItemDrawable selectionnes à un autre document
    m_buttonTransferToDocument = new QPushButton(widgetContainer);
    m_buttonTransferToDocument->setMaximumWidth(33);
    m_buttonTransferToDocument->setMinimumWidth(33);
    m_buttonTransferToDocument->setToolTip(tr("Ajouter les éléments sélectionnés au document..."));
    m_buttonTransferToDocument->setIcon(QIcon(":/Icones/Icones/transfer.png"));

    pluginExporterManagerReloaded();

    QPushButton *buttonPointsAttributes = new QPushButton(widgetContainer);
    buttonPointsAttributes->setMaximumWidth(33);
    buttonPointsAttributes->setMinimumWidth(33);
    buttonPointsAttributes->setToolTip(tr("Configurer les couleurs des points"));
    buttonPointsAttributes->setIcon(QIcon(":/Icones/Icones/gradient.png"));

    _buttonPixelSizeUp = new QPushButton(widgetContainer);
    _buttonPixelSizeUp->setMaximumWidth(33);
    _buttonPixelSizeUp->setMinimumWidth(33);
    _buttonPixelSizeUp->setToolTip(tr("Augmenter la taille des pixels"));
    _buttonPixelSizeUp->setIcon(QIcon(":/Icones/Icones/increase_size.png"));
    _buttonPixelSizeUp->setEnabled(true);

    _buttonPixelSizeDown = new QPushButton(widgetContainer);
    _buttonPixelSizeDown->setMaximumWidth(33);
    _buttonPixelSizeDown->setMinimumWidth(33);
    _buttonPixelSizeDown->setToolTip(tr("Diminuer la taille des pixels"));
    _buttonPixelSizeDown->setIcon(QIcon(":/Icones/Icones/decrease_size.png"));
    _buttonPixelSizeDown->setEnabled(true);

    m_buttonFastDrawMode = new QPushButton(widgetContainer);
    m_buttonFastDrawMode->setMaximumWidth(33);
    m_buttonFastDrawMode->setMinimumWidth(33);
    m_buttonFastDrawMode->setEnabled(true);

    _buttonCameraType = new QPushButton(widgetContainer);
    _buttonCameraType->setMaximumWidth(33);
    _buttonCameraType->setMinimumWidth(33);
    _buttonCameraType->setToolTip(tr("Passer en vue Perspective"));
    _buttonCameraType->setIcon(QIcon(":/Icones/Icones/orthoview.png"));
    _buttonCameraType->setEnabled(true);

    connect(GUI_MANAGER->getPluginManager(), SIGNAL(finishLoading()), this, SLOT(pluginExporterManagerReloaded()));

    // fenetre de configuration des options
    _graphicsOptionsView = new GGraphicsViewOptions();

    // bouton qui appelle la fentre de configuration des options
    QPushButton *buttonShowOptions = new QPushButton(widgetContainer);
    buttonShowOptions->setMaximumWidth(33);
    buttonShowOptions->setMinimumWidth(33);
    buttonShowOptions->setToolTip(tr("Configurer l'affichage"));
    buttonShowOptions->setIcon(QIcon(":/Icones/Icones/preferences-system.png"));

    layout->addWidget(_cameraOptionsView);
    layout->addWidget(_pointOfViewButton);
    layout->addWidget(screenshotButton);
    layout->addWidget(_buttonPixelSizeDown);
    layout->addWidget(_buttonPixelSizeUp);
    layout->addWidget(m_buttonFastDrawMode);
    layout->addWidget(_buttonCameraType);
    layout->addWidget(buttonShowOptions);
    layout->addWidget(buttonPointsAttributes);
    layout->addWidget(_buttonExport);
    layout->addWidget(m_buttonTransferToDocument);
//    layout->addWidget(maximizeButton);
    layout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Expanding));

    ((QVBoxLayout*)parent->layout())->insertWidget(0, widgetContainer);

    connect(screenshotButton, SIGNAL(clicked()), this, SLOT(takeAndSaveScreenshot()));
    connect(buttonShowOptions, SIGNAL(clicked()), this, SLOT(showOptions()));
    connect(buttonPointsAttributes, SIGNAL(clicked()), this, SLOT(showAttributesOptions()));
    connect(_buttonPixelSizeUp, SIGNAL(clicked()), this, SLOT(changePixelSizeUp()));
    connect(_buttonCameraType, SIGNAL(clicked()), this, SLOT(changeCameraType()));
    connect(m_buttonFastDrawMode, SIGNAL(clicked()), this, SLOT(changeFastDrawMode()));
    connect(_buttonPixelSizeDown, SIGNAL(clicked()), this, SLOT(changePixelSizeDown()));
    connect(_pointOfViewMenu, SIGNAL(addActualPointOfView()), this, SLOT(addActualPointOfView()));
    connect(_pointOfViewMenu, SIGNAL(setPointOfView(DM_PointOfView*)), this, SLOT(setPointOfView(DM_PointOfView*)), Qt::DirectConnection);

    connect(_cameraOptionsView, SIGNAL(syncGraphics(bool)), this, SLOT(syncChanged(bool)));

    connect(_graphicsOptionsView, SIGNAL(optionsChanged()), this, SLOT(updateButtonFastDrawMode()));
    connect(_graphicsOptionsView, SIGNAL(optionsChanged()), this, SLOT(updateButtonCameraType()));

    connect((GDocumentManagerView*)GUI_MANAGER->getDocumentManagerView(), SIGNAL(documentAdded(DM_DocumentView*)), this, SLOT(updateMenuOfTransferButton()));
    connect((GDocumentManagerView*)GUI_MANAGER->getDocumentManagerView(), SIGNAL(documentRemoved()), this, SLOT(updateMenuOfTransferButton()));

    updateButtonFastDrawMode();
//    connect (maximizeButton, SIGNAL(clicked()), this, SLOT(detachView()));
}

void GDocumentViewForGraphics::detachView()
{
    if (_viewDetached)
    {
        getSubWindow()->setParent(_previousParent, _flags);
        getSubWindow()->show();
        _viewDetached = false;
    } else {

        getSubWindow()->showMaximized();
        _previousParent = (QWidget*) parent();
        _flags = getSubWindow()->windowFlags();
        getSubWindow()->setParent(nullptr, Qt::Window);
        setCurrentAction(nullptr, false);
        _viewDetached = true;
    }
}

void GDocumentViewForGraphics::createAndAddGraphicsWidgetContainer(QWidget *parent)
{
    QWidget *graphicsWidgetContainer = new QWidget(parent);

    _layoutGraphics = new QHBoxLayout();
    _layoutGraphics->setContentsMargins(0, 0, 0, 0);
    _layoutGraphics->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));

    delete graphicsWidgetContainer->layout();
    graphicsWidgetContainer->setLayout(_layoutGraphics);

    parent->layout()->addWidget(graphicsWidgetContainer);
}

QString GDocumentViewForGraphics::getKeyForPointOfViewManager()
{
    return _type;
}

bool GDocumentViewForGraphics::setCurrentAction(bool internal, CT_AbstractAction *action, bool deleteAction)
{
    if(m_graphics == nullptr) {

        if(deleteAction && !GUI_MANAGER->getActionsManager()->existActionCompareAddress(action))
            delete action;

        return false;
    }

    if(!acceptAction(action))
    {
        if(deleteAction && !GUI_MANAGER->getActionsManager()->existActionCompareAddress(action))
            delete action;

        return false;
    }

    if(action == nullptr)
        m_graphics->setCurrentAction(nullptr);
    else {
        CT_AbstractActionForGraphicsView* newAction = (CT_AbstractActionForGraphicsView*)action->createInstance();

        if(internal)
            m_graphics->setCurrentInternalAction(newAction);
        else
            m_graphics->setCurrentAction(newAction);
    }

    redrawGraphics();

    if(deleteAction && !GUI_MANAGER->getActionsManager()->existActionCompareAddress(action))
        delete action;

    emit currentActionChanged(currentAction());

    return true;
}

void GDocumentViewForGraphics::updateButtonFastDrawMode()
{
    // if Normal
    if (mustReduceNumberOfPointsInFastDraw()
            && !mustReduceNumberOfPointsInNormalDraw())
    {
        m_buttonFastDrawMode->setIcon(QIcon(":/Icones/Icones/fast_onmove.png"));
        m_buttonFastDrawMode->setToolTip(tr("Mode simplifié (lors de l'arrêt du mouvement)"));
    }
    // if Always
    else if (mustReduceNumberOfPointsInFastDraw()
             && mustReduceNumberOfPointsInNormalDraw())
    {
        m_buttonFastDrawMode->setIcon(QIcon(":/Icones/Icones/fast_always.png"));
        m_buttonFastDrawMode->setToolTip(tr("Mode simplifié (toujours)"));
    }
    // if Never
    else
    {
        m_buttonFastDrawMode->setIcon(QIcon(":/Icones/Icones/fast_never.png"));
        m_buttonFastDrawMode->setToolTip(tr("Mode simplifié (jamais)"));
    }
}

void GDocumentViewForGraphics::updateButtonCameraType()
{
    DM_GraphicsViewOptions opt = getOptions();

    if(opt.getCameraType() == CameraInterface::PERSPECTIVE) {
        _buttonCameraType->setIcon(QIcon(":/Icones/Icones/perspview.png"));
        _buttonCameraType->setToolTip(tr("Passer en vue Orthoscopique"));
    } else {
        _buttonCameraType->setIcon(QIcon(":/Icones/Icones/orthoview.png"));
        _buttonCameraType->setToolTip(tr("Passer en vue Perspective"));
    }
}

void GDocumentViewForGraphics::updateMenuOfTransferButton()
{
    delete m_buttonTransferToDocument->menu();

    DM_DocumentMenuGenerator generator;
    QMenu* menu = generator.createMenuXXXToDocuments(tr("Ajouter les items sélectionnés au document"), this, SLOT(addSelectedItemsToDocument()), QList<int>() << getNumber());

    if(menu->isEmpty()) {
        delete menu;
        menu = nullptr;
    } else {
        QMenu* top = new QMenu();
        top->addMenu(menu);
        menu = top;
    }

    m_buttonTransferToDocument->setMenu(menu);
    m_buttonTransferToDocument->setEnabled(menu != nullptr);
}

void GDocumentViewForGraphics::addSelectedItemsToDocument()
{
    QAction *act = (QAction*)sender();
    int number = act->data().toInt();

    QList<DocumentInterface*> documents = GUI_MANAGER->getDocumentManagerView()->documents();
    QListIterator<DocumentInterface*> it(documents);

    while(it.hasNext())
    {
        DM_DocumentView *doc = (DM_DocumentView*)it.next();

        if(doc->getNumber() == number)
        {
            QList<CT_AbstractItemDrawable *> items = getSelectedItemDrawable();

            GUI_MANAGER->asyncAddAllItemDrawableOfListOnView(items, doc, nullptr);

            return;
        }
    }
}

DM_AbstractInfo* GDocumentViewForGraphics::createNewItemInformation(const CT_AbstractItemDrawable *item) const
{
    DM_ItemInfoForGraphics *info = new DM_ItemInfoForGraphics();

    const CT_AbstractSingularItemDrawable *si = dynamic_cast<const CT_AbstractSingularItemDrawable*>(item);

    if(si != nullptr)
        info->setColor(si->defaultColor());

    return info;
}

void GDocumentViewForGraphics::recursiveSetColor(CT_StandardItemGroup *group,
                                                 const QColor &color)
{
    if(group == nullptr)
        return;

    CT_AbstractResult *lastResult = nullptr;
    QHash<CT_AbstractItemDrawable*, DM_AbstractInfo*> *hash = nullptr;

    for(const CT_StandardItemGroup* cChild : group->groups())
    {
        CT_StandardItemGroup* child = const_cast<CT_StandardItemGroup*>(cChild);

        if(lastResult != child->result())
        {
            lastResult = child->result();

            hash = getItemsInformations().value(lastResult, nullptr);

            if(hash == nullptr)
                hash = createItemInformationsForResult(lastResult);
        }

        if(hash != nullptr) {
            DM_ItemInfoForGraphics *childInfo = static_cast<DM_ItemInfoForGraphics*>(hash->value(child, nullptr));

            if(childInfo == nullptr)
            {
                childInfo = static_cast<DM_ItemInfoForGraphics*>(createNewItemInformation(child));
                hash->insert(child, childInfo);
            }

            childInfo->setColor(color);

            recursiveSetColor(child, color);
        }

    }

    for(const CT_AbstractSingularItemDrawable* cChild : group->singularItems())
    {
        CT_AbstractSingularItemDrawable* child = const_cast<CT_AbstractSingularItemDrawable*>(cChild);

        if(lastResult != child->result())
        {
            lastResult = child->result();

            hash = getItemsInformations().value(lastResult, nullptr);

            if(hash == nullptr)
                hash = createItemInformationsForResult(lastResult);

        }

        if(hash != nullptr) {
            DM_ItemInfoForGraphics *childInfo = static_cast<DM_ItemInfoForGraphics*>(hash->value(child, nullptr));

            if(childInfo == nullptr)
            {
                childInfo = static_cast<DM_ItemInfoForGraphics*>(createNewItemInformation(child));
                hash->insert(child, childInfo);
            }

            childInfo->setColor(color);
        }
    }
}
