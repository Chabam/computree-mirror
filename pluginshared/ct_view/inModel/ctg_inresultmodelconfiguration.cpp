#include "ctg_inresultmodelconfiguration.h"
#include "ui_ctg_inresultmodelconfiguration.h"

#include "ct_result/model/inModel/tools/ct_inresultmodelmanager.h"

#include <QMessageBox>

CTG_InResultModelConfiguration::CTG_InResultModelConfiguration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CTG_InResultModelConfiguration)
{
    ui->setupUi(this);

    m_inputResultModelManager = NULL;

    connect(ui->inResultModelPossibilities, SIGNAL(showInResultModelPossibility(const CT_InStdResultModelPossibility*)), ui->inModelPossibilities, SLOT(setInResultModelPossibility(const CT_InStdResultModelPossibility*)), Qt::DirectConnection);
}

CTG_InResultModelConfiguration::~CTG_InResultModelConfiguration()
{
    delete ui;
}

void CTG_InResultModelConfiguration::setInResultModelManager(const CT_InResultModelManager *manager)
{
    m_inputResultModelManager = const_cast<CT_InResultModelManager*>(manager);

    ui->inResultModelPossibilities->setInResultModelManager(m_inputResultModelManager);
}

void CTG_InResultModelConfiguration::setReadOnly(bool enabled)
{
    ui->inResultModelPossibilities->setReadOnly(enabled);
    ui->inModelPossibilities->setReadOnly(enabled);
}

void CTG_InResultModelConfiguration::accept()
{
    if(m_inputResultModelManager != NULL) {
        QStringList errors;

        if(!m_inputResultModelManager->checkIfAllInputModelsWasCorrectlyConfigured(&errors)) {

            QMessageBox::warning(this, tr("Avertissement"), tr("Vous n'avez pas correctement configuré "
                                                               "les données d'entrée. Liste des erreurs détectées :\r\n\r\n- %1").arg(errors.join("\r\n- ")), QMessageBox::Ok);

            return;
        }
    }

    QDialog::accept();
}
