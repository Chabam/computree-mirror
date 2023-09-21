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

        step->inputDescription();
    }

    return result;
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


