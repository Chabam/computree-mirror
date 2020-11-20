#include "ct_modelflowdatamodel.h"

#include "ct_modelflowdata.h"

CT_ModelFlowDataModel::CT_ModelFlowDataModel() :
    mModel(nullptr),
    mPortType(PortType::Out),
    mEmbeddedWidget(nullptr)
{
}

CT_ModelFlowDataModel::CT_ModelFlowDataModel(const QString& title,
                                             const QAbstractItemModel* model,
                                             PortType portType,
                                             QWidget* embeddedWidget) :
    mTitle(title),
    mModel(const_cast<QAbstractItemModel*>(model)),
    mPortType(portType),
    mEmbeddedWidget(embeddedWidget)
{
    if(mModel == nullptr)
        return;

    PortIndex portIndex = 0;
    const int nR = mModel->rowCount();

    for(int r=0; r<nR; ++r)
    {
        QModelIndex root = mModel->index(r, 0);
        recursiveConstructPorts(root, portIndex);
    }
}

QString CT_ModelFlowDataModel::caption() const
{
    return mTitle;
}

bool CT_ModelFlowDataModel::portCaptionVisible(QtNodes::PortType, QtNodes::PortIndex) const
{
    return mEmbeddedWidget != nullptr;
}

QString CT_ModelFlowDataModel::name() const
{
    return QString("CT_ModelFlowDataModel");
}

unsigned int CT_ModelFlowDataModel::nPorts(QtNodes::PortType portType) const
{
    if(portType == mPortType)
        return uint(mModelByPortIndex.size());

    return 0;
}

QtNodes::NodeDataType CT_ModelFlowDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if(portType == mPortType) {
        const PortInfo info = mModelByPortIndex.value(portIndex, PortInfo{});

        QString otherNeeded = "0";
        // IN PROGRESS
        /*QString otherNeeded = "-1";
        bool connectionsRequired = info.mModel->isOptionnal();
        if(connectionsRequired)
        {
          //int maxConnectionsRequired = info.mModel->maximumNumberOfPossibilityThatCanBeSelected();
          //quint8 minConnectionsRequired = info.mModel->minimumNumberOfPossibilityToSelect();
          otherNeeded = "0";
        }*/

        if(info.mModel != nullptr)
            return CT_ModelFlowData(info.mModel, otherNeeded).type();
    }

    return NodeDataType();
}

std::shared_ptr<QtNodes::NodeData> CT_ModelFlowDataModel::outData(QtNodes::PortIndex portIndex) const
{
    if(mPortType == PortType::Out) {

        const PortInfo info = mModelByPortIndex.value(portIndex, PortInfo{});

        return std::make_shared<CT_ModelFlowData>(info.mModel, info.mId);
    }

    return std::make_shared<CT_ModelFlowData>();
}

std::shared_ptr<QtNodes::NodeData> CT_ModelFlowDataModel::inData(QtNodes::PortIndex portIndex) const
{
    if(mPortType == PortType::In) {

        const PortInfo info = mModelByPortIndex.value(portIndex, PortInfo{});

        return std::make_shared<CT_ModelFlowData>(info.mModel, info.mId);
    }

    return std::make_shared<CT_ModelFlowData>();
}

QWidget* CT_ModelFlowDataModel::embeddedWidget()
{
    return mEmbeddedWidget;
}

QtNodes::PortIndex CT_ModelFlowDataModel::portIndexOfModel(const CT_AbstractModel* model) const
{
    QHashIterator<PortIndex, PortInfo> it(mModelByPortIndex);

    while(it.hasNext())
    {
        it.next();

        if(it.value().mModel == model)
            return it.key();
    }

    return -1;
}

void CT_ModelFlowDataModel::recursiveConstructPorts(QModelIndex current, PortIndex& portIndex)
{
    CT_AbstractModel* model = static_cast<CT_AbstractModel*>(current.data(Qt::UserRole).value<void*>());
    mModelByPortIndex.insert(portIndex++, PortInfo{model, current.data(Qt::UserRole+1).toString()});

    const int nR = mModel->rowCount(current);

    for(int r=0; r<nR; ++r)
    {
        QModelIndex child = mModel->index(r, 0, current);
        recursiveConstructPorts(child, portIndex);
    }
}
