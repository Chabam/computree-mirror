#include "ct_stepbeginloop.h"

#include "ct_step/tools/ct_logmanageradapterforstep.h"

#include <limits>

CT_StepBeginLoop::CT_StepBeginLoop() : SuperClass()
{
    m_nTurns = 1;
    _counter = QSharedPointer<CT_Counter>(new CT_Counter(m_nTurns));
}

QString CT_StepBeginLoop::description() const
{
    return tr("Démarre une boucle dans le script");
}

QString CT_StepBeginLoop::detailledDescription() const
{
    return tr("Nécessite une CT_StepEndLoop pour terminer la boucle");
}

CT_VirtualAbstractStep* CT_StepBeginLoop::createNewInstance() const
{
    // cree une copie de cette etape
    return new CT_StepBeginLoop();
}

bool CT_StepBeginLoop::isSettingsModified() const
{
    if (_counter.isNull())
        return SuperClass::isSettingsModified();

    return !_counter->isFinished();
}

void CT_StepBeginLoop::clearCounter()
{
    _counter.clear();
}

void CT_StepBeginLoop::declareInputModels(CT_StepInModelStructureManager& manager)
{
    manager.setNotNeedInputResult();
}

void CT_StepBeginLoop::fillPostInputConfigurationDialog(CT_StepConfigurableDialog* configDialog)
{
    configDialog->addInt(tr("Nombre d'itérations"), "", 1, std::numeric_limits<int>::max(), m_nTurns);
    _counter.clear();
}

void CT_StepBeginLoop::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    manager.addResult(m_hOutResult);
    manager.setRootGroup(m_hOutResult, m_hOutRootGroup);
    manager.addItem(m_hOutRootGroup, m_hOutLoopCounter);
}

void CT_StepBeginLoop::compute()
{
    for(CT_ResultGroup* result : m_hOutResult.iterate()) {
        CT_StandardItemGroup* rootGroup = m_hOutRootGroup.createInstance();
        result->addRootGroupWithOutHandle(m_hOutRootGroup, rootGroup);

        _counter->setTurnName(QString("Turn%1").arg(_counter->currentTurn()));

        rootGroup->addSingularItemWithOutHandle(m_hOutLoopCounter, m_hOutLoopCounter.createInstance(_counter, this));
    }
}

QSharedPointer<CT_Counter> CT_StepBeginLoop::counter() const
{
    return _counter;
}

void CT_StepBeginLoop::addToLogCurrentTurnInformation()
{
    STEP_LOG->addInfoMessage("________________________________________");
    STEP_LOG->addInfoMessage(QString(tr("Début de boucle, tour %1 sur %2")).arg(_counter->currentTurn()).arg(_counter->nTurns()));
    STEP_LOG->addInfoMessage(_counter->turnName());
}

