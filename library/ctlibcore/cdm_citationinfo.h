#ifndef CDM_CITATIONINFO_H
#define CDM_CITATIONINFO_H

#include "computreeCore_global.h"
#include "cdm_stepmanager.h"
#include "cdm_pluginmanager.h"


#include <QObject>

class COMPUTREECORESHARED_EXPORT CDM_CitationInfo : public QObject
{
    Q_OBJECT
public:
    struct StepCitationInfo
    {
        StepCitationInfo(int num, QString stepName, QString stepDescription, QString pluginName)
        {
            _num = num;
            _stepName = stepName;
            _stepDescription = stepDescription;
            _pluginName = pluginName;
        }

        int _num;
        QString _stepName;
        QString _stepDescription;
        QString _pluginName;
    };


    CDM_CitationInfo(CDM_StepManager *stepManager, CDM_PluginManager *pluginManager);

    QString getScriptStepList();
    QString getScriptStepListParameters();
    QList<CDM_CitationInfo::StepCitationInfo> getScriptTable();
    QString getPluginAndStepCitations();
    QString getPluginRIS();

    QString getUsedPlugins();
    QString getPluginListToCite();

    QList<CT_VirtualAbstractStep *> steps() {return _stepList;}

    static QString getComputreeCoreRis();

    bool hasStepCitation();

private:
    QList<CT_VirtualAbstractStep *>         _stepList;
    QList<int>                              _stepIndent;
    QMap<QString, CT_AbstractStepPlugin*>   _pluginList;
    QList<QString>                          _corePluginList;

    CDM_StepManager*                        _stepManager;
    CDM_PluginManager*                      _pluginManager;

    QString                                 _computreeCitationRIS;
};

#endif // CDM_CITATIONINFO_H
