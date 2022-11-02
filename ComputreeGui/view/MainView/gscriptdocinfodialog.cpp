#include "gscriptdocinfodialog.h"
#include "ui_gscriptdocinfodialog.h"

#include <QMessageBox>
#include <QDir>

GScriptDocInfoDialog::GScriptDocInfoDialog(QString mainPath, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GScriptDocInfoDialog)
{
    ui->setupUi(this);

    _mainPath = mainPath;
}

GScriptDocInfoDialog::~GScriptDocInfoDialog()
{
    delete ui;
}

QString GScriptDocInfoDialog::filename()
{
    return ui->le_filename->text();
}

QString GScriptDocInfoDialog::title()
{
    return ui->le_title->text();
}

QString GScriptDocInfoDialog::author()
{
    return ui->le_author->text();
}

QString GScriptDocInfoDialog::description()
{
    return ui->pt_description->toPlainText();
}

void GScriptDocInfoDialog::on_pb_export_clicked()
{
    QString fileName = filename();
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

