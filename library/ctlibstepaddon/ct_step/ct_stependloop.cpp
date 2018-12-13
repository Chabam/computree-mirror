#include "ct_stependloop.h"

#include "ct_stepbeginloop.h"

CT_StepEndLoop::CT_StepEndLoop() : CT_AbstractStep()
{
    m_mustRecheckTree = false;
    setDebuggable(true);
}

QString CT_StepEndLoop::description() const
{
    return tr("2- Termine une boucle dans le script");
}

QString CT_StepEndLoop::detailledDescription() const
{
    return tr("Nécessite une CT_StepBeginLoop auparavant pour initier la boucle");
}

CT_VirtualAbstractStep* CT_StepEndLoop::createNewInstance() const
{
    // cree une copie de cette etape
    return new CT_StepEndLoop();
}

bool CT_StepEndLoop::mustRecheckTree() const
{
    return m_mustRecheckTree;
}

void CT_StepEndLoop::declareInputModels(CT_StepInModelStructureManager& manager)
{
    manager.addResult(m_hInResultCopy);
    manager.setRootGroup(m_hInResultCopy, m_hInRootGroup);
    manager.addItem(m_hInRootGroup, m_hInLoopCounter);
}

void CT_StepEndLoop::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    manager.addResultCopy(m_hInResultCopy);
}

//////////////////// PROTECTED //////////////////

void CT_StepEndLoop::compute()
{    
    m_mustRecheckTree = false;

    for(CT_LoopCounter* counter : m_hInLoopCounter.iterateOutputs(m_hInResultCopy)) {

        STEP_LOG->addInfoMessage(tr("Fin de boucle, tour %1 sur %2").arg(counter->currentTurn()).arg(counter->nTurns()));

        // Use the debug mode at step loop scale (and not at step scale as usual)
        if (counter->currentTurn() % currentNumberOfBreakPointToJump() == 0)
            waitForAckIfInDebugMode();

        if (counter->hasNextTurn())
        {
            m_mustRecheckTree = true;
            counter->beginNextTurn();
        } else {
            counter->setCurrentTurn(1);
            counter->beginLoopStep()->clearCounter();
        }

        return;
    }
}
