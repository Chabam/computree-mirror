#include "steptestmodel2.h"

#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"

#include "ct_itemdrawable/ct_pointcluster.h"
#include "ct_itemdrawable/ct_ellipse.h"

#include <QDebug>

#define DEF_SearchInResult          "ri"
#define DEF_SearchInSectionGroup    "sg"
#define DEF_SearchInClusterGroup    "cg"
#define DEF_SearchInPointCluster    "p"

StepTestModel2::StepTestModel2(CT_StepInitializeData& dataInit) : SuperClass(dataInit)
{
}

CT_VirtualAbstractStep* StepTestModel2::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new StepTestModel2(dataInit);
}

void StepTestModel2::createInResultModelListProtected()
{
    CT_InResultModelGroupToCopy * resultModel = createNewInResultModelForCopy(DEF_SearchInResult, tr("Sections / Clusters"));
    resultModel->setZeroOrMoreRootGroup();
    resultModel->addGroupModel("", m_arInGroup, CT_StandardItemGroup::staticGetType(), "", "", CT_InAbstractGroupModel::CG_ChooseMultipleIfMultiple, CT_InAbstractGroupModel::FG_IsObligatory);
    resultModel->addItemModel(m_arInGroup.completeName(), m_arInPointCluster, CT_PointCluster::staticGetType(), "", "", CT_InAbstractModel::C_ChooseMultipleIfMultiple, CT_InAbstractModel::F_IsObligatory);
}

void StepTestModel2::createOutResultModelListProtected()
{
    CT_OutResultModelGroupToCopyPossibilities* resultCopy = createNewOutResultModelToCopy(DEF_SearchInResult);
    resultCopy->addItemModel(m_arInGroup.completeName(), m_arOutEllipse, new CT_Ellipse(), tr("Ellipses"));
}

void StepTestModel2::compute()
{
}
