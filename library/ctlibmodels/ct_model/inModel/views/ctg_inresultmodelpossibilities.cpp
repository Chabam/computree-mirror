#include "ctg_inresultmodelpossibilities.h"
#include "ui_ctg_inresultmodelpossibilities.h"

#include "ct_model/inModel/tools/ct_instdresultmodelpossibility.h"
#include "ct_model/outModel/abstract/ct_outabstractmodel.h"
#include "ct_model/inModel/tools/ct_modelselectionhelper.h"

#include <QMimeData>
#include <QModelIndex>
#include <QMessageBox>

#define HELP_COLUMN 2

CTG_InResultModelPossibilities::CTG_InResultModelPossibilities(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTG_InResultModelPossibilities)
{
    ui->setupUi(this);

    ui->treeView->setModel(&m_treeViewModel);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);

    // to have text with black color when it was disabled
    ui->treeView->setStyleSheet(QString("QTreeView::item {"
                                "color: rgb(0, 0, 0);"
                                "}"
                                "QTreeView::item:has-children:!hover:!selected {"
                                "background: rgb(172, 226, 169);"
                                "}"
                                "QTreeView::item:!has-children:!hover:!selected {"
                                "background: rgb(220, 220, 220);"
                                "}"));
    m_readOnly = false;

    m_inputResultModelManager = NULL;

    constructHeader();
}

CTG_InResultModelPossibilities::~CTG_InResultModelPossibilities()
{
    delete ui;
}

void CTG_InResultModelPossibilities::setInResultModelManager(const CT_InModelStructureManager* manager)
{
    m_inputResultModelManager = const_cast<CT_InModelStructureManager*>(manager);

    constructModel();

    ui->treeView->expandAll();

    setMinimumWidth(ui->treeView->header()->length());
    setMaximumWidth(ui->treeView->header()->length());
}

void CTG_InResultModelPossibilities::setReadOnly(bool enabled)
{
    m_readOnly = enabled;
    updateModel();
}

void CTG_InResultModelPossibilities::clearModel()
{
    m_treeViewModel.invisibleRootItem()->removeRows(0, m_treeViewModel.invisibleRootItem()->rowCount());
}

void CTG_InResultModelPossibilities::constructModel()
{
    clearModel();

    if(m_inputResultModelManager != NULL)
    {
        m_inputResultModelManager->visitResults([this](const CT_InAbstractResultModel* inputResultModel) -> bool {
            const QList<QStandardItem*> items = this->createItemsForResultModel(inputResultModel);

            if(!items.isEmpty())
                m_treeViewModel.invisibleRootItem()->appendRow(items);

            return true;
        });
    }
}

void CTG_InResultModelPossibilities::constructHeader()
{
    QStringList header;
    header << tr("Nom des résultats");
    header << tr("Etape");
    header << tr("Aide"); // if you change the header don't missing to change HELP_COLUMN defined at the top of this file

    m_treeViewModel.setHorizontalHeaderLabels(header);

    ui->treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeView->header()->setSectionResizeMode(2, QHeaderView::Fixed);
}

void CTG_InResultModelPossibilities::updateModel()
{
    const int n = m_treeViewModel.invisibleRootItem()->rowCount();

    for(int i=0; i<n; ++i) {
        const QStandardItem* parent = m_treeViewModel.invisibleRootItem()->child(i, 0);

        const int m = parent->rowCount();

        for(int j=0; j<m; ++j) {
            QStandardItem* item = parent->child(j, 0);

            CT_InStdResultModelPossibility* possibility = static_cast<CT_InStdResultModelPossibility*>(item->data().value<void*>());

            // Nom du modèle de sortie (avec case à cocher)
            item->setCheckState(possibility->isSelected() ? Qt::Checked : Qt::Unchecked);
            // modifiable ou non en fonction du paramètre readonly
            item->setEnabled(!m_readOnly);
        }
    }
}

void CTG_InResultModelPossibilities::showFirstSelectedOrFirstResultPossibility()
{
    QStandardItem* root = m_treeViewModel.invisibleRootItem();

    if(root != NULL) {

        QStandardItem* firstItem = NULL;
        QStandardItem* firstItemSelected = NULL;

        const int nResult = root->rowCount();

        for(int r=0; (r<nResult) && (firstItemSelected == NULL); ++r) {
            QStandardItem* result = root->child(r, 0);

            const int nChild = result->rowCount();

            for(int i=0; (i<nChild) && (firstItemSelected == NULL); ++i) {
                QStandardItem* item = result->child(i, 0);

                CT_InStdResultModelPossibility* possibility = static_cast<CT_InStdResultModelPossibility*>(item->data().value<void*>());

                if(firstItem == NULL)
                    firstItem = item;

                if(possibility->isSelected())
                    firstItemSelected = item;
            }
        }

        if(firstItemSelected != NULL)
            firstItem = firstItemSelected;

        if(firstItem != NULL) {
            CT_InStdResultModelPossibility* p = static_cast<CT_InStdResultModelPossibility*>(firstItem->data().value<void*>());

            if(!m_readOnly)
                firstItem->setCheckState(p->setSelected(true) ? Qt::Checked : Qt::Unchecked);
            else
                firstItem->setCheckState(Qt::Checked);

            showResultPossibility(p);
        }
    }
}

QList<QStandardItem*> CTG_InResultModelPossibilities::createItemsForResultModel(const CT_InAbstractResultModel* resModel) const
{
    QList<QStandardItem*> retList;

    const QList<CT_InStdModelPossibility*> possibilities = resModel->getPossibilitiesSaved();

    if(possibilities.isEmpty())
        return retList;

    // Displayable name of the input result model
    QStandardItem *rootName = new QStandardItem(resModel->displayableName());
    rootName->setDragEnabled(false);
    rootName->setEditable(false);
    rootName->setData(qVariantFromValue((void*)resModel), Qt::UserRole + 1);    // the model is set here
    rootName->setData(resModel->shortDescription(), Qt::UserRole + 2);               // the description (help) is set on the first column
    retList.append(rootName);

    // Nothing
    QStandardItem *rootOther = new QStandardItem("");
    rootOther->setDragEnabled(false);
    rootOther->setEditable(false);
    rootOther->setData(qVariantFromValue((void*)resModel), Qt::UserRole + 1);    // the model is set here
    retList.append(rootOther);

    // Help icon (description)
    rootOther = new QStandardItem("");
    rootOther->setDragEnabled(false);
    rootOther->setEditable(false);
    rootOther->setData(qVariantFromValue((void*)resModel), Qt::UserRole + 1);    // the model is set here
    rootOther->setIcon(QIcon(":/Icones/Icones/help.png"));
    retList.append(rootOther);

    int i = 0;

    for(const CT_InStdModelPossibility* possibility : possibilities)
    {
        QList<QStandardItem*> rowList;

        // Displayable name of the output model in this possibility (QStandardItem with checkbox)
        QStandardItem *item = new QStandardItem(possibility->outModel()->displayableName());
        item->setDragEnabled(false);
        item->setCheckable(true);
        item->setCheckState(Qt::Unchecked);
        item->setEditable(false);
        item->setEnabled(!m_readOnly);
        item->setData(qVariantFromValue((void*)possibility), Qt::UserRole+1); // pointer to the possibility
        rowList.append(item);

        // Displayable name of the step that has generated this output model
        item = new QStandardItem(possibility->outModel()->step()->stepToolForModel()->displayableCustomName());
        item->setDragEnabled(false);
        item->setEditable(false);
        item->setData(qVariantFromValue((void*)possibility), Qt::UserRole+1); // pointer to the possibility
        rowList.append(item);

        // Nothing
        item = new QStandardItem("");
        item->setDragEnabled(false);
        item->setEditable(false);
        item->setData(qVariantFromValue((void*)possibility), Qt::UserRole+1); // pointer to the possibility
        rowList.append(item);

        rootName->appendRow(rowList);

        ++i;
    }

    return retList;
}

void CTG_InResultModelPossibilities::enableResultPossibility(const CT_InAbstractResultModel* model, CT_InStdResultModelPossibility* possibility)
{
    if(!possibility->isSelected()) {
        possibility->setSelected(true);

        CT_ModelSelectionHelper selectionHelper(model);
        //selectionHelper.debugPrintGraphs();
        selectionHelper.selectOneGraphForPossibilityOfRootModel(possibility);

        showResultPossibility(possibility);
    }
}

void CTG_InResultModelPossibilities::disableResultPossibility(CT_InStdResultModelPossibility* possibility)
{
    if(possibility->isSelected()) {
        possibility->setSelected(false);

        showResultPossibility(NULL);
    }
}

void CTG_InResultModelPossibilities::showResultPossibility(const CT_InStdResultModelPossibility* possibility)
{
    emit showInResultModelPossibility(possibility);
}

void CTG_InResultModelPossibilities::showEvent(QShowEvent *event)
{
    updateModel();

    showFirstSelectedOrFirstResultPossibility();

    QWidget::showEvent(event);
}

void CTG_InResultModelPossibilities::on_treeView_clicked(const QModelIndex &index)
{
    if(!index.isValid()) {
        showResultPossibility(NULL);
        return;
    }

    // if the index has a parent
    if(index.parent().isValid()) {

        const QStandardItem* item = m_treeViewModel.itemFromIndex(index);
        CT_InStdResultModelPossibility* possibility = static_cast<CT_InStdResultModelPossibility*>(item->data().value<void*>());

        // if the user click on the first column
        if(index.column() == 0) {

            const QStandardItem* parent = m_treeViewModel.itemFromIndex(index.parent());
            const CT_InAbstractResultModel* model = static_cast<CT_InAbstractResultModel*>(parent->data().value<void*>());

            if(!m_readOnly) {
                if(item->checkState() == Qt::Checked)
                    enableResultPossibility(model, possibility);
                else
                    disableResultPossibility(possibility);

                updateModel();
            } else {
                showResultPossibility(possibility);
            }
        }
    } else if(index.column() == HELP_COLUMN) { // index has no parent and the column is the help column

        // get the item on the first column
        const QStandardItem* item = m_treeViewModel.invisibleRootItem()->child(index.row(), 0);

        QMessageBox::information(this, tr("Aide"), tr("<html>Description du résultat d'entrée : <br/><br/>%1</html>").arg(item->data(Qt::UserRole + 2).toString()));
    }
}

void CTG_InResultModelPossibilities::on_treeView_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid()) {
        showResultPossibility(NULL);
        return;
    }

    // if the index has a parent
    if(index.parent().isValid()) {

        const QStandardItem* parent = m_treeViewModel.itemFromIndex(index.parent());
        const CT_InAbstractResultModel* model = static_cast<CT_InAbstractResultModel*>(parent->data().value<void*>());

        QStandardItem* item = parent->child(index.row(), 0);
        CT_InStdResultModelPossibility* possibility = static_cast<CT_InStdResultModelPossibility*>(item->data().value<void*>());

        if(!m_readOnly) {
            if(item->checkState() == Qt::Checked)
                item->setCheckState(Qt::Unchecked);
            else
                item->setCheckState(Qt::Checked);

            if(item->checkState() == Qt::Checked)
                enableResultPossibility(model, possibility);
            else
                disableResultPossibility(possibility);

            updateModel();
        } else {
            showResultPossibility(possibility);
        }
    }
}
