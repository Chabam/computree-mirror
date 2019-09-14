#include "gaboutstepdialog.h"
#include "ui_gaboutstepdialog.h"

#include "dm_guimanager.h"

#include "ct_abstractstepplugin.h"
#include "ct_result/abstract/ct_abstractresult.h"
#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"
#include "ct_itemattributes/abstract/ct_abstractitemattribute.h"

#include "cdm_citationinfo.h"

#include "ct_categories/abstract/ct_abstractcategory.h"
#include "ct_global/ct_context.h"
#include "ct_categories/tools/ct_categorymanager.h"

GAboutStepDialog::GAboutStepDialog(CT_VirtualAbstractStep *step, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GAboutStepDialog)
{
    ui->setupUi(this);

    initView(step);
}

GAboutStepDialog::~GAboutStepDialog()
{
    delete ui;
}

void GAboutStepDialog::initView(CT_VirtualAbstractStep *step)
{
    ui->scrollArea->setWidgetResizable(true);

    QString pluginName = GUI_MANAGER->getPluginManager()->getPluginName(step->pluginStaticCastT<>());

    if (pluginName.left(5) == "plug_")
    {
        pluginName.remove(0, 5);
    }

    ui->pluginName->setText(pluginName);
    ui->stepName->setText(step->pluginStaticCastT<>()->getKeyForStep(*step));

    if (step->pluginStaticCastT<>()->getPluginURL() == step->URL())
    {
        ui->lb_wiki_link->setText(QString(tr("Aide en ligne : <a href=\"%1\">Page internet du plugin</a>")).arg(step->pluginStaticCastT<>()->getPluginURL()));
    } else {
        ui->lb_wiki_link->setText(QString(tr("Aide en ligne : <a href=\"%1\">Page internet de cette étape</a>")).arg(step->URL()));
    }

    ui->briefDescription->setText(step->description());

    QStringList citations = step->getStepRISCitations();

    if (citations.size() > 0)
    {
        ui->citation->setVisible(true);
        ui->cb_ris->setVisible(true);
        ui->ris->setVisible(false);
        QString citation = tr("<em>References</em> :<br><br>");
        QString risData;

        for (int i = 0 ; i < citations.size() ; i++)
        {
            citation.append(CDM_CitationInfo::parseRIS(citations.at(i)));
            if (i < citations.size() - 1) {citation.append("<br>");}

            risData.append(citations.at(i));
        }
        ui->citation->setText(citation);
        ui->ris->setText(risData);
    } else {
        ui->citation->setVisible(false);
        ui->cb_ris->setVisible(false);
        ui->ris->setVisible(false);
    }


    ui->detailledDescription->setText(tr("<em>Description détaillée</em> :<br><br>") + step->detailledDescription());

    // In models
    QTreeWidgetItem *inRootItem = new QTreeWidgetItem(ui->inModels);
    inRootItem->setData(0, Qt::DisplayRole, tr("Données en entrée :"));
    inRootItem->setExpanded(true);

    step->visitInResultModels([this, &inRootItem](const CT_InAbstractResultModel* child) -> bool {
        this->recursiveCreateItemsForResultModel(inRootItem, child);
        return true;
    });

    //ui->inModels->expandAll();

    // Out models
    QTreeWidgetItem *outRootItem = new QTreeWidgetItem(ui->outModels);
    outRootItem->setData(0, Qt::DisplayRole, tr("Données en sortie :"));
    outRootItem->setExpanded(true);

    step->visitOutResultModels([this, &outRootItem](const CT_OutAbstractResultModel* child) -> bool {
        this->recursiveCreateItemsForResultModel(outRootItem, child);
        return true;
    });

    //ui->outModels->expandAll();

    // Resize widgets to contents...
    int inModelsItemCount = 0;
    QTreeWidgetItemIterator iti(ui->inModels);
    while (*iti) {
      ++inModelsItemCount;
      ++iti;
    }

    int outModelsItemCount = 0;
    QTreeWidgetItemIterator ito(ui->outModels);
    while (*ito) {
      ++outModelsItemCount;
      ++ito;
    }

    int inSize = (inModelsItemCount + 1)*ui->inModels->visualItemRect(inRootItem).height();
    int outSize = (outModelsItemCount + 1)*ui->outModels->visualItemRect(outRootItem).height();

    ui->inModels->setMinimumHeight(inSize);
    ui->outModels->setMinimumHeight(outSize);

    ui->inModels->setMaximumHeight(inSize);
    ui->outModels->setMaximumHeight(outSize);
}

void GAboutStepDialog::recursiveCreateItemsForModel(QTreeWidgetItem *parent, const CT_OutAbstractModel *model) const
{
    const CT_OutAbstractResultModel *rModel = dynamic_cast<const CT_OutAbstractResultModel*>(model);

    if(rModel != nullptr)
    {
        recursiveCreateItemsForResultModel(parent, rModel);
    }
    else
    {
        const CT_OutAbstractItemModel *iModel = dynamic_cast<const CT_OutAbstractItemModel*>(model);

        if(iModel != nullptr)
        {
            recursiveCreateItemsForItemModel(parent, iModel);
        }
        else
        {
            const CT_OutAbstractItemAttributeModel *iaModel = dynamic_cast<const CT_OutAbstractItemAttributeModel*>(model);

            if(iaModel != nullptr)
                recursiveCreateItemsForItemAttributesModel(parent, iaModel);
            else
                qFatal("In GAboutStepDialog::recursiveCreateItemsForModel : model is not known");
        }
    }
}

void GAboutStepDialog::recursiveCreateItemsForModel(QTreeWidgetItem *parent, const CT_InAbstractModel *model) const
{
    const CT_InAbstractResultModel *rModel = dynamic_cast<const CT_InAbstractResultModel*>(model);

    if(rModel != nullptr)
    {
        recursiveCreateItemsForResultModel(parent, rModel);
    }
    else
    {
        const CT_InAbstractItemModel *iModel = dynamic_cast<const CT_InAbstractItemModel*>(model);

        if(iModel != nullptr)
        {
            recursiveCreateItemsForItemModel(parent, iModel);
        }
        else
        {
            const CT_InAbstractItemAttributeModel *iaModel = dynamic_cast<const CT_InAbstractItemAttributeModel*>(model);

            if(iaModel != nullptr)
                recursiveCreateItemsForItemAttributesModel(parent, iaModel);
            else
                qFatal("In GAboutStepDialog::recursiveCreateItemsForModel : model is not known");
        }
    }
}

void GAboutStepDialog::recursiveCreateItemsForResultModel(QTreeWidgetItem *parent, const CT_OutAbstractResultModel *rModel) const
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    //item->setText(0, QString("Result") + " / " + rModel->uniqueName() + " / " + rModel->modelTypeDisplayable() + " : " + rModel->resultName());
    item->setText(0, QString("Result / %1").arg(rModel->uniqueIndex()));

    item->setExpanded(true);
    createForChildrens(item, rModel);
}

void GAboutStepDialog::recursiveCreateItemsForResultModel(QTreeWidgetItem *parent, const CT_InAbstractResultModel *rModel) const
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, "Result : " + rModel->displayableName());

    setFontForInModel(item, rModel);

    item->setExpanded(true);
    createForChildrens(item, rModel);
}

void GAboutStepDialog::recursiveCreateItemsForItemModel(QTreeWidgetItem *parent, const CT_OutAbstractItemModel *iModel) const
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    //item->setText(0, iModel->displayableName() + " [" + iModel->itemDrawableStaticCastT<>()->name() + "]");

    QString stepName = (iModel->recursiveOriginalModelWithAStep() != nullptr ? iModel->recursiveOriginalModelWithAStep()->stepStaticCastT<>()->displayableCustomName() : "???");

    if(stepName.isEmpty())
        stepName = "???";

    item->setText(0, iModel->displayableName() + " (" + iModel->itemDrawableStaticCastT<>()->name() + ")" + " / " + stepName + QString(" / %1").arg(iModel->uniqueIndex()));
    item->setExpanded(true);
    createForChildrens(item, iModel);
}

void GAboutStepDialog::recursiveCreateItemsForItemModel(QTreeWidgetItem *parent, const CT_InAbstractItemModel *iModel) const
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, iModel->displayableName() + " [" + CT_AbstractItemDrawable::nameFromType(iModel->itemType()) + "]");

    setFontForInModel(item, iModel);

    item->setExpanded(true);
    createForChildrens(item, iModel);
}

void GAboutStepDialog::recursiveCreateItemsForItemAttributesModel(QTreeWidgetItem *parent, const CT_OutAbstractItemAttributeModel *iaModel) const
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, iaModel->itemAttributeStaticCastT<>()->displayableName() + " [" + iaModel->itemAttributeStaticCastT<>()->valueTypeToString() + "] / " + QString().setNum(iaModel->uniqueIndex()));
    //item->setText(0, iaModel->itemAttribute()->displayableName() + " (" + iaModel->itemAttribute()->typeToString() + ")" + " / " + (iaModel->lastOriginalModelWithAStep() != nullptr ? iaModel->lastOriginalModelWithAStep()->step()->getStepCustomName() : "???") + " / " + iaModel->uniqueName() + " / " + iaModel->modelTypeDisplayable());

    item->setExpanded(false);
    parent->setExpanded(false);
    createForChildrens(item, iaModel);
}

void GAboutStepDialog::recursiveCreateItemsForItemAttributesModel(QTreeWidgetItem *parent, const CT_InAbstractItemAttributeModel *iaModel) const
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, iaModel->displayableName() + " [" + CT_AbstractCategory::valueTypeToString(CT_AbstractCategory::ValueType(iaModel->valueType())) + "]");

    setFontForInModel(item, iaModel);

    item->setExpanded(false);
    parent->setExpanded(true);
    createForChildrens(item, iaModel);
}

void GAboutStepDialog::createForChildrens(QTreeWidgetItem *parent, const CT_OutAbstractModel *model) const
{
    model->visitOutChildrens([this, &parent](const CT_OutAbstractModel* child) -> bool {
        this->recursiveCreateItemsForModel(parent, child);
        return true;
    });
}

void GAboutStepDialog::createForChildrens(QTreeWidgetItem *parent, const CT_InAbstractModel *model) const
{
    model->visitInChildrens([this, &parent](const CT_InAbstractModel* child) -> bool {
        this->recursiveCreateItemsForModel(parent, child);
        return true;
    });
}

void GAboutStepDialog::setFontForInModel(QTreeWidgetItem* item, const CT_InAbstractModel* model) const
{
    // model->parentModel() == nullptr if it is a result model
    const bool obligatory = model->parentModel() == nullptr ? model->isObligatory() : model->recursiveAtLeastOneChildrenOrThisIsObligatory();
    QFont f = item->font(0);
    f.setItalic(!obligatory);
    f.setBold(obligatory);
    item->setFont(0, f);
}

void GAboutStepDialog::on_cb_ris_toggled(bool checked)
{
    Q_UNUSED(checked);
    if (ui->cb_ris->isChecked())
    {
        ui->ris->setVisible(true);
    } else {
        ui->ris->setVisible(false);
    }
}
