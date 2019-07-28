#include "ct_inresultmodelconfigurationmanager.h"

#include "ct_model/inModel/abstract/ct_inabstractresultmodel.h"
#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"
#include "ct_model/inModel/tools/ct_modelselectionhelper.h"

CT_InResultModelConfigurationManager::CT_InResultModelConfigurationManager(CT_InModelStructureManager& inManager) : m_inModelsStructureManager(inManager)
{
    m_inputModelsConfigurationDialog = nullptr;
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

    if(m_inputModelsConfigurationDialog == nullptr)
    {
        m_inputModelsConfigurationDialog = new CTG_InResultModelConfiguration(nullptr);
        m_inputModelsConfigurationDialog->setWindowTitle(m_inputModelsConfigurationDialog->windowTitle());
        m_inputModelsConfigurationDialog->setInResultModelManager(&m_inModelsStructureManager);
        m_inputModelsConfigurationDialog->setWindowFlags(m_inputModelsConfigurationDialog->windowFlags() | Qt::WindowMaximizeButtonHint);
    }

    return returnVal;
}

CT_InResultModelConfigurationManager::ConfigureReturn CT_InResultModelConfigurationManager::configureInResultModel()
{
    if(checkIfMustCreateOrShowConfigurationDialog()) {
        Q_ASSERT(m_inputModelsConfigurationDialog != nullptr);
        return (m_inputModelsConfigurationDialog->exec() == QDialog::Accepted) ? CT_InResultModelConfigurationManager::ConfigureSuccess : CT_InResultModelConfigurationManager::ConfigureCanceled;
    }

    // select all possibilities by default
    const bool ok = m_inModelsStructureManager.visitResults([](const CT_InAbstractResultModel* resultModel) -> bool {

        const_cast<CT_InAbstractResultModel*>(resultModel)->recursiveVisitPossibilities([](const CT_InAbstractModel*, const CT_InStdModelPossibility* possibility) -> bool {
            const_cast<CT_InStdModelPossibility*>(possibility)->setSelected(false);
            return true;
        });

        return CT_ModelSelectionHelper(resultModel).recursiveSelectAllPossibilitiesByDefault();
    });

    return ok ? CT_InResultModelConfigurationManager::ConfigureSuccess : CT_InResultModelConfigurationManager::ConfigureError;
}

void CT_InResultModelConfigurationManager::showReadOnlyInResultModel()
{
    if(m_inputModelsConfigurationDialog != nullptr)
    {
        m_inputModelsConfigurationDialog->setReadOnly(true);
        m_inputModelsConfigurationDialog->exec();
    }
}

void CT_InResultModelConfigurationManager::saveSettings(SettingsWriterInterface& writer) const
{
    m_inModelsStructureManager.visitResults([&writer](const CT_InAbstractResultModel* resultModel) -> bool {
        resultModel->saveSettings(writer);
        return true;
    });
}

bool CT_InResultModelConfigurationManager::restoreSettings(SettingsReaderInterface& reader)
{
    // unselect all possibilities
    m_inModelsStructureManager.visitResults([](const CT_InAbstractResultModel* resultModel) -> bool {

        const_cast<CT_InAbstractResultModel*>(resultModel)->recursiveVisitPossibilities([](const CT_InAbstractModel*, const CT_InStdModelPossibility* possibility) -> bool {
            const_cast<CT_InStdModelPossibility*>(possibility)->setSelected(false);
            return true;
        });

        return true;
    });

    delete m_inputModelsConfigurationDialog;
    m_inputModelsConfigurationDialog = nullptr;

    createInResultModelConfigurationDialog();

    const bool restoreOK = m_inModelsStructureManager.visitResults([&reader](const CT_InAbstractResultModel* resultModel) -> bool {
        return const_cast<CT_InAbstractResultModel*>(resultModel)->restoreSettings(reader);
    });

    if(restoreOK)
        return true;

    delete m_inputModelsConfigurationDialog;
    m_inputModelsConfigurationDialog = nullptr;

    return false;
}

bool CT_InResultModelConfigurationManager::checkIfMustCreateOrShowConfigurationDialog() const
{
    return !m_inModelsStructureManager.visitResults([](const CT_InAbstractResultModel* resultModel) -> bool {
        // if there is at least one possibility already checked we must show the configuration dialog
        if(resultModel->isAtLeastOnePossibilitySelected())
            return false;

        // we check if it can select all possibilities by default
        return CT_ModelSelectionHelper(resultModel).recursiveCanSelectAllPossibilitiesByDefault();
    });
}

void CT_InResultModelConfigurationManager::aboutToBeDeleted()
{
    delete m_inputModelsConfigurationDialog;
    m_inputModelsConfigurationDialog = nullptr;
}
