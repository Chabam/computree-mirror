#include "ct_inabstractresultmodel.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibilityselectiongroup.h"
#include "ct_model/inModel/tools/ct_instdresultmodelpossibility.h"
#include "ct_model/outModel/abstract/ct_outabstractresultmodel.h"

bool CT_InAbstractResultModel::FORCE_RECURSIVITY = false;

CT_InAbstractResultModel::CT_InAbstractResultModel(const QString &displayableName) : SuperClass(displayableName),
    m_recursive(FORCE_RECURSIVITY),
    m_currentInResultModelCopy(NULL)
{
}

CT_InAbstractResultModel::CT_InAbstractResultModel(const CT_InAbstractResultModel& other) : SuperClass(other),
    m_recursive(other.m_recursive),
    m_currentInResultModelCopy(NULL)
{
}

void CT_InAbstractResultModel::setRecursive(bool r)
{
    setRecursiveWithoutUseForceRecursivity(r | FORCE_RECURSIVITY);
}

bool CT_InAbstractResultModel::isRecursive() const
{
    // if step of this model is a prototype (not used in a tree)
    if((step() != NULL)
            && step()->stepToolForModel()->isStepAPrototype())
        return m_recursive | FORCE_RECURSIVITY;

    return m_recursive;
}

bool CT_InAbstractResultModel::recursiveFindAllPossibilitiesInModel(const CT_OutAbstractModel& rootModel, bool mustClearPossibilitiesSaved)
{
    emit startFindNewPossibilities(this);

    if(mustClearPossibilitiesSaved)
        recursiveClearPossibilitiesSaved();

    // create a copy of this result model but without possibilities
    CT_InAbstractResultModel* copyOfThis = static_cast<CT_InAbstractResultModel*>(this->copy(false));

    // if copyOfThis is compatible with the rootModel it will call the method
    // "createAndAddAPossibility" that will call the method "createNewPossibility" that will
    // set the m_currentInResultModelCopy of copyOfThis to the possibility. So
    // the possibility created by copyOfThis will be linked to it.
    copyOfThis->m_currentInResultModelCopy = copyOfThis;

    // the current copy of in result model that will be used in method "createNewPossibility"
    m_currentInResultModelCopy = copyOfThis;

    // if all elements has been found for this result and his childrens (recursively)
    if(copyOfThis->recursiveFindPossibilitiesInModel(rootModel) != 0) {
        // we create and add it to a new possibility

        // this will call createNewPossibility(...) and set m_currentInResultModelCopy in a CT_InStdResultModelPossibility
        createAndAddAPossibility(rootModel);

        // delete this variable because may be a problem has happened in createAndAddAPossibility(...). If there was
        // no problem this variable is NULL so this will do nothing.
        delete m_currentInResultModelCopy;
        m_currentInResultModelCopy = NULL;

        return true;
    }

    delete m_currentInResultModelCopy;
    m_currentInResultModelCopy = NULL;

    return false;
}

bool CT_InAbstractResultModel::visitRealResultsThatMatchWithSelectedPossibilities(const CT_InAbstractResultModel::RealResultVisitor& visitor) const
{
    const auto visitorAdapter = [this, &visitor](const CT_InStdModelPossibility* possibility) -> bool {

        CT_OutAbstractResultModel* outModel = static_cast<CT_OutAbstractResultModel*>(possibility->outModel());

        return this->visitRealResultForModel(outModel, visitor);
    };

    return possibilitiesGroup()->visitSelectedPossibilities(visitorAdapter);
}

bool CT_InAbstractResultModel::visitRealResultsThatMatchWithThisPossibility(const CT_InStdModelPossibility* possibility,
                                                                            const CT_InAbstractResultModel::RealResultVisitor& visitor) const
{
    MODELS_ASSERT(possibilitiesGroup()->indexOf(const_cast<CT_InStdModelPossibility*>(possibility)) != -1);

    CT_OutAbstractResultModel* outModel = static_cast<CT_OutAbstractResultModel*>(possibility->outModel());

    return this->visitRealResultForModel(outModel, visitor);
}

CT_InAbstractModel* CT_InAbstractResultModel::recursiveSearchTheModelThatWasACopiedModelFromThisOriginalModel(const CT_InAbstractModel* originalModel) const
{
    Q_ASSERT(originalModel != NULL);

    const CT_InAbstractModel* firstOriginalOutModel = originalModel->recursiveOriginalModel();

    CT_InAbstractResultModel* rootModel = dynamic_cast<CT_InAbstractResultModel*>(firstOriginalOutModel->rootModel());

    if((rootModel == firstOriginalOutModel) && (rootModel->recursiveOriginalModel() == this->recursiveOriginalModel()))
        return const_cast<CT_InAbstractResultModel*>(this);

    CT_InAbstractModel* copiedModel = NULL;

    const auto visitor = [&copiedModel, &firstOriginalOutModel](const CT_InAbstractModel* child) -> bool {
        if(child->recursiveOriginalModel() == firstOriginalOutModel) {
            copiedModel = const_cast<CT_InAbstractModel*>(child);
            return false;
        }

        return true;
    };

    recursiveVisitInChildrens(visitor);

    return copiedModel;
}

void CT_InAbstractResultModel::setRecursiveWithoutUseForceRecursivity(bool e)
{
    m_recursive = e;
}

void CT_InAbstractResultModel::staticSetRecursiveWithoutUseForceRecursivity(CT_InAbstractResultModel *model, bool e)
{
    model->setRecursiveWithoutUseForceRecursivity(e);
}

bool CT_InAbstractResultModel::visitInChildrensOrInModelOfPossibilities(const InChildrenVisitor& visitor) const
{
    bool mustVisitChildrens = false;

    const auto visitorAdapter = [this, &visitor, &mustVisitChildrens](const CT_InStdModelPossibility* possibility) -> bool {
        CT_InAbstractModel* inModel = static_cast<const CT_InStdResultModelPossibility*>(possibility)->inResultModel();

        // if the input model inside the possibility is this model
        // we must not visit possibilities otherwise we will go into a infinite loop !
        if(inModel == this) {
            mustVisitChildrens = true; // so we inform that we must visit childrens
            return false;
        }

        return visitor(inModel);
    };

    const bool ret = possibilitiesGroup()->visitPossibilities(visitorAdapter);

    if(mustVisitChildrens)
        return visitInChildrens(visitor);

    return ret;
}

CT_InStdModelPossibility* CT_InAbstractResultModel::createNewPossibility() const
{
    CT_InStdResultModelPossibility* p = new CT_InStdResultModelPossibility();
    p->setInResultModel(m_currentInResultModelCopy);

    if(m_currentInResultModelCopy == this)
        p->setAutoDeleteInResultModel(false);

    m_currentInResultModelCopy = NULL;

    return p;
}

bool CT_InAbstractResultModel::visitRealResultForModel(const CT_OutAbstractResultModel* model,
                                                       const CT_InAbstractResultModel::RealResultVisitor& visitor) const
{
    // get the step which has created this model (it was the step that create the real result)
    IStepForModel* step = model->step();

    MODELS_ASSERT(step != NULL);

    return step->stepToolForModel()->visitStepHisOutResults([&model, &visitor](const IResultForModel* result) -> bool {

        if(result->resultToolForModel()->resultModel() == model)
            return visitor(result);

        return true;
    });
}
