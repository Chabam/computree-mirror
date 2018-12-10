#include "ctg_inresultmodelconfiguration.h"
#include "ui_ctg_inresultmodelconfiguration.h"

#include <QMessageBox>
#include <QTimer>

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

void CTG_InResultModelConfiguration::setInResultModelManager(const CT_InModelStructureManager* manager)
{
    m_inputResultModelManager = const_cast<CT_InModelStructureManager*>(manager);

    ui->inResultModelPossibilities->setInResultModelManager(m_inputResultModelManager);
}

void CTG_InResultModelConfiguration::setReadOnly(bool enabled)
{
    ui->inModelPossibilities->setReadOnly(enabled);
    ui->inResultModelPossibilities->setReadOnly(enabled);
}

void CTG_InResultModelConfiguration::accept()
{
    if(ui->inModelPossibilities->isReadOnly()) {
        QTimer::singleShot(100, this, SLOT(reject()));
        return;
    }

    if(m_inputResultModelManager != NULL) {
        QStringList errors;

        const bool ok = m_inputResultModelManager->visitResults([&errors](const CT_InAbstractResultModel* resultModel) -> bool {
            return resultModel->recursiveHasTheMinimumNumberOfSelectedPossibilityRequired(&errors);
        });

        if(!ok) {
            QMessageBox::warning(this, tr("Avertissement"), tr("Vous n'avez pas correctement configuré "
                                                               "les données d'entrée. Liste des erreurs détectées :\r\n\r\n- %1").arg(errors.join("\r\n- ")), QMessageBox::Ok);

            return;
        }
    }

    QDialog::accept();
}
