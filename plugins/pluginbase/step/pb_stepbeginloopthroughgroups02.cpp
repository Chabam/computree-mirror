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
    return tr("Cette étape permet d'effectuer des traitements par lots.<br>"
            "Elle créée une boucle permettant de traiter successivement les différents éléments sélectionnés en entrée.<br>"
            "IMPORTANT : la boucle devra être fermée avec l'étape \"Fin de boucle\".<br><br>"
            "Le cas d'utilisation le plus classique est après l'étape \"Créer une liste de fichiers\", afin d'itérer sur les fichiers de la liste.");
}

QString PB_StepBeginLoopThroughGroups02::inputDescription() const
{
    return SuperClass::inputDescription() + tr("<br><br>L'item choisi définit sur quel liste d'éléments il faut itérer.<br>"
                                                "Pour cet item il faut choisir un attribut \"Nom\", qui permettra de nommer les tours de boucles.<br>"
                                                "Cela permet par exemple de créer dans la boucle des exports adaptatifs, dont les noms de fichiers de sortie seront constitués à partir du nom des items en entrée (nom du fichier en cours en général).");
}

QString PB_StepBeginLoopThroughGroups02::outputDescription() const
{
    return tr("Cette étape génère deux résultats :<br>"
                "<ul>"
                "<li>Une copie du résultat d'entrée, mais dans laquelle seul l'item de l'itération en cours est disponible,</li>"
                "<li>Un résultat Compteur, permettant la gestion de la boucle. Ce compteur est en particulier utilisé pour connaître le nom du tour courant.</li>"
                "</ul>");
}

QString PB_StepBeginLoopThroughGroups02::detailsDescription() const
{
    return tr("L'utilisation la plus habituelle de cette étape est de traiter successivement une liste de fichier."
                "Dans ce cas, la structure du script est la suivante :"
                "<ol>"
                "<li>Créer une liste de fichier (séléction de la liste des fichiers à parcourir)</li>"
                "<li>Boucle standard (début de la boucle)</li>"
                "<li>Charger les fichiers d'une liste (chargement du fichier correspondant au tour de boucle courant)</li>"
                "<li>... (étapes de traitement et d'export)</li>"
                "<li>Fin de boucle</li>"
                "</ol><br>"
                "Cependant cette étape est générique et peut donc être utilisée dans de nombreux autres cas.<br>"
                "Il est même possible d'emboiter plusieurs boucles, en prenant garde de bien gérer les correspondances de résultats compteurs entre chaque début et fin de boucle.");
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
