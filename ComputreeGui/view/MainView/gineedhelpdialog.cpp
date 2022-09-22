#include "gineedhelpdialog.h"
#include "ui_gineedhelpdialog.h"

GINeedHelpDialog::GINeedHelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GINeedHelpDialog)
{
    ui->setupUi(this);


    ui->content->append(tr("<strong>Site officiel de Computree</strong> : aide sur l'utilisation de l'interface, tutoriels, liste des plugins,..."));
    ui->content->append(tr("-> <a href=\'https://computree.onf.fr'>computree.onf.fr</a>"));
    ui->content->append("<br>");

    ui->content->append(tr("<strong>Aide sur les étapes de traitement</strong> :"));
    ui->content->append(tr("-> Index : Aller dans le menu Aide, puis cliquer sur <em>Documentation des étapes</em>"));
    ui->content->append(tr("-> Aide sur une étape insérée : faire un clic droit sur l'étape insérée, puis <em>Documentation de l'étape</em>"));
    ui->content->append(tr("-> Aide sur une étape du menu (pas encore insérée) : faire un clic droit sur l'étape dans le menu, puis <em>Documentation de l'étape</em>"));
    ui->content->append("<br>");

    ui->content->append(tr("<strong>Discord Computree</strong> : pour échanger avec la communauté, poser des questions, signaler un bug ou proposer une amélioration :"));
    ui->content->append(tr("-> <a href=\'https://discord.gg/T55URXTYX8'>Discord Computree</a>"));
    ui->content->append("<br>");
}

GINeedHelpDialog::~GINeedHelpDialog()
{
    delete ui;
}
