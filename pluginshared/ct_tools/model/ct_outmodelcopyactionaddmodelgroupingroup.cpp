#include "ct_outmodelcopyactionaddmodelgroupingroup.h"

#include "ct_step/abstract/ct_virtualabstractstep.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibility.h"

#include "ct_itemdrawable/model/inModel/ct_instdgroupmodel.h"
#include "ct_tools/model/ct_generateoutresultmodelname.h"

#include "ct_result/model/outModel/tools/ct_iteratorgroupsmodelforresultmodelgroup.h"

#include <assert.h>

CT_OutModelCopyActionAddModelGroupInGroup::CT_OutModelCopyActionAddModelGroupInGroup(const QString &inGroupModelNameToSearch,
                                                                                     DEF_CT_AbstractGroupModelOut *groupModelToAdd,
                                                                                     CT_AutoRenameModels &autoRenameGroupModelToAdd) : CT_AbstractOutModelCopyAction()
{
    _inGroupModelNameToSearch = inGroupModelNameToSearch;
    _autoRenameGroupModelToSearch = NULL;

    Q_ASSERT(groupModelToAdd != NULL);
    Q_ASSERT(groupModelToAdd->isEmpty());

    _groupModelToAdd = groupModelToAdd;
    _autoRenameGroupModelToAdd = &autoRenameGroupModelToAdd;
}

CT_OutModelCopyActionAddModelGroupInGroup::CT_OutModelCopyActionAddModelGroupInGroup(const CT_AutoRenameModels &otherNewGroupModelNameToSearch,
                                                                                     DEF_CT_AbstractGroupModelOut *groupModelToAdd,
                                                                                     CT_AutoRenameModels &autoRenameGroupModelToAdd)
{
    _autoRenameGroupModelToSearch = (CT_AutoRenameModels*)&otherNewGroupModelNameToSearch;

    Q_ASSERT(groupModelToAdd != NULL);
    Q_ASSERT(groupModelToAdd->isEmpty());

    _groupModelToAdd = groupModelToAdd;
    _autoRenameGroupModelToAdd = &autoRenameGroupModelToAdd;
}

CT_OutModelCopyActionAddModelGroupInGroup::~CT_OutModelCopyActionAddModelGroupInGroup()
{
    delete _groupModelToAdd;
}

bool CT_OutModelCopyActionAddModelGroupInGroup::execute(const CT_VirtualAbstractStep *step, CT_OutResultModelGroupToCopyPossibility *modelCopy)
{
    // on récupère le résultat modèle de sortie à copier/modifier
    CT_OutAbstractResultModel *outResModel = modelCopy->outModelForModification();

    if(_autoRenameGroupModelToSearch == NULL)
    {
        // on récupère le groupe défini dans "createInResultModelListProtected()"
        // pour la recherche
        DEF_CT_AbstractGroupModelIn *inGroupModel = dynamic_cast<DEF_CT_AbstractGroupModelIn*>(step->getInModelForResearch(modelCopy->outModelForSearch(), _inGroupModelNameToSearch));

        Q_ASSERT(inGroupModel != NULL);

        // on recherche tous les groupes du type qui ont été sélectionnés
        // par l'utilisateur

        CT_IteratorGroupsModelForResultModelGroup it(outResModel);

        if(it.recursiveBeginIterateGroups(inGroupModel))
        {
            DEF_CT_AbstractGroupModelOut *outGroupModel;

            while((outGroupModel = it.recursiveNextGroup()) != NULL)
            {
                // on passe un objet permettant de renommer le groupe au cas où le nom
                // du modèle existe déjà (obligatoire dans ce cas puisqu'on ne connait pas les noms
                // des modèles du résultat que l'on copie et il risque d'y avoir conflit)
                if(!outGroupModel->addGroup(_groupModelToAdd->copyGroup(), *_autoRenameGroupModelToAdd))
                    return false;
            }
        }
    }
    else
    {
        // on récupère le groupe déjà ajouter au résultat modèle
        QList<CT_AbstractModel*> models = outResModel->recursiveGetAllModels();
        QListIterator<CT_AbstractModel*> it(models);

        while(it.hasNext())
        {
            CT_AbstractModel *pModel = it.next();

            if(pModel->uniqueName() == _autoRenameGroupModelToSearch->completeName())
            {
                DEF_CT_AbstractGroupModelOut *outGroupModel = dynamic_cast<DEF_CT_AbstractGroupModelOut*>(pModel);

                Q_ASSERT(outGroupModel != NULL);

                if(!outGroupModel->addGroup(_groupModelToAdd->copyGroup(), *_autoRenameGroupModelToAdd))
                    return false;
            }
        }
    }

    return true;
}
