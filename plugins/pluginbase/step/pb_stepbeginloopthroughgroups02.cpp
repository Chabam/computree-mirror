#include "pb_stepbeginloopthroughgroups02.h"

PB_StepBeginLoopThroughGroups02::PB_StepBeginLoopThroughGroups02() : SuperClass()
{
}

QString PB_StepBeginLoopThroughGroups02::description() const
{
    return tr("Boucle standard");
}

QString PB_StepBeginLoopThroughGroups02::detailledDescription() const
{
    return tr("A chaque tour de boucle, cette étape conserve uniquement un élément de la liste séléctionnée en entrée.<br>"
            "L'utilisateur doit choisir un attribut servant à nommer les tours, ce qui permet par exemple des exports adaptatifs.<br>"
            "Nécessite une étape \"Fin de boucle\" pour terminer la boucle. ");
}

CT_VirtualAbstractStep* PB_StepBeginLoopThroughGroups02::createNewInstance() const
{
    // cree une copie de cette etape
    return new PB_StepBeginLoopThroughGroups02();
}

void PB_StepBeginLoopThroughGroups02::declareInputModels(CT_StepInModelStructureManager& manager)
{
    manager.addResult(m_hInResultCopy);
    manager.setZeroOrMoreRootGroup(m_hInResultCopy, m_hInZeroOrMoreRootGroup);
    manager.addGroup(m_hInZeroOrMoreRootGroup, m_hInGroup, tr("Groupe"));
    manager.addItem(m_hInGroup, m_hInItem, tr("Item"));
    manager.addItemAttribute(m_hInItem, m_hInItemAttribute, QStringList() << CT_AbstractCategory::DATA_FILE_NAME << CT_AbstractCategory::DATA_VALUE, tr("Nom"));
}

void PB_StepBeginLoopThroughGroups02::fillPostInputConfigurationDialog(CT_StepConfigurableDialog*)
{
}

void PB_StepBeginLoopThroughGroups02::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    SuperClass::declareOutputModels(manager);

    manager.addResultCopy(m_hInResultCopy);
}

void PB_StepBeginLoopThroughGroups02::compute()
{
    SuperClass::compute();

    const int currentTurn = _counter->currentTurn();

    if(currentTurn == 1)
    {
        _ids.clear();

        for(const CT_AbstractSingularItemDrawable* item : m_hInItem.iterateInputs(m_hInResultCopy))
        {
            _ids.append(item); // one item = one turn
        }

        _counter->setNTurns(_ids.isEmpty() ? 1 : _ids.size());
    }

    if((currentTurn >= 0) && ((currentTurn - 1) < _ids.size()))
    {
        const CT_AbstractSingularItemDrawable* currentItem = _ids.at(currentTurn - 1);

        const CT_AbstractItemAttribute* att = currentItem->itemAttribute(m_hInItemAttribute);

        const QString turnName = (att != nullptr) ? att->toString(currentItem, nullptr) : QString("Turn%1").arg(currentTurn);

        _counter->setTurnName(turnName);

        addToLogCurrentTurnInformation();

        for(CT_StandardItemGroup* group : m_hInGroup.iterateOutputs(m_hInResultCopy))
        {
            if(!group->containsSingularItem(currentItem))
                group->removeFromParent(true);
        }
    }

    setProgress( 100 );
}
