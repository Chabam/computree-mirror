#include "gaboutpluginsdialog.h"
#include "ui_gaboutpluginsdialog.h"

#include "view/MainView/gaboutstepdialog.h"

#include "cdm_tools.h"
#include "dm_guimanager.h"

#include "ct_actions/ct_actionsseparator.h"
#include "ct_actions/abstract/ct_abstractaction.h"
#include "ct_exporter/ct_standardexporterseparator.h"
#include "ct_exporter/abstract/ct_abstractexporter.h"
#include "ct_step/abstract/ct_abstractsteploadfile.h"
#include "ct_step/abstract/ct_abstractstepcanbeaddedfirst.h"
#include "ct_step/tools/menu/ct_menulevel.h"
#include "ct_abstractstepplugin.h"

#include <QListWidgetItem>
#include <QFileDialog>
#include <QDebug>

#define EOL "&#13;&#10;"

GAboutPluginsDialog::GAboutPluginsDialog(const CDM_PluginManager &pManager,
                                         QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GAboutPluginsDialog)
{
    ui->setupUi(this);

    //ui->pb_redmineExport->setVisible(false);

    _pManager = (CDM_PluginManager*)&pManager;
    _mustReloadStep = false;

    connect(ui->pushButtonOk, SIGNAL(clicked()), this, SLOT(close()));

    initView();
}

GAboutPluginsDialog::~GAboutPluginsDialog()
{
    delete ui;
    _stepList.clear();
}

bool GAboutPluginsDialog::mustReloadStepPlugins() const
{
    return _mustReloadStep;
}

void GAboutPluginsDialog::initView()
{
    ui->treeWidget->clear();

    int n = _pManager->countPluginLoaded();

    // plugins d'étapes
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(ui->treeWidget);
    rootItem->setData(0, Qt::DisplayRole, tr("Plugins"));

    for(int i=0; i<n; ++i)
    {
        QString pluginName = _pManager->getPluginName(i);
        if (pluginName.left(5) == "plug_")
        {
            pluginName.remove(0, 5);
        }

        QTreeWidgetItem *item = new QTreeWidgetItem(rootItem);
        item->setData(0, Qt::DisplayRole, pluginName);
        item->setData(0, Qt::UserRole, I_Step);
        item->setData(0, Qt::UserRole+1, i);

        CT_AbstractStepPlugin *sPManager = _pManager->getPlugin(i);

        // Levels
        QList<CT_MenuLevel*> levels = _pManager->stepsMenu()->levels();
        QListIterator<CT_MenuLevel*> itL(levels);

        while(itL.hasNext()) {
            createItemsForLevelRecursively(item, itL.next(), sPManager);
        }

        // Actions
        QList<CT_ActionsSeparator*> l3 = sPManager->getActionsAvailable();
        QListIterator<CT_ActionsSeparator*> it3(l3);

        if(it3.hasNext())
        {
            QTreeWidgetItem *itemF = new QTreeWidgetItem(item);
            itemF->setText(0, tr("Action"));

            while(it3.hasNext())
            {
                QList<CT_AbstractAction*> l33 = it3.next()->actions();

                while(!l33.isEmpty())
                    createItemsForAction(itemF, l33.takeFirst());
            }
        }

        // Exporters
        QList<CT_StandardExporterSeparator*> l4 = sPManager->getExportersAvailable();
        QListIterator<CT_StandardExporterSeparator*> it4(l4);

        if(it4.hasNext())
        {
            QTreeWidgetItem *itemF = new QTreeWidgetItem(item);
            itemF->setText(0, tr("Exporter"));

            while(it4.hasNext())
            {
                QList<CT_AbstractExporter*> l44 = it4.next()->exporters();

                while(!l44.isEmpty())
                    createItemsForExporter(itemF, l44.takeFirst());
            }
        }
    }

    ui->treeWidget->insertTopLevelItem(0, rootItem);

    ui->pushButtonRecharger->setEnabled(GUI_MANAGER->getStepManager()->getStepRootList().isEmpty());

    if(!ui->pushButtonRecharger->isEnabled())
        ui->pushButtonRecharger->setToolTip(tr("Vous ne pouvez pas recharger les plugins tant que vous avez des étapes dans l'arbre des traitements"));
    else
        ui->pushButtonRecharger->setToolTip("");
}

void GAboutPluginsDialog::createItemsForLevelRecursively(QTreeWidgetItem *parent, CT_MenuLevel *level, const CT_AbstractStepPlugin *plugin)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setData(0, Qt::DisplayRole, level->displayableName());

    QList<CT_MenuLevel*> levels = level->levels();
    QListIterator<CT_MenuLevel*> itL(levels);

    while(itL.hasNext()) {
        createItemsForLevelRecursively(item, itL.next(), plugin);
    }

    QList<CT_VirtualAbstractStep*> steps = level->steps();
    QListIterator<CT_VirtualAbstractStep*> itS(steps);

    while(itS.hasNext()) {
        CT_VirtualAbstractStep *step = itS.next();

        if(step->pluginStaticCastT<>() == plugin)
            createItemsForStep(item, step);
    }

    if(item->childCount() == 0)
        delete item;
}

void GAboutPluginsDialog::createItemsForStep(QTreeWidgetItem *parent, CT_VirtualAbstractStep *step)
{
    QString stepName = step->pluginStaticCastT<>()->getKeyForStep(*step);
    QString keyName = parent->text(0)+ "_" + stepName;

    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, stepName);

    _stepList.insert(keyName, step);

    QTreeWidgetItem *itemIN = new QTreeWidgetItem(item);
    itemIN->setText(0, "IN");

    step->visitInResultModels([this, &itemIN](const CT_InAbstractResultModel* model) -> bool {
        this->recursiveCreateItemsForResultModel(itemIN, model);
        return true;
    });

    QTreeWidgetItem *itemOUT = new QTreeWidgetItem(item);
    itemOUT->setText(0, "OUT");

    step->visitOutResultModels([this, &itemOUT](const CT_OutAbstractResultModel* model) -> bool {
        this->recursiveCreateItemsForResultModel(itemOUT, model);
        return true;
    });
}

void GAboutPluginsDialog::createItemsForAction(QTreeWidgetItem *parent, CT_AbstractAction *act)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, act->title());
    item->setIcon(0, act->icon());
}

void GAboutPluginsDialog::createItemsForExporter(QTreeWidgetItem *parent, CT_AbstractExporter *ex)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, ex->uniqueName() + tr(" [%1]").arg(ex->displayableName()));
    /*item->setData(0, Qt::DisplayRole, _eManager->getPluginName(i));
    item->setData(0, Qt::UserRole, I_Export);
    item->setData(0, Qt::UserRole+1, i);*/
}

void GAboutPluginsDialog::recursiveCreateItemsForResultModel(QTreeWidgetItem *parent, const CT_OutAbstractResultModel *rModel) const
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, "Result : " + rModel->resultName());

    rModel->visitOutChildrens([this, &item](const CT_OutAbstractModel* child) -> bool {
        this->recursiveCreateItemsForModel(item, child);
        return true;
    });
}

void GAboutPluginsDialog::recursiveCreateItemsForResultModel(QTreeWidgetItem *parent, const CT_InAbstractResultModel *rModel) const
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, "Result : " + rModel->displayableName());

    rModel->visitInChildrens([this, &item](const CT_InAbstractModel* child) -> bool {
        this->recursiveCreateItemsForModel(item, child);
        return true;
    });
}

void GAboutPluginsDialog::recursiveCreateItemsForModel(QTreeWidgetItem *parent, const CT_AbstractModel *rModel) const
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0,rModel->displayableName());

    rModel->visitChildrens([this, &item](const CT_AbstractModel* child) -> bool {
        this->recursiveCreateItemsForModel(item, child);
        return true;
    });
}

void GAboutPluginsDialog::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);

    if(current != nullptr)
    {
        bool ok;
        int from = current->data(0, Qt::UserRole).toInt(&ok);

        if(ok)
        {
            if(from == I_Step)
            {
                CT_AbstractStepPlugin *tinterface = _pManager->getPlugin(current->data(0, Qt::UserRole+1).toInt());
                ui->pushButtonConfigurer->setEnabled((tinterface != nullptr)
                                                     && tinterface->canBeConfigured());
            }
        }
    }
}

void GAboutPluginsDialog::on_pushButtonConfigurer_clicked()
{
    QTreeWidgetItem *current = ui->treeWidget->currentItem();

    if(current != nullptr)
    {
        bool ok;
        int from = current->data(0, Qt::UserRole).toInt(&ok);

        if(ok)
        {
            if(from == I_Step)
            {
                int currentRow = current->data(0, Qt::UserRole+1).toInt();
                CT_AbstractStepPlugin *tinterface = _pManager->getPlugin(currentRow);

                if(tinterface != nullptr)
                    tinterface->configure();
            }
        }
    }
}

void GAboutPluginsDialog::on_pushButtonRecharger_clicked()
{
    _mustReloadStep = true;
    close();
}

void GAboutPluginsDialog::on_pb_redmineExport_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Choisir le fichier d'export"), "", tr("Fichier texte (*.txt)"));

    QFile exportFile(filename);
    if (exportFile.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream stream(&exportFile);

        stream << tr("StepName\tStepKey\tStepShortDescription\tMenu\tType\tPlugin\n");

        CT_StepsMenu *menu = _pManager->stepsMenu();

        QList<CT_MenuLevel*> levels = menu->levels();
        QListIterator<CT_MenuLevel*> it(levels);

        while(it.hasNext()) {
            CT_MenuLevel *level = it.next();
            exportStepsForLevel(stream, level, level->displayableName());
        }

    }
    exportFile.close();
}

void GAboutPluginsDialog::exportStepsForLevel(QTextStream &stream, CT_MenuLevel *level, QString levelName)
{
    // sub levels
    QList<CT_MenuLevel*> levels = level->levels();
    QListIterator<CT_MenuLevel*> it(levels);

    while(it.hasNext()) {

        CT_MenuLevel* subLevel = it.next();
        QString name = levelName;
        name.append("/");
        name.append(subLevel->displayableName());
        exportStepsForLevel(stream, subLevel, name);
    }

    // steps
    QList<CT_VirtualAbstractStep*> steps = level->steps();
    QListIterator<CT_VirtualAbstractStep*> itS(steps);

    while(itS.hasNext()) {
        CT_VirtualAbstractStep *step = itS.next();

        QString type = "";
        if(dynamic_cast<CT_AbstractStepLoadFile*>(step) != nullptr) {
            type = "LoadFile";
        } else if(dynamic_cast<CT_AbstractStepCanBeAddedFirst*>(step) != nullptr) {
            type = "CanBeAddedFirst";
        } else {
            type = "Generic";
        }

        stream << step->name() << "\t";
        stream << step->pluginStaticCastT<>()->getKeyForStep(*step) << "\t";
        stream << step->description() << "\t";
        stream << levelName << "\t";
        stream << type << "\t";
        stream << _pManager->getPluginName(_pManager->getPluginIndex(step->pluginStaticCastT<>())) << "\n";
    }
}




void GAboutPluginsDialog::on_pb_stepInfo_clicked()
{
    QTreeWidgetItem* currentItem = ui->treeWidget->currentItem();

    if (currentItem != nullptr)
    {
        QString stepName = currentItem->text(0);
        QTreeWidgetItem* parentItem = currentItem->parent();

        if (parentItem != nullptr)
        {
            QString keyName = parentItem->text(0)+ "_" + stepName;

            if (_stepList.contains(keyName))
            {
                CT_VirtualAbstractStep *step = _stepList.value(keyName);
                GAboutStepDialog dialog(step);
                dialog.exec();
            }
        }
    }
}
