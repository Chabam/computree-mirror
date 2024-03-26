#ifndef CDM_STEPLISTINFOMANAGER_H
#define CDM_STEPLISTINFOMANAGER_H

#include "computreeCore_global.h"
#include "cdm_stepmanager.h"
#include "cdm_pluginmanager.h"

#include "ct_model/inModel/ct_inresultmodelgroup.h"
#include "ct_model/outModel/abstract/ct_outabstractresultmodelgroup.h"
#include "ct_model/inModel/tools/ct_instdresultmodelpossibility.h"

#include <QObject>

class COMPUTREECORESHARED_EXPORT CDM_StepListInfoManager : public QObject
{
    Q_OBJECT
public:
    struct StepInfo
    {
        StepInfo(int num, QString stepName, QString stepDescription, QString pluginName)
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


    CDM_StepListInfoManager(CDM_StepManager *stepManager, CDM_PluginManager *pluginManager);

    QString getScriptStepList();
    QString getScriptStepListParameters();
    QString getScriptStepListInputConfig();
    void recursiveCreateInputTree(QString root, const DEF_CT_AbstractGroupModelOut *group, QList<const CT_OutAbstractModel *> &allPoss, QMultiMap<const CT_OutAbstractModel *, int> &selectedPoss);

    QList<CDM_StepListInfoManager::StepInfo> getScriptTable();
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


    void recursiveCountHierachicalRank(int &count, const CT_AbstractModel *model);
    QString getTabsForHierachicalRank(int count);
};

#endif // CDM_STEPLISTINFOMANAGER_H
