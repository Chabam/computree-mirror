#include "ctg_inresultmodelconfiguration.h"
#include "ui_ctg_inresultmodelconfiguration.h"

#include <QMessageBox>
#include <QTimer>

CTG_InResultModelConfiguration::CTG_InResultModelConfiguration(IInModelPossibilitiesChoice* widgetInModelPossibilities, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CTG_InResultModelConfiguration)
{
    ui->setupUi(this);

    m_inputResultModelManager = nullptr;

    mInModelPossibilities = widgetInModelPossibilities;
    ui->horizontalLayout->addWidget(mInModelPossibilities);

    connect(ui->inResultModelPossibilities, SIGNAL(showInResultModelPossibility(const CT_InStdResultModelPossibility*)), mInModelPossibilities, SLOT(setInResultModelPossibility(const CT_InStdResultModelPossibility*)), Qt::DirectConnection);
}

CTG_InResultModelConfiguration::~CTG_InResultModelConfiguration()
{
    delete mInModelPossibilities;

    delete ui;
}

void CTG_InResultModelConfiguration::setInResultModelManager(const CT_InModelStructureManager* manager)
{
    m_inputResultModelManager = const_cast<CT_InModelStructureManager*>(manager);

    ui->inResultModelPossibilities->setInResultModelManager(m_inputResultModelManager);
}

void CTG_InResultModelConfiguration::setReadOnly(bool enabled)
{
    mInModelPossibilities->setReadOnly(enabled);
    ui->inResultModelPossibilities->setReadOnly(enabled);
}

void CTG_InResultModelConfiguration::accept()
{
    if(mInModelPossibilities->isReadOnly()) {
        QTimer::singleShot(100, this, SLOT(reject()));
        return;
    }

    if(m_inputResultModelManager != nullptr) {
        QStringList errors;

        const bool ok = m_inputResultModelManager->visitResults([&errors](const CT_InAbstractResultModel* resultModel) -> bool {
            return resultModel->recursiveHasTheMinimumNumberOfSelectedPossibilityRequired(&errors);
        });

        if(!ok) {
            QMessageBox::warning(this, tr("Warning"), tr("You have not correctly configured the input data. List of detected errors :\r\n\r\n- %1").arg(errors.join("\r\n- ")), QMessageBox::Ok);

            return;
        }
    }

    QDialog::accept();
}
