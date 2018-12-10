#ifndef BATCH_H
#define BATCH_H

#include "interfacesbatch.h"

#include "signalhandler.h"

#include "cdm_pluginmanager.h"
#include "cdm_scriptmanagerabstract.h"
#include "cdm_stepmanager.h"
#include "cdm_actionsmanager.h"

#include "ct_log/abstract/ct_abstractloglistener.h"

#include <QCommandLineParser>

class Batch : public BatchInterface, public SignalHandler, public CT_AbstractLogListener
{
    Q_OBJECT
    Q_INTERFACES(BatchInterface)

public:
    Batch();
    ~Batch();

    void processArgs(QCoreApplication& a);
    bool canInitWithArgs();
    bool initWithArgs();
    bool mustUseTrayIcon() const;
    bool mustBeFullBatch() const;

    void asyncLoadScriptFromString(const QString &script);
    void asyncLoadScriptFromFile(QString filepath);
    bool asyncRemoveAllStep();

    bool execute();
    bool isRunning();
    void stop();

    bool handleSignal(int signal);

    void addMessage(const int &severity, const int &type, const QString &s, const QString &filter = "");

    CDM_PluginManager* getPluginManager() const;
    CDM_StepManager* getStepManager() const;
    CDM_ActionsManager *getActionManager() const;

private:
    QCommandLineParser      m_parser;
    QCommandLineOption      m_helpOption = QCommandLineOption("a");
    QCommandLineOption      m_versionOption = QCommandLineOption("a");
    QCommandLineOption      m_nosystrayOption = QCommandLineOption("a");
    QCommandLineOption      m_miniguiOption = QCommandLineOption("a");
    QCommandLineOption      m_startstepsOption = QCommandLineOption("a");
    QCommandLineOption      m_noLogsOption = QCommandLineOption("a");
    CDM_PluginManager       *_pluginManager;
    CDM_ScriptManagerAbstract    *_scriptManager;
    CDM_StepManager         *_stepManager;
    CDM_ActionsManager      *_actionManager;

    QString                 _script;

    void initParser();

signals:

    void loadError(QString error);
    void privateLoadScriptText();
    void privateLoadScriptFilePath();
    void privateRemoveAllStep();

    void mustQuitApplication();

private slots:
    void loadScriptText();
    void loadScriptFilePath();
    void executionFinished();

public slots:
    void removeAllStep();
};

#endif // BATCH_H
