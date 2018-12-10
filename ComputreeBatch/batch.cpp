#include "batch.h"

#include "cdm_configfile.h"
#include "cdm_scriptmanagerxmlallversions.h"

#include <QApplication>
#include <QDebug>
#include <QSystemTrayIcon>

Batch::Batch()
{
    _pluginManager = new CDM_PluginManager();
    _scriptManager = new CDM_ScriptManagerXMLAllVersions(*_pluginManager);
    _actionManager = new CDM_ActionsManager();
    _stepManager = new CDM_StepManager(_scriptManager, _actionManager);

    initParser();

    connect(this, SIGNAL(privateLoadScriptText()), this, SLOT(loadScriptText()), Qt::QueuedConnection);
    connect(this, SIGNAL(privateLoadScriptFilePath()), this, SLOT(loadScriptFilePath()), Qt::QueuedConnection);
    connect(this, SIGNAL(privateRemoveAllStep()), this, SLOT(removeAllStep()), Qt::QueuedConnection);

    connect(_stepManager, SIGNAL(started()), this, SIGNAL(started()), Qt::QueuedConnection);
    connect(_stepManager, SIGNAL(completed(bool)), this, SIGNAL(finished(bool)), Qt::QueuedConnection);
}

Batch::~Batch()
{
    delete _stepManager;
    delete _scriptManager;
    delete _pluginManager;
    delete _actionManager;

    delete CONFIG_FILE;
}

void Batch::processArgs(QCoreApplication &a)
{
    m_parser.process(a);
}

bool Batch::canInitWithArgs()
{
    if(m_parser.isSet(m_helpOption)) {
        m_parser.showHelp();
        return false;
    }

    if(m_parser.isSet(m_versionOption)) {
        m_parser.showVersion();
        return false;
    }

    if (mustUseTrayIcon() && !QSystemTrayIcon::isSystemTrayAvailable()) {
        qWarning().noquote() << tr("Les icônes système ne sont pas disponibles sur ce système.");
        m_parser.showHelp();
        return false;
    }

    const QStringList args = m_parser.positionalArguments();

    if(args.size() != 1) {
        m_parser.showHelp();
        return false;
    }

    if(!QFile(args.first()).exists()) {
        qWarning().noquote() << tr("Fichier \"%1\" non trouvé.").arg(args.first());
        return false;
    }

    return true;
}

void Batch::asyncLoadScriptFromString(const QString &script)
{
    if(isRunning())
        return;

    _script = script;

    emit privateLoadScriptText();
}

void Batch::asyncLoadScriptFromFile(QString filepath)
{
    if(isRunning())
        return;

    _script = filepath;

    emit privateLoadScriptFilePath();
}

bool Batch::asyncRemoveAllStep()
{
    if(isRunning())
        return false;

    emit privateRemoveAllStep();

    return true;
}

bool Batch::execute()
{
    if(isRunning())
        return false;

    return _stepManager->executeStep(NULL);
}

bool Batch::isRunning()
{
    return _stepManager->isRunning();
}

void Batch::stop()
{
    _stepManager->stop();
}

bool Batch::handleSignal(int signal)
{
    Q_UNUSED(signal)

    if(!_stepManager->isRunning())
        return false;

    qWarning().noquote() << tr("Arrêt des traitements en cours, veuillez patienter...");
    _stepManager->stop();

    while(_stepManager->isRunning());

    return true;
}

void Batch::addMessage(const int &severity, const int &type, const QString &s, const QString &filter)
{
    qDebug().noquote() << s;
}

CDM_PluginManager* Batch::getPluginManager() const
{
    return _pluginManager;
}

CDM_StepManager* Batch::getStepManager() const
{
    return _stepManager;
}

CDM_ActionsManager *Batch::getActionManager() const
{
    return _actionManager;
}

void Batch::initParser()
{
    m_parser.setApplicationDescription("Aide d'utilisation du mode batch");
    m_helpOption = m_parser.addHelpOption();
    m_versionOption = m_parser.addVersionOption();

    m_nosystrayOption = QCommandLineOption("nosystray",
                                         tr("Ne pas utiliser l'icone système mais simplement la fenêtre de progression."));

    m_miniguiOption = QCommandLineOption("smallgui",
                                         tr("Afficher une fenêtre afin de voir la progression."));

    m_startstepsOption = QCommandLineOption("startsteps",
                                         tr("Lancer les traitements à la fin du chargement du script (non nécessaire si l'option fullbatch est activée)."));

    m_noLogsOption = QCommandLineOption("nologs",
                                        tr("Désactive les logs de la console."));

    m_parser.addOption(m_nosystrayOption);
    m_parser.addOption(m_miniguiOption);
    m_parser.addOption(m_startstepsOption);
    m_parser.addOption(m_noLogsOption);
    m_parser.addPositionalArgument("source", tr("Chemin du script à charger"));
}

bool Batch::initWithArgs()
{
    if(!canInitWithArgs())
        return false;

    if(!_pluginManager->load()) {
        qWarning().noquote() << tr("Erreur lors du chargement des plugins : %1").arg(_pluginManager->getError());
        return false;
    }

    const QString script = m_parser.positionalArguments().first();
    const QString error = _scriptManager->loadScript(script, *_stepManager);

    if(!error.isEmpty())
    {
        qWarning().noquote() << tr("Erreur lors du chargement du script \"%1\" :\r\n").arg(script) + error;
        return false;
    } else {
        qDebug().noquote() << tr("Chargement du scrip terminé avec succès.");
    }

    if(!m_parser.isSet(m_noLogsOption))
        _pluginManager->log()->addNormalLogListener(this);

    if(m_parser.isSet(m_startstepsOption) || mustBeFullBatch())
    {
        connect(_stepManager, SIGNAL(completed()), this, SLOT(executionFinished()));

        qDebug().noquote() << tr("Lancement des traitements.");

        if(!_stepManager->executeStep()) {
            qWarning().noquote() << tr("Erreur inconnu lors de l'exécution des traitements.");
            return false;
        }

        qDebug().noquote() << tr("Traitements en cours veuillez patienter... (CTRL+C pour annuler)");
    }

    return true;
}

bool Batch::mustUseTrayIcon() const
{
    return !m_parser.isSet(m_nosystrayOption);
}

bool Batch::mustBeFullBatch() const
{
    return !m_parser.isSet(m_miniguiOption);
}

void Batch::loadScriptText()
{
    QString error = _scriptManager->loadScriptText(_script, *_stepManager);

    emit scriptLoaded(error);
}

void Batch::loadScriptFilePath()
{
    QString error = _scriptManager->loadScript(_script, *_stepManager);

    emit scriptLoaded(error);
}

void Batch::executionFinished()
{
    qDebug().noquote() << tr("Traitements terminés.");

    emit mustQuitApplication();
}

void Batch::removeAllStep()
{
    QList<CT_VirtualAbstractStep*> rootList = _stepManager->getStepRootList();

    bool ok = true;
    while(!rootList.isEmpty()
          && ok)
    {
        CT_VirtualAbstractStep *step = rootList.takeFirst();

        step->aboutToBeDeleted();

        step->recursiveVisitChildrens([](const CT_VirtualAbstractStep*, const CT_VirtualAbstractStep* child) -> bool {
            const_cast<CT_VirtualAbstractStep*>(child)->aboutToBeDeleted();
            return true;
        });

        ok = _stepManager->removeStep(step);
    }

    emit allStepRemoved();
}


