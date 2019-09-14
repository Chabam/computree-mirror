#include "dm_resultgroupgui.h"

#include "ct_model/outModel/ct_outresultmodelgroup.h"

DM_ResultGroupGui::DM_ResultGroupGui() : SuperClass(),
    mRootGroup(nullptr),
    mFinalized(false)
{
}

void DM_ResultGroupGui::init()
{
    DM_ItemGroupGui::ModelType* rootGroupModel = new DM_ItemGroupGui::ModelType(tr("Group GUI"));
    CT_OutResultModelGroup<DM_ResultGroupGui>* resultModel = new CT_OutResultModelGroup<DM_ResultGroupGui>(rootGroupModel, tr("Result GUI"), tr("Result GUI"));

    setModel(resultModel);

    mRootGroup = new DM_ItemGroupGui();
    mRootGroup->setModel(rootGroupModel);

    addRootGroupWithOutModel(rootGroupModel, mRootGroup);

    mManager.setResultModel(resultModel);
}

void DM_ResultGroupGui::setGraphicsView(const GraphicsViewInterface* gv)
{
    mRootGroup->setGraphicsView(gv);
}

void DM_ResultGroupGui::addItemDrawable(CT_AbstractItemDrawable& item)
{
    mRootGroup->addItemDrawable(item);
    mFinalized = false;
}

void DM_ResultGroupGui::removeItemDrawable(CT_AbstractItemDrawable& item)
{
    mRootGroup->removeItemDrawable(item);
    mFinalized = false;
}

void DM_ResultGroupGui::setAttributes(const QList<CT_AbstractAttributes*>& attributes)
{
    mRootGroup->setAttributes(attributes);
    mFinalized = false;
}

void DM_ResultGroupGui::finalizeBeforeUseIt()
{
    if(mFinalized)
        return;

    mRootGroup->finalizeBeforeUseIt();

    int index = 0;
    const auto ig = [&index]() -> int { return index++; };
    mManager.finalize(ig);

    modelStaticT<CT_OutResultModelGroup<DM_ResultGroupGui>>()->setResult(this);
}

const DM_ResultGroupGui::DM_OutModelStructureManager* DM_ResultGroupGui::outModelStructureManager() const
{
    if(mFinalized)
        return nullptr;

    return &mManager;
}

CT_AbstractResult* DM_ResultGroupGui::copy(const CT_OutAbstractResultModel*) const
{
    return nullptr;
}
