#include "ct_inresultmodelconfigurationmanager.h"

#include "ct_view/inModel/ctg_inresultmodelconfiguration.h"
#include "ct_result/model/inModel/tools/ct_inresultmodelmanager.h"
#include "ct_model/tools/ct_modelselectionhelper.h"

CT_InResultModelConfigurationManager::CT_InResultModelConfigurationManager(CT_InResultModelManager &inManager) : m_inputResultModelManager(inManager)
{
    m_inputModelsConfigurationDialog = NULL;
}

CT_InResultModelConfigurationManager::~CT_InResultModelConfigurationManager()
{
    delete m_inputModelsConfigurationDialog;
}

CT_InResultModelConfigurationManager::CreateDialogReturn CT_InResultModelConfigurationManager::createInResultModelConfigurationDialog()
{
    CT_InResultModelConfigurationManager::CreateDialogReturn returnVal = CT_InResultModelConfigurationManager::CreateSuccess;

    // si on a pas plusieurs possibilités il n'y a rien a configurer
    if(!checkIfMustCreateOrShowConfigurationDialog())
        returnVal = CT_InResultModelConfigurationManager::CreateNotNecessary;

    if(m_inputModelsConfigurationDialog == NULL)
    {
        m_inputModelsConfigurationDialog = new CTG_InResultModelConfiguration(NULL);
        m_inputModelsConfigurationDialog->setWindowTitle(m_inputModelsConfigurationDialog->windowTitle());
        m_inputModelsConfigurationDialog->setInResultModelManager(&m_inputResultModelManager);
        m_inputModelsConfigurationDialog->setWindowFlags(m_inputModelsConfigurationDialog->windowFlags() | Qt::WindowMaximizeButtonHint);
    }

    return returnVal;
}

CT_InResultModelConfigurationManager::ConfigureReturn CT_InResultModelConfigurationManager::configureInResultModel()
{
    if(checkIfMustCreateOrShowConfigurationDialog())
        return (m_inputModelsConfigurationDialog->exec() == QDialog::Accepted) ? CT_InResultModelConfigurationManager::ConfigureSuccess : CT_InResultModelConfigurationManager::ConfigureCanceled;

    m_inputResultModelManager.recursiveUnselectAllPossibilities();

    const QList<CT_InAbstractResultModel*>& inputResultModels = m_inputResultModelManager.models();

    for(CT_InAbstractResultModel* inputResultModel : inputResultModels) {
        CT_ModelSelectionHelper selectionHelper(inputResultModel);

        bool ok = selectionHelper.selectAllPossibilitiesByDefault();

        Q_ASSERT(ok);

        if(!ok)
            return CT_InResultModelConfigurationManager::ConfigureError;
    }

    return CT_InResultModelConfigurationManager::ConfigureSuccess;
}

void CT_InResultModelConfigurationManager::showReadOnlyInResultModel()
{
    if(m_inputModelsConfigurationDialog != NULL)
    {
        m_inputModelsConfigurationDialog->setReadOnly(true);
        m_inputModelsConfigurationDialog->exec();
    }
}

void CT_InResultModelConfigurationManager::saveSettings(SettingsWriterInterface &writer) const
{
    m_inputResultModelManager.saveSettings(writer);
}

bool CT_InResultModelConfigurationManager::restoreSettings(SettingsReaderInterface &reader)
{
    m_inputResultModelManager.recursiveUnselectAllPossibilities();

    delete m_inputModelsConfigurationDialog;
    m_inputModelsConfigurationDialog = NULL;

    // on crée la fenêtre de config si il y en a besoin
    createInResultModelConfigurationDialog();

    if(!m_inputResultModelManager.restoreSettings(reader)) {
        delete m_inputModelsConfigurationDialog;
        m_inputModelsConfigurationDialog = NULL;

        return false;
    }

    return true;
}

bool CT_InResultModelConfigurationManager::checkIfMustCreateOrShowConfigurationDialog() const
{
    const QList<CT_InAbstractResultModel*>& inputResultModels = m_inputResultModelManager.models();

    for(CT_InAbstractResultModel* inputResultModel : inputResultModels) {
        CT_ModelSelectionHelper selectionHelper(inputResultModel);

        // we check if it can select all possibilities by default
        if(!selectionHelper.canSelectAllPossibilitiesByDefault())
            return true; // if not we return true to inform that the dialog must be created or displayed
    }

    return false;
}

void CT_InResultModelConfigurationManager::aboutToBeDeleted()
{
    delete m_inputModelsConfigurationDialog;
    m_inputModelsConfigurationDialog = NULL;
}
