#include "cdm_citationinfo.h"
#include "ct_abstractstepplugin.h"
#include "ct_step/abstract/ct_virtualabstractstep.h"

#include "ct_risformat/ct_parseris.h"

CDM_CitationInfo::CDM_CitationInfo(CDM_StepManager *stepManager, CDM_PluginManager *pluginManager) : QObject(nullptr)
{
    _stepManager = stepManager;
    _pluginManager = pluginManager;

    QList<CT_VirtualAbstractStep *>& thisStepList = _stepList;
    QHash<const CT_VirtualAbstractStep*, const CT_VirtualAbstractStep*> parents;

    const QList<CT_VirtualAbstractStep*> rootList = stepManager->getStepRootList();

    for(CT_VirtualAbstractStep* currentStep : rootList)
    {
        thisStepList.append(currentStep);

        currentStep->recursiveVisitChildrens([&thisStepList, &parents](const CT_VirtualAbstractStep* step, const CT_VirtualAbstractStep* child) -> bool {
            thisStepList.append(const_cast<CT_VirtualAbstractStep*>(child));
            parents.insert(child, step);
            return true;
        });
    }

    // Compute step indentation and plugin list
    for (int i = 0 ; i < _stepList.size() ; i++)
    {
        const CT_VirtualAbstractStep *step = _stepList.at(i);

        // Compute plugin list
        CT_AbstractStepPlugin* plugin = step->pluginStaticCastT<>();
        QString pluginName = _pluginManager->getPluginName(_pluginManager->getPluginIndex(plugin));
        QString pluginOfficialName = plugin->getPluginOfficialName();

        if (pluginName.left(5) == "plug_")
        {
            pluginName.remove(0, 5);
        }
        if (pluginOfficialName != "")
        {
            pluginName = pluginOfficialName;
        }

        if (!_pluginList.contains(pluginName))
        {
            _pluginList.insert(pluginName, plugin);
        }

        // Compute indentation
        int indent = -1;
        while (step != nullptr)
        {
            ++indent;
            step = parents.value(step, nullptr);
        }
        _stepIndent.append(indent);
    }

    // Create Core plugin list
    _corePluginList.append("Base");
    _corePluginList.append("Toolkit");
    _corePluginList.append("Generate");

}

QString CDM_CitationInfo::getScriptStepList()
{
    QString result;
    for (int i = 0 ; i < _stepList.size() ; i++)
    {
        const CT_VirtualAbstractStep *step = _stepList.at(i);

        CT_AbstractStepPlugin* plugin = step->pluginStaticCastT<>();
        QString pluginName = _pluginManager->getPluginName(_pluginManager->getPluginIndex(plugin));
        QString pluginOfficialName = plugin->getPluginOfficialName();

        if (pluginName.left(5) == "plug_")
        {
            pluginName.remove(0, 5);
        }
        if (pluginOfficialName != "")
        {
            pluginName = pluginOfficialName;
        }

        // create indentation
        int indent = _stepIndent.at(i);
        QString spaces = "&nbsp;&nbsp;&nbsp;&nbsp;";
        for (int j = 0 ; j < indent ; j++) {spaces.append("&nbsp;&nbsp;&nbsp;&nbsp;");}

        result.append(spaces + "<strong>" + step->description() + "</strong> <em>(plugin " + pluginName + ")</em><br><br>\n");
    }

    return result;
}

QString CDM_CitationInfo::getScriptStepListParameters()
{
    QString result;
    for (int i = 0 ; i < _stepList.size() ; i++)
    {
        const CT_VirtualAbstractStep *step = _stepList.at(i);

        CT_AbstractStepPlugin* plugin = step->pluginStaticCastT<>();
        QString pluginName = _pluginManager->getPluginName(_pluginManager->getPluginIndex(plugin));
        QString pluginOfficialName = plugin->getPluginOfficialName();

        if (pluginName.left(5) == "plug_")
        {
            pluginName.remove(0, 5);
        }
        if (pluginOfficialName != "")
        {
            pluginName = pluginOfficialName;
        }

        result.append("<h2>" + step->description() + " <em>(plugin " + pluginName + ")</em></h2>\n");
        QString parametersString = const_cast<CT_VirtualAbstractStep*>(step)->parametersDescription(false);

        if (parametersString.isEmpty())
        {
            result.append("<div style='margin-left:50px'>\n<em>" + tr("Pas de paramètres pour cette étape.") + "</em>\n</div>\n");
        } else {
            result.append("<div style='margin-left:50px'>\n" + parametersString + "\n</div>\n");
        }
    }

    return result;
}


// TESTTUTO
QString CDM_CitationInfo::getScriptStepListInputConfig()
{
    QString result;
    for (int i = 0 ; i < _stepList.size() ; i++)
    {
        const CT_VirtualAbstractStep *step = _stepList.at(i);

        QList<const CT_InAbstractResultModel*> resultInModels;

        step->visitInResultModels([this, step, &resultInModels](const CT_InAbstractResultModel* child) -> bool {
            resultInModels.append(child);
            qDebug();
            qDebug() << step->displayableName();

            child->visitSelectedPossibilities([this](const CT_InAbstractModel* inModel, const CT_InStdModelPossibility* possibility) -> bool {
                Q_UNUSED(inModel);
                const CT_InStdResultModelPossibility* resultPossibility = dynamic_cast<const CT_InStdResultModelPossibility*>(possibility);


                if (resultPossibility != nullptr)
                {
                    CT_OutAbstractResultModelGroup* outResModel = dynamic_cast<CT_OutAbstractResultModelGroup*>(resultPossibility->outModel());
                    CT_InResultModelGroup* inResModel = dynamic_cast<CT_InResultModelGroup*>(resultPossibility->inResultModel());

                    QList<const CT_OutAbstractModel*> allPoss;
                    QMultiMap<const CT_OutAbstractModel*, int> selectedPoss;

                    qDebug() << "------------IN-----------------";
                    int modelCount = 1;
                    inResModel->recursiveVisitInChildrens([this, &allPoss, &selectedPoss, &modelCount] (const CT_InAbstractModel* child)-> bool {

                        if (child->displayableName() != "*")
                        {

                            int count = 0;
                            this->recursiveCountHierachicalRank(count, child);

                            qDebug() << this->getTabsForHierachicalRank(count) << child->displayableName() << " " << modelCount;

                            child->visitPossibilities([this, &allPoss, &selectedPoss, &modelCount](const CT_InAbstractModel* inModel, const CT_InStdModelPossibility* possibility) -> bool {
                                Q_UNUSED(inModel);
                                allPoss.append(possibility->outModel());
                                if (possibility->isSelected())
                                {
                                    selectedPoss.insert(possibility->outModel(), modelCount);
                                }
                                return true;
                            });
                            modelCount++;
                        }

                        return true;
                    });

                    qDebug() << "-----------------------------";

                    if((outResModel != nullptr)
                        && (inResModel != nullptr)
                        && (inResModel->rootGroup() != nullptr))
                    {
                        QList<QString> retList;

                        // Nom du résultat modèle d'entrée
                        QString rootName = outResModel->step() == nullptr ? tr("No Step") : static_cast<CT_VirtualAbstractStep *>(outResModel->step())->displayableName();
                        retList.append(rootName);
                        qDebug() << rootName;

                        this->recursiveCreateInputTree(rootName, outResModel->rootGroup(), allPoss, selectedPoss);
                    }
                } else {
                    qDebug() << "nullptr";
                }


                return true;
            });

            return true;
        });
    }

    // TESTTUTO
    // Aller voir ici : CTG_InModelPossibilitiesChoice::constructModel
    // CT_ModelSelectionHelper::recursiveSelectAllPossibilitiesByDefault
    // CTG_ModelsLinkConfigurationFlowView

    return result;
}


void CDM_CitationInfo::recursiveCreateInputTree(QString root, const DEF_CT_AbstractGroupModelOut* group, QList<const CT_OutAbstractModel*> &allPoss, QMultiMap<const CT_OutAbstractModel*, int> &selectedPoss)
{
    if(group == nullptr)
        return;

    QList<QString> list;

    // le nom du modèle de sortie
    QString rootItem = group->displayableName();
    list.append(rootItem);


    if (allPoss.contains(group))
    {
        QList<int> modelCounts = selectedPoss.values(group);
        std::sort(modelCounts.begin(), modelCounts.end());
        QString str;
        for (int i = 0 ; i < modelCounts.size() ; i++)
        {
            str.append(QString("%1").arg(modelCounts.at(i)));
            if (i < modelCounts.size() - 1)
            {
                str.append(";");
            }
        }

        int count = 0;
        this->recursiveCountHierachicalRank(count, group);

        QString itemGroup = group->displayableName();
        qDebug() << this->getTabsForHierachicalRank(count) << itemGroup << ((modelCounts.size() > 0)?QString(" *** %1").arg(str):"");

        group->visitGroups([this, &rootItem, &allPoss, &selectedPoss](const DEF_CT_AbstractGroupModelOut* g) -> bool {
            this->recursiveCreateInputTree(rootItem, g, allPoss, selectedPoss);
            return true;
        });
    }

    group->visitItems([this, &rootItem, &allPoss, &selectedPoss](const CT_OutAbstractSingularItemModel* item) -> bool {

        QList<QString> list2;

        // le nom du modèle de sortie
        QString itemItem = item->displayableName();
        list2.append(itemItem);

        if (allPoss.contains(item))
        {
            QList<int> modelCounts = selectedPoss.values(item);
            std::sort(modelCounts.begin(), modelCounts.end());
            QString str;
            for (int i = 0 ; i < modelCounts.size() ; i++)
            {
                str.append(QString("%1").arg(modelCounts.at(i)));
                if (i < modelCounts.size() - 1)
                {
                    str.append(";");
                }
            }

            int count = 0;
            this->recursiveCountHierachicalRank(count, item);

            qDebug() << this->getTabsForHierachicalRank(count) << itemItem << ((modelCounts.size() > 0)?QString(" *** %1").arg(str):"");
        }

        item->visitAttributes([this, &itemItem, &allPoss, &selectedPoss](const CT_OutAbstractItemAttributeModel* itemAtt) -> bool {
            QList<QString> list3;

            // le nom du modèle de sortie
            QString itemItemAtt = itemAtt->displayableName();
            list3.append(itemItemAtt);

            if (allPoss.contains(itemAtt))
            {
                QList<int> modelCounts = selectedPoss.values(itemAtt);
                std::sort(modelCounts.begin(), modelCounts.end());
                QString str;
                for (int i = 0 ; i < modelCounts.size() ; i++)
                {
                    str.append(QString("%1").arg(modelCounts.at(i)));
                    if (i < modelCounts.size() - 1)
                    {
                        str.append(";");
                    }
                }

                int count = 0;
                this->recursiveCountHierachicalRank(count, itemAtt);

                qDebug() << this->getTabsForHierachicalRank(count) << itemItemAtt << ((modelCounts.size() > 0)?QString(" *** %1").arg(str):"");
            }

            //itemItem->appendRow(list3);

            return true;
        });

        //rootItem->appendRow(list2);

        return true;
    });

    //root->appendRow(list);
}

void CDM_CitationInfo::recursiveCountHierachicalRank(int &count, const CT_AbstractModel* model)
{
    if (model->parentModel() != nullptr)
    {
        count++;
        recursiveCountHierachicalRank(count, model->parentModel());
    }
}

QString CDM_CitationInfo::getTabsForHierachicalRank(int count)
{
    QString str;
    for (int i = 0 ; i < count ; i++)
    {
        str.append("  ");
    }
    return str;
}


QList<CDM_CitationInfo::StepCitationInfo> CDM_CitationInfo::getScriptTable()
{
    QList<CDM_CitationInfo::StepCitationInfo> list;

    for (int i = 0 ; i < _stepList.size() ; i++)
    {
        CT_VirtualAbstractStep* currentStep = _stepList.at(i);

        CT_AbstractStepPlugin* plugin = currentStep->pluginStaticCastT<>();
        QString pluginOfficialName = plugin->getPluginOfficialName();
        QString pluginName = _pluginManager->getPluginName(_pluginManager->getPluginIndex(plugin));

        if (pluginName.left(5) == "plug_")
        {
            pluginName.remove(0, 5);
        }
        if (pluginOfficialName != "")
        {
            pluginName = pluginOfficialName;
        }

        list.append(CDM_CitationInfo::StepCitationInfo(currentStep->uniqueID(), currentStep->name(), currentStep->description(), pluginName));
    }
    return list;
}

QString CDM_CitationInfo::getPluginAndStepCitations()
{
    QList<CT_AbstractStepPlugin*> pluginDone;

    QString str;
    QString str2;

    str.append("<h3>" + tr("Citation de la plateforme Computree") + "</h3>");
    str.append("<p>" + tr("Pour citer la plateforme Computree :") + "</p>");
    str.append("<p class=\"descBlock\">");
    str.append(CT_ParseRIS::parseRIS(getComputreeCoreRis()));
    str2.append("</p>");
    str.append("<br><br>");
    str.append("<h3>" + tr("Citation des plugins") + "</h3>");
    str.append("<p>" + tr("Pour citer les plugins utilisés :") + "</p>");

    for (int i = 0 ; i < _stepList.size() ; i++)
    {
        CT_VirtualAbstractStep* currentStep = _stepList.at(i);

        CT_AbstractStepPlugin* plugin = currentStep->pluginStaticCastT<>();
        QString pluginOfficialName = plugin->getPluginOfficialName();
        QString pluginName = _pluginManager->getPluginName(_pluginManager->getPluginIndex(plugin));
        QString stepName = currentStep->description();

        if (pluginName.left(5) == "plug_")
        {
            pluginName.remove(0, 5);
        }
        if (pluginOfficialName != "")
        {
            pluginName = pluginOfficialName;
        }

        if (!pluginDone.contains(plugin))
        {
            pluginDone.append(plugin);

            if (!_corePluginList.contains(pluginName)) // core plugins don't need to be cite individually
            {
                str.append(tr("Plugin %1 :<br>").arg(pluginName));
                QStringList pluginRIS = plugin->getPluginRISCitationList();
                if (pluginRIS.isEmpty())
                {
                    str.append(tr("<em>No official citation was provided</em><br>"));
                } else {
                    for (int pp = 0 ; pp < pluginRIS.size() ; pp++)
                    {
                        str.append("<p class=\"descBlock\">");
                        str.append(CT_ParseRIS::parseRIS(pluginRIS.at(pp)));
                        str.append("</p>");
                    }
                }
                str.append("<br>");
            }
        }

        QStringList stepsRis = currentStep->getStepRISCitations();
        if (stepsRis.size() > 0)
        {
            str2.append(tr("Etape <b>%1</b> <em>(plugin %2)</em> : <br>").arg(stepName, pluginName));
        }

        for (int i = 0 ; i < stepsRis.size() ; i++)
        {
            str2.append("<p class=\"descBlock\">");
            str2.append(CT_ParseRIS::parseRIS(stepsRis.at(i)));
            str2.append("</p>");
            str2.append("<br>");
        }
    }

    if (!str2.isEmpty())
    {
        str.append("<h3>" + tr("Citations spécifiques pour les étapes") + "</h3>");
        str.append("<p>" + tr("Ci-dessous, la liste des étapes fournissant des références bibliographiques spécifiques (une même référence peut être liée à plusieurs étapes) :") + "</p>");
        str.append(str2);
    }


    return str;
}

bool CDM_CitationInfo::hasStepCitation()
{
    for (int i = 0 ; i < _stepList.size() ; i++)
    {
        CT_VirtualAbstractStep* currentStep = _stepList.at(i);

        QStringList stepsRis = currentStep->getStepRISCitations();
        if (stepsRis.size() > 0) {return true;}
    }
    return false;
}

QString CDM_CitationInfo::getPluginRIS()
{
    QString str;

    str.append(getComputreeCoreRis());

    QMapIterator<QString, CT_AbstractStepPlugin*> it(_pluginList);
    while (it.hasNext())
    {
        it.next();
        if (!_corePluginList.contains(it.key()))
        {
            QStringList citationList = it.value()->getPluginRISCitationList();
            for (int pp = 0 ; pp < citationList.size() ; pp++)
            {
                str.append(citationList.at(pp));
            }
        }
    }

    QList<QString> stepCitations;

    for (int i = 0 ; i < _stepList.size() ; i++)
    {
        CT_VirtualAbstractStep* currentStep = _stepList.at(i);

        QStringList stepsRis = currentStep->getStepRISCitations();
        for (int i = 0 ; i < stepsRis.size() ; i++)
        {
            QString ris = stepsRis.at(i);
            if (!stepCitations.contains(ris))
            {
                stepCitations.append(ris);
            }
        }
    }

    for (int i = 0 ; i < stepCitations.size() ; i++)
    {
        str.append(stepCitations.at(i));
    }

    return str;
}

QString CDM_CitationInfo::getUsedPlugins()
{
    QString str;

    QMapIterator<QString, CT_AbstractStepPlugin*> it(_pluginList);
    while (it.hasNext())
    {
        it.next();
        QString pluginName = it.key();
        CT_AbstractStepPlugin* plugin = it.value();

        str.append(tr("<strong>Plugin %1</strong> : <a href=\"%2\" target=\"_blank\" rel=\"noreferrer noopener\">%2</a><br><br>").arg(pluginName, plugin->getPluginURL()));
    }

    return str;
}

QString CDM_CitationInfo::getPluginListToCite()
{
    QString str;

    QMapIterator<QString, CT_AbstractStepPlugin*> it(_pluginList);
    while (it.hasNext())
    {
        it.next();
        QString pluginName = it.key();
        if (!_corePluginList.contains(pluginName))
        {
            if (str.isEmpty())
            {
                str.append(pluginName);
            } else {
                str.append(", " + pluginName);
            }
        }
    }

    return str;
}


QString CDM_CitationInfo::getComputreeCoreRis()
{
    QString computreeCitationRIS = "TY  - COMP\n"
                            "TI  - Computree platform\n"
                            "AU  - Computree Core Team\n"
                            "PB  - Computree group\n"
                            "PY  - 2022\n"
                            "UR  - https://computree.onf.fr\n"
                            "ER  - \n";

    return computreeCitationRIS;
}


