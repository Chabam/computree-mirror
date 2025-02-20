#include "batchdialog.h"
#include "ui_batchdialog.h"

#include <QMenu>
#include <QMessageBox>

BatchDialog::BatchDialog(Batch& batch, QWidget *parent) :
    _batch(batch),
    QDialog(parent),
    ui(new Ui::BatchDialog)
{
    ui->setupUi(this);

    _batchPluginManager.load();
    _pluginSelected = nullptr;
    _trayIcon = nullptr;

    connect(&_batch, SIGNAL(loadError(QString)), this, SLOT(batchLoadingError(QString)));
    connect(&_batch, SIGNAL(mustQuitApplication()), this, SLOT(quitApplication()), Qt::DirectConnection);

    initUi();

    if(_trayIcon != nullptr)
        _trayIcon->show();
}

BatchDialog::~BatchDialog()
{
    delete ui;
}

const Batch& BatchDialog::getBatchCore() const
{
    return _batch;
}

void BatchDialog::quitApplication()
{
    QTimer::singleShot(3000, this, SLOT(quit()));
}

void BatchDialog::initUi()
{
    QApplication::setQuitOnLastWindowClosed(false);

    if(_batch.mustUseTrayIcon())
        createTrayIconAndMenu();

    ui->pushButtonStart->setEnabled(false);
    ui->pushButtonStop->setEnabled(false);
    ui->pushButtonReload->setEnabled(true);

    populatePluginsComboBox();

    if(!_batch.initWithArgs())
        quitApplication();
}

void BatchDialog::createTrayIconAndMenu()
{
    QAction *minimizeAction = new QAction(tr("Mi&nimizer"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    QAction *maximizeAction = new QAction(tr("Ma&ximizer"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    QAction *restoreAction = new QAction(tr("&Restorer"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    QAction *quitAction = new QAction(tr("&Quitter"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));

    _trayIconMenu = new QMenu(this);
    _trayIconMenu->addAction(minimizeAction);
    _trayIconMenu->addAction(maximizeAction);
    _trayIconMenu->addAction(restoreAction);
    _trayIconMenu->addSeparator();
    _trayIconMenu->addAction(quitAction);

    _trayIcon = new QSystemTrayIcon(this);
    _trayIcon->setContextMenu(_trayIconMenu);
    _trayIcon->setIcon(QIcon(":/img/trayicon.png"));

    connect(_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
}

void BatchDialog::populatePluginsComboBox()
{
    ui->comboBoxPlugins->clear();

    int size = _batchPluginManager.nPlugins();

    if(size > 0)
    {
        for(int i=0;i<size;++i)
        {
            ui->comboBoxPlugins->addItem(_batchPluginManager.getPluginName(i));
        }

        ui->comboBoxPlugins->setEnabled(true);
        ui->pushButtonStart->setEnabled(true);
    }
    else
    {
        ui->comboBoxPlugins->setEnabled(false);
    }
}

void BatchDialog::on_comboBoxPlugins_currentIndexChanged(int index)
{
    if(index == -1)
    {
        ui->pushButtonStart->setEnabled(false);
        ui->pushButtonStop->setEnabled(false);
    }
}

void BatchDialog::on_pushButtonStart_clicked()
{
    if(_pluginSelected != nullptr)
        disconnect(_pluginSelected, nullptr, this, nullptr);

    _pluginSelected = _batchPluginManager.getPlugin(ui->comboBoxPlugins->currentIndex());
    _pluginSelected->setBatchInterface(&_batch);

    connect(_pluginSelected, SIGNAL(started()), this, SLOT(pluginSelectedStarted()), Qt::QueuedConnection);
    connect(_pluginSelected, SIGNAL(finished()), this, SLOT(pluginSelectedFinished()), Qt::QueuedConnection);

    if(!_pluginSelected->isInitialized())
        _pluginSelected->init();

    _pluginSelected->run();
}

void BatchDialog::on_pushButtonStop_clicked()
{
    _pluginSelected->stop();
}

void BatchDialog::on_pushButtonReload_clicked()
{
    _batchPluginManager.load();
    populatePluginsComboBox();
}

void BatchDialog::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if((reason == QSystemTrayIcon::Trigger) && isHidden())
        showNormal();
}

void BatchDialog::pluginSelectedStarted()
{
    ui->pushButtonStart->setEnabled(false);
    ui->pushButtonStop->setEnabled(true);
    ui->pushButtonReload->setEnabled(false);
}

void BatchDialog::pluginSelectedFinished()
{
    ui->pushButtonStart->setEnabled(true);
    ui->pushButtonStop->setEnabled(false);
    ui->pushButtonReload->setEnabled(true);
}

void BatchDialog::batchLoadingError(QString error)
{
    QMessageBox::critical(this, tr("Error"), tr("Error while loading :\r\n\r\n%1").arg(error));
}

void BatchDialog::quit()
{
    _batch.removeAllStep();

    qApp->quit();
}
