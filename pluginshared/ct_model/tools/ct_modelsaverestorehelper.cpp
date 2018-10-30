#include "ct_modelsaverestorehelper.h"

#include "ct_step/abstract/ct_virtualabstractstep.h"

void CT_ModelSaveRestoreHelper::saveSettingsOfOutModel(const QObject* caller, SettingsWriterInterface& writer, const CT_OutAbstractModel* model, const QString& modelUniqueName)
{
    if(model == NULL) {
        writer.addParameter(caller, modelUniqueName, false);
        return;
    }

    CT_AbstractModel* originalModel = model->lastOriginalModelWithAStep();

    const CT_OutAbstractResultModel* resultModel = getResultModelThatContainsThisModel(originalModel);

    if(resultModel == NULL) {
        writer.addParameter(caller, modelUniqueName, false);
        return;
    }

    const int id = writer.addParameter(caller, modelUniqueName, true);

    // construct the hierarchy of steps
    QString stepsHierarchy;
    CT_VirtualAbstractStep *step = originalModel->step();
    while(step != NULL) {
        stepsHierarchy += step->getStepName() + "|" + QString("%1").arg(step->parentStep() != NULL ? step->parentStep()->getStepChildList().indexOf(step) : -1);
        step = step->parentStep();

        if(step != NULL)
            stepsHierarchy += ";";
    }

    writer.addParameterInfo(caller, id, "StepsHierarchy", stepsHierarchy);
    writer.addParameterInfo(caller, id, "resultModelUniqueName", resultModel->uniqueName());
    writer.addParameterInfo(caller, id, "modelUniqueName", originalModel->uniqueName());
}

CT_OutAbstractModel* CT_ModelSaveRestoreHelper::restoreOutModelFromSettings(const QObject* caller, SettingsReaderInterface& reader, const CT_VirtualAbstractStep* step, const QString& modelUniqueName, bool& savedAndFoundedOrNotSaved)
{
    QVariant value;

    // for the moment we assume that it was saved
    // so if it was a problem we return false in variable "savedAndFoundedOrNotSaved"
    savedAndFoundedOrNotSaved = false;

    const int id = reader.parameter(caller, modelUniqueName, value);

    if(id <= 0)
        return NULL;

    // if it was not saved the boolean value == false and
    // we return true in variable "savedAndFoundedOrNotSaved"
    savedAndFoundedOrNotSaved = !value.toBool();

    // if not saved
    if(savedAndFoundedOrNotSaved)
        return NULL;

    // now if it was a problem we return false in variable "savedAndFoundedOrNotSaved"

    if(!reader.parameterInfo(caller, id, "StepsHierarchy", value))
        return NULL;

    const QStringList stepsHierarchy = value.toString().split(";");

    if(stepsHierarchy.isEmpty())
        return NULL;

    if(!reader.parameterInfo(caller, id, "resultModelUniqueName", value))
        return NULL;

    const QString resultModelUniqueName = value.toString();

    if(!reader.parameterInfo(caller, id, "modelUniqueName", value))
        return NULL;

    const QString originalModelUniqueName = value.toString();

    if(resultModelUniqueName.isEmpty() || originalModelUniqueName.isEmpty())
        return NULL;

    CT_VirtualAbstractStep *pStep = searchStepInHierarchy(stepsHierarchy, step->rootStep());

    if(pStep == NULL)
        return NULL;

    CT_OutAbstractResultModel* resultModel = searchResultModelInStep(resultModelUniqueName, pStep);

    if(resultModel == NULL)
        return NULL;

    CT_OutAbstractModel* foundedModel = dynamic_cast<CT_OutAbstractModel*>(resultModel->findModelInTree(originalModelUniqueName));

    // if founded we return true in variable "savedAndFoundedOrNotSaved"
    savedAndFoundedOrNotSaved = (foundedModel != NULL);

    return foundedModel;
}

CT_OutAbstractResultModel* CT_ModelSaveRestoreHelper::getResultModelThatContainsThisModel(const CT_AbstractModel* originalModel)
{
    if((originalModel == NULL)
            || (originalModel->step() == NULL)
            || originalModel->uniqueName().isEmpty())
        return NULL;

    CT_OutAbstractResultModel* mg = [&]() -> CT_OutAbstractResultModel* {
                                        const QString originalModelUniqueName = originalModel->uniqueName();
                                        const QList<CT_OutAbstractResultModel*> outputResultModels = originalModel->step()->getOutResultModels();

                                        for(CT_OutAbstractResultModel* outputResultModel : outputResultModels) {
                                            if(outputResultModel->findModelInTree(originalModelUniqueName))
                                                return outputResultModel;
                                        }

                                        return NULL;
                                    }();

    if((mg == NULL) || mg->uniqueName().isEmpty())
        return NULL;

    return mg;
}

CT_VirtualAbstractStep* CT_ModelSaveRestoreHelper::searchStepInHierarchy(const QStringList &hierarchy, const CT_VirtualAbstractStep* rootStep)
{
    bool ok;
    int i = 0;

    CT_VirtualAbstractStep* pStep = const_cast<CT_VirtualAbstractStep*>(rootStep);
    QListIterator<QString> it(hierarchy);
    it.toBack();

    while(it.hasPrevious()) {
        const QString& stepInfo = it.previous();
        const QStringList stepNameAndStepPositionInParent = stepInfo.split("|");

        if(stepNameAndStepPositionInParent.size() != 2)
            return NULL;

        const int positionInParent = stepNameAndStepPositionInParent.last().toInt(&ok);

        if(!ok || ((i == 0) && (positionInParent != -1)))
            return NULL;

        if(positionInParent == -1)
        {
            if(stepNameAndStepPositionInParent.first() != pStep->getStepName())
                return NULL;
        }
        else
        {
            if((positionInParent < 0) || (positionInParent >= pStep->getStepChildList().size()))
                return NULL;

            pStep = pStep->getStepChildList().at(positionInParent);

            if(stepNameAndStepPositionInParent.first() != pStep->getStepName())
                return NULL;
        }

        ++i;
    }

    return pStep;
}

CT_OutAbstractResultModel* CT_ModelSaveRestoreHelper::searchResultModelInStep(const QString &resultModelUniqueName, const CT_VirtualAbstractStep *pStep)
{
    const QList<CT_OutAbstractResultModel*> outputResultModels = pStep->getOutResultModels();

    for(CT_OutAbstractResultModel* outputResultModel : outputResultModels) {
        if(outputResultModel->uniqueName() == resultModelUniqueName)
            return outputResultModel;
    }

    return NULL;
}
