#include "pb_stepusereadertoloadfiles.h"

#include "ct_step/tools/ct_logmanageradapterforstep.h"

PB_StepUseReaderToLoadFiles::PB_StepUseReaderToLoadFiles() : SuperClass()
{
    _conditionnal = false;
}

QString PB_StepUseReaderToLoadFiles::description() const
{
    return tr("2- Charger des fichiers à l'aide de readers");
}

CT_VirtualAbstractStep* PB_StepUseReaderToLoadFiles::createNewInstance() const
{
    return new PB_StepUseReaderToLoadFiles();
}

void PB_StepUseReaderToLoadFiles::fillPreInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog)
{
    preInputpostInputConfigDialog->addBool(tr("Conditionner le chargement à un attribut booléen"), "", "", _conditionnal);
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
    manager.addResultCopy(m_hInResultGroupCopy);

    CT_ReaderOutModelStructureManager rManager = CT_ReaderOutModelStructureManager::createFromInHandle(manager, m_hInGroup);

    // TODO : how to add reader models ?
    //m_reader->declareOutputModels(rManager);
}

/*void PB_StepUseReaderToLoadFiles::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    m_readerAddingTools.clear();

    CT_OutResultModelGroupToCopyPossibilities *res = createNewOutResultModelToCopy(DEFin_res);

    m_readerAddingTools.addReaderResults(DEFin_group, DEFin_reader, res, m_readerAutoIndex);
}*/

void PB_StepUseReaderToLoadFiles::compute()
{
    auto groupIterator = m_hInGroup.iterate(m_hInResultGroupCopy);

    m_totalReaderProgress = groupIterator.count();
    m_currentReaderProgress = 0;

    if(m_totalReaderProgress > 0) {
        float readerStepProgress = 100.0/m_totalReaderProgress;

        for(CT_StandardItemGroup* group : groupIterator) {

            auto readerItemIterator = group->singularItemsWithInHandle(m_hInReaderItem);

            if(!readerItemIterator.isEmpty())
            {
                CT_ReaderItem* readerItem = *readerItemIterator.begin();

                bool load = true;

                if (_conditionnal)
                {
                    load = false;

                    auto conditionnalItemIterator = group->singularItemsWithInHandle(m_hInConditionnalItem);

                    if(!conditionnalItemIterator.isEmpty())
                    {
                        CT_AbstractSingularItemDrawable* conditionnalItem = *conditionnalItemIterator.begin();

                        auto attributeIterator = conditionnalItem->itemAttributeWithInHandle(m_hInConditionnalAttribute);

                        if(!attributeIterator.isEmpty())
                        {
                            auto attribute = *attributeIterator.begin();
                            load = attribute->toBool(conditionnalItem, nullptr);
                        }
                    }
                }

                if (load)
                {
                    CT_AbstractReader* reader = readerItem->getReader();

                    if(reader != nullptr) {
                        connect(this, SIGNAL(stopped()), reader, SLOT(cancel()), Qt::DirectConnection);
                        connect(reader, SIGNAL(progressChanged(int)), this, SLOT(readerProgressChanged(int)), Qt::DirectConnection);

                        if(reader->readFile(group))
                        {
                            STEP_LOG->addInfoMessage(tr("Chargement du fichier %1").arg(reader->filepath()));
                            //m_readerAddingTools.addReaderResults(outRes, group, reader, m_readerAutoIndex);
                        }

                        disconnect(this, SIGNAL(stopped()), reader, SLOT(cancel()));
                        disconnect(reader, SIGNAL(progressChanged(int)), this, SLOT(readerProgressChanged(int)));
                    }
                }
            }

            m_currentReaderProgress += readerStepProgress;

            if(isStopped())
                return;
        }
    }
}

void PB_StepUseReaderToLoadFiles::readerProgressChanged(int p)
{
    setProgress(m_currentReaderProgress + (p/m_totalReaderProgress));
}
