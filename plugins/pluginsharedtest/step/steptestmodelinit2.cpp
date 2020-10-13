#include "steptestmodelinit2.h"

#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_itemdrawable/ct_pointcluster.h"

#define DEF_OutResult       "rr"
#define DEF_OutSectionGroup    "sgr"
#define DEF_OutClusterGroup    "cgr"
#define DEF_OutClusterItem     "cir"
#define DEF_OutClusterGroup2    "cgr2"
#define DEF_OutClusterItem2     "cir2"

StepTestModelInit2::StepTestModelInit2(CT_StepInitializeData& dataInit) : SuperClass(dataInit)
{

}

CT_VirtualAbstractStep* StepTestModelInit2::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new StepTestModelInit2(dataInit);
}

void StepTestModelInit2::createInResultModelListProtected()
{
    setNotNeedInputResult();
}

void StepTestModelInit2::createOutResultModelListProtected()
{
    CT_OutResultModelGroup *rModel = createNewOutResultModel(DEF_OutResult);
    rModel->setRootGroup(DEF_OutSectionGroup);
    rModel->addGroupModel(DEF_OutSectionGroup, DEF_OutClusterGroup);
    rModel->addItemModel(DEF_OutClusterGroup, DEF_OutClusterItem, new CT_PointCluster());
    rModel->addGroupModel(DEF_OutSectionGroup, DEF_OutClusterGroup2);
    rModel->addItemModel(DEF_OutClusterGroup2, DEF_OutClusterItem2, new CT_PointCluster());
}

void StepTestModelInit2::compute()
{
}
