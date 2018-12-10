#include "ct_result/model/inModel/tools/ct_inresultmodelmanager.h"

#include "ct_step/abstract/ct_virtualabstractstep.h"

#include "ct_model/tools/ct_modelselectionhelper.h"

#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/inModel/tools/ct_instdresultmodelpossibility.h"

#include "ct_result/model/outModel/abstract/ct_outabstractresultmodel.h"

CT_InResultModelManager::CT_InResultModelManager()
{
}

CT_InResultModelManager::~CT_InResultModelManager()
{
    clearAllResultModels();
}

bool CT_InResultModelManager::addResultModel(CT_InAbstractResultModel *model)
{
    if(exist(model))
    {
        delete model;
        return false;
    }

    m_inputResultModels.append(model);

    return true;
}

void CT_InResultModelManager::clearAllResultModels()
{
    clearSearchModelCollection();

    qDeleteAll(m_inputResultModels.begin(), m_inputResultModels.end());
    m_inputResultModels.clear();
}

bool CT_InResultModelManager::checkFromThisStepIfAllInputModelsFindMatchingOutputModels(const CT_VirtualAbstractStep* step)
{
    return findFromThisStepAllMatchingOutputModels(step, DontSavePossibilities);
}

bool CT_InResultModelManager::findFromThisStepAllMatchingOutputModelsAndSavePossibilities(const CT_VirtualAbstractStep* step)
{
    return findFromThisStepAllMatchingOutputModels(step, SavePossibilities);
}

bool CT_InResultModelManager::checkIfAllInputModelsWasCorrectlyConfigured(QStringList* errors) const
{
    for(CT_InAbstractResultModel* inputResultModel : m_inputResultModels)
    {
        if(inputResultModel->needOutputModel()
                && (inputResultModel->getPossibilitiesSelected().size() < inputResultModel->minimumNumberOfPossibilityThatMustBeSelectedForOneTurn())) {

            if(errors != NULL)
                errors->append(QObject::tr("Le modèle %1 (%2) a %3 possibilité(s) sélectionnée(s) cependant il faut en sélectionner au minimum %4").arg(inputResultModel->displayableName()).arg(inputResultModel->uniqueName()).arg(inputResultModel->getPossibilitiesSelected().size()).arg(inputResultModel->minimumNumberOfPossibilityThatMustBeSelectedForOneTurn()));

            return false;
        }

        if((inputResultModel->minimumNumberOfPossibilityThatMustBeSelectedForOneTurn() > 0) && !inputResultModel->recursiveHasTheMinimumNumberOfSelectedPossibilityRequired(errors))
            return false;
    }

    return true;
}

bool CT_InResultModelManager::isEmpty() const
{
    return m_inputResultModels.isEmpty();
}

bool CT_InResultModelManager::needInputResults() const
{
    for(CT_InAbstractResultModel* inputResultModel : m_inputResultModels) {
        if(inputResultModel->needOutputModel())
            return true;
    }

    return false;
}

const QList<CT_InAbstractResultModel*>& CT_InResultModelManager::models() const
{
    return m_inputResultModels;
}

bool CT_InResultModelManager::exist(const CT_InAbstractResultModel *model) const
{
    const QString uniqueName = model->uniqueName();

    for(CT_InAbstractResultModel* inputResultModel : m_inputResultModels) {
        if(inputResultModel->uniqueName() == uniqueName)
            return true;
    }

    return false;
}

void CT_InResultModelManager::saveSettings(SettingsWriterInterface& writer) const
{
    for(CT_InAbstractResultModel* inputResultModel : m_inputResultModels) {
        inputResultModel->saveSettings(writer);
    }
}

bool CT_InResultModelManager::restoreSettings(SettingsReaderInterface& reader)
{
    for(CT_InAbstractResultModel* inputResultModel : m_inputResultModels) {
        if(!inputResultModel->restoreSettings(reader))
            return false;
    }

    return true;
}

void CT_InResultModelManager::recursiveUnselectAllPossibilities()
{
    std::function<void(CT_InAbstractModel*, CT_InStdModelPossibility*)> visitor = [](CT_InAbstractModel* model, CT_InStdModelPossibility* possibility) {
                                                                                        Q_UNUSED(model)
                                                                                        possibility->setSelected(false);
                                                                                    };

    recursiveVisitPossibilities(visitor);
}

void CT_InResultModelManager::recursiveVisitPossibilities(std::function<void(CT_InAbstractModel*, CT_InStdModelPossibility*)>& visitor)
{
    for(CT_InAbstractResultModel* inputResultModel : m_inputResultModels) {
        inputResultModel->recursiveVisitPossibilities(visitor);
    }
}

void CT_InResultModelManager::createSearchModelCollection()
{
    typedef std::function<void (CT_InAbstractModel*, CT_InStdModelPossibility*)> FuncVisitor;

    FuncVisitor visitor = [this](CT_InAbstractModel* model, CT_InStdModelPossibility* possibility) {
                            Q_UNUSED(model)
                            if(possibility->isSelected()) {
                                CT_InStdResultModelPossibility* resultModelPossibility = static_cast<CT_InStdResultModelPossibility*>(possibility);

                                // get the output result model that is link with this possibility
                                CT_OutAbstractResultModel* outModel = static_cast<CT_OutAbstractResultModel*>(resultModelPossibility->outModel());

                                // get all input models recursively (childrens and childrens of childrens, etc...)
                                const QList<CT_AbstractModel*> childrens = resultModelPossibility->inResultModel()->recursiveGetAllModels();

                                // create a map with key = unique name AND value = the model
                                SearchModelCollectionValueType* map = new SearchModelCollectionValueType();
                                map->reserve(childrens.size());

                                // and insert it in the map that contains all models for current
                                // turn with key = a model that represent a result AND value = the map
                                m_searchModelCollection.insertMulti(outModel, map);

                                for(CT_AbstractModel* inModel : childrens)
                                {
                                    // all unique name is unique !
                                    Q_ASSERT(!map->contains(inModel->uniqueName()));

                                    // insert it in the map
                                    map->insert(inModel->uniqueName(), static_cast<CT_InAbstractModel*>(inModel));
                                }
                            }
                          };

    clearSearchModelCollection();

    for(CT_InAbstractResultModel* inputResultModel : m_inputResultModels) {
        inputResultModel->visitPossibilities(visitor);
    }
}

void CT_InResultModelManager::clearSearchModelCollection()
{
    qDeleteAll(m_searchModelCollection.begin(), m_searchModelCollection.end());
    m_searchModelCollection.clear();
}

CT_InAbstractResultModel* CT_InResultModelManager::resultModelByUniqueName(const QString& uniqueName) const
{
    for(CT_InAbstractResultModel* model : m_inputResultModels) {
        if(model->uniqueName() == uniqueName)
            return model;
    }

    return NULL;
}

CT_InAbstractModel* CT_InResultModelManager::modelByUniqueName(const CT_OutAbstractResultModel& resultModel, const QString& uniqueName) const
{
    SearchModelCollectionValueType* map = m_searchModelCollection.value(const_cast<CT_OutAbstractResultModel*>(&resultModel), NULL);

    Q_ASSERT(map != NULL);

    return map->value(uniqueName, NULL);
}

bool CT_InResultModelManager::findFromThisStepAllMatchingOutputModels(const CT_VirtualAbstractStep* step,
                                                                      const ToDoWithPossibilities& toDoWithPossibilities)
{
    if((step == NULL) || m_inputResultModels.isEmpty())
        return false;

    // if we must save possibilities
    if(toDoWithPossibilities == SavePossibilities)
    {
        // we clear all possibilities of models
        for(CT_InAbstractResultModel* inputResultModel : m_inputResultModels) {
            inputResultModel->recursiveClearPossibilitiesSaved();
        }

        // for each models and while we found obligatory elements
        for(CT_InAbstractResultModel* inputResultModel : m_inputResultModels) {
            if(!staticFindFromThisStepAllMatchingOutputModelsThatMatchWithTheInputModel(step, inputResultModel))
                return false;
        }

        return true;
    }

    for(CT_InAbstractResultModel* model : m_inputResultModels)
    {
        // we copy the model (without his possibilities) and his childrens, so we don't alter possibilities of the original model
        CT_InAbstractResultModel* cpyModel = static_cast<CT_InAbstractResultModel*>(model->copy(false));

        const bool valid = [&]()->bool {
                                if(staticFindFromThisStepAllMatchingOutputModelsThatMatchWithTheInputModel(step, cpyModel)) {
                                    // TODO : MK 25.10.18 ==> check if this is necessary ???!!!
                                    CT_ModelSelectionHelper selectionHelper(cpyModel);
                                    return selectionHelper.isValid();
                                }

                                return false;
                            }();

        // delete the temporary model
        delete cpyModel;

        if(!valid)
            return false;
    }

    return true;
 }

bool CT_InResultModelManager::staticFindRecursivelyFromThisStepAllMatchingOutputModelsThatMatchWithTheInputModel(const CT_VirtualAbstractStep* step,
                                                                                                                 CT_InAbstractResultModel* inputResultModel)
{
    // search only in output model of this step
    bool found = staticFindOnlyForThisStepAllMatchingOutputModelsThatMatchWithTheInputModel(step, inputResultModel);

    if(step != NULL)
    {
        CT_VirtualAbstractStep *parent = step->parentStep();

        // if this step has a parent step
        if(parent != NULL)
        {
            // do the same thing for the parent step
            if(staticFindRecursivelyFromThisStepAllMatchingOutputModelsThatMatchWithTheInputModel(parent, inputResultModel))
                found = true;
        }
    }

    return found;
}

bool CT_InResultModelManager::staticFindOnlyForThisStepAllMatchingOutputModelsThatMatchWithTheInputModel(const CT_VirtualAbstractStep* step,
                                                                                                         CT_InAbstractResultModel* inputResultModel)
{
    if(step == NULL)
        return false;

    bool found = false;

    // get all output result models generated by this step
    const QList<CT_OutAbstractResultModel*> outResultModels = step->getOutResultModels();

    for(CT_OutAbstractResultModel* outResultModel : outResultModels)
    {
        // ask the input result model to save possibilities if this output result model and his childrens match with criteria
        // defined by input models (childrens of the input result model)
        if(inputResultModel->recursiveFindAllPossibilitiesInModel(*outResultModel))
            found = true;
    }

    return found;
}

bool CT_InResultModelManager::staticFindFromThisStepAllMatchingOutputModelsThatMatchWithTheInputModel(const CT_VirtualAbstractStep* step,
                                                                                                      CT_InAbstractResultModel* inputResultModel)
{
    if(inputResultModel->needOutputModel())
    {
        const bool found = [&]() -> bool {
                                // if this model must search recursively we call the appropriate method to search in this step and in parents step of the tree
                                if(inputResultModel->isRecursive())
                                    return staticFindRecursivelyFromThisStepAllMatchingOutputModelsThatMatchWithTheInputModel(step, inputResultModel);

                                // otherwise we call the other method to search only in this step
                                return staticFindOnlyForThisStepAllMatchingOutputModelsThatMatchWithTheInputModel(step, inputResultModel);
                           }();

        if(!found && (inputResultModel->minimumNumberOfPossibilityThatMustBeSelectedForOneTurn() > 0))
            return false;
    }

    return true;
}
