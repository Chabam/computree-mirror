#include "ct_topfnodegroup.h"

#include "ct_itemdrawable/model/outModel/ct_outopfnodegroupmodel.h"

CT_TOPFNodeGroup::CT_TOPFNodeGroup() : CT_TNodeGroup()
{
}

CT_TOPFNodeGroup::CT_TOPFNodeGroup(const CT_OutOPFNodeGroupModel *model,
                                   const CT_AbstractResult *result) : CT_TNodeGroup(model, result)
{
}

CT_TOPFNodeGroup::CT_TOPFNodeGroup(const QString &modelName,
                                   const CT_AbstractResult *result) : CT_TNodeGroup(modelName, result)
{
}

CT_AbstractItemDrawable* CT_TOPFNodeGroup::copy(const CT_OutAbstractItemModel* model,
                                                const CT_AbstractResult* result,
                                                CT_ResultCopyModeList copyModeList)
{
    Q_ASSERT(dynamic_cast<const CT_OutOPFNodeGroupModel*>(model) != NULL);
    Q_UNUSED(copyModeList)

    const CT_OutOPFNodeGroupModel* newModel = static_cast<const CT_OutOPFNodeGroupModel*>(model);

    CT_TOPFNodeGroup* copyGroup = new CT_TOPFNodeGroup(newModel, result);
    copyGroup->setId(id());
    copyGroup->setAlternativeDrawManager(getAlternativeDrawManager());
    this->copyChildrensToGroupIfTheyAreStillPresentInModel(copyGroup);

    // we can not copy successor etc... because we don't know the model

    return copyGroup;
}

void CT_TOPFNodeGroup::setOPFMatrix(const QMatrix4x4 &matrix)
{
    m_opfMatrix = matrix;
}

QMatrix4x4 CT_TOPFNodeGroup::opfMatrix() const
{
    return m_opfMatrix;
}

CT_OutOPFNodeGroupModel* CT_TOPFNodeGroup::opfModel() const
{
    return (CT_OutOPFNodeGroupModel*)model();
}

void CT_TOPFNodeGroup::setOPFID(const size_t &id)
{
    setId(id);
}
