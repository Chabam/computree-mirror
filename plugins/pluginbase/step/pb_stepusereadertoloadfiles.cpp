#include "pb_stepusereadertoloadfiles.h"

#include "ct_step/tools/ct_logmanageradapterforstep.h"
#include "ct_reader/abstract/ct_abstractreader.h"


PB_StepUseReaderToLoadFiles::PB_StepUseReaderToLoadFiles() : SuperClass()
{
    m_readerPrototype = nullptr;
}

QString PB_StepUseReaderToLoadFiles::description() const
{
    return tr("Charger les fichiers d'une liste");
}

QString PB_StepUseReaderToLoadFiles::detailledDescription() const
{
    return tr("Cette étape permet de charger effectivement un fichier, dont l'entête a préalablement été créée, par exemple avec l'étape \"Créer une liste de fichiers\".");
}

QString PB_StepUseReaderToLoadFiles::inputDescription() const
{
    return SuperClass::inputDescription() + tr("<br><br>L'item choisi définit sur quel fichier il faut charger. Ces fichiers doivent avoir été préalable listés. C'est l'objet \"reader\" qui est utilisé pour réaliser le chargement.");
}

QString PB_StepUseReaderToLoadFiles::outputDescription() const
{
    return tr("Cette étape charge les données du fichier. La structure de données dépend du format du fichier.");
}

QString PB_StepUseReaderToLoadFiles::detailsDescription() const
{
    return tr("L'utilisation la plus habituelle de charger un fichier par tour de boucle, à partie d'une liste de fichiers préalablement crée. "
                "Dans ce cas, la structure du script est la suivante :"
                "<ol>"
                "<li>Créer une liste de fichier (séléction de la liste des fichiers à parcourir)</li>"
                "<li>Boucle standard (début de la boucle)</li>"
                "<li><strong>Charger les fichiers d'une liste</strong> (chargement du fichier correspondant au tour de boucle courant)</li>"
                "<li>... (étapes de traitement et d'export)</li>"
                "<li>Fin de boucle</li>"
                "</ol><br>");
}


CT_VirtualAbstractStep* PB_StepUseReaderToLoadFiles::createNewInstance() const
{
    return new PB_StepUseReaderToLoadFiles();
}

void PB_StepUseReaderToLoadFiles::declareInputModels(CT_StepInModelStructureManager& manager)
{
    manager.addResult(m_hInResultGroupCopy, tr("Fichiers"));
    manager.setZeroOrMoreRootGroup(m_hInResultGroupCopy, m_hInRootGroup);
    manager.addGroup(m_hInRootGroup, m_hInGroup, tr("Fichier"));
    manager.addItem(m_hInGroup, m_hInReaderItem, tr("Entête"));
}

void PB_StepUseReaderToLoadFiles::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    m_readerPrototype = nullptr;

    manager.addResultCopy(m_hInResultGroupCopy);

    CT_OutAbstractModel* outReaderItemModel = m_hInReaderItem.outModelSelected(m_hInResultGroupCopy);

    if(outReaderItemModel != nullptr)
    {
        CT_FileHeader* readerItemPrototype = dynamic_cast<CT_FileHeader*>(outReaderItemModel->prototype());

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
            const CT_FileHeader* readerItem = group->singularItem(m_hInReaderItem);

            if(readerItem != nullptr)
            {
                if(m_readerPrototype != nullptr)
                {
                    if(m_readerPrototype->setFilePath(readerItem->filePath()))
                    {
                        if(m_readerPrototype->readFile(group))
                            STEP_LOG->addInfoMessage(tr("Chargement du fichier %1").arg(m_readerPrototype->filepath()));
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
