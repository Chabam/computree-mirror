#include "gpointsattributesmanager.h"
#include "ui_gpointsattributesmanager.h"

#include "dm_guimanager.h"
#include "gdocumentviewforgraphics.h"
#include "gminmaxattributesscalarconfiguration.h"

#include "tools/attributes/dm_attributesbuildingcollectiont.h"
#include "tools/attributes/worker/dm_attributescolort.h"
#include "tools/attributes/worker/dm_attributesnormalt.h"
#include "tools/attributes/worker/dm_attributesscalart.h"

#include <QMessageBox>
#include <QProgressDialog>

GPointsAttributesManager::GPointsAttributesManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GPointsAttributesManager)
{
    ui->setupUi(this);

    m_manager = nullptr;
    m_doc = nullptr;
    m_itemPointRootColor = nullptr;
    m_itemPointRootScalar = nullptr;
    m_itemPointRootNormal = nullptr;
    m_itemFaceRootColor = nullptr;
    m_itemFaceRootScalar = nullptr;
    m_itemFaceRootNormal = nullptr;
    m_itemEdgeRootColor = nullptr;
    m_itemEdgeRootScalar = nullptr;
    m_itemEdgeRootNormal = nullptr;
    m_internalSetColor = false;

    ui->pushButtonAddColor->setEnabled(false);

    ui->pushButtonNormalsColorPicker->setColor(Qt::white);
    ui->pushButtonApplyEditNormals->setEnabled(false);

    ui->treeView->setModel(&m_model);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(ui->treeView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(treeView_currentRowChanged(QModelIndex,QModelIndex)));
    connect(&m_model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)));

    connect(ui->gradientManagerView, SIGNAL(newGradientSelected(QLinearGradient)), ui->colorGradientView, SLOT(fromLinearGradient(QLinearGradient)));
    connect(ui->colorGradientView, SIGNAL(arrowMove(qreal,GradientArrow)), this, SLOT(updateArrowValue(qreal,GradientArrow)));
    connect(ui->pushButtonGradientColorPicker, SIGNAL(colorChanged(const QColor&)), this, SLOT(colorChanged(const QColor&)));

}

GPointsAttributesManager::~GPointsAttributesManager()
{
    delete ui;
}
void GPointsAttributesManager::setManager(const DM_AttributesManager *man)
{
    m_manager = const_cast<DM_AttributesManager*>(man);

    initTreeView();
}

void GPointsAttributesManager::setDocument(const GDocumentViewForGraphics *doc)
{
    m_doc = const_cast<GDocumentViewForGraphics*>(doc);

    if(m_doc != nullptr)
        ui->checkBoxShowNormals->setChecked(m_doc->mustShowNormals());
}

void GPointsAttributesManager::closeEvent(QCloseEvent *e)
{
    checkAndSave(dynamic_cast<DM_AbstractAttributesScalar*>(attributesSelected()));

    QDialog::closeEvent(e);
}

void GPointsAttributesManager::initTreeView()
{
    typedef DM_AttributesScalarT<CT_AbstractPointsAttributes>    PointAttributesScalar;
    typedef DM_AttributesColorT<CT_AbstractPointsAttributes>     PointAttributesColor;
    typedef DM_AttributesNormalT<CT_AbstractPointsAttributes>    PointAttributesNormal;

    typedef DM_AttributesScalarT<CT_AbstractFaceAttributes>      FaceAttributesScalar;
    typedef DM_AttributesColorT<CT_AbstractFaceAttributes>       FaceAttributesColor;
    typedef DM_AttributesNormalT<CT_AbstractFaceAttributes>      FaceAttributesNormal;

    typedef DM_AttributesScalarT<CT_AbstractEdgeAttributes>      EdgeAttributesScalar;
    typedef DM_AttributesColorT<CT_AbstractEdgeAttributes>       EdgeAttributesColor;
    typedef DM_AttributesNormalT<CT_AbstractEdgeAttributes>      EdgeAttributesNormal;

    m_model.clear();
    constructHeader();

    m_itemPointRoot = new QStandardItem(tr("Point"));
    m_itemPointRoot->setEditable(false);

    m_model.appendRow(m_itemPointRoot);

    m_itemFaceRoot = new QStandardItem(tr("Face"));
    m_itemFaceRoot->setEditable(false);

    m_model.appendRow(m_itemFaceRoot);

    m_itemEdgeRoot = new QStandardItem(tr("Edge"));
    m_itemEdgeRoot->setEditable(false);

    m_model.appendRow(m_itemEdgeRoot);

    QList<CT_VirtualAbstractStep*> steps = GUI_MANAGER->getStepManager()->getStepRootList();

    while(!steps.isEmpty())
    {
        CT_VirtualAbstractStep *st = steps.takeFirst();
        buildTreeViewTForStep< CT_AbstractPointsAttributes, PointAttributesScalar, PointAttributesColor, PointAttributesNormal >(st);
        buildTreeViewTForStep< CT_AbstractFaceAttributes, FaceAttributesScalar, FaceAttributesColor, FaceAttributesNormal >(st);
        buildTreeViewTForStep< CT_AbstractEdgeAttributes, EdgeAttributesScalar, EdgeAttributesColor, EdgeAttributesNormal >(st);
    }
}

template<typename IAttributesType, typename AttributesScalarType, typename AttributesColorType, typename AttributesNormalType>
void GPointsAttributesManager::buildTreeViewTForStep(CT_VirtualAbstractStep *step)
{
    DM_AttributesBuildingCollectionT<IAttributesType> builderPoints;

    builderPoints.visitFrom(step, [this](const CT_OutAbstractModel* model, IAttributesType* pa) -> bool
    {
        CT_AbstractAttributesScalar* pas = dynamic_cast<CT_AbstractAttributesScalar*>(pa);

        if(pas != nullptr)
        {
            AttributesScalarType* localDPAS = static_cast<AttributesScalarType*>(m_manager->getAttributesFromInterface(pa));

            if(localDPAS != nullptr)
            {
                QStandardItem* rootItem = rootItemForType<IAttributesType>();
                QStandardItem* scalarRoot = getOrCreateScalarRootItemForType<IAttributesType>(rootItem);

                QStandardItem* modelRoot = findOrCreateChildForAttributeModel(model,
                                                                              [this, pa]() -> QVariant
                {
                    AttributesScalarType* globalDPAS = static_cast<AttributesScalarType*>(m_manager->getAttributesFromInterface(pa, false));
                    return QVariant::fromValue(static_cast<void*>(globalDPAS));
                },
                scalarRoot);

                ui->treeView->expand(m_model.indexFromItem(rootItem));
                ui->treeView->expand(m_model.indexFromItem(scalarRoot));

                QList<QStandardItem*> items = createAttributesScalarForModel(localDPAS);
                modelRoot->appendRow(items);

                createWidgetForItems(items);
            }
        }
        else
        {
            CT_AttributesColor<IAttributesType>* pac = dynamic_cast<CT_AttributesColor<IAttributesType>*>(pa);

            if(pac != nullptr)
            {
                AttributesColorType* localDPAC = static_cast<AttributesColorType*>(m_manager->getAttributesFromInterface(pa));

                if(localDPAC != nullptr)
                {
                    QStandardItem* rootItem = rootItemForType<IAttributesType>();
                    QStandardItem* colorRoot = getOrCreateColorRootItemForType<IAttributesType>(rootItem);

                    QStandardItem* modelRoot = findOrCreateChildForAttributeModel(model,
                                                                                  [this, pa]() -> QVariant
                    {
                        AttributesColorType* globalDPAC = static_cast<AttributesColorType*>(m_manager->getAttributesFromInterface(pa, false));

                        return QVariant::fromValue(static_cast<void*>(globalDPAC));
                    },
                    colorRoot);

                    ui->treeView->expand(m_model.indexFromItem(rootItem));
                    ui->treeView->expand(m_model.indexFromItem(colorRoot));

                    QList<QStandardItem*> items = createAttributesColorForModel(localDPAC);
                    modelRoot->appendRow(items);

                    createWidgetForItems(items);
                }
            }
            else
            {
                CT_AttributesNormal<IAttributesType>* pan = dynamic_cast<CT_AttributesNormal<IAttributesType>*>(pa);

                if(pan != nullptr)
                {
                    AttributesNormalType* localDPAN = static_cast<AttributesNormalType*>(m_manager->getAttributesFromInterface(pa));

                    if(localDPAN == nullptr)
                    {
                        QStandardItem* rootItem = rootItemForType<IAttributesType>();
                        QStandardItem* normalRoot = getOrCreateNormalRootItemForType<IAttributesType>(rootItem);

                        QStandardItem* modelRoot = findOrCreateChildForAttributeModel(model,
                                                                                      [this, pa]() -> QVariant
                        {
                            AttributesNormalType* globalDPAN = static_cast<AttributesNormalType*>(m_manager->getAttributesFromInterface(pa, false));

                            return QVariant::fromValue(static_cast<void*>(globalDPAN));
                        },
                        normalRoot);

                        ui->treeView->expand(m_model.indexFromItem(rootItem));
                        ui->treeView->expand(m_model.indexFromItem(modelRoot));

                        QList<QStandardItem*> items = createAttributesNormalForModel(localDPAN);
                        modelRoot->appendRow(items);

                        createWidgetForItems(items);
                    }
                }
            }
        }

        return true;
    });
}

void GPointsAttributesManager::constructHeader()
{
    QStringList header;
    header << tr("Nom");
    header << tr("Appliquer");
    //header << tr("Utiliser seul ?");
    header << tr("Utiliser gradient partagé ?");
    header << tr("Configurer");

    m_model.setHorizontalHeaderLabels(header);

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    for(int i=0; i<header.size(); ++i)
        ui->treeView->header()->setResizeMode(i, QHeaderView::ResizeToContents);
#else
    for(int i=0; i<header.size(); ++i)
        ui->treeView->header()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
#endif
}

void GPointsAttributesManager::checkAndSave(DM_AbstractAttributesScalar *pas)
{
    if((pas != nullptr)
            && (ui->pushButtonSave->isEnabled()))
    {
        int ret = QMessageBox::question(this, tr("Enregistrer"), tr("Vous n'avez pas enregistré le gradient. Voulez vous le faire maintenant ?"), QMessageBox::Yes | QMessageBox::No);

        if(ret == QMessageBox::Yes)
            saveCurrentGradientTo(pas);
    }
}

QList<QStandardItem*> GPointsAttributesManager::createAttributesScalarForModel(DM_AbstractAttributesScalar *pa)
{
    QList<QStandardItem*> items;

    // NOM
    QStandardItem *item = new QStandardItem(pa->displayableName());
    item->setEditable(false);
    item->setData(QVariant::fromValue(static_cast<void*>(pa)));
    items << item;

    // BOUTON APPLIQUER
    item = new QStandardItem(tr("Appliquer"));
    item->setEditable(false);
    item->setData(QVariant::fromValue(static_cast<void*>(pa)));
    items << item;

    // CHECKBOX "UTILISER SEUL ?"
    // attention si vous decommentez cette ligne il faut vérifier
    // les comparaison avec les "column()" des items
    /*item = new QStandardItem();
    item->setEditable(false);
    item->setCheckable(true);
    item->setCheckState(pa->isDisplayedAlone() ? Qt::Checked : Qt::Unchecked);
    item->setData(QVariant::fromValue(static_cast<void*>(pa)));
    items << item;*/

    // CHECKBOX "UTILISER GRADIENT PARTAGE ?"
    item = new QStandardItem();
    item->setEditable(false);
    item->setCheckable(true);
    item->setCheckState(pa->isUsedSharedGradient() ? Qt::Checked : Qt::Unchecked);
    item->setData(QVariant::fromValue(static_cast<void*>(pa)));
    items << item;

    // BOUTON CONFIGURER
    item = new QStandardItem(tr("Configurer"));
    item->setEditable(false);
    item->setData(QVariant::fromValue(static_cast<void*>(pa)));
    items << item;

    return items;
}

QList<QStandardItem*> GPointsAttributesManager::createAttributesColorForModel(DM_AbstractAttributesColor *pa)
{
    QList<QStandardItem*> items;

    // NOM
    QStandardItem *item = new QStandardItem(pa->displayableName());
    item->setEditable(false);
    item->setData(QVariant::fromValue(static_cast<void*>(pa)));
    items << item;

    // BOUTON APPLIQUER
    item = new QStandardItem(tr("Appliquer"));
    item->setEditable(false);
    item->setData(QVariant::fromValue(static_cast<void*>(pa)));
    items << item;

    // CHECKBOX "UTILISER SEUL ?"
    // attention si vous decommentez cette ligne il faut vérifier
    // les comparaison avec les "column()" des items
    /*item = new QStandardItem();
    item->setEditable(false);
    item->setCheckable(true);
    item->setCheckState(pa->isDisplayedAlone() ? Qt::Checked : Qt::Unchecked);
    item->setData(QVariant::fromValue(static_cast<void*>(pa)));
    items << item;*/

    return items;
}

QList<QStandardItem *> GPointsAttributesManager::createAttributesNormalForModel(DM_AbstractAttributesNormal *pa)
{
    QList<QStandardItem*> items;

    // NOM
    QStandardItem *item = new QStandardItem(pa->displayableName());
    item->setEditable(false);
    item->setData(QVariant::fromValue(static_cast<void*>(pa)));
    items << item;

    // BOUTON APPLIQUER
    item = new QStandardItem(tr("Appliquer"));
    item->setEditable(false);
    item->setData(QVariant::fromValue(static_cast<void*>(pa)));
    items << item;

    // CHECKBOX "UTILISER SEUL ?"
    // attention si vous decommentez cette ligne il faut vérifier
    // les comparaison avec les "column()" des items
    /*item = new QStandardItem();
    item->setEditable(false);
    item->setCheckable(true);
    item->setCheckState(pa->isDisplayedAlone() ? Qt::Checked : Qt::Unchecked);
    item->setData(QVariant::fromValue(static_cast<void*>(pa)));
    items << item;*/

    return items;
}

void GPointsAttributesManager::createScalarRootItemIfNull(QStandardItem*& root, QStandardItem* parent) const
{
    if(root == nullptr)
    {
        root = new QStandardItem(tr("Gradients"));
        root->setEditable(false);

        parent->appendRow(root);
    }
}

void GPointsAttributesManager::createColorRootItemIfNull(QStandardItem*& root, QStandardItem* parent) const
{
    if(root == nullptr)
    {
        root = new QStandardItem(tr("Couleurs"));
        root->setEditable(false);

        parent->appendRow(root);
    }
}

void GPointsAttributesManager::createNormalRootItemIfNull(QStandardItem*& root, QStandardItem* parent) const
{
    if(root == nullptr)
    {
        root = new QStandardItem(tr("Normales"));
        root->setEditable(false);

        parent->appendRow(root);
    }
}

QStandardItem* GPointsAttributesManager::findOrCreateChildForAttributeModel(const CT_OutAbstractModel* attModel, std::function<QVariant ()> data, QStandardItem* parent) const
{
    QStandardItem* child = nullptr;
    const int n = parent->rowCount();

    for(int i=0; i<n; ++i)
    {
        if(parent->child(i, 0)->data(Qt::UserRole + 2).value<void*>() == attModel)
        {
            child = parent->child(i, 0);
            break;
        }
    }

    if(child == nullptr)
    {
        QList<QStandardItem*> items;

        child = new QStandardItem(attModel->displayableName());
        child->setEditable(false);
        child->setData(data());
        child->setData(QVariant::fromValue(static_cast<void*>(const_cast<CT_OutAbstractModel*>(attModel))), Qt::UserRole + 2);
        items << child;

        QStandardItem* apply = new QStandardItem(tr("Appliquer"));
        apply->setEditable(false);
        apply->setData(child->data());
        items << apply;

        QStandardItem* item = new QStandardItem();
        item->setEditable(false);
        item->setData(child->data());
        items << item;

        // BOUTON CONFIGURER
        item = new QStandardItem(tr("Configurer"));
        item->setEditable(false);
        item->setData(child->data());
        items << item;

        parent->appendRow(items);

        createWidgetForItems(items);
    }

    return child;
}

void GPointsAttributesManager::createWidgetForItems(const QList<QStandardItem *> &items) const
{
    QStandardItem *item = items.at(1);
    QPushButton *pushButton = new QPushButton(item->text(), ui->treeView);
    pushButton->setProperty("userdata", item->data());

    ui->treeView->setIndexWidget(m_model.indexFromItem(item), pushButton);

    connect(pushButton, SIGNAL(clicked()), this, SLOT(pushButtonApplyClicked()));

    if(items.size() > 3)
    {
        item = items.at(3);
        QPushButton *pushButtonC = new QPushButton(item->text(), ui->treeView);
        pushButtonC->setProperty("userdata", item->data());

        ui->treeView->setIndexWidget(m_model.indexFromItem(item), pushButtonC);

        connect(pushButtonC, SIGNAL(clicked()), this, SLOT(pushButtonConfigureClicked()));
    }
}

void GPointsAttributesManager::editAttributesScalar(DM_AbstractAttributesScalar *pas)
{
    QGradientStops stops = pas->gradient().stops();

    QVectorIterator<QGradientStop> it(stops);

    int i = 0;

    while(it.hasNext())
    {
        QGradientStop stop = it.next();

        GradientArrow arrow;
        arrow.setHasFocus(i==0);
        arrow.setColor(stop.second);
        arrow.setPosition(stop.first);
        arrow.setIndex(i);

        ui->colorGradientView->changeArrow(arrow);
        ++i;
    }

    ui->pushButtonAddColor->setEnabled(true);
    ui->pushButtonDeleteColor->setEnabled(ui->colorGradientView->nArrows() > 2);
    ui->widgetEditGradient->setVisible(true);

    ui->doubleSpinBoxGradientArrowValue->setMinimum(pas->min());
    ui->doubleSpinBoxGradientArrowValue->setMaximum(pas->max());
}

void GPointsAttributesManager::clearEditGradient()
{
    ui->colorGradientView->clearArrows();
    ui->pushButtonSave->setEnabled(false);
    ui->pushButtonDeleteColor->setEnabled(false);
    ui->pushButtonAddColor->setEnabled(false);
}

void GPointsAttributesManager::saveCurrentGradientTo(DM_AbstractAttributesScalar *pas)
{
    QLinearGradient gradient = ui->colorGradientView->toLinearGradient();

    if(gradient.stops().size() > 1)
    {
        pas->setGradient(gradient);

        // si il utilise un gradient partagé
        if(pas->isUsedSharedGradient())
        {
            // on applique le gradient à tous ceux qui utilise un gradient partagé
            QListIterator<DM_AbstractAttributes*> it(m_manager->attributes());

            while(it.hasNext())
            {
                DM_AbstractAttributesScalar *pass = dynamic_cast<DM_AbstractAttributesScalar*>(it.next());

                if((pass != nullptr)
                        && (pass != pas)
                        && (pass->isUsedSharedGradient()))
                {
                    pass->setGradient(gradient);
                }
            }
        }
    }

    ui->pushButtonSave->setEnabled(false);
}

void GPointsAttributesManager::editAttributesNormal(DM_AbstractAttributesNormal *pan)
{
    Q_UNUSED(pan)

    GDocumentViewForGraphics::NormalsConfiguration c = m_doc->getNormalsConfiguration();

    ui->pushButtonNormalsColorPicker->setColor(c.normalColor);
    ui->doubleSpinBoxNormalsLength->setValue(double(c.normalLength));

    ui->widgetEditNormals->setVisible(true);
}

void GPointsAttributesManager::clearEditNormal()
{
    ui->pushButtonApplyEditNormals->setEnabled(false);
}

void GPointsAttributesManager::applyAndSaveNormal()
{
    GDocumentViewForGraphics::NormalsConfiguration c;
    c.normalColor = ui->pushButtonNormalsColorPicker->getColor();
    c.normalLength = float(ui->doubleSpinBoxNormalsLength->value());

    m_doc->applyNormalsConfiguration(c);
}

DM_AbstractAttributes *GPointsAttributesManager::attributesSelected() const
{
    QModelIndexList list = ui->treeView->selectionModel()->selectedIndexes();

    if(list.isEmpty())
        return nullptr;

    return static_cast<DM_AbstractAttributes*>(m_model.itemFromIndex(list.first())->data().value<void*>());
}

void GPointsAttributesManager::on_pushButtonAddColor_clicked()
{
    GradientArrow newArrow;
    newArrow.setIndex(ui->colorGradientView->indexUnused());
    newArrow.setHasFocus(true);
    newArrow.setPosition(ui->colorGradientView->positionUnused());
    newArrow.setColor(Qt::white);

    ui->colorGradientView->changeArrow(newArrow);
    ui->pushButtonDeleteColor->setEnabled(ui->colorGradientView->nArrows() > 2);
    ui->pushButtonSave->setEnabled(true);
}

void GPointsAttributesManager::on_pushButtonDeleteColor_clicked()
{
    ui->colorGradientView->removeArrow(ui->colorGradientView->currentArrow().index());
    ui->pushButtonDeleteColor->setEnabled(ui->colorGradientView->nArrows() > 2);
    ui->pushButtonSave->setEnabled(true);
}

void GPointsAttributesManager::on_pushButtonSave_clicked()
{
    saveCurrentGradientTo(dynamic_cast<DM_AbstractAttributesScalar*>(attributesSelected()));
}

void GPointsAttributesManager::pushButtonApplyClicked()
{
    QPushButton* pushButton = static_cast<QPushButton*>(sender());
    DM_AbstractAttributes* dpa = static_cast<DM_AbstractAttributes*>(pushButton->property("userdata").value<void*>());
    DM_AbstractAttributesScalar* as = dynamic_cast<DM_AbstractAttributesScalar*>(attributesSelected());

    if(dpa == as)
        saveCurrentGradientTo(static_cast<DM_AbstractAttributesScalar*>(dpa));

    m_doc->applyAttributes(dpa);
}

void GPointsAttributesManager::pushButtonConfigureClicked()
{
    QPushButton *pushButton = static_cast<QPushButton*>(sender());
    DM_AbstractAttributes *dpa = static_cast<DM_AbstractAttributes*>(pushButton->property("userdata").value<void*>());
    DM_AbstractAttributesScalar *pas = dynamic_cast<DM_AbstractAttributesScalar*>(dpa);

    if(pas != nullptr)
    {
        GMinMaxAttributesScalarConfiguration dialog(this);
        dialog.setAttributes(pas);

        dialog.exec();
    }
}

void GPointsAttributesManager::itemChanged(QStandardItem *item)
{
    DM_AbstractAttributes *pa = static_cast<DM_AbstractAttributes*>(item->data().value<void*>());

    if(pa != nullptr)
    {
        // use shared gradient
        if(item->column() == 2)
        {
            DM_AbstractAttributesScalar *pas = dynamic_cast<DM_AbstractAttributesScalar*>(pa);

            if(pas != nullptr)
            {
                pas->setUseSharedGradient(item->checkState() == Qt::Checked);

                if(pas->isUsedSharedGradient())
                {
                    // recherche d'un autre qui utilise le gradient partagé
                    QListIterator<DM_AbstractAttributes*> it(m_manager->attributes());

                    while(it.hasNext())
                    {
                        DM_AbstractAttributesScalar *pass = dynamic_cast<DM_AbstractAttributesScalar*>(it.next());

                        if((pass != nullptr)
                                && (pass != pas)
                                && (pass->isUsedSharedGradient()))
                        {
                            // on applique le gradient partagé a l'attribut de points courant
                            pas->setGradient(pass->gradient());

                            if(attributesSelected() == pa)
                                editAttributesScalar(pas);

                            return;
                        }
                    }
                }
            }
        }
    }
}

void GPointsAttributesManager::colorChanged(const QColor &color)
{
    if(!m_internalSetColor)
    {
        GradientArrow arrow = ui->colorGradientView->currentArrow();
        arrow.setColor(color);

        ui->colorGradientView->changeArrow(arrow);
        ui->pushButtonSave->setEnabled(true);
    }
    m_internalSetColor = false;
}

void GPointsAttributesManager::on_colorGradientView_newFocusColor(const QColor &color, int arrowIndex)
{
    Q_UNUSED(arrowIndex)

    if(ui->pushButtonGradientColorPicker->getColor() != color)
    {
        m_internalSetColor = true;
        ui->pushButtonGradientColorPicker->setColor(color);
    }

    GradientArrow arr = ui->colorGradientView->arrowByIndex(arrowIndex);

    updateArrowValue(arr.position(), arr);
}

void GPointsAttributesManager::on_colorGradientView_arrowMove(qreal lastPos, const GradientArrow &arrow)
{
    Q_UNUSED(lastPos)
    Q_UNUSED(arrow)

    ui->pushButtonSave->setEnabled(true);
}

void GPointsAttributesManager::on_checkBoxShowNormals_stateChanged(int state)
{
    m_doc->setMustShowNormals(state == Qt::Checked);
}

void GPointsAttributesManager::on_doubleSpinBoxNormalsLength_valueChanged(double v)
{
    if(m_doc != nullptr) {
        GDocumentViewForGraphics::NormalsConfiguration c = m_doc->getNormalsConfiguration();

        if(!qFuzzyCompare(v, double(c.normalLength)))
            ui->pushButtonApplyEditNormals->setEnabled(true);
    }
}

void GPointsAttributesManager::on_pushButtonNormalsColorPicker_colorChanged(QColor c)
{
    if(m_doc != nullptr) {
        GDocumentViewForGraphics::NormalsConfiguration co = m_doc->getNormalsConfiguration();

        if(c != co.normalColor)
            ui->pushButtonApplyEditNormals->setEnabled(true);
    }
}

void GPointsAttributesManager::on_pushButtonApplyEditNormals_clicked()
{
    ui->pushButtonApplyEditNormals->setEnabled(false);

    applyAndSaveNormal();
}

void GPointsAttributesManager::on_doubleSpinBoxGradientArrowValue_editingFinished()
{
    GradientArrow arr = ui->colorGradientView->currentArrow();
    DM_AbstractAttributesScalar *pas = dynamic_cast<DM_AbstractAttributesScalar*>(attributesSelected());

    if((pas != nullptr) && (arr.index() != -1)) {

        const double range = pas->max() - pas->min();

        if(!qFuzzyIsNull(range))  {
            double pos = (ui->doubleSpinBoxGradientArrowValue->value()-pas->min())/range;

            if(pos < 0)
                pos = 0;

            if(pos > 1)
                pos = 1;

            arr.setPosition(pos);
            ui->colorGradientView->changeArrow(arr);
        }
    }
}

void GPointsAttributesManager::treeView_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    QStandardItem *pItem = m_model.itemFromIndex(previous);

    if((pItem != nullptr)
            && !pItem->data().isNull())
    {
        DM_AbstractAttributesScalar *pas = dynamic_cast<DM_AbstractAttributesScalar*>(static_cast<DM_AbstractAttributes*>(pItem->data().value<void*>()));

        checkAndSave(pas);
    }

    clearEditGradient();
    ui->widgetEditGradient->setVisible(false);
    ui->widgetEditNormals->setVisible(false);

    QStandardItem *cItem = m_model.itemFromIndex(current);

    if((cItem != nullptr)
            && !cItem->data().isNull())
    {
        DM_AbstractAttributesScalar *pas = dynamic_cast<DM_AbstractAttributesScalar*>(static_cast<DM_AbstractAttributes*>(cItem->data().value<void*>()));

        if(pas != nullptr)
        {
            editAttributesScalar(pas);
            return;
        }

        DM_AbstractAttributesNormal *pan = dynamic_cast<DM_AbstractAttributesNormal*>(static_cast<DM_AbstractAttributes*>(cItem->data().value<void*>()));

        if(pan != nullptr)
        {
            editAttributesNormal(pan);
            return;
        }
    }
}

void GPointsAttributesManager::updateArrowValue(qreal val, GradientArrow arr)
{
    Q_UNUSED(val)

    bool ok = false;

    DM_AbstractAttributesScalar *pas = dynamic_cast<DM_AbstractAttributesScalar*>(attributesSelected());

    if((pas != nullptr) && (arr.index() != -1)) {

        const double range = pas->max() - pas->min();

        ok = !qFuzzyIsNull(range);

        if(ok)
            ui->doubleSpinBoxGradientArrowValue->setValue((arr.position()*range)+pas->min());
    }

    if(!ok)
        ui->doubleSpinBoxGradientArrowValue->setValue(0);
}

template<>
QStandardItem* GPointsAttributesManager::rootItemForType<CT_AbstractPointsAttributes>() const
{
    return m_itemPointRoot;
}

template<>
QStandardItem* GPointsAttributesManager::rootItemForType<CT_AbstractFaceAttributes>() const
{
    return m_itemFaceRoot;
}

template<>
QStandardItem* GPointsAttributesManager::rootItemForType<CT_AbstractEdgeAttributes>() const
{
    return m_itemEdgeRoot;
}

template<>
QStandardItem* GPointsAttributesManager::getOrCreateScalarRootItemForType<CT_AbstractPointsAttributes>(QStandardItem* parent)
{
    createScalarRootItemIfNull(m_itemPointRootScalar, parent);
    return m_itemPointRootScalar;
}

template<>
QStandardItem* GPointsAttributesManager::getOrCreateScalarRootItemForType<CT_AbstractFaceAttributes>(QStandardItem* parent)
{
    createScalarRootItemIfNull(m_itemFaceRootScalar, parent);
    return m_itemFaceRootScalar;
}

template<>
QStandardItem* GPointsAttributesManager::getOrCreateScalarRootItemForType<CT_AbstractEdgeAttributes>(QStandardItem* parent)
{
    createScalarRootItemIfNull(m_itemEdgeRootScalar, parent);
    return m_itemEdgeRootScalar;
}

template<>
QStandardItem* GPointsAttributesManager::getOrCreateColorRootItemForType<CT_AbstractPointsAttributes>(QStandardItem* parent)
{
    createColorRootItemIfNull(m_itemPointRootColor, parent);
    return m_itemPointRootColor;
}

template<>
QStandardItem* GPointsAttributesManager::getOrCreateColorRootItemForType<CT_AbstractFaceAttributes>(QStandardItem* parent)
{
    createColorRootItemIfNull(m_itemFaceRootColor, parent);
    return m_itemFaceRootColor;
}

template<>
QStandardItem* GPointsAttributesManager::getOrCreateColorRootItemForType<CT_AbstractEdgeAttributes>(QStandardItem* parent)
{
    createColorRootItemIfNull(m_itemEdgeRootColor, parent);
    return m_itemEdgeRootColor;
}

template<>
QStandardItem* GPointsAttributesManager::getOrCreateNormalRootItemForType<CT_AbstractPointsAttributes>(QStandardItem* parent)
{
    createNormalRootItemIfNull(m_itemPointRootNormal, parent);
    return m_itemPointRootColor;
}

template<>
QStandardItem* GPointsAttributesManager::getOrCreateNormalRootItemForType<CT_AbstractFaceAttributes>(QStandardItem* parent)
{
    createNormalRootItemIfNull(m_itemFaceRootNormal, parent);
    return m_itemFaceRootNormal;
}

template<>
QStandardItem* GPointsAttributesManager::getOrCreateNormalRootItemForType<CT_AbstractEdgeAttributes>(QStandardItem* parent)
{
    createNormalRootItemIfNull(m_itemEdgeRootNormal, parent);
    return m_itemEdgeRootNormal;
}

#if defined(_WIN32) && defined(_MSC_VER) // Microsoft Visual Studio Compiler
#pragma warning( disable : 4506 )
#elif (defined(__linux__) || defined(_WIN32)) && defined(__GNUC__) // GNU Compiler (gcc,g++) for Linux, Unix, and MinGW (Windows)
#elif defined(__APPLE__) // Clang Compiler (Apple)
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wint-in-bool-context"
#endif

