#include "gstepmanager2.h"
#include "ui_gstepmanager2.h"

#include "cdm_stepmanager.h"
#include "dm_guimanager.h"
#include "ct_step/abstract/ct_abstractsteploadfile.h"
#include "ct_result/abstract/ct_abstractresult.h"

#include <QFileInfo>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QList>

GStepManager2::GStepManager2(CDM_StepManager& stepManager,
                             QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GStepManager2),
    m_stepManager(stepManager)
{
    ui->setupUi(this);

    m_protectTreeMutex = new QMutex(QMutex::Recursive);

    m_contextMenu = new GTreeStepContextMenu(stepManager, this);

    m_updateTreeTimer.setInterval(33);
    m_updateTreeTimer.setSingleShot(false);

    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeWidget->topLevelItem(0)->setData(0, Qt::UserRole, DT_Others);

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    ui->treeWidget->header()->setResizeMode(0, QHeaderView::ResizeToContents);
#else
    ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
#endif

    ui->treeWidget->setStyleSheet(QString("QTreeView::item:selected{"
                                             "color: rgb(255, 255, 255);"
                                             "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #F3C870, stop: 1 #BDA757);"
                                             "}"));

    connect(&m_updateTreeTimer, SIGNAL(timeout()), this, SLOT(updateTree()), Qt::QueuedConnection);
    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(itemClicked(QTreeWidgetItem*,int)), Qt::DirectConnection);
    connect(ui->treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(itemDoubleClicked(QTreeWidgetItem*,int)), Qt::DirectConnection);
    connect(ui->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), Qt::DirectConnection);

    connect(&stepManager, SIGNAL(stepAdded(CT_VirtualAbstractStep*)), this, SLOT(stepAdded(CT_VirtualAbstractStep*)), Qt::QueuedConnection);
    connect(&stepManager, SIGNAL(stepInserted(int,CT_VirtualAbstractStep*)), this, SLOT(stepInserted(int,CT_VirtualAbstractStep*)), Qt::QueuedConnection);
    connect(&stepManager, SIGNAL(stepToBeRemoved(CT_VirtualAbstractStep*)), this, SLOT(stepToBeRemoved(CT_VirtualAbstractStep*)), Qt::DirectConnection);

    connect(m_contextMenu, SIGNAL(expandAll()), this, SLOT(expandAllTypeSameAsCurrentItem()));
    connect(m_contextMenu, SIGNAL(collapseAll()), this, SLOT(collapseAllTypeSameAsCurrentItem()));
    connect(m_contextMenu, SIGNAL(locateSelectedStepInMenu(CT_VirtualAbstractStep*)), this, SIGNAL(locateStepInMenu(CT_VirtualAbstractStep*)));

    m_updateTreeTimer.start();
}

GStepManager2::~GStepManager2()
{
    delete ui;
    delete m_protectTreeMutex;
    delete m_contextMenu;
}

void GStepManager2::setStepNameConfiguration(GStepViewDefault::DisplayNameConfigs config)
{
    m_stepNameConfig = config;
}

void GStepManager2::addOpenFileStep(QString filePath)
{
    QMutexLocker locker(m_protectTreeMutex);

    CDM_PluginManager *pluginManager = m_stepManager.getScriptManager()->getPluginManager();

    if(pluginManager->isAPluginLoaded())
    {
        QList<CT_AbstractStepLoadFile*>    stepLfList;
        CT_AbstractStepLoadFile            *stepToCopy = nullptr;

        int count = pluginManager->countPluginLoaded();

        for(int i =0; i<count; ++i)
        {
            QList<CT_AbstractStepLoadFile*> steps = pluginManager->getPlugin(i)->getOpenFileStep(filePath);

            if(!steps.isEmpty())
            {
                stepToCopy = steps.first();
                stepLfList.append(steps);
            }
        }

        if(stepLfList.size() > 1)
        {
            stepToCopy = nullptr;

            QDialog dialog(this);
            dialog.setLayout(new QVBoxLayout());

            QComboBox *cb = new QComboBox(&dialog);
            QDialogButtonBox *dbb = new QDialogButtonBox(QDialogButtonBox::Ok
                                                         | QDialogButtonBox::Cancel,
                                                         Qt::Horizontal,
                                                         &dialog);

            connect(dbb, SIGNAL(accepted()), &dialog, SLOT(accept()));
            connect(dbb, SIGNAL(rejected()), &dialog, SLOT(reject()));

            dialog.layout()->addWidget(cb);
            dialog.layout()->addWidget(dbb);

            QListIterator<CT_AbstractStepLoadFile*> itLf(stepLfList);

            while(itLf.hasNext())
            {
                CT_AbstractStepLoadFile *stepLf = itLf.next();

                cb->addItem(GStepViewDefault::staticGetStepNameFromConfiguration(stepLf, m_stepNameConfig, true) + " (" + pluginManager->getPluginName(stepLf->pluginStaticCastT<>()) + ")");
            }

            if(dialog.exec() == 1)
            {
                stepToCopy = stepLfList.at(cb->currentIndex());
            }
        }

        if(stepToCopy != nullptr)
        {
            stepLfList.removeOne(stepToCopy);

            CT_VirtualAbstractStep *newStep = stepToCopy->pluginStaticCastT<>()->createNewInstanceOfStep(*stepToCopy, nullptr);

            CT_AbstractStepLoadFile *newStepLF = dynamic_cast<CT_AbstractStepLoadFile*>(newStep);

            if(newStepLF != nullptr)
            {
                if(newStepLF->setFilePath(filePath))
                {
                    if(newStepLF->showPreInputConfigurationDialog())
                    {
                        if(newStepLF->showInputResultConfigurationDialog())
                        {
                            if(newStepLF->finalizeConfiguration())
                            {
                                m_stepManager.addStep(newStepLF);
                                return;
                            }
                        }
                    }
                }
            }

            delete newStep;
        }
    }
}

void GStepManager2::executeStep(CT_VirtualAbstractStep* step)
{
    QMutexLocker locker(m_protectTreeMutex);

    if(!m_stepManager.isRunning())
    {
        m_contextMenu->setSelectedStep(step);

        if(m_contextMenu->checkIfCanExecuteStepAndShowWarningMessage(false))
            m_stepManager.executeStep(step);

        return;
    }

    m_stepManager.executeStep(step);
}

void GStepManager2::executeOrForwardStepInDebugMode(CT_VirtualAbstractStep *step)
{
    QMutexLocker locker(m_protectTreeMutex);

    if(!m_stepManager.isRunning())
    {
        m_contextMenu->setSelectedStep(step);

        if(m_contextMenu->checkIfCanExecuteStepAndShowWarningMessage(false))
            m_stepManager.executeOrForwardStepInDebugMode(step);

        return;
    }

    m_stepManager.executeOrForwardStepInDebugMode(step);
}

void GStepManager2::executeOrForwardStepFastInDebugMode(CT_VirtualAbstractStep *step)
{
    QMutexLocker locker(m_protectTreeMutex);

    if(!m_stepManager.isRunning())
    {
        m_contextMenu->setSelectedStep(step);

        if(m_contextMenu->checkIfCanExecuteStepAndShowWarningMessage(false))
            m_stepManager.executeOrForwardStepFastInDebugMode(step);

        return;
    }

    m_stepManager.executeOrForwardStepFastInDebugMode(step);
}

void GStepManager2::executeOrForwardStepAutoInDebugMode(CT_VirtualAbstractStep *step)
{
    QMutexLocker locker(m_protectTreeMutex);

    if(!m_stepManager.isRunning())
    {
        m_contextMenu->setSelectedStep(step);

        if(m_contextMenu->checkIfCanExecuteStepAndShowWarningMessage(false))
            m_stepManager.executeOrForwardStepAutoInDebugMode(step);

        return;
    }

    m_stepManager.executeOrForwardStepAutoInDebugMode(step);
}

void GStepManager2::addStepToCurrentStepOrToRootAndConfigure(CT_VirtualAbstractStep* stepToCopy)
{
    QMutexLocker locker(m_protectTreeMutex);

    if((stepToCopy != nullptr) && (stepToCopy->pluginStaticCastT<>() != nullptr))
    {
        CT_VirtualAbstractStep* parentStep = extractStepFromItem(ui->treeWidget->currentItem());

        if(!stepToCopy->acceptAddAfterThisStep(parentStep)) {

            QString error = tr("Impossible d'ajouter l'étape %1").arg(GStepViewDefault::staticGetStepNameFromConfiguration(stepToCopy, m_stepNameConfig, true));

            if(parentStep != nullptr)
                error += tr(" après l'étape %2 car elles ne sont pas compatible !").arg(GStepViewDefault::staticGetStepNameFromConfiguration(parentStep, m_stepNameConfig, true));
            else
                error += tr(" à la racine !");

            GUI_LOG->addErrorMessage(LogInterface::gui, error);
        } else {
            CT_VirtualAbstractStep *stepCopied = stepToCopy->pluginStaticCastT<>()->createNewInstanceOfStep(*stepToCopy, parentStep);

            if(stepCopied->showPreInputConfigurationDialog())
            {
                if(stepCopied->showInputResultConfigurationDialog())
                {
                    if(stepCopied->showPostConfigurationDialog())
                    {
                        if(stepCopied->finalizeConfiguration())
                        {
                            m_stepManager.addStep(stepCopied, parentStep);
                            return;
                        }
                    }
                }
            }

            delete stepCopied;
        }
    }
}

CT_VirtualAbstractStep* GStepManager2::extractStepFromItem(QTreeWidgetItem *item)
{
    if(item == nullptr)
        return nullptr;

    if(item->data(0, Qt::UserRole).toInt() == DT_Step)
        return static_cast<CT_VirtualAbstractStep*>(item->data(0, Qt::UserRole+1).value<void*>());

    return nullptr;
}

CT_AbstractResult* GStepManager2::extractResultFromItem(QTreeWidgetItem *item)
{
    if(item == nullptr)
        return nullptr;

    if(item->data(0, Qt::UserRole).toInt() == DT_Result)
        return static_cast<CT_AbstractResult*>(item->data(0, Qt::UserRole+1).value<void*>());

    return nullptr;
}

void GStepManager2::setStepToItem(CT_VirtualAbstractStep *step, QTreeWidgetItem *item)
{
    item->setData(0, Qt::UserRole, DT_Step);
    item->setData(0, Qt::UserRole+1, QVariant::fromValue((void*)step));
}

void GStepManager2::setResultToItem(CT_AbstractResult *result, QTreeWidgetItem *item)
{
    item->setData(0, Qt::UserRole, DT_Result);
    item->setData(0, Qt::UserRole+1, QVariant::fromValue((void*)result));
}

void GStepManager2::recursiveUpdateTree(QTreeWidgetItem *parent)
{
    for(int i=0; i<parent->childCount(); ++i) {
        QTreeWidgetItem* child = parent->child(i);

        const int type = child->data(0, Qt::UserRole).toInt();

        bool updated = false;

        if(type == DT_Step)
            updated = updateStepInTree(child);
        else if(type == DT_Result)
            updated = updateResultInTree(child);

        if(updated)
            recursiveUpdateTree(child);
        else
            --i;
    }
}

void GStepManager2::addResultsThatMustBe()
{
    QMutexLocker locker(m_protectTreeMutex);

    QMutableHashIterator<CT_AbstractResult*, QPair<CT_VirtualAbstractStep*, QTreeWidgetItem*> > it(m_resultsToAdd);

    while(it.hasNext()) {
        it.next();

        const QPair<CT_VirtualAbstractStep*, QTreeWidgetItem*> pair = it.value();

        QTreeWidgetItem* parentItem = m_steps.value(pair.first);

        if(parentItem != nullptr) {
            parentItem->insertChild(0, pair.second);
            m_results.insert(it.key(), pair.second);
            ui->treeWidget->expandItem(parentItem);
        } else {
            delete pair.second;
        }

        it.remove();
    }
}

QTreeWidgetItem* GStepManager2::createAndAddItemForStep(CT_VirtualAbstractStep *step, QTreeWidgetItem *parent, int insertRow)
{
    QTreeWidgetItem* item = new QTreeWidgetItem();
    setStepToItem(step, item);

    if(insertRow < 0)
        parent->addChild(item);
    else
        parent->insertChild(insertRow, item);

    ui->treeWidget->setItemWidget(item, 1, new QLabel(""));
    ui->treeWidget->expandItem(parent);
    ui->treeWidget->setCurrentItem(item);

    updateStepInTree(item);

    connect(step, SIGNAL(resultAdded(const CT_AbstractResult*)), this, SLOT(resultAdded(const CT_AbstractResult*)), Qt::DirectConnection);
    connect(step, SIGNAL(resultToBeRemoved(const CT_AbstractResult*)), this, SLOT(resultToBeRemoved(const CT_AbstractResult*)), Qt::DirectConnection);

    return item;
}

QTreeWidgetItem *GStepManager2::createItemForResult(const CT_AbstractResult* result)
{
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setCheckState(0, Qt::Unchecked);
    setResultToItem((CT_AbstractResult*)result, item);

    updateResultInTree(item);

    return item;
}

void GStepManager2::showContextMenu(const QPoint &p)
{
    QMutexLocker locker(m_protectTreeMutex);

    QTreeWidgetItem* item = ui->treeWidget->currentItem();

    if(item != nullptr) {
        const int type = item->data(0, Qt::UserRole).toInt();

        if(type == DT_Step) {
            CT_VirtualAbstractStep* step = extractStepFromItem(item);

            if(step != nullptr) {
                m_contextMenu->setSelectedStep(step);
                m_contextMenu->exec(ui->treeWidget->viewport()->mapToGlobal(p));
            }
        } else if(item == ui->treeWidget->topLevelItem(0)) {

            QMenu menu(this);

            menu.addAction(QIcon(":/Icones/Icones/delete.png"), tr("Supprimer toutes les étapes"), this, SLOT(removeAllStepInWorkflow()));
            menu.addAction(QIcon(":/Icones/Icones/expand.png"), tr("Déplier"), this, SLOT(expandCurrentItem()));
            menu.addAction(QIcon(":/Icones/Icones/collapse.png"), tr("Replier"), this, SLOT(collapseCurrentItem()));

            menu.exec(ui->treeWidget->viewport()->mapToGlobal(p));
        }
    }
}

void GStepManager2::itemClicked(QTreeWidgetItem *item, int column)
{
    QMutexLocker locker(m_protectTreeMutex);

    if(item != nullptr) {

        if(column == 0) {
            CT_AbstractResult* result = extractResultFromItem(item);

            if(result != nullptr)
                userCheckItem(item, item->checkState(0) == Qt::Checked);

        } else if(column == 1) {
            CT_VirtualAbstractStep* step = extractStepFromItem(item);

            if(step != nullptr) {
                if(step->isDebuggable())
                    step->setDebugModeOn(!step->isDebugModeOn());
                else
                    QMessageBox::warning(this, tr("Impossible"), tr("L'étape n'est pas débogable."));
            }
        }
    }
}

void GStepManager2::itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)

    QMutexLocker locker(m_protectTreeMutex);

    if(item != nullptr) {
        CT_AbstractResult* result = extractResultFromItem(item);

        if(result != nullptr) {
            item->setCheckState(0, (item->checkState(0) == Qt::Checked) ? Qt::Unchecked : Qt::Checked);
            userCheckItem(item, item->checkState(0) == Qt::Checked);
        }
    }
}

void GStepManager2::currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous)

    emit currentStepChanged(extractStepFromItem(current));
}

void GStepManager2::removeAllStepInWorkflow()
{
    GUI_MANAGER->asyncRemoveAllStep(nullptr);
}

void GStepManager2::expandCurrentItem()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();

    if(item != nullptr)
        ui->treeWidget->expandItem(item);
}

void GStepManager2::collapseCurrentItem()
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();

    if(item != nullptr)
        ui->treeWidget->collapseItem(item);
}

void GStepManager2::expandAllTypeSameAsCurrentItem()
{
    expandOrCollapseAllTypeSameAsCurrentItem(true);
}

void GStepManager2::collapseAllTypeSameAsCurrentItem()
{
    expandOrCollapseAllTypeSameAsCurrentItem(false);
}

void GStepManager2::stepAdded(CT_VirtualAbstractStep *step)
{
    // GUI thread
    QMutexLocker locker(m_protectTreeMutex);

    QTreeWidgetItem* parentItem = ui->treeWidget->topLevelItem(0);

    if(step->parentStep() != nullptr)
        parentItem = m_steps.value(step->parentStep());

    if(parentItem != nullptr) {
        QTreeWidgetItem* child = createAndAddItemForStep(step, parentItem);
        m_steps.insert(step, child);
    }
}

void GStepManager2::stepInserted(int row, CT_VirtualAbstractStep *step)
{
    // GUI thread
    QMutexLocker locker(m_protectTreeMutex);

    QTreeWidgetItem* parentItem = m_steps.value(step, nullptr);

    if(parentItem != nullptr) {
        QTreeWidgetItem* child = createAndAddItemForStep(step, parentItem, row);
        m_steps.insert(step, child);
    }
}

void GStepManager2::stepToBeRemoved(CT_VirtualAbstractStep* step)
{
    // NOT GUI thread !
    QMutexLocker locker(m_protectTreeMutex);

    setStepToItem(nullptr, m_steps.take(step));
}

void GStepManager2::resultAdded(const CT_AbstractResult* result)
{
    // NOT GUI thread !
    QMutexLocker locker(m_protectTreeMutex);

    QTreeWidgetItem* parentItem = m_steps.value(result->parentStepStaticCastT<>());
    if(parentItem != nullptr) {
        QTreeWidgetItem* child = createItemForResult(result);
        m_resultsToAdd.insert((CT_AbstractResult*)result, qMakePair(result->parentStepStaticCastT<>(), child));
    }
}

void GStepManager2::resultToBeRemoved(const CT_AbstractResult *result)
{
    // NOT GUI thread !
    QMutexLocker locker(m_protectTreeMutex);

    QTreeWidgetItem* item = m_results.take((CT_AbstractResult*)result);

    if(item == nullptr)
        delete m_resultsToAdd.take((CT_AbstractResult*)result).second;
    else
        setResultToItem(nullptr, item);
}

void GStepManager2::updateTree()
{
    QMutexLocker locker(m_protectTreeMutex);

    // GUI thread
    QTreeWidgetItem* workflowRoot = ui->treeWidget->topLevelItem(0);

    recursiveUpdateTree(workflowRoot);

    addResultsThatMustBe();

    deleteItems();
}

bool GStepManager2::updateResultInTree(QTreeWidgetItem* child)
{
    QString name;
    bool isClearedFromMemory;
    bool isBusy;
    bool hasModel;
    bool isVisibleInItemModelView;

    if(extractResultInformationFromTree(child, name, isClearedFromMemory, isBusy, hasModel, isVisibleInItemModelView)) {
        child->setDisabled(isBusy || !hasModel);
        child->setText(0, name);
        if((!hasModel || !isVisibleInItemModelView) && (child->checkState(0) == Qt::Checked))
            child->setCheckState(0, Qt::Unchecked);

        for(int i=0; i<ui->treeWidget->columnCount(); ++i) {
            if(isClearedFromMemory)
                child->setBackground(i, QBrush(QColor(236, 208, 209)));
            else
                child->setBackground(i, QBrush(QColor(220, 220, 220)));
        }

        return true;
    }

    if(child->parent() != nullptr)
        child->parent()->removeChild(child);

    // delete child;
    m_itemsToDelete.append(child);

    return false;
}

bool GStepManager2::extractResultInformationFromTree(QTreeWidgetItem *child, QString &name, bool& isClearedFromMemory, bool &isBusy, bool& hasModel, bool& isVisibleInItemModelView)
{
    QMutexLocker locker(m_protectTreeMutex);

    CT_AbstractResult* result = extractResultFromItem(child);

    if(result != nullptr) {
        name = result->displayableName();
        isClearedFromMemory = result->isClearedFromMemory();
        isBusy = result->isBusy();
        hasModel = (result->model() != nullptr);
        isVisibleInItemModelView = GUI_MANAGER->getItemDrawableModelManager()->containsResult(result);
    }

    return (result != nullptr);
}

bool GStepManager2::updateStepInTree(QTreeWidgetItem* child)
{
    QString name;
    QString filepath;
    int progress;
    bool isSettingsModified;
    bool debugModeOn;
    int executeTime;
    bool isManual;

    if(extractStepInformationFromTree(child, name, filepath, progress, isSettingsModified, debugModeOn, isManual, executeTime)) {
        updateStepNameInTree(child, name, isSettingsModified, progress, filepath);
        updateStepDebugInTree(child, debugModeOn);
        updateStepExecuteTimeInTree(child, executeTime);
        updateStepColorsInTree(child, isManual);
        return true;
    }

    if(child->parent() != nullptr)
        child->parent()->removeChild(child);

    // delete child;
    m_itemsToDelete.append(child);

    return false;
}

bool GStepManager2::extractStepInformationFromTree(QTreeWidgetItem* child,
                                                   QString& name,
                                                   QString& filepath,
                                                   int& progress,
                                                   bool& isSettingsModified,
                                                   bool& debugModeOn,
                                                   bool& isManual,
                                                   int& executeTime)
{
    QMutexLocker locker(m_protectTreeMutex);

    CT_VirtualAbstractStep* step = extractStepFromItem(child);

    if(step != nullptr) {
        filepath = "";
        name = GStepViewDefault::staticGetStepNameFromConfiguration(step, m_stepNameConfig, true);
        debugModeOn = step->isDebugModeOn();
        progress = step->progressValue();
        isSettingsModified = step->isSettingsModified();
        executeTime = step->executeTime();
        isManual = step->isManual();

        CT_AbstractStepLoadFile *stepLF = dynamic_cast<CT_AbstractStepLoadFile*>(step);

        if(stepLF != nullptr)
            filepath = stepLF->filePath();
    }

    return (step != nullptr);
}

void GStepManager2::updateStepNameInTree(QTreeWidgetItem* child, QString name, const bool& isSettingsModified, const int& progress, const QString& filepath)
{
    if(progress != 100)
        name = "(" + QString().setNum(progress) + "%)" + name;

    if(!filepath.isEmpty()) {
        QFileInfo info(filepath);

        if(!info.fileName().isEmpty())
            name += tr("( %1 )").arg(info.fileName());
    }

    child->setText(0, name);

    if(isSettingsModified && ((progress == 0) || (progress == 100)))
        child->setIcon(0, QIcon(":/Icones/Icones/view-refresh.png"));
    else if(progress == 100)
        child->setIcon(0, QIcon(":/Icones/Icones/valid.png"));
    else
        child->setIcon(0, QIcon(":/Icones/Icones/in_progress.png"));
}

void GStepManager2::updateStepDebugInTree(QTreeWidgetItem* child, bool debugModeOn)
{
    QLabel* debugLabel = static_cast<QLabel*>(ui->treeWidget->itemWidget(child, 1));

    if(debugModeOn)
        debugLabel->setPixmap(QPixmap(":/Icones/Icones/debug_on.png"));
    else
        debugLabel->setPixmap(QPixmap(":/Icones/Icones/debug_off.png"));
}

void GStepManager2::updateStepExecuteTimeInTree(QTreeWidgetItem* child, int executeTime)
{
    const int hour = executeTime/3600000;
    executeTime -= hour*3600000;
    const int min = executeTime/60000;
    executeTime -= min*60000;
    const int sec = executeTime/1000;
    executeTime -= sec*1000;
    const int ms = executeTime;

    child->setText(2, QString("%1h:%2m:%3s:%4ms").arg(hour).arg(min).arg(sec).arg(ms));
}

void GStepManager2::updateStepColorsInTree(QTreeWidgetItem* child, bool isManual)
{
    for(int i=0; i<ui->treeWidget->columnCount(); ++i) {
        child->setBackground(i, isManual ? QBrush(QColor(183, 225, 170)) : QBrush(QColor(170, 186, 225)));
    }
}

void GStepManager2::expandOrCollapseAllTypeSameAsCurrentItem(bool expand)
{
    QQueue<QTreeWidgetItem*> items;

    QTreeWidgetItem* item = ui->treeWidget->currentItem();

    if(item != nullptr) {
        const int type = item->data(0, Qt::UserRole).toInt();

        items.enqueue(item);

        while(!items.isEmpty()) {

            item = items.dequeue();

            if(expand)
                ui->treeWidget->expandItem(item);
            else
                ui->treeWidget->collapseItem(item);

            const int size = item->childCount();

            for(int i=0; i<size; ++i) {
                QTreeWidgetItem* child = item->child(i);

                if(child->data(0, Qt::UserRole).toInt() == type)
                    items.enqueue(child);
            }
        }
    }
}

void GStepManager2::userCheckItem(QTreeWidgetItem *item, bool checked)
{
    QMutexLocker locker(m_protectTreeMutex);

    CT_AbstractResult* result = extractResultFromItem(item);

    if(result != nullptr) {
        if(checked) {
            GUI_MANAGER->editItemDrawableModelOfResult(*result);
        } else {
            if(GUI_MANAGER->removeEditItemDrawableModelOfResult(*result))
                GUI_MANAGER->asyncRemoveAllItemDrawableOfResultFromView(*result, nullptr);
        }
    }
}

void GStepManager2::deleteItems()
{
    QListIterator<QTreeWidgetItem*> it(m_itemsToDelete);

    while(it.hasNext())
    {
        QTreeWidgetItem* item = it.next();
        delete item;
    }

    m_itemsToDelete.clear();
}
