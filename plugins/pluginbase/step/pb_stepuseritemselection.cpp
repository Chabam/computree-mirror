#include "pb_stepuseritemselection.h"

#include "documentinterface.h"

//Inclusion of actions
#include "ctlibstdactions/action/ct_actionselectitemdrawablegv.h"
#include "ctlibstdactions/action/pb_actionpickitemsinlist.h"

#include <QMessageBox>

// Alias for indexing in models

PB_StepUserItemSelection::PB_StepUserItemSelection() : SuperClass()
{
    m_doc = nullptr;
    setManual(true);
}

QString PB_StepUserItemSelection::description() const
{
    return tr("Séléction interactive d'items");
}

QString PB_StepUserItemSelection::detailledDescription() const
{
    return tr("Cette étape permet à l'utilisateur de faire une séléction manuelle interactive des items à conserver. ");
}

QString PB_StepUserItemSelection::inputDescription() const
{
    return SuperClass::inputDescription() + tr("<br><br>Il faut simplement choisir les items parmi lesquels faire la séléction interactive. ");
}

QString PB_StepUserItemSelection::outputDescription() const
{
    return tr("Cette étape génère une copie du résultat d'entrée, mais où seuls les items sélectionnés sont conservés. Le reste des données n'est pas impacté. ");
}

CT_VirtualAbstractStep* PB_StepUserItemSelection::createNewInstance() const
{
    return new PB_StepUserItemSelection();
}

//////////////////// PROTECTED METHODS //////////////////

void PB_StepUserItemSelection::declareInputModels(CT_StepInModelStructureManager& manager)
{
    manager.addResult(mInResultCpy);
    manager.setZeroOrMoreRootGroup(mInResultCpy, mInRootGroup);
    manager.addGroup(mInRootGroup, mInGroup);
    manager.addItem(mInGroup, mInItem, tr("Item"));
}

void PB_StepUserItemSelection::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    manager.addResultCopy(mInResultCpy);
}

void PB_StepUserItemSelection::compute()
{
    m_doc = nullptr;
    m_status = 0;
    m_itemDrawableToAdd.clear();

    QList<CT_StandardItemGroup*> groupsToRemove;

    // create a list of itemdrawable to add in the document
    auto itR = mInItem.iterateOutputs(mInResultCpy);
    auto begin = itR.begin();
    const auto end = itR.end();

    while(begin != end)
    {
        CT_StandardItemGroup* groupOut_G = begin.currentParent();
        CT_AbstractGeometricalItem* itemOut_I = (*begin);

        if (itemOut_I != nullptr)
            m_itemDrawableToAdd.insert(itemOut_I, groupOut_G);

        ++begin;
    }

    // request the manual mode
    requestManualMode();

    // remove item selected from the list (to not remove them from the out result)
    QListIterator<CT_AbstractGeometricalItem*> it(m_itemDrawableSelected);

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
        group->removeFromParent(false);
    }

    m_status = 1;
    requestManualMode();
}

void PB_StepUserItemSelection::initManualMode()
{
    // create a new 3D document
    if(m_doc == nullptr)
        m_doc = guiContext()->documentManager()->new3DDocument();

    m_itemDrawableSelected.clear();
    m_doc->removeAllItemDrawable();

    // TODO add async with GuiManagerInterface
    QHashIterator<CT_AbstractGeometricalItem*, CT_StandardItemGroup*> it(m_itemDrawableToAdd);
    m_doc->beginAddMultipleItemDrawable();

    while(it.hasNext())
        m_doc->addItemDrawable(*it.next().key());

    m_doc->endAddMultipleItemDrawable();

    // set the action (a copy of the action is added at all graphics view, and the action passed in parameter is deleted)
    m_doc->setCurrentAction(new CT_ActionSelectItemDrawableGV());

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
            const auto l = m_doc->getSelectedItemDrawable();
            m_itemDrawableSelected.reserve(l.size());

            for(auto item : l)
            {
                m_itemDrawableSelected.append(static_cast<CT_AbstractGeometricalItem*>(item));
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
