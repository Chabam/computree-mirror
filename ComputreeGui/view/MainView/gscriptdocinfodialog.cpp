#include "gscriptdocinfodialog.h"
#include "ui_gscriptdocinfodialog.h"

#include <QMessageBox>
#include <QDir>

GScriptDocInfoDialog::GScriptDocInfoDialog(QString mainPath, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GScriptDocInfoDialog)
{
    ui->setupUi(this);

    ui->le_filename->setFocus();

    _mainPath = mainPath;
}

GScriptDocInfoDialog::~GScriptDocInfoDialog()
{
    delete ui;
}

void GScriptDocInfoDialog::getInfo(CDM_StepListDocExporter::DocumentatedScriptInfo &docInfo)
{
    docInfo._scriptFileName = ui->le_filename->text();

    docInfo._scriptName = ui->le_title->text();
    if (docInfo._scriptName.isEmpty()) {docInfo._scriptName = docInfo._scriptFileName;}

    docInfo._description = ui->pt_description->toPlainText();

    docInfo._author = ui->le_author->text();
}

void GScriptDocInfoDialog::on_pb_export_clicked()
{
    QString fileName = ui->le_filename->text();
    if (fileName.isEmpty())
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(tr("Erreur"));
        msgBox.setText(tr("Aucun nom de fichier choisi !"));
        msgBox.setInformativeText(tr("Le choix d'un nom de fichier est obligatoire."));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    QString outDirPath = _mainPath + "/" + fileName;

    QDir outDir(outDirPath);
    if (outDir.exists())
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(tr("Attention"));
        msgBox.setText(tr("Attention un dossier avec ce nom existe déjà !"));
        msgBox.setInformativeText(tr("Le sous-dossier %1 existe déjà dans le dossier %2.\nEtes-vous sûr de vouloir l'écraser ?").arg(fileName, _mainPath));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Yes);
        msgBox.addButton(QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);

        if(msgBox.exec() == QMessageBox::No) {return;}

    }

    accept();
}


void GScriptDocInfoDialog::on_pb_cancel_clicked()
{
    reject();
}

