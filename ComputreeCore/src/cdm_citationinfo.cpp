#include "cdm_citationinfo.h"
#include "ct_abstractstepplugin.h"
#include "ct_step/abstract/ct_virtualabstractstep.h"

CDM_CitationInfo::CDM_CitationInfo(CDM_StepManager *stepManager, CDM_PluginManager *pluginManager) : QObject(NULL)
{
    _stepManager = stepManager;
    _pluginManager = pluginManager;

    QList<CT_VirtualAbstractStep *>& thisStepList = _stepList;

    const QList<CT_VirtualAbstractStep*> rootList = stepManager->getStepRootList();

    for(CT_VirtualAbstractStep* currentStep : rootList)
    {
        thisStepList.append(currentStep);

        currentStep->recursiveVisitChildrens([&thisStepList](const CT_VirtualAbstractStep*, const CT_VirtualAbstractStep* child) -> bool {
            thisStepList.append(const_cast<CT_VirtualAbstractStep*>(child));
            return true;
        });
    }
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

    str.append(parseRIS(getComputreeCoreRis()));
    str.append("<br>");

    for (int i = 0 ; i < _stepList.size() ; i++)
    {
        CT_VirtualAbstractStep* currentStep = _stepList.at(i);

        CT_AbstractStepPlugin* plugin = currentStep->pluginStaticCastT<>();
        QString pluginOfficialName = plugin->getPluginOfficialName();
        QString pluginName = _pluginManager->getPluginName(_pluginManager->getPluginIndex(plugin));
        QString stepName = currentStep->name();

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

            str.append(QString("[<b>Plugin %1</b>]<br>").arg(pluginName));
            QStringList pluginRIS = plugin->getPluginRISCitationList();
            if (pluginRIS.isEmpty())
            {
                str.append(tr("<em>No official citation was provided</em><br>"));
            } else {
                for (int pp = 0 ; pp < pluginRIS.size() ; pp++)
                {
                    str.append(parseRIS(pluginRIS.at(pp)));
                }
            }
            str.append("<br>");
        }

        QStringList stepsRis = currentStep->getStepRISCitations();
        if (stepsRis.size() > 0)
        {
            str.append(QString("[<b>%1 - in plugin %2</b>]<br>").arg(stepName).arg(pluginName));
        }

        for (int i = 0 ; i < stepsRis.size() ; i++)
        {
            str.append(parseRIS(stepsRis.at(i)));
            str.append("<br>");
        }
    }

    return str;
}

QString CDM_CitationInfo::getPluginRIS()
{
    QString str;

    str.append(getComputreeCoreRis());

    QList<QString> stepCitations;

    QMap<QString, CT_AbstractStepPlugin*> plugins;
    for (int i = 0 ; i < _stepList.size() ; i++)
    {
        CT_VirtualAbstractStep* currentStep = _stepList.at(i);
        CT_AbstractStepPlugin* plugin = currentStep->pluginStaticCastT<>();
        QString pluginName = _pluginManager->getPluginName(_pluginManager->getPluginIndex(plugin));

        plugins.insert(pluginName, plugin);

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

    QMapIterator<QString, CT_AbstractStepPlugin*> it(plugins);
    while (it.hasNext())
    {
        it.next();
        QStringList citationList = it.value()->getPluginRISCitationList();
        for (int pp = 0 ; pp < citationList.size() ; pp++)
        {
            str.append(citationList.at(pp));
        }
    }

    for (int i = 0 ; i < stepCitations.size() ; i++)
    {
        str.append(stepCitations.at(i));
    }

    return str;
}

QString CDM_CitationInfo::parseRIS(QString ris)
{    
    QMultiMap<QString, QString> fields;
    QStringList lines = ris.split("\n", QString::SkipEmptyParts);

    for (int i = 0 ; i < lines.size() ; i++)
    {
        QString line = lines.at(i);

        QString fieldValue = line.mid(0,2);
        int cpt = 2;
        while (line.mid(cpt,1) == " " || line.mid(cpt,1) == "-")
        {
            ++cpt;
        }

        QString value = line.mid(cpt);

        QString code = fieldValue.toUpper();
        if (code == "AU") {code = "A1";}
        if (code.size() == 2)
        {
            fields.insert(code, value);
        }
    }

    QString str;

    if (fields.contains("A1") || fields.contains("PY"))
    {
        str.append("<b>");
    }


    QList<QString> authors = fields.values("A1");

    for (int i = authors.size() - 1 ; i >= 0 ; i--)
    {

        QString author = authors.at(i);
        author.replace(", ", " ");
        author.replace(",", " ");
        str.append(author);
        if (i > 0)
        {
            str.append(", ");
        }
    }

    if (str.size() > 0)
    {
        str.append(". ");
    }

    if (fields.contains("PY"))
    {
        str.append(fields.values("PY").first());
        str.append(". ");
    }else if (fields.contains("Y1"))
    {
        str.append(fields.values("Y1").first());
        str.append(". ");
    }

    if (str.size() > 0)
    {
        str.append("</b>");
    }

    if (fields.contains("TI"))
    {
        str.append("<em>");
        str.append(fields.values("TI").first());
        str.append("</em>. ");
    } else if (fields.contains("T1"))
    {
        str.append("<em>");
        str.append(fields.values("T1").first());
        str.append("</em>. ");
    }

    if (fields.contains("PB"))
    {
        str.append(fields.values("PB").first());
        str.append(". ");
    }
    if (fields.contains("UR"))
    {
        str.append("<br><a href=\"");
        str.append(fields.values("UR").first());
        str.append("\">");
        str.append(fields.values("UR").first());
        str.append("</a>. ");
    }
    str.append("<br>");

    return str;
}

QString CDM_CitationInfo::getComputreeCoreRis()
{
    QString computreeCitationRIS = "TY  - COMP\n"
                            "TI  - Computree platform\n"
                            "AU  - Computree Core Team\n"
                            "PB  - Office National des Forêts, RDI Department\n"
                            "PY  - 2017\n"
                            "UR  - http://rdinnovation.onf.fr/computree\n"
                            "ER  - \n";

    return computreeCitationRIS;
}


