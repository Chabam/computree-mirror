#include "pb_stepusereadertoloadfiles.h"

#include "ct_step/tools/ct_logmanageradapterforstep.h"

PB_StepUseReaderToLoadFiles::PB_StepUseReaderToLoadFiles() : SuperClass()
{
    _conditionnal = false;
    m_readerPrototype = nullptr;
}

QString PB_StepUseReaderToLoadFiles::description() const
{
    return tr("Charger les fichiers d'une liste");
}

CT_VirtualAbstractStep* PB_StepUseReaderToLoadFiles::createNewInstance() const
{
    return new PB_StepUseReaderToLoadFiles();
}

void PB_StepUseReaderToLoadFiles::fillPreInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog)
{
    preInputConfigDialog->addBool(tr("Conditionner le chargement à un attribut booléen"), "", "", _conditionnal);
}

void PB_StepUseReaderToLoadFiles::declareInputModels(CT_StepInModelStructureManager& manager)
{
    manager.addResult(m_hInResultGroupCopy, tr("Fichiers"));
    manager.setZeroOrMoreRootGroup(m_hInResultGroupCopy, m_hInRootGroup);
    manager.addGroup(m_hInRootGroup, m_hInGroup, tr("Fichier"));
    manager.addItem(m_hInGroup, m_hInReaderItem, tr("Reader"));

    if (_conditionnal)
    {
        manager.addItem(m_hInGroup, m_hInConditionnalItem, tr("Item conditionnant le chargement"));
        manager.addItemAttribute(m_hInConditionnalItem, m_hInConditionnalAttribute, CT_AbstractCategory::DATA_VALUE, tr("Attribut conditionnel"));
    }
}

void PB_StepUseReaderToLoadFiles::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    m_readerPrototype = nullptr;

    manager.addResultCopy(m_hInResultGroupCopy);

    CT_OutAbstractModel* outReaderItemModel = m_hInReaderItem.outModelSelected(m_hInResultGroupCopy);

    if(outReaderItemModel != nullptr)
    {
        CT_ReaderItem* readerItemPrototype = dynamic_cast<CT_ReaderItem*>(outReaderItemModel->prototype());

        if((readerItemPrototype != nullptr) && (readerItemPrototype->reader() != nullptr)) {
            m_readerPrototype = readerItemPrototype->reader();
            m_readerPrototype->declareOutputModelsInGroup(manager, m_hInGroup);
        }
    }
}

void PB_StepUseReaderToLoadFiles::compute()
{
    auto groupIterator = m_hInGroup.iterateOutputs(m_hInResultGroupCopy);

    m_totalReaderProgress = groupIterator.count();
    m_currentReaderProgress = 0;

    if(m_totalReaderProgress > 0)
    {
        if(m_readerPrototype != nullptr)
        {
            connect(this, SIGNAL(stopped()), m_readerPrototype, SLOT(cancel()), Qt::DirectConnection);
            connect(m_readerPrototype, SIGNAL(progressChanged(int)), this, SLOT(readerProgressChanged(int)), Qt::DirectConnection);
        }

        float readerStepProgress = 100.0f/m_totalReaderProgress;

        for(CT_StandardItemGroup* group : groupIterator)
        {
            const CT_ReaderItem* readerItem = group->singularItem(m_hInReaderItem);

            if(readerItem != nullptr)
            {
                bool load = true;

                if (_conditionnal)
                {
                    load = false;

                    auto conditionnalItem = group->singularItem(m_hInConditionnalItem);

                    if(conditionnalItem != nullptr)
                    {
                        auto attribute = conditionnalItem->itemAttribute(m_hInConditionnalAttribute);

                        if(attribute != nullptr)
                            load = attribute->toBool(conditionnalItem, nullptr);
                    }
                }

                if (load)
                {
                    if(m_readerPrototype != nullptr)
                    {
                        if(m_readerPrototype->setFilePath(readerItem->readerFilePath()))
                        {
                            if(m_readerPrototype->readFile(group))
                                STEP_LOG->addInfoMessage(tr("Chargement du fichier %1").arg(m_readerPrototype->filepath()));
                        }
                    }
                }
            }

            m_currentReaderProgress += readerStepProgress;

            if(isStopped())
                break;
        }        

        if(m_readerPrototype != nullptr)
        {
            disconnect(this, SIGNAL(stopped()), m_readerPrototype, SLOT(cancel()));
            disconnect(m_readerPrototype, SIGNAL(progressChanged(int)), this, SLOT(readerProgressChanged(int)));
        }
    }
}

void PB_StepUseReaderToLoadFiles::readerProgressChanged(int p)
{
    setProgress(m_currentReaderProgress + (p/m_totalReaderProgress));
}
