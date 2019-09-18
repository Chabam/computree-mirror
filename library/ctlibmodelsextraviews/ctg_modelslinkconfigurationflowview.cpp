#include "ctg_modelslinkconfigurationflowview.h"
#include "ui_ctg_modelslinkconfigurationflowview.h"

#include "tools/ct_modelflowdatamodel.h"
#include "tools/ct_modelflowdata.h"
#include "tools/ct_indatatypetooutdatatypeconverter.h"
#include "tools/ct_helpgraphicsitem.h"

#include "ct_model/inModel/ct_inresultmodelgroup.h"
#include "ct_model/outModel/abstract/ct_outabstractmodel.h"

#include <nodes/DataModelRegistry>
#include <nodes/Node>
#include <nodes/FlowView>
#include <nodes/FlowScene>
#include <nodes/ConnectionStyle>

#include "PortType.hpp"

#include <QTreeWidget>
#include <QHeaderView>
#include <QScrollBar>
#include <QAction>
#include <QTimer>

using QtNodes::FlowView;
using QtNodes::FlowScene;
using QtNodes::DataModelRegistry;
using QtNodes::Node;
using QtNodes::TypeConverter;
using QtNodes::Connection;
using QtNodes::ConnectionStyle;

#define TREE_WIDGET_ITEM_SPACING 21

CTG_ModelsLinkConfigurationFlowView::CTG_ModelsLinkConfigurationFlowView(QWidget *parent) :
    SuperClass(parent),
    ui(new Ui::CTG_ModelsLinkConfigurationFlowView),
    mInResultModelGroup(nullptr),
    mOutResultModelGroup(nullptr),
    mInModelPortType(PT_OUT),
    mInNode(nullptr),
    mOutNode(nullptr),
    mInTreeWidget(nullptr),
    mOutTreeWidget(nullptr),
    mRestoreConnectionOrCreatePreviewInProgress(true)
{
    ui->setupUi(this);

    mModelOfLastIndexClicked.fill(nullptr, 2);

    std::shared_ptr<DataModelRegistry> ret = std::make_shared<DataModelRegistry>();
    ret->registerModel<CT_ModelFlowDataModel>();

    mFlowScene = new FlowScene(ret);
    mFlowView = new FlowView(mFlowScene, this);

    auto hgi = new CT_HelpGraphicsItem();
    hgi->setPos(-100, -100);
    hgi->setToolTip(tr("Tooltip à modifier par Alexandre Piboule. Il suffit de faire rechercher dans Qt Creator pour me trouver !"));
    mFlowScene->addItem(hgi);

    connect(mFlowScene, &FlowScene::connectionSelected, this, &CTG_ModelsLinkConfigurationFlowView::connectionSelected);
    connect(mFlowScene, &FlowScene::connectionDoubleClicked, this, &CTG_ModelsLinkConfigurationFlowView::connectionDoubleClicked);
    connect(mFlowScene, &FlowScene::nodeSelected, this, &CTG_ModelsLinkConfigurationFlowView::nodeSelected);

    connect(mFlowView, &FlowView::rubberBandChanged, this, &CTG_ModelsLinkConfigurationFlowView::rubberBandChanged);

    ui->verticalLayout->addWidget(mFlowView);

    ConnectionStyle::setConnectionStyle(
       R"(
     {
       "ConnectionStyle": {
         "UseDataDefinedColors": true
       }
     }
     )");
}

CTG_ModelsLinkConfigurationFlowView::~CTG_ModelsLinkConfigurationFlowView()
{
    disconnect(mConnectionDeletedSignalSlotQtConnection);

    for(QMetaObject::Connection c : mPossibilitiesSignalSlotQtConnection) {
        disconnect(c);
    }

    mPreviewConnections.clear();

    delete ui;
    delete mFlowScene;
}

bool CTG_ModelsLinkConfigurationFlowView::isReadOnly() const
{
    return mFlowView->isReadOnly();
}

void CTG_ModelsLinkConfigurationFlowView::setInResultModelPossibility(const CT_InStdResultModelPossibility* possibility)
{
    mInResultModelGroup = nullptr;
    mOutResultModelGroup = nullptr;

    if(possibility != nullptr)
    {
        mInResultModelGroup = dynamic_cast<CT_InResultModelGroup*>(possibility->inResultModel());
        mOutResultModelGroup = possibility->outModel();
    }

    changeInNodePortType(mInModelPortType);
}

void CTG_ModelsLinkConfigurationFlowView::setReadOnly(bool enabled)
{
    mFlowView->setReadOnly(enabled);
}

void CTG_ModelsLinkConfigurationFlowView::fitViewToScene()
{
    auto items = mFlowScene->items();
    QRectF br;
    for(auto item : items)
    {
        br = br.united(item->mapToScene(item->boundingRect()).boundingRect());
    }

    mFlowView->setSceneRect(br);
}

void CTG_ModelsLinkConfigurationFlowView::changeInNodePortType(CTG_PortType portType)
{
    reset();

    mInModelPortType = portType;

    if((mInResultModelGroup != nullptr)
            && (mOutResultModelGroup != nullptr))
    {
        construct();

        auto inputDataValidator = [this](const NodeDataModel* inNodeDataModel, std::shared_ptr<NodeData> outNodeData, PortIndex inPortIndex) -> bool {
            return doesAcceptInputDataForNodeDataModel(inNodeDataModel, outNodeData, inPortIndex);
        };

        auto inputDataSetter = [this](NodeDataModel* inNodeDataModel, const Connection* connection, std::shared_ptr<NodeData> outNodeData, PortIndex inPortIndex) -> void {
            setInputDataForNodeDataModel(inNodeDataModel, connection, outNodeData, inPortIndex);
        };

        std::unique_ptr<NodeDataModel> inType = std::make_unique<CT_ModelFlowDataModel>(tr("Données recherchées"), mInTreeWidget->model(), PortType(mInModelPortType), mInTreeWidget);

        if(mInModelPortType == PT_IN)
        {
            inType->setInDataValidator(inputDataValidator);
            inType->setInDataSetter(inputDataSetter);
        }

        mInNode = &mFlowScene->createNode(std::move(inType));
        mInNode->nodeGraphicsObject().setPos(mInModelPortType == PT_OUT ? 0 : 400, 0);
        mFlowScene->nodePlaced(*mInNode);

        std::unique_ptr<NodeDataModel> outType = std::make_unique<CT_ModelFlowDataModel>(tr("Données disponibles"), mOutTreeWidget->model(), oppositePort(PortType(mInModelPortType)), mOutTreeWidget);

        if(mInModelPortType == PT_OUT)
        {
            outType->setInDataValidator(inputDataValidator);
            outType->setInDataSetter(inputDataSetter);
        }

        mOutNode = &mFlowScene->createNode(std::move(outType));
        mOutNode->nodeGraphicsObject().setPos(mInModelPortType == PT_IN ? 0 : 400, 0);
        mFlowScene->nodePlaced(*mOutNode);

        createConnectionForSelectedPossibilies();

        fitViewToScene();
    }
}

CTG_ModelsLinkConfigurationFlowView::CTG_PortType CTG_ModelsLinkConfigurationFlowView::inNodePortType() const
{
    return mInModelPortType;
}

void CTG_ModelsLinkConfigurationFlowView::reset()
{
    mModelOfLastIndexClicked.fill(nullptr);

    mConnectionByPossibility.clear();
    mInModelByPossibility.clear();

    disconnect(mConnectionDeletedSignalSlotQtConnection);

    for(QMetaObject::Connection c : mPossibilitiesSignalSlotQtConnection) {
        disconnect(c);
    }

    mPreviewConnections.clear();

    if(mInNode != nullptr)
        mFlowScene->removeNode(*mInNode);

    if(mOutNode != nullptr)
        mFlowScene->removeNode(*mOutNode);

    mInTreeWidget = nullptr;
    mOutTreeWidget = nullptr;

    mInNode = nullptr;
    mOutNode = nullptr;

    mConnectionDeletedSignalSlotQtConnection = connect(mFlowScene, &FlowScene::connectionDeleted, this, &CTG_ModelsLinkConfigurationFlowView::unselectPossibilityWhenConnectionIsDeleted);
}

void CTG_ModelsLinkConfigurationFlowView::construct()
{
    mModelOfLastIndexClicked.fill(nullptr);

    mInTreeWidget = new QTreeWidget;
    mInTreeWidget->setColumnCount(1);
    mInTreeWidget->setHeaderHidden(true);
    mInTreeWidget->setItemDelegate(new RowDelegate());
    mInTreeWidget->setStyleSheet(QString("background-color: rgba(0,0,0,0); "
                                         "border-style: solid;"
                                         "border-width: 0px;"));
    mInTreeWidget->setWindowFlags(Qt::FramelessWindowHint);
    mInTreeWidget->setAttribute(Qt::WA_NoSystemBackground);
    mInTreeWidget->setAttribute(Qt::WA_TranslucentBackground);
    mInTreeWidget->setAttribute(Qt::WA_TransparentForMouseEvents);
    mInTreeWidget->setItemsExpandable(false);
    mInTreeWidget->header()->setStretchLastSection(false);

    mOutTreeWidget = new QTreeWidget;
    mOutTreeWidget->setColumnCount(1);
    mOutTreeWidget->setHeaderHidden(true);
    mOutTreeWidget->setItemDelegate(new RowDelegate());
    mOutTreeWidget->setStyleSheet(QString("background-color: rgba(0,0,0,0); "
                                          "border-style: solid;"
                                          "border-width: 0px;"));
    mOutTreeWidget->setWindowFlags(Qt::FramelessWindowHint);
    mOutTreeWidget->setAttribute(Qt::WA_NoSystemBackground);
    mOutTreeWidget->setAttribute(Qt::WA_TranslucentBackground);
    mOutTreeWidget->setAttribute(Qt::WA_TransparentForMouseEvents);
    mOutTreeWidget->setItemsExpandable(false);
    mOutTreeWidget->header()->setStretchLastSection(false);
    mOutTreeWidget->horizontalScrollBar()->setStyleSheet("height:0px;");
    mOutTreeWidget->verticalScrollBar()->setStyleSheet("width:0px;");

    QHash<const CT_AbstractModel*, QTreeWidgetItem*> inItems;
    QHash<const CT_AbstractModel*, QTreeWidgetItem*> outItems;

    mInResultModelGroup->recursiveVisitInChildrens([this, &inItems, &outItems] (const CT_InAbstractModel* child)-> bool {

        QTreeWidgetItem* inTreeitem = new QTreeWidgetItem(inItems.value(child->parentModel(), nullptr), QStringList(child->displayableName()));
        inTreeitem->setForeground(0, Qt::white);
        inTreeitem->setData(0, Qt::UserRole, qVariantFromValue(static_cast<void*>(const_cast<CT_InAbstractModel*>(child))));
        inTreeitem->setData(0, Qt::UserRole+1, QString().setNum(inItems.size()*100));
        inTreeitem->setToolTip(0, child->detailledDescription());

        const bool obligatory = child->parentModel() == nullptr ? child->isObligatory() : child->recursiveAtLeastOneChildrenOrThisIsObligatory();

        QFont font = inTreeitem->font(0);
        font.setItalic(!obligatory);
        font.setBold(obligatory);
        inTreeitem->setFont(0, font);

        if(mInTreeWidget->topLevelItem(0) == nullptr)
            mInTreeWidget->addTopLevelItem(inTreeitem);

        inItems.insert(child, inTreeitem);

        child->visitPossibilities([this, &inTreeitem, &outItems](const CT_InAbstractModel* inModel, const CT_InStdModelPossibility* possibility) -> bool {

            NodeDataType inDataType;
            NodeDataType outDataType;

            if(mInModelPortType == PT_OUT)
            {
                inDataType = CT_ModelFlowData(inModel).type();
                outDataType = CT_ModelFlowData(possibility->outModel()).type();
            }
            else
            {
                outDataType = CT_ModelFlowData(inModel).type();
                inDataType = CT_ModelFlowData(possibility->outModel()).type();
            }

            QTreeWidgetItem* outTreeItem = createOrGetOutTreeItemForModel(possibility->outModel(), outItems);
            outTreeItem->setData(0, Qt::UserRole+1, inTreeitem->data(0, Qt::UserRole+1));

            mFlowScene->registry().registerTypeConverter(std::make_pair(inDataType, outDataType),
                                                         TypeConverter{CT_InDataTypeToOutDataTypeConverter()});

            mInModelByPossibility.insert(possibility, inModel);

            mPossibilitiesByModel.insert(inModel, possibility);
            mPossibilitiesByModel.insert(possibility->outModel(), possibility);

            mPossibilitiesSignalSlotQtConnection.append(connect(possibility, SIGNAL(selectStateChange(bool)), this, SLOT(createOrDeleteConnectionWhenPossibilitySelectionStateHasChanged(bool))));

            return true;
        });

        return true;
    });

    mInTreeWidget->expandAll();
    mOutTreeWidget->expandAll();

    mInTreeWidget->resizeColumnToContents(0);
    mOutTreeWidget->resizeColumnToContents(0);

    mInTreeWidget->resize(static_cast<RowDelegate*>(mInTreeWidget->itemDelegate())->maxWidth + 100, inItems.size() * TREE_WIDGET_ITEM_SPACING);
    mOutTreeWidget->resize(static_cast<RowDelegate*>(mOutTreeWidget->itemDelegate())->maxWidth + 100, outItems.size() * TREE_WIDGET_ITEM_SPACING);

    connect(mInTreeWidget, &QTreeWidget::clicked, this, &CTG_ModelsLinkConfigurationFlowView::displayPreviewConnectionsForIndexClicked);
    connect(mOutTreeWidget, &QTreeWidget::clicked, this, &CTG_ModelsLinkConfigurationFlowView::displayPreviewConnectionsForIndexClicked);

    connect(mInTreeWidget, &QTreeWidget::doubleClicked, this, &CTG_ModelsLinkConfigurationFlowView::convertPreviewConnectionBetweenIndexAndPreviewIndexClickedToConnection);
    connect(mOutTreeWidget, &QTreeWidget::doubleClicked, this, &CTG_ModelsLinkConfigurationFlowView::convertPreviewConnectionBetweenIndexAndPreviewIndexClickedToConnection);
}

QTreeWidgetItem* CTG_ModelsLinkConfigurationFlowView::createOrGetOutTreeItemForModel(CT_AbstractModel* model, QHash<const CT_AbstractModel*, QTreeWidgetItem*>& outItems)
{
    QTreeWidgetItem* outTreeItem = outItems.value(model, nullptr);

    if(outTreeItem != nullptr)
        return outTreeItem;

    QTreeWidgetItem* parentOutTreeItem = nullptr;

    const bool isRootGroup = (model->parentModel()->parentModel() == nullptr);

    // get or create parentOutTreeItem only if this model is not the root group
    if(!isRootGroup)
    {
        parentOutTreeItem = outItems.value(model->parentModel(), nullptr);

        if(parentOutTreeItem == nullptr)
            parentOutTreeItem = createOrGetOutTreeItemForModel(model->parentModel(), outItems);
    }

    outTreeItem = new QTreeWidgetItem(parentOutTreeItem, QStringList(model->displayableName()));
    outTreeItem->setForeground(0, Qt::white);
    outTreeItem->setData(0, Qt::UserRole, qVariantFromValue(static_cast<void*>(model)));
    outTreeItem->setData(0, Qt::UserRole+1, QString());
    outTreeItem->setToolTip(0, model->detailledDescription());

    if((mOutTreeWidget->topLevelItem(0) == nullptr) || (parentOutTreeItem == nullptr))
        mOutTreeWidget->addTopLevelItem(outTreeItem);

    outItems.insert(model, outTreeItem);

    return outTreeItem;
}

void CTG_ModelsLinkConfigurationFlowView::createConnectionForSelectedPossibilies()
{
    mRestoreConnectionOrCreatePreviewInProgress = true;

    QHashIterator<const CT_InStdModelPossibility*, const CT_InAbstractModel*> it(mInModelByPossibility);

    while(it.hasNext())
    {
        it.next();

        const CT_InStdModelPossibility* possibility = it.key();

        if(possibility->isSelected() && !mConnectionByPossibility.contains(possibility))
            createOrDeleteConnectionWhenPossibilitySelectionStateHasChanged(possibility, true);
    }

    mRestoreConnectionOrCreatePreviewInProgress = false;
}

std::shared_ptr<QtNodes::Connection> CTG_ModelsLinkConfigurationFlowView::createConnection(const int& inPortIndex, const int& outPortIndex, const bool& isAPreview)
{
    return (mInModelPortType == PT_IN) ? mFlowScene->createConnection(*mInNode, inPortIndex, *mOutNode, outPortIndex, TypeConverter{CT_InDataTypeToOutDataTypeConverter()}, isAPreview) : mFlowScene->createConnection(*mOutNode, outPortIndex, *mInNode, inPortIndex, TypeConverter{CT_InDataTypeToOutDataTypeConverter()}, isAPreview);
}

void CTG_ModelsLinkConfigurationFlowView::convertPreviewConnectionsSelectedToConnections()
{
    QMutableHashIterator<QtNodes::Connection*, const CT_InStdModelPossibility*> it(mPreviewConnections);

    const CT_InAbstractModel* inModel = nullptr;

    if(it.hasNext())
        inModel = mInModelByPossibility.value(it.next().value());
    else
        return;

    it.toFront();

    QList<const CT_InStdModelPossibility*> possibilitiesToSelect;
    const int maxToSelect = inModel->maximumNumberOfPossibilityThatCanBeSelected();

    while(it.hasNext())
    {
        it.next();

        QtNodes::Connection* connection = it.key();

        if(connection->getConnectionGraphicsObject().isSelected())
        {
            const CT_InStdModelPossibility* possibility = it.value();

            it.remove();
            mFlowScene->deleteConnection(*connection);
            possibilitiesToSelect.append(possibility);
        }

        if((maxToSelect != -1) && (possibilitiesToSelect.size() >= maxToSelect))
            break;
    }

    for(const CT_InStdModelPossibility* p : possibilitiesToSelect)
    {
        const_cast<CT_InStdModelPossibility*>(p)->setSelected(true);
    }

    displayPreviewConnectionsForModel(mModelOfLastIndexClicked[0]);
}

void CTG_ModelsLinkConfigurationFlowView::deletePreviewConnections()
{
    QHashIterator<QtNodes::Connection*, const CT_InStdModelPossibility*> it(mPreviewConnections);

    while(it.hasNext())
    {
        it.next();
        mFlowScene->deleteConnection(*it.key());
    }

    mPreviewConnections.clear();
}

bool CTG_ModelsLinkConfigurationFlowView::doesAcceptInputDataForNodeDataModel(const QtNodes::NodeDataModel* inNodeDataModel,
                                                                              std::shared_ptr<QtNodes::NodeData> outNodeData,
                                                                              int inPortIndex)
{
    const CT_InAbstractModel* inModel = nullptr;
    const CT_OutAbstractModel* outModel = nullptr;

    // get the out model at the end of the connection
    // and the in model at the other end of the connection
    resolveInModelAndOutModel(inNodeDataModel,
                              outNodeData,
                              inPortIndex,
                              inModel,
                              outModel);

    // only accept the input data if the in model has a possibility that use the out model
    return !inModel->visitPossibilities([&outModel](const CT_InAbstractModel*, const CT_InStdModelPossibility* possibility) -> bool {
        return possibility->outModel() != outModel;
    });
}

void CTG_ModelsLinkConfigurationFlowView::setInputDataForNodeDataModel(QtNodes::NodeDataModel* inNodeDataModel,
                                                                       const QtNodes::Connection* connection,
                                                                       std::shared_ptr<QtNodes::NodeData> outNodeData,
                                                                       int inPortIndex)
{
    // if the connection is a preview we don't select the possibility
    if(connection->connectionState().isAPreview() || mRestoreConnectionOrCreatePreviewInProgress)
        return;

    CT_ModelFlowData* myOutNodeData = static_cast<CT_ModelFlowData*>(outNodeData.get());

    // happen when connection will be deleted (she propagate an invalid node data)
    if(myOutNodeData == nullptr)
        return;

    const CT_InAbstractModel* inModel = nullptr;
    const CT_OutAbstractModel* outModel = nullptr;

    // get the out model at the end of the connection
    // and the in model at the other end of the connection
    resolveInModelAndOutModel(inNodeDataModel,
                              outNodeData,
                              inPortIndex,
                              inModel,
                              outModel);

    // visit possibilities to find the possibility that use the out model
    inModel->visitPossibilities([this, &outModel, &connection](const CT_InAbstractModel* inModel, const CT_InStdModelPossibility* possibility) -> bool
    {
        // if it is this possibility
        if(possibility->outModel() == outModel)
        {
            // hold the connection that match this possibility for future use
            mConnectionByPossibility.insert(possibility, const_cast<Connection*>(connection));

            // select the possibility
            const_cast<CT_InStdModelPossibility*>(possibility)->setSelected(true);

            // go up (recursively) to select parents
            CT_InAbstractModel* parentInModel = static_cast<CT_InAbstractModel*>(inModel->parentModel());

            int nP = 0;

            // if the current model has a parent model and the parent model has at least one possibility saved
            if(parentInModel != nullptr
                    && (nP = parentInModel->nPossibilitySaved()) > 0)
            {
                for(int i=0; i<nP; ++i)
                {
                    CT_InStdModelPossibility* parentPossibility = parentInModel->possibilitySavedAt(i);

                    // if the out model used by the possibility if the parent of the current out model
                    if(parentPossibility->outModel() == outModel->parentModel()) {
                        // we select it (this will call the method "createOrDeleteConnectionWhenPossibilitySelectionStateHasChanged"
                        // that will create a connection and this method will be called again until the out model has no parent)
                        parentPossibility->setSelected(true);
                        break;
                    }
                }
            }

            return false;
        }

        return true;
    });
}

void CTG_ModelsLinkConfigurationFlowView::createOrDeleteConnectionWhenPossibilitySelectionStateHasChanged(const CT_InStdModelPossibility* possibility, bool state)
{
    Connection* c = mConnectionByPossibility.value(possibility, nullptr);

    if((c == nullptr) && (state == true))
    {
        PortIndex inPortIndex = static_cast<CT_ModelFlowDataModel*>(mInNode->nodeDataModel())->portIndexOfModel(mInModelByPossibility.value(possibility, nullptr));
        PortIndex outPortIndex = static_cast<CT_ModelFlowDataModel*>(mOutNode->nodeDataModel())->portIndexOfModel(possibility->outModel());

        auto sharedC = createConnection(inPortIndex, outPortIndex, false);
        mConnectionByPossibility.insert(possibility, sharedC.get());
    }
    else if((c != nullptr) && (state == false))
    {
        mConnectionByPossibility.remove(possibility);
        mFlowScene->deleteConnection(*c);
    }
}

bool CTG_ModelsLinkConfigurationFlowView::resolveInModelAndOutModel(const QtNodes::NodeDataModel* inNodeDataModel,
                                                                    std::shared_ptr<QtNodes::NodeData> outNodeData,
                                                                    int inPortIndex,
                                                                    const CT_InAbstractModel*& inModel,
                                                                    const CT_OutAbstractModel*& outModel)
{
    CT_ModelFlowData* myOutNodeData = static_cast<CT_ModelFlowData*>(outNodeData.get());

    // get the out model at the end of the connection
    if(mInModelPortType == PT_IN)
        outModel = dynamic_cast<const CT_OutAbstractModel*>(myOutNodeData->model());
    else
        inModel = dynamic_cast<const CT_InAbstractModel*>(myOutNodeData->model());

    // get the in model at the other end of the connection
    std::shared_ptr<NodeData> inNodeData = inNodeDataModel->inData(inPortIndex);
    CT_ModelFlowData* myInNodeData = static_cast<CT_ModelFlowData*>(inNodeData.get());

    if(mInModelPortType == PT_IN)
        inModel = dynamic_cast<const CT_InAbstractModel*>(myInNodeData->model());
    else
        outModel = dynamic_cast<const CT_OutAbstractModel*>(myInNodeData->model());

    return (inModel != nullptr) && (outModel != nullptr);
}

void CTG_ModelsLinkConfigurationFlowView::displayPreviewConnectionsForModel(CT_AbstractModel* model)
{
    mRestoreConnectionOrCreatePreviewInProgress = true;

    deletePreviewConnections();

    const QList<const CT_InStdModelPossibility*> possibilities = mPossibilitiesByModel.values(model);

    for(const CT_InStdModelPossibility* possibility : possibilities)
    {
        if(!possibility->isSelected())
        {
            const PortIndex inPortIndex = static_cast<CT_ModelFlowDataModel*>(mInNode->nodeDataModel())->portIndexOfModel(mInModelByPossibility.value(possibility, nullptr));
            const PortIndex outPortIndex = static_cast<CT_ModelFlowDataModel*>(mOutNode->nodeDataModel())->portIndexOfModel(possibility->outModel());

            if(inPortIndex != -1 && outPortIndex != -1)
                mPreviewConnections.insert(createConnection(inPortIndex, outPortIndex, true).get(), possibility);
        }
    }

    mRestoreConnectionOrCreatePreviewInProgress = false;
}

void CTG_ModelsLinkConfigurationFlowView::createOrDeleteConnectionWhenPossibilitySelectionStateHasChanged(bool state)
{
    const CT_InStdModelPossibility* possibility = static_cast<CT_InStdModelPossibility*>(sender());

    createOrDeleteConnectionWhenPossibilitySelectionStateHasChanged(possibility, state);
}

void CTG_ModelsLinkConfigurationFlowView::unselectPossibilityWhenConnectionIsDeleted(const QtNodes::Connection& c)
{
    if(c.connectionState().isAPreview())
        return;

    Node* inNode = c.getNode(PortType::In);
    Node* outNode = c.getNode(PortType::Out);
    PortIndex inPortIndex = c.getPortIndex(PortType::In);
    PortIndex outPortIndex = c.getPortIndex(PortType::Out);

    const CT_InAbstractModel* inModel = nullptr;
    const CT_OutAbstractModel* outModel = nullptr;

    // get the out model at the end of the connection
    // and the in model at the other end of the connection
    resolveInModelAndOutModel(inNode->nodeDataModel(),
                              outNode->nodeDataModel()->outData(outPortIndex),
                              inPortIndex,
                              inModel,
                              outModel);

    // unselect the possibility that use this out model
    inModel->visitPossibilities([this, &outModel](const CT_InAbstractModel*, const CT_InStdModelPossibility* possibility) -> bool
    {
        if(possibility->outModel() == outModel)
        {
            mConnectionByPossibility.remove(possibility);
            const_cast<CT_InStdModelPossibility*>(possibility)->setSelected(false);
            return false;
        }

        return true;
    });

    // unselect selected possibility and delete connection of childrens (if the out model of the child's possibility is a child of the current out model)
    inModel->visitInChildrens([this, &outModel](const CT_InAbstractModel* child) -> bool
    {
        CT_InAbstractModel::Possibilities childSelectedPossibilities = child->selectedPossibilities();

        for(CT_InStdModelPossibility* childSelectedPossibility : childSelectedPossibilities)
        {
            CT_OutAbstractModel* outChildModel = childSelectedPossibility->outModel();

            // check if the out model of this possibility is a child or the current out model
            bool outModelOfPossibilityIsAChildOfTheCurrentOutModel = false;

            auto visitor = [&outModelOfPossibilityIsAChildOfTheCurrentOutModel, &outChildModel](const CT_AbstractModel* m) -> bool {
                outModelOfPossibilityIsAChildOfTheCurrentOutModel = (m == outChildModel);
                return !outModelOfPossibilityIsAChildOfTheCurrentOutModel;
            };

            outModel->visitChildrens(visitor);

            // only delete the connection if it is a child of the current out model
            if(outModelOfPossibilityIsAChildOfTheCurrentOutModel)
            {
                Connection* c = mConnectionByPossibility.take(childSelectedPossibility);
                childSelectedPossibility->setSelected(false);
                mFlowScene->deleteConnection(*c);
            }
        }

        return true;
    });

    displayPreviewConnectionsForModel(mModelOfLastIndexClicked[0]);
}

void CTG_ModelsLinkConfigurationFlowView::displayPreviewConnectionsForIndexClicked(const QModelIndex& index)
{
    if(isReadOnly())
        return;

    if(index.model() == mInTreeWidget->model())
        mOutTreeWidget->setCurrentIndex(QModelIndex());
    else
        mInTreeWidget->setCurrentIndex(QModelIndex());

    CT_AbstractModel* model = static_cast<CT_AbstractModel*>(index.data(Qt::UserRole).value<void*>());
    mModelOfLastIndexClicked[1] = mModelOfLastIndexClicked[0];
    mModelOfLastIndexClicked[0] = model;

    displayPreviewConnectionsForModel(model);
}

void CTG_ModelsLinkConfigurationFlowView::convertPreviewConnectionBetweenIndexAndPreviewIndexClickedToConnection(const QModelIndex& index)
{
    if(isReadOnly() || (mModelOfLastIndexClicked[1] == nullptr))
        return;

    CT_AbstractModel* model = static_cast<CT_AbstractModel*>(index.data(Qt::UserRole).value<void*>());

    if(model == mModelOfLastIndexClicked[1])
        return;

    const QList<const CT_InStdModelPossibility*> p1 = mPossibilitiesByModel.values(model);
    const QList<const CT_InStdModelPossibility*> p2 = mPossibilitiesByModel.values(mModelOfLastIndexClicked[1]);

    const QSet<const CT_InStdModelPossibility*> pI = p1.toSet().intersect(p2.toSet());

    if(!pI.isEmpty())
        const_cast<CT_InStdModelPossibility*>(*pI.begin())->setSelected(true);

    mModelOfLastIndexClicked[1] = mModelOfLastIndexClicked[0];
    mModelOfLastIndexClicked[0] = model;
    displayPreviewConnectionsForModel(model);
}

void CTG_ModelsLinkConfigurationFlowView::connectionSelected(QtNodes::Connection& c)
{
    if(c.connectionState().isAPreview()) {
        mFlowScene->deleteConnection(c);
        const CT_InStdModelPossibility* possibility = mPreviewConnections.take(&c);
        const_cast<CT_InStdModelPossibility*>(possibility)->setSelected(true);

        displayPreviewConnectionsForModel(mModelOfLastIndexClicked[0]);
    }
}

void CTG_ModelsLinkConfigurationFlowView::connectionDoubleClicked(QtNodes::Connection& c)
{
    if(!c.connectionState().isAPreview())
        mFlowScene->deleteConnection(c);
}

void CTG_ModelsLinkConfigurationFlowView::nodeSelected(QtNodes::Node& /*n*/)
{
    mInTreeWidget->clearSelection();
    mOutTreeWidget->clearSelection();

    mOutTreeWidget->setCurrentIndex(QModelIndex());
    mInTreeWidget->setCurrentIndex(QModelIndex());

    deletePreviewConnections();

    mModelOfLastIndexClicked.fill(nullptr);
}

void CTG_ModelsLinkConfigurationFlowView::rubberBandChanged(QRect rubberBandRect, QPointF fromScenePoint, QPointF toScenePoint)
{
    // all is null when rubberband selection ends
    if(rubberBandRect.isNull() && fromScenePoint.isNull() && toScenePoint.isNull())
        convertPreviewConnectionsSelectedToConnections();
}

QSize RowDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    const QSize s = QStyledItemDelegate::sizeHint(option, index);

    const_cast<RowDelegate*>(this)->maxWidth = qMax(maxWidth, s.width());

    return QSize(s.width(), TREE_WIDGET_ITEM_SPACING);
}
