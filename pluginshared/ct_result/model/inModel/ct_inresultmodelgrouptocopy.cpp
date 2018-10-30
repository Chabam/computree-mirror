#include "ct_inresultmodelgrouptocopy.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibilitygroup.h"
#include "ct_model/outModel/abstract/ct_outabstractmodel.h"

#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibility.h"

CT_InResultModelGroupToCopy::CT_InResultModelGroupToCopy(const QString &uniqueName,
                                                         CT_InAbstractGroupModel *rootGroupModel,
                                                         const QString &displayableName,
                                                         const QString &description,
                                                         bool recursive) : CT_InResultModelGroup(uniqueName,
                                                                                                 rootGroupModel,
                                                                                                 displayableName,
                                                                                                 description,
                                                                                                 recursive)
{
}

QList<CT_OutResultModelGroupToCopyPossibility*> CT_InResultModelGroupToCopy::getOutResultModelGroupsSelectedToCopy() const
{
    QList<CT_OutResultModelGroupToCopyPossibility*> retList;

    QList<CT_InStdModelPossibility*> checked = getPossibilitiesSavedSelected();

    QListIterator<CT_InStdModelPossibility*> it(checked);

    while(it.hasNext())
    {
        CT_InStdModelPossibility *possibilityForSearch = it.next();

        // on copie la possibilité
        CT_InStdModelPossibility *possibility = possibilityForSearch->copy();
        // ainsi que son outModel. On fait ça car l'utilisateur va rajouter des choses
        // dans le outModel puis il peut décider qu'il veut changer ses modèles d'entrées et
        // recréer des modèles de sorties. Avant de recréer les modèles de sorties les anciens
        // tours seront supprimés de la mémoire ce qui va supprimer de la mémoire ses possibilités et les
        // nouveaux modèles ajoutés et SURTOUT remettre à zéro les CT_AutoRenameModels !
        possibility->setOutModel(*(possibility->outModel()->copy()));
        possibility->setAutoDeleteOutModel(true);

        CT_OutResultModelGroupToCopyPossibility *copy = new CT_OutResultModelGroupToCopyPossibility();
        // la possibilité sera supprimé de la mémoire lorsque la copy sera supprimé de la mémoire donc pas de problème
        copy->setInModelAndPossibility(this, possibility, possibilityForSearch);

        retList.append(copy);
    }

    return retList;
}

QList<const CT_OutAbstractResultModel *> CT_InResultModelGroupToCopy::getOutResultModelForSearchInModel() const
{
    QList<const CT_OutAbstractResultModel*> list;

    QList<CT_OutResultModelGroupToCopyPossibility*> pList = getOutResultModelGroupsSelectedToCopy();

    while(!pList.isEmpty())
    {
        CT_OutResultModelGroupToCopyPossibility *pToDelete = pList.takeFirst();

        list.append(pToDelete->outModelForSearch());

        delete pToDelete;
    }

    return list;
}

void CT_InResultModelGroupToCopy::saveSettings(SettingsWriterInterface &writer) const
{
    SuperClass::saveSettings(writer);

    writer.addParameter(this, "Type", staticMetaObject.className());
}

bool CT_InResultModelGroupToCopy::restoreSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restoreSettings(reader))
        return false;

    QVariant value;

    if(!reader.parameter(this, "Type", value) || (value.toString() != staticMetaObject.className()))
        return false;

    return true;
}

CT_InAbstractModel* CT_InResultModelGroupToCopy::copy(bool withPossibilities) const
{
    CT_InAbstractGroupModel *rootCpy = NULL;

    if(rootGroup() != NULL)
        rootCpy = (CT_InAbstractGroupModel*)rootGroup()->copy(false);

    CT_InResultModelGroupToCopy *cpy = new CT_InResultModelGroupToCopy(uniqueName(), rootCpy, displayableName(), description(), isRecursive());
    cpy->setStep(step());
    cpy->setOriginalModel(this);
    cpy->setMinimumNumberOfPossibilityThatMustBeSelectedForOneTurn(minimumNumberOfPossibilityThatMustBeSelectedForOneTurn());
    cpy->setMaximumNumberOfPossibilityThatCanBeSelectedForOneTurn(maximumNumberOfPossibilityThatCanBeSelectedForOneTurn());

    if(withPossibilities)
        CT_InAbstractModel::staticCopyPossibilitiesToModel(this, cpy);

    return cpy;
}
