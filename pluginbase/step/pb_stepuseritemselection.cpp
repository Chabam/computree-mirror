#include "pb_stepuseritemselection.h"

// Inclusion of in models
#include "ct_itemdrawable/model/inModel/ct_inzeroormoregroupmodel.h"
#include "ct_itemdrawable/model/inModel/ct_instdgroupmodel.h"
#include "ct_itemdrawable/model/inModel/ct_instdsingularitemmodel.h"

// Inclusion of out models
#include "ct_itemdrawable/model/outModel/ct_outstdgroupmodel.h"

// Inclusion of used ItemDrawable classes
#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

//Inclusion of actions
#include "actions/pb_actionselectitemdrawablegv.h"
#include "actions/pb_actionpickitemsinlist.h"

#include "ct_model/tools/ct_modelsearchhelper.h"

#include <QMessageBox>

// Alias for indexing in models

PB_StepUserItemSelection::PB_StepUserItemSelection() : SuperClass()
{
    m_doc = nullptr;
    m_removeGroupsWithoutItemResearched = false;
    m_removeParents = false;

    _mode = 0;

    setManual(true);
}

QString PB_StepUserItemSelection::description() const
{
    return tr("Séléction d'items");
}

CT_VirtualAbstractStep* PB_StepUserItemSelection::createNewInstance()
{
    return new PB_StepUserItemSelection();
}

//////////////////// PROTECTED METHODS //////////////////

void PB_StepUserItemSelection::declareInputModels(CT_StepInModelStructureManager& manager)
{
    CT_InResultModelGroupToCopy *resultInModel_R = createNewInResultModelForCopy(DEF_resultIn_R,
                                                                                 tr("Result"),
                                                                                 tr(""),
                                                                                 false);
    resultInModel_R->setZeroOrMoreRootGroup();
    resultInModel_R->addStdGroupModel("", DEF_groupIn_G);
    resultInModel_R->addStdItemModel(DEF_groupIn_G,
                                  DEF_itemIn_I,
                                  CT_AbstractSingularItemDrawable::staticGetType(),
                                  tr("Item"));
}

void PB_StepUserItemSelection::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    createNewOutResultModelToCopy(DEF_resultIn_R);
}

void PB_StepUserItemSelection::fillPostInputConfigurationDialog(CT_StepConfigurableDialog* postInputConfigDialog)
{
    // No parameter dialog for this step
    CT_StepConfigurableDialog *dialog = newStandardPostConfigurationDialog();

    dialog->addText("Mode de séléction des items", "", "");
    CT_ButtonGroup &bg_mode = dialog->addButtonGroup(_mode);

//    dialog->addExcludeValue("", "", tr("Par proximité des centres des items"), bg_mode, 1);
    dialog->addExcludeValue("", "", tr("Séléction classique (picking)"), bg_mode, 0);

//    dialog->addEmpty();
//    dialog->addBool("", "", tr("Supprimer les groupes ne contenant pas l'ItemDrawable recherché"), m_removeGroupsWithoutItemResearched);
//    dialog->addBool("", "", tr("Supprimer les groupes parents si l'ItemDrawable recherché n'existe plus dans la structure sous-jacente"), m_removeParents);
}

void PB_StepUserItemSelection::compute()
{
    m_doc = nullptr;
    m_status = 0;

    // ----------------------------------------------------------------------------
    // Get the group model corresponding to DEF_groupIn_G
    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup *resultOut_R = outResultList.first();
    CT_InAbstractGroupModel* groupInModel_G = (CT_InAbstractGroupModel*)PS_MODELS->searchModel(DEF_groupIn_G, resultOut_R, this);

    // ---------------------------
    // Gets OUT results and models
    m_itemDrawableToAdd.clear();

    QList<CT_StandardItemGroup*> groupsToRemove;

    CT_ResultGroupIterator itG(resultOut_R, groupInModel_G);

    // create a list of itemdrawable to add in the document
    while(itG.hasNext() && !isStopped())
    {
        const CT_StandardItemGroup *groupOut_G = itG.next();
        CT_AbstractSingularItemDrawable *itemOut_I = groupOut_G->firstItemByINModelName(this, DEF_itemIn_I);

        if (itemOut_I != nullptr)
            m_itemDrawableToAdd.insert(itemOut_I, (CT_StandardItemGroup*)groupOut_G);
        else if(m_removeGroupsWithoutItemResearched || m_removeParents)
            groupsToRemove.append((CT_StandardItemGroup*)groupOut_G);
    }

    // request the manual mode
    requestManualMode();

    // remove item selected from the list (to not remove them from the out result)
    QListIterator<CT_AbstractItemDrawable*> it(m_itemDrawableSelected);

    while(it.hasNext()
          && !isStopped())
        m_itemDrawableToAdd.remove(it.next());

    groupsToRemove.append(m_itemDrawableToAdd.values());

    // we remove the parent group of all ItemDrawable that must be deleted from the out result
    // and all groups that don't contains a ItemDrawable researched
    QListIterator<CT_StandardItemGroup*> itE(groupsToRemove);

    while(itE.hasNext())
    {
        CT_StandardItemGroup *group = itE.next();

        recursiveRemoveGroup(group->parentGroup(), group);
    }

    m_status = 1;
    requestManualMode();
}

void PB_StepUserItemSelection::recursiveRemoveGroup(CT_StandardItemGroup *parent, CT_StandardItemGroup *group) const
{
    if(parent != nullptr)
    {
        parent->removeGroup(group);

        if(m_removeParents && parent->isEmptyOfGroups())
            recursiveRemoveGroup(parent->parentGroup(), parent);
    }
    else
    {
        ((CT_ResultGroup*)group->result())->removeGroupSomethingInStructure(group);
    }
}

void PB_StepUserItemSelection::initManualMode()
{
    // create a new 3D document
    if(m_doc == nullptr)
        m_doc = getGuiContext()->documentManager()->new3DDocument();

    m_itemDrawableSelected.clear();
    m_doc->removeAllItemDrawable();

    if (_mode == 0)
    {
        // TODO add async with GuiManagerInterface
        QHashIterator<CT_AbstractItemDrawable*, CT_StandardItemGroup*> it(m_itemDrawableToAdd);

        while(it.hasNext())
            m_doc->addItemDrawable(*it.next().key());

        // set the action (a copy of the action is added at all graphics view, and the action passed in parameter is deleted)
        m_doc->setCurrentAction(new PB_ActionSelectItemDrawableGV());
    } else {

        QList<CT_AbstractItemDrawable*> keys = m_itemDrawableToAdd.keys();
        m_doc->setCurrentAction(new PB_ActionPickItemsInList(keys, &m_itemDrawableSelected));

    }

    QMessageBox::information(nullptr, tr("Mode manuel"), tr("Bienvenue dans le mode manuel de cette "
                                                         "étape de filtrage. Veuillez sélectionner les "
                                                         "éléments dans la vue graphique puis valider en cliquant "
                                                         "sur le pouce en haut de la fenêtre principale. Les éléments "
                                                         "sélectionnés seront gardés dans le résultat de sortie."), QMessageBox::Ok);
}

void PB_StepUserItemSelection::useManualMode(bool quit)
{
    if(m_status == 0)
    {
        if(quit)
        {
            if (_mode == 0)
            {
                m_itemDrawableSelected = m_doc->getSelectedItemDrawable();
            }
        }
    }
    else if(m_status == 1)
    {
        if(!quit)
        {
            m_doc = nullptr;

            quitManualMode();
        }
    }
}
