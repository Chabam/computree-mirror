/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : MichaÃ«l KREBS (ARTS/ENSAM)

 This file is part of Computree version 2.0.

 Computree is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Computree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Computree.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/


#include "gmainwindow.h"
#include "ui_gmainwindow.h"

#include "dm_guimanager.h"
#include "cdm_citationinfo.h"

#include "tools/graphicsview/dm_pointofviewmanager.h"

#include "gdocumentviewforgraphics.h"
#include "gdocumentviewforitemmodel.h"
#include "gcitationdialog.h"
#include "gstepmanageroptions.h"

#include "dm_stepsfrompluginsmodelconstructor.h"
#include "view/Steps/gstepviewdefault.h"
#include "view/Steps/dm_steptreeviewdefaultproxymodel.h"

#include "gaboutdialog.h"
#include "gaboutpluginsdialog.h"
#include "gineedhelpdialog.h"
#include "gfavoritesmenudialog.h"
#include "gaboutmemory.h"
#include "gscriptdocinfodialog.h"

#include "ct_abstractstepplugin.h"
#include "ct_step/abstract/ct_abstractsteploadfile.h"
#include "ct_step/abstract/ct_abstractstepcanbeaddedfirst.h"
#include "ct_step/tools/menu/ct_menulevel.h"
#include "ct_global/ct_context.h"
#include "ct_coordinates/tools/ct_coordinatesystemmanager.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QDialogButtonBox>
#include <QToolTip>
#include <QDesktopWidget>
#include <QDir>

#include <QSpinBox>

GMainWindow::GMainWindow(QWidget *parent) :
    QMainWindow(parent), DM_MainWindow(),
    ui(new Ui::GMainWindow)
{
    ui->setupUi(this);
}

GMainWindow::~GMainWindow()
{
    delete ui;

    writeConfiguration();

    delete m_newStepManagerView;
    delete _docManagerView;
}

void GMainWindow::init()
{
    m_inLoadConfiguration = false;

    initUI();

    arrangeDocksInColumn();

    getScriptManager()->setScriptLoadCallBack(this);
}

void GMainWindow::closeEvent(QCloseEvent *ev)
{
    int res;

    if((res = GUI_MANAGER->asyncRemoveAllStep(nullptr)) != -1)
    {
        if(res == 0)
        {
            if(GUI_MANAGER->getStepManager()->isInManualMode())
                QMessageBox::warning(this, tr("Attention"), tr("Vous êtes dans le mode manuel, veuillez quitter ce mode avant de fermer l'application."), QMessageBox::Ok);
            else if(GUI_MANAGER->getStepManager()->isRunning())
                QMessageBox::warning(this, tr("Attention"), tr("Une étape est en cours de traiements, veuillez terminer les traitements avant de fermer l'application."), QMessageBox::Ok);
        }

        ev->ignore();
    }
}

DocumentManagerInterface* GMainWindow::documentManager() const
{
    return getDocumentManagerView();
}

QString GMainWindow::stepDisplayableNameLikeInGui(CT_VirtualAbstractStep* step, bool showExtended)
{
    return GStepViewDefault::staticGetStepNameFromConfiguration(step, m_stepChooserDialog->stepsChooserWidget()->stepNameConfiguration(), showExtended);
}

DM_DocumentManagerView* GMainWindow::getDocumentManagerView() const
{
    return _docManagerView;
}

DM_MultipleItemDrawableModelManager* GMainWindow::getItemDrawableModelManager() const
{
    return _itemDrawableModelView;
}

GItemDrawableConfigurationManagerView *GMainWindow::getItemDrawableConfigurationManager() const
{
    return _itemDrawableConfigurationView;
}

void GMainWindow::newDocument()
{
    _docManagerView->new3DDocument(true, m_inLoadConfiguration);
}

void GMainWindow::new2DDocument()
{
    _docManagerView->new2DDocument(true, m_inLoadConfiguration);
}

void GMainWindow::newItemModelDocument()
{
    _docManagerView->newTreeViewDocument(true, m_inLoadConfiguration);
}

void GMainWindow::openFile()
{
    openFile(_defaultOpenDirPath);
}

void GMainWindow::openFile(QString &loadDirectory)
{
    QString s = QFileDialog::getOpenFileName(this, tr("Ouvrir un fichier"), loadDirectory, createFileExtensionAvailable());

    if(!s.isEmpty())
    {
        QFileInfo info(s);

        loadDirectory = info.path();

        if(getScriptManager()->acceptFile(s))
            getScriptManager()->loadScript(s, *getStepManager());
        else
            m_newStepManagerView->addOpenFileStep(s);
    }
}

void GMainWindow::saveScriptUsingDefaultPath()
{
    saveScript(_defaultSaveDirPath);
}

void GMainWindow::saveScript(QString &saveDirectory)
{
    QString s = QFileDialog::getSaveFileName(this, tr("Sauvegarder l'arbre des étapes sous..."), saveDirectory, createScriptManagerExtension(tr("Script File ("), ");;"));

    if(!s.isEmpty())
    {
        QFileInfo info(s);

        if(info.suffix().isEmpty())
        {
            s += getScriptManager()->getFileExtensionAccepted().first();
        }

        saveDirectory = info.path();


        if(getScriptManager()->acceptFile(s))
        {
            getScriptManager()->writeScript(s, true, *getStepManager());
        }
    }
}

void GMainWindow::saveScriptInDir(QString saveDirectory, QString name)
{
    QDir outDir(saveDirectory);

    if(!saveDirectory.isEmpty() && !name.isEmpty() && outDir.exists())
    {
        QString s = saveDirectory + "/" + name + getScriptManager()->getFileExtensionAccepted().first();

        if(getScriptManager()->acceptFile(s))
        {
            getScriptManager()->writeScript(s, true, *getStepManager());
        }
    }
}


void GMainWindow::citationInfo()
{
    QString mainDirPath = QFileDialog::getExistingDirectory(this, tr("Sauvegarder le script dans le dossier..."), _defaultSaveDirPath);
    QDir mainDir(mainDirPath);

    if(mainDir.exists())
    {
        GScriptDocInfoDialog dialog(mainDirPath);

        if (dialog.exec())
        {
            QString scriptFileName = dialog.filename();

            QString outDirPath = mainDirPath + "/" + scriptFileName;
            QDir outDir(outDirPath);
            if (!outDir.exists()) {mainDir.mkdir(scriptFileName);;}

            QString scriptName = dialog.title();
            if (scriptName.isEmpty()) {scriptName = scriptFileName;}

            QString author = dialog.author();
            QString description = dialog.description();

            CDM_CitationInfo citationInfo(getStepManager(), getPluginManager());

            QDateTime date = QDateTime::currentDateTime();
            QString formattedTime = date.toString("dd/MM/yyyy, hh:mm");


            // export script in subdir
            saveScriptInDir(outDirPath, scriptFileName);


            // export RIS information in subir
            QString RISPath = QString("%1/%2.ris").arg(outDirPath, scriptFileName);
            QFile exportRISFile(RISPath);
            if (exportRISFile.open(QFile::WriteOnly | QFile::Text))
            {
                QTextStream stream(&exportRISFile);
                stream << citationInfo.getPluginRIS();
                exportRISFile.close();
            }

            outDir.mkdir("content");
            outDir.mkdir("steps");
            outDir.mkdir("css");


            // export css file
            createCSS(outDirPath + "/css/style.css");


            // create index html file
            QFile findex(outDirPath + "/" + scriptFileName + ".html");
            if (findex.open(QFile::WriteOnly | QFile::Text))
            {
                QTextStream stream(&findex);

                stream << "<!DOCTYPE html>\n";
                stream << "<html>\n";
                stream << "<head>\n";
                stream << "<metacharset=\"utf-8\">\n";
                stream << "<title>Computree Script Documentation</title><link rel=\"stylesheet\" href=\"css/style.css\" /></head>\n";
                stream << "<body>\n";
                stream << "<div class=\"mainBlock\" style = \"display:flex; flex-direction:row; overflow:hidden; min-height:100vh;\">\n";
                stream << "<iframe id=\"frameSummary\" name=\"frameSummary\" src=\"content/summary.html\"  style=\"width:33%; border:none; flex-grow:1;\"></iframe>\n";
                stream << "<iframe id=\"frameContent\" name=\"frameContent\" src=\"content/mainContent.html\" style=\"width:65%; border:none; flex-grow:1;\"></iframe>\n";
                stream << "</div>\n";
                stream << "</body>\n";
                stream << "</html>\n";

                findex.close();
            }


            // Create mainContent.html file
            QFile fMainContent(outDirPath + "/content/mainContent.html");
            if (fMainContent.open(QFile::WriteOnly | QFile::Text))
            {
                QTextStream stream(&fMainContent);

                stream << "<!DOCTYPE html>\n";
                stream << "<html>\n";
                stream << "<head>\n";
                stream << "<metacharset=\"utf-8\">\n";
                stream << "<title>Computree Script Documentation</title><link rel=\"stylesheet\" href=\"../css/style.css\" /></head>\n";
                stream << "<body>\n";
                stream << "<div class=\"mainBlock\">\n";
                stream << "<h1>" << tr("Description du script") << "</h1>\n";
                stream << "<h2>" << tr("Informations sur le script") << "</h1>\n";
                stream << "<p><em>" << tr("Titre du script : ") << "</em><strong>" << scriptName << "</strong></p>";

                if (!author.isEmpty())
                {
                    stream << "<p><em>" << tr("Auteur : ") << "</em><strong>" << author << "</strong></p>";
                }
                stream << "<p><em>" << tr("Fichier script Computree : ") << "</em>" << scriptFileName << ".cts</p>";
                stream << "<p><em>" << tr("Fichier de citations : ") << "</em>" << scriptFileName << ".ris</p>";
                stream << "<p><em>" << tr("Date d'enregistrement : ") << "</em>" << formattedTime << "</p>";

                if (!description.isEmpty())
                {
                    stream << "<p><em>" << tr("Description : ") << "</em>" << "</p>";
                    stream << "<p>" << description << "</p>";
                }

                stream << "<br>\n";
                stream << "<p>" + tr("Pour savoir comment référencer les travaux utilisés dans ce script, merci de consulter la page ") + "<a target=\"frameContent\" href=\"citations.html\">" + tr("Comment citer ce script.") + "<br><br></a>\n";

                stream << "<br>\n";
                stream << "<h2>" << tr("Structure du script") << "</h1>\n";
                stream << "<p>" << tr("Le script est composé de l'enchaînement d'étapes suivant :") << "</p>";
                stream << citationInfo.getScriptStepList();
                stream << "<br>";
                stream << "<h2>" << tr("Plugins utilisés") << "</h1>\n";
                stream << "<p>" << tr("Ce script utilise les plugins Computree suivants :") << "</p>";
                stream << "<div class=\"linksummary01\" >";
                stream << citationInfo.getUsedPlugins();
                stream << "</div>";
                stream << "<br>";
                stream << "</div>\n";
                stream << "</body>\n";
                stream << "</html>\n";

                fMainContent.close();
            }


            // Create citation.html file
            QFile fcitations(outDirPath + "/content/citations.html");
            if (fcitations.open(QFile::WriteOnly | QFile::Text))
            {
                QTextStream stream(&fcitations);

                stream << "<!DOCTYPE html>\n";
                stream << "<html>\n";
                stream << "<head>\n";
                stream << "<metacharset=\"utf-8\">\n";
                stream << "<title>Computree Script Citation</title><link rel=\"stylesheet\" href=\"../css/style.css\" /></head>\n";
                stream << "<body>\n";
                stream << "<div class=\"mainBlock\">\n";
                stream << "<h1>" << tr("Comment citer ce script") << "</h1>\n";
                stream << "<p>" << tr("Pour référencer les travaux utilisés dans ce script, il faut citer :<br>") << "</p>";
                stream << "<ul>";
                stream << "<li>" + tr("La plateforme Computree") + "</li>";
                stream << "<li>" + tr("Les plugins utilisés : <strong>%1</strong>").arg(citationInfo.getPluginListToCite()) + "</li>";
                if (citationInfo.hasStepCitation())
                {
                    stream << "<li>" + tr("Les citations spécifiques liées à certaines étapes utilisées") + "</li>";
                }
                stream << "</ul>";
                stream << "<p>" << tr("L'ensemble des références bibliographiques fournies dans cette page, sont disponibles dans le fichier %1.ris (au format bibliographique standard RIS).").arg(scriptFileName) << "</p>";
                stream << "<br>";
                stream << citationInfo.getPluginAndStepCitations();
                stream << "</div>\n";
                stream << "</body>\n";
                stream << "</html>\n";

                fcitations.close();
            }


            // Create Summmary and steps documentation pages
            QFile f(outDirPath + "/content/summary.html");

            if (f.open(QFile::WriteOnly | QFile::Text))
            {
                QTextStream stream(&f);
                stream << "<!DOCTYPE html>\n";
                stream << "<html>\n";
                stream << "<head>\n";
                stream << "<metacharset=\"utf-8\">\n";
                stream << "<title>Documentation Summary</title>";
                stream << "<link rel=\"stylesheet\" href=\"../css/style.css\" />";
                stream << "</head>\n";
                stream << "<body>";
                stream << "<div class=\"mainBlock\">";
                stream << "<h1>" << tr("Documentation du script") << "</h1>\n";
                stream << "<div class=\"linksummary01\" >";
                stream << "<a target=\"frameContent\" href=\"mainContent.html\">" + tr("Description du script") + "<br><br></a>\n";
                stream << "<a target=\"frameContent\" href=\"citations.html\">" + tr("Comment citer ce script") + "<br><br></a>\n";
                stream << "</div>";
                stream << "<h1>" << tr("Etapes du script") << "</h1>\n";

                QList<CT_VirtualAbstractStep*> steps = citationInfo.steps();

                if (steps.size() > 0) {stream << "<div class=\"linksummary01\" >";}
                for(CT_VirtualAbstractStep* step : steps)
                {
                    stream << "<a target=\"frameContent\" href=\"../steps/" << step->name() << ".html\">" << step->description() << "<br><br></a>\n";

                    // Create documentation page for this step
                    step->generateHTMLDocumentation(outDirPath + "/steps", "../css");
                }
                if (steps.size() > 0) {stream << "</div>";}

                if (steps.size() > 0) {stream << "</div>\n";}
                stream << "</body>\n";
                stream << "</html>";
                f.close();
            }

            // Open script documentation
            QDesktopServices::openUrl(QUrl("file:///" + outDirPath + "/" + scriptFileName + ".html"));
        }
    }

    // Old version
//    QList<CDM_CitationInfo::StepCitationInfo> stepInfos = citationInfo.getScriptTable();
//    GCitationDialog dialog(stepInfos, citationInfo.getPluginAndStepCitations(), citationInfo.getPluginRIS(), this);
//    dialog.exec();
}

void GMainWindow::showAboutDialog()
{
    GAboutDialog dialog;
    dialog.exec();
}

void GMainWindow::showAboutPluginsDialog()
{
    CDM_PluginManager *pluginManager = getPluginManager();

    GAboutPluginsDialog dialog(*pluginManager,
                               this);

    dialog.exec();

    if(dialog.mustReloadStepPlugins())
    {
        loadPlugins();
    }
}

void GMainWindow::showAboutMemory()
{
    GAboutMemory *gMem = new GAboutMemory(nullptr);
    gMem->setWindowModality(Qt::ApplicationModal);
    gMem->show();
}

void GMainWindow::openStepHelp()
{
    QStringList languages = GUI_MANAGER->getLanguageManager()->languageAvailable();
    QString currentLanguageDir = "doc_" + languages.at(GUI_MANAGER->getLanguageManager()->currentLanguage());

    QFile currentFile(currentLanguageDir + "/steps/current.html");
    if (currentFile.exists()) {currentFile.remove();}
    QFile::copy(currentLanguageDir + "/index_default.html", currentLanguageDir + "/steps/current.html");

    QDesktopServices::openUrl(QUrl("file:///" + QCoreApplication::applicationDirPath() + "/" + currentLanguageDir + "/index.html"));
}

void GMainWindow::createStepHelp()
{    
    QStringList languages = GUI_MANAGER->getLanguageManager()->languageAvailable();
    QString currentLanguageDir = "doc_" + languages.at(GUI_MANAGER->getLanguageManager()->currentLanguage());

    QProgressDialog progressDialog(tr("Génération de la documentation des étapes en cours"), tr(""), 0, 100);
    progressDialog.setCancelButton(nullptr);
    progressDialog.setWindowTitle(tr("Génération de l'aide"));
    progressDialog.setMinimumDuration(0);
    progressDialog.setValue(1);
    progressDialog.setModal(true);
    progressDialog.show();
    progressDialog.setValue(1);

    // Create documentation directories if necessary and clean doc_xx/steps content
    QDir dir(currentLanguageDir);
    if (!dir.exists()) {dir.mkpath(".");}

    QDir dir2(currentLanguageDir + "/steps");
    if (!dir2.exists()) {
        dir2.mkpath(".");
    } else {
        dir2.setNameFilters(QStringList() << "*.*");
        dir2.setFilter(QDir::Files);
        foreach(QString dirFile, dir2.entryList())
        {
            dir2.remove(dirFile);
        }
    }

    QDir dir3(currentLanguageDir + "/css");
    if (!dir3.exists()) {dir3.mkpath(".");}


    // Create index.html
    QFile findex(currentLanguageDir + "/index.html");
    if (findex.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream stream(&findex);

        stream << "<!DOCTYPE html>\n";
        stream << "<html>\n";
        stream << "<head>\n";
        stream << "<metacharset=\"utf-8\">\n";
        stream << "<title>Computree Documentation</title><link rel=\"stylesheet\" href=\"css/style.css\" /></head>\n";
        stream << "<body>\n";
        stream << "<div class=\"mainBlock\" style = \"display:flex; flex-direction:row; overflow:hidden; min-height:100vh;\">\n";
        stream << "<iframe id=\"frameSummary\" name=\"frameSummary\" src=\"summary.html\"  style=\"width:33%; border:none; flex-grow:1;\"></iframe>\n";
        stream << "<iframe id=\"frameContent\" name=\"frameContent\" src=\"steps/current.html\" style=\"width:65%; border:none; flex-grow:1;\"></iframe>\n";
        stream << "</div>\n";
        stream << "</body>\n";
        stream << "</html>\n";

        findex.close();
    }

    // Create index_default.html
    QFile findexdefault(currentLanguageDir + "/index_default.html");
    if (findexdefault.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream stream(&findexdefault);

        stream << "<!DOCTYPE html>\n";
        stream << "<html>\n";
        stream << "<head>\n";
        stream << "<metacharset=\"utf-8\">\n";
        stream << "<title>Computree Documentation</title><link rel=\"stylesheet\" href=\"css/style.css\" /></head>\n";
        stream << "<body>\n";
        stream << "<div class=\"mainBlock\">\n";
        stream << "<h1>" << tr("Documentation des étapes Computree") << "</h1>\n";
        stream << "<p>" << tr("Utiliser l'index sur la gauche pour séléctionner l'étape pour laquelle afficher la documentation. ") << "</p>\n";
        stream << "<br>";
        stream << "<p>" << "<a href=\"http://computree.onf.fr\"target=\"_blank\" rel=\"noopener noreferrer\"><strong>" << tr("Site internet officiel de Computree") << "</strong></a></p>\n";
        stream << "</div>\n";
        stream << "</body>\n";
        stream << "</html>\n";

        findexdefault.close();
    }

    QFile::copy(currentLanguageDir + "/index_default.html", currentLanguageDir + "/steps/current.html");


    // Create style.css
    createCSS(currentLanguageDir + "/css/style.css");

    // Create Summmary and steps documentation pages
    QFile f(currentLanguageDir + "/summary.html");
    QFile fst(currentLanguageDir + "/status.txt");

    if (f.open(QFile::WriteOnly | QFile::Text) && fst.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream streamFst(&fst);
        streamFst << "Step\tDescription\tDetailledDescription\tParameters\tInput\tOutput\tDetails\tReferences\n";

        QTextStream stream(&f);
        stream << "<!DOCTYPE html>\n";
        stream << "<html>\n";
        stream << "<head>\n";
        stream << "<metacharset=\"utf-8\">\n";
        stream << "<title>Documentation Summary</title>";
        stream << "<link rel=\"stylesheet\" href=\"css/style.css\" />";
        stream << "</head>\n";
        stream << "<body>";
        stream << "<div class=\"mainBlock\">";
        stream << "<h1>" << tr("Index des étapes") << "</h1>\n";

        CDM_PluginManager *pluginManager = getPluginManager();
        CT_StepsMenu *menu = pluginManager->stepsMenu();
        QList<CT_MenuLevel*> levels = menu->levels();
        QListIterator<CT_MenuLevel*> it(levels);

        progressDialog.setMaximum(levels.size());

        int cpt = 0;
        while(it.hasNext())
        {
            CT_MenuLevel* level = it.next();
            const QString favoritesName = CT_StepsMenu::staticOperationToDisplayableName(CT_StepsMenu::LO_Favorites);

            if(level->displayableName() != favoritesName)
            {
                stream << "<details>";
                stream << "<summary><span class=\"h1summary\">" << level->displayableName() << "</span></summary>";

                const QList<CT_VirtualAbstractStep*> steps = level->steps();

                if (steps.size() > 0) {stream << "<div class=\"linksummary01\" >";}
                for(CT_VirtualAbstractStep* step : steps)
                {
                    stream << "<a target=\"frameContent\" href=\"steps/" << step->name() << ".html\">" << step->description() << "<br></a>\n";

                    // Create documentation page for this step
                    QString helpFileName = step->generateHTMLDocumentation(currentLanguageDir + "/steps", "../css");

                    streamFst << step->name() << "\t";
                    streamFst << step->description() << "\t";
                    if (step->detailledDescription() == "No detailled description for this step") {streamFst << "0\t";}
                    else {streamFst << (step->detailledDescription().size()) << "\t";}
                    streamFst << (step->parametersDescription().isEmpty()?"0":"1") << "\t";
                    streamFst << (step->inputDescription().isEmpty()?"0":"1") << "\t";
                    streamFst << (step->outputDescription().isEmpty()?"0":"1") << "\t";
                    streamFst << (step->detailsDescription().isEmpty()?"0":"1") << "\t";
                    streamFst << (step->getStepRISCitations().isEmpty()?"0":"1") << "\n";
                }
                if (steps.size() > 0) {stream << "</div>";}

                QList<CT_MenuLevel*> sublevels = level->levels();
                QListIterator<CT_MenuLevel*> its(sublevels);
                while(its.hasNext())
                {
                    CT_MenuLevel* subLevel = its.next();
                    stream << "<details>";
                    stream << "<summary><span class=\"h2summary\">" << subLevel->displayableName() << "</span></summary>";

                    const QList<CT_VirtualAbstractStep*> steps = subLevel->steps();

                    if (steps.size() > 0) {stream << "<div class=\"linksummary02\" >";}
                    for(CT_VirtualAbstractStep* step : steps)
                    {
                        stream << "<a target=\"frameContent\" href=\"steps/" << step->name() << ".html\">" << step->description() << "<br></a>\n";

                        // Create documentation page for this step
                        QString helpFileName = step->generateHTMLDocumentation(currentLanguageDir + "/steps", "../css");

                        streamFst << step->name() << "\t";
                        streamFst << step->description() << "\t";
                        if (step->detailledDescription() == "No detailled description for this step") {streamFst << "0\t";}
                        else {streamFst << (step->detailledDescription().size()) << "\t";}
                        streamFst << (step->parametersDescription().isEmpty()?"0":"1") << "\t";
                        streamFst << (step->inputDescription().isEmpty()?"0":"1") << "\t";
                        streamFst << (step->outputDescription().isEmpty()?"0":"1") << "\t";
                        streamFst << (step->detailsDescription().isEmpty()?"0":"1") << "\t";
                        streamFst << (step->getStepRISCitations().isEmpty()?"0":"1") << "\n";
                    }
                    if (steps.size() > 0) {stream << "</div>";}
                    stream << "</details>\n";
                }
                stream << "</details>\n";
            }
            progressDialog.setValue(++cpt);
        }
        stream << "</div>\n";
        stream << "</body>\n";
        stream << "</html>";
        f.close();
        fst.close();
    }

    progressDialog.setValue(progressDialog.maximum());

}
void GMainWindow::createCSS(QString filename)
{
    QFile fcss(filename);
    if (fcss.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream stream(&fcss);

        stream << ".mainBlock\n";
        stream << "{\n";
        stream << "    display: block;\n";
        stream << "    margin-left: auto;\n";
        stream << "    margin-right: auto;\n";
        stream << "    padding-left: 1.25rem;\n";
        stream << "    padding-right: 1.25rem;\n";
        stream << "}\n";
        stream << "\n";
        stream << "h1\n";
        stream << "{\n";
        stream << "    color:darkred;\n";
        stream << "}\n";
        stream << "h2\n";
        stream << "{\n";
        stream << "    color:black;\n";
        stream << "}\n";
        stream << "h3\n";
        stream << "{\n";
        stream << "    color:darkblue;\n";
        stream << "}\n";
        stream << ".parameterDescr\n";
        stream << "{\n";
        stream << "    color:darkblue;\n";
        stream << "}\n";
        stream << ".resultInDescr\n";
        stream << "{\n";
        stream << "    color:darkblue;\n";
        stream << "}\n";
        stream << ".resultOutDescr\n";
        stream << "{\n";
        stream << "    color:darkblue;\n";
        stream << "}\n";
        stream << ".descBlock\n";
        stream << "{\n";
        stream << "    margin-left:10px;\n";
        stream << "}\n";
        stream << ".descBlocklvl2\n";
        stream << "{\n";
        stream << "    margin-left:30px;\n";
        stream << "}\n";
        stream << "summary {\n";
        stream << "  display: flex;\n";
        stream << "  cursor: pointer;\n";
        stream << "}\n";
        stream << "summary::marker\n";
        stream << "{\n";
        stream << "  display: none;\n";
        stream << "}\n";
        stream << ".h1summary\n";
        stream << "{\n";
        stream << "    color:darkred;\n";
        stream << "}\n";
        stream << ".h2summary\n";
        stream << "{\n";
        stream << "    font-weight: 700;\n";
        stream << "    margin-left: 20px;\n";
        stream << "}\n";
        stream << ".linksummary01\n";
        stream << "{\n";
        stream << "    margin-left: 20px;\n";
        stream << "}\n";
        stream << ".linksummary02\n";
        stream << "{\n";
        stream << "    margin-left: 40px;\n";
        stream << "}\n";
        stream << "\n";
        fcss.close();
    }
}

void GMainWindow::cleanItemDrawableOfAllDocuments()
{
    GUI_MANAGER->cleanItemDrawableOfAllDocuments();
}

void GMainWindow::showINeedHelpDialog()
{
    GINeedHelpDialog dialog;
    dialog.exec();
}

void GMainWindow::showStepManagerOptions()
{
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Configuration"));

    QVBoxLayout *layout = new QVBoxLayout();

    GStepManagerOptions *widget = new GStepManagerOptions(&dialog);
    widget->setOptions(getStepManager()->getOptions());

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                                       Qt::Horizontal,
                                                       &dialog);

    layout->addWidget(widget);
    layout->addWidget(buttonBox);

    delete dialog.layout();
    dialog.setLayout(layout);

    connect(buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    if(dialog.exec())
        getStepManager()->setOptions(widget->getOptions());
}

GMainProgressDialog* GMainWindow::createWaitProgressDialog()
{
    return new GMainProgressDialog(this);
}

void GMainWindow::arrangeDocksInTabs()
{
    addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidgetStepManager);
    addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidgetItemDrawableModelManager);
    addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidgetActionsManager);
    addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidgetLog);

    tabifyDockWidget(ui->dockWidgetItemDrawableModelManager, ui->dockWidgetActionsManager);
    tabifyDockWidget(ui->dockWidgetActionsManager, ui->dockWidgetLog);

    addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidgetItemDrawableConfigurationManager);
    addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidgetGraphicsViewSynchronizedGroup);
    tabifyDockWidget(ui->dockWidgetItemDrawableConfigurationManager, ui->dockWidgetGraphicsViewSynchronizedGroup);

    ui->dockWidgetItemDrawableModelManager->raise();
    ui->dockWidgetItemDrawableConfigurationManager->raise();
    ui->dockWidgetStepManager->raise();
}

void GMainWindow::arrangeDocksInColumnWithLogAtBottom()
{
    addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidgetStepManager);
    addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidgetItemDrawableModelManager);
    addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidgetItemDrawableConfigurationManager);
    addDockWidget(Qt::RightDockWidgetArea, ui->dockWidgetActionsManager);
    addDockWidget(Qt::RightDockWidgetArea, ui->dockWidgetGraphicsViewSynchronizedGroup);
    addDockWidget(Qt::BottomDockWidgetArea, ui->dockWidgetLog);

    ui->dockWidgetStepManager->raise();
}

void GMainWindow::arrangeDocksInColumn()
{
    addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidgetStepManager);
    addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidgetItemDrawableModelManager);
    addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidgetItemDrawableConfigurationManager);
    addDockWidget(Qt::RightDockWidgetArea, ui->dockWidgetActionsManager);
    addDockWidget(Qt::RightDockWidgetArea, ui->dockWidgetLog);

    tabifyDockWidget(ui->dockWidgetActionsManager, ui->dockWidgetGraphicsViewSynchronizedGroup);

    ui->dockWidgetActionsManager->raise();
    ui->dockWidgetStepManager->raise();
}

///////////// PRIVATE ////////////

void GMainWindow::initUI()
{
    getPluginManager()->log()->addNormalLogListener(ui->widgetLog);

    _docManagerView = new GDocumentManagerView(this);
    getActionsManager()->setDocumentManagerView(_docManagerView);

    ui->widgetActionsManager->setActionsManager(getActionsManager());
    ui->widgetActionsManager->setDocumentManagerView(_docManagerView);

    getStepManager()->setGuiContext(this);

    m_newStepManagerView = new GStepManager2(*getStepManager(), this);

    m_stepChooserDialog = new GStepChooserDialog(this);
    m_stepChooserDialog->init();

    connect(m_stepChooserDialog, SIGNAL(replaceToDefault(bool)), this, SLOT(replaceStepChooserDialogToDefaults(bool)));

    QAction *actionOpenFile = new QAction(tr("Ouvrir un fichier (CTRL+O)"), this);
    actionOpenFile->setIcon(QIcon(":/Icones/Icones/folder_add_32.png"));
    actionOpenFile->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));

    QAction *actionStart = new QAction(tr("Lancer les traitements (F3)"), this);
    actionStart->setIcon(QIcon(QPixmap(":/Icones/Icones/play.png").scaled(QSize(20,20), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    actionStart->setShortcut(QKeySequence(Qt::Key_F3));

    QAction *actionStop = new QAction(tr("Stop"), this);
    actionStop->setIcon(QIcon(QPixmap(":/Icones/Icones/stop.png").scaled(QSize(20,20), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));

    QAction *actionAckManualMode = new QAction(tr("Valider le mode manuel et continuer les traitements automatiques (F4)"), this);
    actionAckManualMode->setIcon(QIcon(QPixmap(":/Icones/Icones/hand.png").scaled(QSize(20,20), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    actionAckManualMode->setDisabled(true);
    actionAckManualMode->setShortcut(QKeySequence(Qt::Key_F4));

    QAction *actionForwardOneStepDebug = new QAction(tr("Lancer les traitements en mode debug ou avancer d'un pas (F5)"), this);
    actionForwardOneStepDebug->setIcon(QIcon(":/Icones/Icones/play_debug.png"));
    actionForwardOneStepDebug->setShortcut(Qt::Key_F5);

    QAction *actionForwardFastStepDebug = new QAction(tr("Lancer les traitements en mode debug ou avancer de N pas (F6)"), this);
    actionForwardFastStepDebug->setIcon(QIcon(":/Icones/Icones/play_debug_fast.png"));
    actionForwardFastStepDebug->setShortcut(Qt::Key_F6);

    QAction *actionForwardOneStepAutoDebug = new QAction(tr("Avancer de N pas automatiquement jusqu'à la fin"), this);
    actionForwardOneStepAutoDebug->setIcon(QIcon(":/Icones/Icones/play_movie.png"));

    QAction *actionNewDocument = new QAction(tr("Ajouter un nouveau document 3D (F7)"), this);
    actionNewDocument->setIcon(QIcon(QPixmap(":/Icones/Icones/new-document.png").scaled(QSize(20,20), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    actionNewDocument->setShortcut(Qt::Key_F7);

    QAction *actionNew2DDocument = new QAction(tr("Ajouter un nouveau document 2D (F8)"), this);
    actionNew2DDocument->setIcon(QIcon(":/Icones/Icones/new-document-2d.png"));
    actionNew2DDocument->setShortcut(Qt::Key_F8);

    QAction *actionNewItemModelDocument = new QAction(tr("Ajouter un nouveau document de type tableur (F9)"), this);
    actionNewItemModelDocument->setIcon(QIcon(":/Icones/Icones/new-document-treeview.png"));
    actionNewItemModelDocument->setShortcut(Qt::Key_F9);

    QAction *actionStepManagerConfiguration = new QAction(tr("Configurer"), this);
    actionStepManagerConfiguration->setIcon(QIcon(":/Icones/Icones/preferences-system.png"));

    QAction *actionCleanAllDocuments = new QAction(tr("Nettoyer toutes les vues (F10)"), this);
    actionCleanAllDocuments->setIcon(QIcon(":/Icones/Icones/broom.png"));
    actionCleanAllDocuments->setShortcut(Qt::Key_F10);

    QAction *actionINeedHelp = new QAction(tr("J'ai besoin d'aide !!! (F1)"), this);
    actionINeedHelp->setIcon(QIcon(":/Icones/Icones/help.png"));
    actionINeedHelp->setShortcut(Qt::Key_F1);

    QAction *actionQuitter = new QAction(tr("Quitter"), this);

    QAction *actionShowStepManagerView = new QAction(ui->dockWidgetStepManager->windowTitle(), this);
    actionShowStepManagerView->setDisabled(true);
    QAction *actionShowItemDrawableModelManagerView = new QAction(ui->dockWidgetItemDrawableModelManager->windowTitle(), this);
    actionShowItemDrawableModelManagerView->setDisabled(true);
    QAction *actionShowItemDrawableConfigurationManagerView = new QAction(ui->dockWidgetItemDrawableConfigurationManager->windowTitle(), this);
    actionShowItemDrawableConfigurationManagerView->setDisabled(true);
    QAction *actionShowGraphicsSyncManagerView = new QAction(ui->dockWidgetGraphicsViewSynchronizedGroup->windowTitle(), this);
    actionShowGraphicsSyncManagerView->setDisabled(true);
    QAction *actionShowActionsManagerView = new QAction(ui->dockWidgetActionsManager->windowTitle(), this);
    actionShowActionsManagerView->setDisabled(true);
    QAction *actionShowLog = new QAction(ui->dockWidgetLog->windowTitle(), this);
    actionShowLog->setDisabled(true);
    QAction *actionShowStepChooserDialog = new QAction(m_stepChooserDialog->windowTitle(), this);
    actionShowStepChooserDialog->setIcon(QIcon(":/Icones/Icones/add.png"));
    actionShowStepChooserDialog->setToolTip(tr("Ajouter une étape (affiche la fenêtre de choix des étapes, F2)"));
    actionShowStepChooserDialog->setShortcut(QKeySequence(Qt::Key_F2));

    QAction *actionArrangeDocksInTabs = new QAction(tr("Composants en onglets"), this);
    QAction *actionArrangeDocksInColumn = new QAction(tr("Composants en colonne"), this);
    QAction *actionArrangeDocksInColumnWithLogAtBottom = new QAction(tr("Composants en colonne (Log en bas)"), this);

    QAction *actionAboutSteps = new QAction(tr("Documentation des étapes"), this);
    QAction *actionGenerateStepDocumentation = new QAction(tr("Recréer la documentation des étapes"), this);
    actionAProposPlugin = new QAction(tr("Liste des plugins"), this);
    QAction *actionAboutMemory = new QAction(tr("Utilisation mémoire"), this);
    QAction *actionAPropos = new QAction(tr("A propos de Computree..."), this);

    actionSaveScript = new QAction(tr("Exporter script (CTRL+S)"), this);
    actionSaveScript->setIcon(QIcon(":/Icones/Icones/media-floppy.png"));
    actionSaveScript->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));

    actionGetCitationInfo = new QAction(tr("Exporter script documenté (dont citations)"), this);
    actionGetCitationInfo->setIcon(QIcon(":/Icones/Icones/citation.png"));

    ui->toolBar->addAction(actionOpenFile);
    ui->toolBar->addAction(actionShowStepChooserDialog);
    ui->toolBar->addAction(actionStart);
    ui->toolBar->addAction(actionStop);
    ui->toolBar->addAction(actionSaveScript);
    ui->toolBar->addAction(actionGetCitationInfo);


    ui->toolBar->addSeparator();
    ui->toolBar->addAction(actionAckManualMode);

    ui->toolBar->addSeparator();
    ui->toolBar->addAction(actionForwardOneStepDebug);
    ui->toolBar->addAction(actionForwardFastStepDebug);
    QSpinBox *fastForwardSpinBox = new QSpinBox(this);
    fastForwardSpinBox->setMinimum(1);
    fastForwardSpinBox->setMaximum(9999999);
    fastForwardSpinBox->setMaximumWidth(90);
    fastForwardSpinBox->setToolTip(tr("Indiquez le nombre de pas à sauter avant le prochain arrêt de l'étape"));

    ui->toolBar->addWidget(fastForwardSpinBox);
    ui->toolBar->addAction(actionForwardOneStepAutoDebug);

    QSpinBox *jumpAutoDebugSpinBox = new QSpinBox(this);
    jumpAutoDebugSpinBox->setMinimum(1);
    jumpAutoDebugSpinBox->setMaximum(9999999);
    jumpAutoDebugSpinBox->setMaximumWidth(70);
    jumpAutoDebugSpinBox->setToolTip(tr("Indiquez le nombre de pas à sauter avant la prochaine actualisation automatique"));
    ui->toolBar->addWidget(jumpAutoDebugSpinBox);

    QSpinBox *timeAutoDebugSpinBox = new QSpinBox(this);
    timeAutoDebugSpinBox->setMinimum(1);
    timeAutoDebugSpinBox->setMaximum(9999999);
    timeAutoDebugSpinBox->setMaximumWidth(70);
    timeAutoDebugSpinBox->setToolTip(tr("Indiquez le temps en ms entre deux actualisation automatique"));
    ui->toolBar->addWidget(timeAutoDebugSpinBox);

    ui->toolBar->addSeparator();
    ui->toolBar->addAction(actionNewDocument);
    ui->toolBar->addAction(actionNew2DDocument);
    ui->toolBar->addAction(actionNewItemModelDocument);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(actionStepManagerConfiguration);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(actionCleanAllDocuments);
    ui->toolBar->addAction(actionINeedHelp);

    ui->menuFichier->addAction(actionOpenFile);
    ui->menuFichier->addSeparator();
    ui->menuFichier->addAction(actionSaveScript);
    ui->menuFichier->addAction(actionGetCitationInfo);
    ui->menuFichier->addSeparator();
    ui->menuFichier->addAction(actionNewDocument);
    ui->menuFichier->addAction(actionNew2DDocument);
    ui->menuFichier->addAction(actionNewItemModelDocument);
    ui->menuFichier->addSeparator();
    ui->menuFichier->addAction(actionQuitter);

    ui->menuEdition->addAction(actionStart);
    ui->menuEdition->addAction(actionStop);
    ui->menuEdition->addSeparator();
    ui->menuEdition->addAction(actionAckManualMode);
    ui->menuEdition->addSeparator();
    ui->menuEdition->addAction(actionForwardOneStepDebug);
    ui->menuEdition->addAction(actionForwardFastStepDebug);
    ui->menuEdition->addAction(actionForwardOneStepAutoDebug);

    ui->menuFenetre->addAction(actionShowStepManagerView);
    ui->menuFenetre->addAction(actionShowItemDrawableModelManagerView);
    ui->menuFenetre->addAction(actionShowItemDrawableConfigurationManagerView);
    ui->menuFenetre->addAction(actionShowGraphicsSyncManagerView);
    ui->menuFenetre->addAction(actionShowActionsManagerView);
    ui->menuFenetre->addAction(actionShowLog);
    ui->menuFenetre->addAction(actionShowStepChooserDialog);
    ui->menuFenetre->addAction(actionArrangeDocksInTabs);
    ui->menuFenetre->addAction(actionArrangeDocksInColumnWithLogAtBottom);
    ui->menuFenetre->addAction(actionArrangeDocksInColumn);

    ui->menuAide->addAction(actionINeedHelp);
    ui->menuAide->addAction(actionAboutSteps);
    ui->menuAide->addAction(actionGenerateStepDocumentation);
    ui->menuAide->addAction(actionAProposPlugin);
    ui->menuAide->addAction(actionAboutMemory);
    ui->menuAide->addAction(actionAPropos);

    ui->horizontalLayoutDocumentView->addWidget(_docManagerView);
    ui->verticalLayoutStepTreeView->addWidget(m_newStepManagerView);

    _itemDrawableModelView = new GMultipleItemDrawableModelManager(this);
    _itemDrawableModelView->setDocumentManagerView(_docManagerView);
    ui->verticalLayoutItemDrawableModelView->addWidget(_itemDrawableModelView);

    connect(_itemDrawableModelView, SIGNAL(newGradientSelected(QLinearGradient)), _docManagerView, SLOT(newGradientSelected(QLinearGradient)));
    _docManagerView->newGradientSelected(_itemDrawableModelView->gradientSelected());

    _itemDrawableConfigurationView = new GItemDrawableConfigurationManagerView(this);
    ui->verticalLayoutItemDrawableConfigurationManager->addWidget(_itemDrawableConfigurationView);

    m_itemDrawableConfigurationSyncWithGraphicsViewManager.setItemDrawableConfigurationView(_itemDrawableConfigurationView);

    _graphicsViewSyncGroupView = new GGraphicsViewSynchronizedGroup(this);
    ui->verticalLayoutGraphicsViewSyncGroup->addWidget(_graphicsViewSyncGroupView);

    _docManagerView->setSyncManager(_graphicsViewSyncGroupView);

    _permanentLabelForMessage = new QLabel(ui->statusbar);
    ui->statusbar->addPermanentWidget(_permanentLabelForMessage);

    actionStop->setEnabled(false);
    actionSaveScript->setEnabled(false);
    actionGetCitationInfo->setEnabled(true);

    QStringList dLA = GUI_MANAGER->getLanguageManager()->displayableLanguageAvailable();
    QStringList lA = GUI_MANAGER->getLanguageManager()->languageAvailable();
    int i = 0;

    QActionGroup *actGroup = new QActionGroup(this);
    actGroup->setExclusive(true);

    foreach (QString l, dLA) {
        QAction *languageAction = ui->menuLangue->addAction(QIcon(QString(":/Icones/Icones/") + "flag_" + lA.at(i) + ".png"), l, this, SLOT(changeLanguage()));
        languageAction->setData(i);
        languageAction->setCheckable(true);
        languageAction->setChecked(GUI_MANAGER->getLanguageManager()->currentLanguage() == i);
        actGroup->addAction(languageAction);
        ++i;
    }

    connect(actionNewDocument, SIGNAL(triggered()), this, SLOT(newDocument()));
    connect(actionNew2DDocument, SIGNAL(triggered()), this, SLOT(new2DDocument()));
    connect(actionNewItemModelDocument, SIGNAL(triggered()), this, SLOT(newItemModelDocument()));
    connect(actionOpenFile, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(actionQuitter, SIGNAL(triggered()), this, SLOT(close()));

    connect(actionStart, SIGNAL(triggered()), m_newStepManagerView, SLOT(executeStep()));
    connect(actionStop, SIGNAL(triggered()), getStepManager(), SLOT(stop()));
    connect(actionSaveScript, SIGNAL(triggered()), this, SLOT(saveScriptUsingDefaultPath()));
    connect(actionGetCitationInfo, SIGNAL(triggered()), this, SLOT(citationInfo()));

    connect(actionAckManualMode, SIGNAL(triggered()), getStepManager(), SLOT(quitManualMode()));

    connect(actionForwardOneStepDebug, SIGNAL(triggered()), m_newStepManagerView, SLOT(executeOrForwardStepInDebugMode()));
    connect(actionForwardFastStepDebug, SIGNAL(triggered()), m_newStepManagerView, SLOT(executeOrForwardStepFastInDebugMode()));
    connect(actionForwardOneStepAutoDebug, SIGNAL(triggered()), m_newStepManagerView, SLOT(executeOrForwardStepAutoInDebugMode()));
    connect(fastForwardSpinBox, SIGNAL(valueChanged(int)), getStepManager(), SLOT(setFastForwardJumpInDebugMode(int)));
    connect(jumpAutoDebugSpinBox, SIGNAL(valueChanged(int)), getStepManager(), SLOT(setNJumpInAutoDebugMode(int)));
    connect(timeAutoDebugSpinBox, SIGNAL(valueChanged(int)), getStepManager(), SLOT(setTimeToSleepInAutoDebugMode(int)));

    connect(actionStepManagerConfiguration, SIGNAL(triggered()), this, SLOT(showStepManagerOptions()));

    connect(actionCleanAllDocuments, SIGNAL(triggered()), this, SLOT(cleanItemDrawableOfAllDocuments()));
    connect(actionINeedHelp, SIGNAL(triggered()), this, SLOT(showINeedHelpDialog()));

    connect(actionShowStepManagerView, SIGNAL(triggered()), ui->dockWidgetStepManager, SLOT(showNormal()));
    connect(actionShowItemDrawableModelManagerView, SIGNAL(triggered()), ui->dockWidgetItemDrawableModelManager, SLOT(showNormal()));
    connect(actionShowItemDrawableConfigurationManagerView, SIGNAL(triggered()), ui->dockWidgetItemDrawableConfigurationManager, SLOT(showNormal()));
    connect(actionShowGraphicsSyncManagerView, SIGNAL(triggered()), ui->dockWidgetGraphicsViewSynchronizedGroup, SLOT(showNormal()));
    connect(actionShowActionsManagerView, SIGNAL(triggered()), ui->dockWidgetActionsManager, SLOT(showNormal()));
    connect(actionShowLog, SIGNAL(triggered()), ui->dockWidgetLog, SLOT(showNormal()));
    connect(actionShowStepChooserDialog, SIGNAL(triggered()), m_stepChooserDialog, SLOT(toggleVisibility()));
    connect(actionArrangeDocksInTabs, SIGNAL(triggered()), this, SLOT(arrangeDocksInTabs()));
    connect(actionArrangeDocksInColumnWithLogAtBottom, SIGNAL(triggered()), this, SLOT(arrangeDocksInColumnWithLogAtBottom()));
    connect(actionArrangeDocksInColumn, SIGNAL(triggered()), this, SLOT(arrangeDocksInColumn()));

    connect(ui->actionCascade, SIGNAL(triggered()), _docManagerView, SLOT(cascadeSubWindows()));
    connect(ui->actionTile, SIGNAL(triggered()), _docManagerView, SLOT(tileSubWindows()));

    connect(actionAPropos, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
    connect(actionAProposPlugin, SIGNAL(triggered()), this, SLOT(showAboutPluginsDialog()));
    connect(actionAboutMemory, SIGNAL(triggered()), this, SLOT(showAboutMemory()));
    connect(actionAboutSteps, SIGNAL(triggered()), this, SLOT(openStepHelp()));
    connect(actionGenerateStepDocumentation, SIGNAL(triggered()), this, SLOT(createStepHelp()));

    connect(getStepManager(), SIGNAL(started(bool)), actionStart, SLOT(setDisabled(bool)));
    connect(getStepManager(), SIGNAL(started(bool)), actionStop, SLOT(setEnabled(bool)));
    connect(getStepManager(), SIGNAL(started(bool)), actionStepManagerConfiguration, SLOT(setDisabled(bool)));
    connect(getStepManager(), SIGNAL(started(bool)), actionForwardOneStepDebug, SLOT(setDisabled(bool)));
    connect(getStepManager(), SIGNAL(started(bool)), actionForwardFastStepDebug, SLOT(setDisabled(bool)));
    connect(getStepManager(), SIGNAL(started(bool)), actionForwardOneStepAutoDebug, SLOT(setDisabled(bool)));

    connect(getStepManager(), SIGNAL(completed(bool)), actionStart, SLOT(setEnabled(bool)));
    connect(getStepManager(), SIGNAL(completed(bool)), actionStop, SLOT(setDisabled(bool)));
    connect(getStepManager(), SIGNAL(completed(bool)), actionStepManagerConfiguration, SLOT(setEnabled(bool)));
    connect(getStepManager(), SIGNAL(completed(bool)), actionForwardOneStepDebug, SLOT(setEnabled(bool)));
    connect(getStepManager(), SIGNAL(completed(bool)), actionForwardFastStepDebug, SLOT(setEnabled(bool)));
    connect(getStepManager(), SIGNAL(completed(bool)), actionForwardOneStepAutoDebug, SLOT(setEnabled(bool)));

    connect(getStepManager(), SIGNAL(stepInManualMode(bool)), actionAckManualMode, SLOT(setEnabled(bool)));

    connect(getStepManager(), SIGNAL(stepFinishExecuted(CT_VirtualAbstractStep*)), _docManagerView, SLOT(stepFinished(CT_VirtualAbstractStep*)), Qt::QueuedConnection);
    connect(getStepManager(), SIGNAL(stepRequiredManualMode(CT_VirtualAbstractStep*)), _docManagerView, SLOT(stepRequiredManualMode(CT_VirtualAbstractStep*)), Qt::DirectConnection);

    connect(getStepManager(), SIGNAL(stepWaitForAckInDebugMode(bool)), actionStart, SLOT(setEnabled(bool)));
    connect(getStepManager(), SIGNAL(stepWaitForAckInDebugMode(bool)), actionForwardOneStepDebug, SLOT(setEnabled(bool)));
    connect(getStepManager(), SIGNAL(stepWaitForAckInDebugMode(bool)), actionForwardFastStepDebug, SLOT(setEnabled(bool)));
    connect(getStepManager(), SIGNAL(stepWaitForAckInDebugMode(bool)), actionForwardOneStepAutoDebug, SLOT(setEnabled(bool)));

    connect(getStepManager(), SIGNAL(stepAdded(CT_VirtualAbstractStep*)), this, SLOT(stepAdded(CT_VirtualAbstractStep*)), Qt::QueuedConnection);
    connect(getStepManager(), SIGNAL(stepToBeRemoved(CT_VirtualAbstractStep*)), this, SLOT(stepToBeRemoved(CT_VirtualAbstractStep*)), Qt::QueuedConnection);

    connect(getStepManager(), SIGNAL(stepNeedShowMessage(QString)), _permanentLabelForMessage, SLOT(setText(QString)));

    connect(m_newStepManagerView, SIGNAL(currentStepChanged(CT_VirtualAbstractStep*)), m_stepChooserDialog, SLOT(setCurrentStep(CT_VirtualAbstractStep*)), Qt::QueuedConnection);
    connect(m_newStepManagerView, SIGNAL(locateStepInMenu(CT_VirtualAbstractStep*)), m_stepChooserDialog->stepsChooserWidget(), SLOT(searchOriginalStepAndExpandParent(CT_VirtualAbstractStep*)));
    connect(m_stepChooserDialog->stepsChooserWidget(), SIGNAL(displayNameConfigurationChanged(GStepViewDefault::DisplayNameConfigs)), m_newStepManagerView, SLOT(setStepNameConfiguration(GStepViewDefault::DisplayNameConfigs)));
    connect(m_stepChooserDialog, SIGNAL(addStepToCurrentStepOrToRootAndConfigure(CT_VirtualAbstractStep*)), m_newStepManagerView, SLOT(addStepToCurrentStepOrToRootAndConfigure(CT_VirtualAbstractStep*)), Qt::DirectConnection);

    connect(_docManagerView, SIGNAL(documentToBeClosed(DM_DocumentView*)), this, SLOT(documentToBeClosed(DM_DocumentView*)), Qt::DirectConnection);
    connect(_docManagerView, SIGNAL(documentActivated(DM_DocumentView*)), _itemDrawableConfigurationView, SLOT(setDocument(DM_DocumentView*)), Qt::DirectConnection);

    connect(ui->dockWidgetStepManager, SIGNAL(visibilityChanged(bool)), actionShowStepManagerView, SLOT(setDisabled(bool)));
    connect(ui->dockWidgetItemDrawableModelManager, SIGNAL(visibilityChanged(bool)), actionShowItemDrawableModelManagerView, SLOT(setDisabled(bool)));
    connect(ui->dockWidgetItemDrawableConfigurationManager, SIGNAL(visibilityChanged(bool)), actionShowItemDrawableConfigurationManagerView, SLOT(setDisabled(bool)));
    connect(ui->dockWidgetGraphicsViewSynchronizedGroup, SIGNAL(visibilityChanged(bool)), actionShowGraphicsSyncManagerView, SLOT(setDisabled(bool)));
    connect(ui->dockWidgetActionsManager, SIGNAL(visibilityChanged(bool)), actionShowActionsManagerView, SLOT(setDisabled(bool)));
    connect(ui->dockWidgetLog, SIGNAL(visibilityChanged(bool)), actionShowLog, SLOT(setDisabled(bool)));

    fastForwardSpinBox->setValue(getStepManager()->getOptions().getFastJumpValueInDebugMode());
    jumpAutoDebugSpinBox->setValue(getStepManager()->getOptions().getNJumpInAutoDebugMode());
    timeAutoDebugSpinBox->setValue(getStepManager()->getOptions().getTimeToSleepInAutoDebugMode());

    showStepChooser();

}

void GMainWindow::loadPlugins(bool showMessageIfNoPluginsFounded)
{
    CDM_PluginManager *pluginManager = getPluginManager();

    bool reload = true;
    bool first = true;

    while(reload)
    {
        pluginManager->load();

        if(showMessageIfNoPluginsFounded)
        {
            if((reload = (!pluginManager->isAPluginLoaded())) == true)
            {
                QMessageBox msgBox(this);
                msgBox.setText(tr("Attention aucun plugin n'a été trouvé dans :\n%1").arg(pluginManager->getSearchPath()));
                msgBox.setInformativeText(tr("Voulez-vous spécifier dans quel dossier rechercher les plugins ?"));
                msgBox.setDetailedText(QString("%1").arg(pluginManager->getError()));
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.addButton(tr("Parcourir..."), QMessageBox::AcceptRole);
                QPushButton *rejectButton = msgBox.addButton(tr("Annuler"), QMessageBox::RejectRole);

                msgBox.exec();

                if(msgBox.clickedButton() == rejectButton)
                {
                    reload = false;
                }
                else
                {
                    QString dirPath = QFileDialog::getExistingDirectory(this, tr("Dossier contenant les plugins..."), pluginManager->getSearchPath());

                    if(dirPath.isNull() == false)
                    {
                        pluginManager->setSearchPath(dirPath);
                    }
                    else
                    {
                        reload = false;
                    }
                }

                first = false;
            }
            else if(!pluginManager->getError().isEmpty())
            {
                QMessageBox msgBox(this);

                msgBox.setText(tr("Attention il y a eu des erreurs lors du chargement des plugins du dossier :\n%1").arg(pluginManager->getSearchPath()));
                msgBox.setDetailedText(QString("%1").arg(pluginManager->getError()));
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.exec();
            }
            else if(!first)
            {
                QMessageBox::information(this, tr("Succès"), tr("Le plugin %1 a été chargé avec succès !").arg(pluginManager->getPluginName()));
            }
        }
        else
        {
            reload = false;
        }
    }
}

void GMainWindow::showMessageIfScriptBackupIsAvailable()
{
    if(getStepManager()->isScriptBackupAvailable()) {

        int ret = QMessageBox::warning(this, tr("Récupération automatique"), tr("<html>L'application a semble-t-il rencontrée un problème "
                                                                                "lors de la dernière exécution des étapes. Un script a été "
                                                                                "sauvegardé automatiquement afin de rétablir votre dernière "
                                                                                "configuration.<br/><br/><b>Voulez-vous recharger votre dernière "
                                                                                "configuration connue ?</b></html>"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(ret == QMessageBox::Yes)
        {
            getStepManager()->restoreScriptBackup();
        }
        else if(ret == QMessageBox::No)
        {
            QFile(getStepManager()->getScriptBackupFilePath()).remove();
        }
    }
}

void GMainWindow::loadConfiguration()
{
    m_inLoadConfiguration = true;
    bool mustShowMaximized = false;

    CONFIG_FILE->beginGroup("MainWindow");

    _defaultOpenDirPath = CONFIG_FILE->value("defaultOpenDirPath", "").toString();
    _defaultSaveDirPath = CONFIG_FILE->value("defaultSaveDirPath", "").toString();

    if(CONFIG_FILE->value("windowSize", QVariant()).isValid()) {
        QSize size = CONFIG_FILE->value("windowSize", QSize()).toSize();
        QPoint pos = CONFIG_FILE->value("windowPos", QPoint()).toPoint();
        bool isMaximized = CONFIG_FILE->value("windowIsMaximized", false).toBool();

        setWindowState(Qt::WindowNoState);

        if(!isMaximized) {
            resize(size);
            move(pos);
        } else if(!windowState().testFlag(Qt::WindowMaximized)) {
            mustShowMaximized = true;
        }

    } else if(!windowState().testFlag(Qt::WindowMaximized)){
        setWindowState(Qt::WindowNoState);
        mustShowMaximized = true;
    }

    CONFIG_FILE->beginGroup("Document");

    bool ok;
    int nDoc = CONFIG_FILE->value("nDocument", 0).toInt(&ok);

    CONFIG_FILE->endGroup(); // Document

    CONFIG_FILE->endGroup(); // MainWindow

    if(ok)
    {
        for(int i=0; i<nDoc; ++i)
        {
            CONFIG_FILE->beginGroup("MainWindow");
            CONFIG_FILE->beginGroup("Document");
            CONFIG_FILE->beginGroup(QString("Doc%1").arg(i));

            QString type =CONFIG_FILE->value("Type", "").toString();
            QByteArray geometry = CONFIG_FILE->value("Geometry", "").toByteArray();

            CONFIG_FILE->endGroup(); // DocX
            CONFIG_FILE->endGroup(); // Document
            CONFIG_FILE->endGroup(); // MainWindow

            if(type == "2D")
                new2DDocument();
            else if(type == "3D")
                newDocument();
            else if(type == "TAB")
                newItemModelDocument();
            else
                ok = false;

            CONFIG_FILE->beginGroup("MainWindow");
            CONFIG_FILE->beginGroup("Document");
            CONFIG_FILE->beginGroup(QString("Doc%1").arg(i));

            DM_DocumentView *view = getDocumentManagerView()->getDocumentView(i);

            if(view != nullptr) {
                if(CONFIG_FILE->value("Size", QVariant()).isNull()) {
                    view->restoreGeometry(geometry);
                } else {
                    QSize size = CONFIG_FILE->value("Size", QSize()).toSize();
                    QPoint pos = CONFIG_FILE->value("Pos", QPoint()).toPoint();
                    bool isMaximized = CONFIG_FILE->value("Maximized", false).toBool();

                    if(isMaximized)
                        view->setMaximized(true);
                    else {
                        view->setMaximized(false);
                        view->resize(size);
                        view->move(pos);
                    }
                }
            }

            CONFIG_FILE->endGroup(); // DocX
            CONFIG_FILE->endGroup(); // Document
            CONFIG_FILE->endGroup(); // MainWindow
        }
    }

    loadPlugins();

    m_inLoadConfiguration = false;

    QApplication::processEvents();

    if(getDocumentManagerView()->nbDocumentView() == 0) {
        newDocument();

        DM_DocumentView *view = getDocumentManagerView()->getDocumentView(0);

        if(view != nullptr)
            view->setMaximized(true);
    }

    showMessageIfScriptBackupIsAvailable();

    CONFIG_FILE->beginGroup("MainWindow");

    restoreState(CONFIG_FILE->value("windowState").toByteArray());

    CONFIG_FILE->beginGroup("StepsChooser");

    QPoint defaultPos;
    QSize defaultSize;
    computeStepChooserDialogDefaults(defaultPos, defaultSize, true);

    bool defaultVisible = true;

    QSize size = CONFIG_FILE->value("Size", defaultSize).toSize();
    QPoint pos = CONFIG_FILE->value("Pos", defaultPos).toPoint();

    m_stepChooserDialog->stepsChooserWidget()->setDisplayConfiguration((GStepViewDefault::DisplayNameConfigs)CONFIG_FILE->value("StepNameConfig", (int)m_stepChooserDialog->stepsChooserWidget()->displayConfiguration()).toInt());
    m_stepChooserDialog->stepsChooserWidget()->proxy()->setFilterConfiguration((DM_StepTreeViewDefaultProxyModel::FilterConfigs)CONFIG_FILE->value("FilterConfig", (int)m_stepChooserDialog->stepsChooserWidget()->proxy()->filterConfiguration()).toInt());
    m_stepChooserDialog->stepsChooserWidget()->setShowAtLastPositionCheckboxVisible(true);
    m_stepChooserDialog->stepsChooserWidget()->setShowAtLastPosition(CONFIG_FILE->value("ShowAtLastPosition", m_stepChooserDialog->stepsChooserWidget()->showAtLastPosition()).toBool());
    m_stepChooserDialog->stepsChooserWidget()->setShowAtLastPosition(CONFIG_FILE->value("ShowAtLastPosition", m_stepChooserDialog->stepsChooserWidget()->showAtLastPosition()).toBool());
    defaultVisible = CONFIG_FILE->value("Visible", defaultVisible).toBool();

    if(!m_stepChooserDialog->stepsChooserWidget()->showAtLastPosition()) {
        pos = defaultPos;
        size = defaultSize;
        defaultVisible = true;
    }

    m_stepChooserDialog->resize(size);
    m_stepChooserDialog->move(pos);

    if(defaultVisible)
        showStepChooser();
    else
        hideStepChooser();

    CONFIG_FILE->endGroup(); // StepsChooser
    CONFIG_FILE->endGroup(); // MainWindow

    if(mustShowMaximized)
        showMaximized();

    QStringList languages = GUI_MANAGER->getLanguageManager()->languageAvailable();
    QString currentLanguage = languages.at(GUI_MANAGER->getLanguageManager()->currentLanguage());

    CT_AbstractStep::setCurrentLangage(currentLanguage);

    QDir docDir("doc_" + currentLanguage);
    if (!docDir.exists()) {createStepHelp();}
}

void GMainWindow::computeStepChooserDialogDefaults(QPoint &defaultPos, QSize &defaultSize, bool left)
{
    if (left)
    {
        defaultPos = ui->dockWidgetStepManager->pos();
        defaultPos.setY(defaultPos.y() + 90);
        defaultPos.setX(defaultPos.x() + ui->dockWidgetStepManager->width() + 10);
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect rec = screen->geometry();
        defaultSize.setWidth(m_stepChooserDialog->width());
        defaultSize.setHeight(rec.height()-defaultPos.y()-100);
    } else {
        defaultPos = this->pos();
        defaultPos.setY(ui->dockWidgetStepManager->pos().y() + 90);
        defaultPos.setX(defaultPos.x() + this->width() - m_stepChooserDialog->width() - 3);
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect rec = screen->geometry();
        defaultSize.setWidth(m_stepChooserDialog->width());
        defaultSize.setHeight(rec.height()-defaultPos.y()-100);
    }
}

void GMainWindow::replaceStepChooserDialogToDefaults(bool left)
{
    QPoint defaultPos;
    QSize defaultSize;
    computeStepChooserDialogDefaults(defaultPos, defaultSize, left);

    m_stepChooserDialog->resize(defaultSize);
    m_stepChooserDialog->move(defaultPos);
}

void GMainWindow::writeConfiguration()
{
    CONFIG_FILE->beginGroup("MainWindow");

    CONFIG_FILE->setValue("defaultOpenDirPath", _defaultOpenDirPath);
    CONFIG_FILE->setValue("defaultSaveDirPath", _defaultSaveDirPath);
    CONFIG_FILE->setValue("windowState", saveState());
    CONFIG_FILE->setValue("windowSize", size());
    CONFIG_FILE->setValue("windowPos", pos());
    CONFIG_FILE->setValue("windowIsMaximized", windowState().testFlag(Qt::WindowMaximized));

    CONFIG_FILE->beginGroup("Document");
    CONFIG_FILE->setValue("nDocument", getDocumentManagerView()->nbDocumentView());

    for(int i=0; i<getDocumentManagerView()->nbDocumentView(); ++i)
    {
        DM_DocumentView *docV = getDocumentManagerView()->getDocumentView(i);
        QString type = "unknown";

        CONFIG_FILE->beginGroup(QString("Doc%1").arg(i));

        if(dynamic_cast<GDocumentViewForGraphics*>(docV) != nullptr)
        {
            if(!((GDocumentViewForGraphics*)docV)->getGraphicsList().isEmpty())
            {
                if((((GDocumentViewForGraphics*)docV)->getGraphicsList().first())->is2DView())
                    type = "2D";
                else
                    type = "3D";
            }
        }
        else if(dynamic_cast<GDocumentViewForItemModel*>(docV) != nullptr)
        {
            type = "TAB";
        }

        CONFIG_FILE->setValue("Type", type);
        CONFIG_FILE->setValue("Geometry", docV->saveGeometry());
        CONFIG_FILE->setValue("Size", docV->size());
        CONFIG_FILE->setValue("Pos", docV->pos());
        CONFIG_FILE->setValue("Maximized", docV->isMaximized());
        CONFIG_FILE->endGroup();
    }

    CONFIG_FILE->endGroup(); // Document

    CONFIG_FILE->beginGroup("StepsChooser");

    CONFIG_FILE->setValue("Geometry", m_stepChooserDialog->saveGeometry());
    CONFIG_FILE->setValue("Size", m_stepChooserDialog->size());
    CONFIG_FILE->setValue("Pos", m_stepChooserDialog->pos());
    CONFIG_FILE->setValue("FilterConfig", (int)m_stepChooserDialog->stepsChooserWidget()->proxy()->filterConfiguration());
    CONFIG_FILE->setValue("StepNameConfig", (int)m_stepChooserDialog->stepsChooserWidget()->displayConfiguration());
    CONFIG_FILE->setValue("ShowAtLastPosition", m_stepChooserDialog->stepsChooserWidget()->showAtLastPosition());
    CONFIG_FILE->setValue("Visible", m_stepChooserDialog->isVisible());

    CONFIG_FILE->endGroup(); // StepsChooser

    CONFIG_FILE->endGroup(); // MainWindow
}

void GMainWindow::loadScriptError(CDM_ScriptProblem &problem)
{
    QString title = tr("Erreur de chargement du script");
    QString info = tr("Une erreur est survenue lors de la lecture du script :<br/><br/><i>%1</i>").arg(problem.getProblemString());

    // problem of type : plugin name not informed in script or invalid
    if((problem.getProblemType() == CDM_ScriptProblem::TOP_PluginNotInformed) || (problem.getProblemType() == CDM_ScriptProblem::TOP_PluginNotFound)) {

        int s = getPluginManager()->countPluginLoaded();

        if(s > 0)
        {
            // solution is to choose a plugin to use
            QStringList l;

            int ci = 0;

            QString pn = problem.getProblemPluginName().trimmed();

            for(int i=0; i<s; ++i) {
                QString name = getPluginManager()->getPluginName(i);

                if(!pn.isEmpty() && name.startsWith(pn))
                    ci = i;

                l << name;
            }

            bool ok;
            QString pluginName = QInputDialog::getItem(this, title, tr("<html>%1<br/><br/><b>Choisissez le plugin à utiliser ?</b></html>").arg(info), l, ci, false, &ok);

            if(ok) {
                problem.setSolutionPluginToUse(l.indexOf(pluginName));
                return;
            }
        }

        // problem of type : step not found in the plugin
    } else if(problem.getProblemType() == CDM_ScriptProblem::TOP_StepNotFound) {

        // solution is to choose a step
        QDialog dialog(this);
        dialog.setWindowTitle(title);

        QHBoxLayout *hLayout = new QHBoxLayout(&dialog);
        QVBoxLayout *vLayout = new QVBoxLayout();

        QLabel *label = new QLabel(&dialog);
        label->setText(tr("<html>%1<br/><br/><b>Choisissez une étape de remplacement :</b></html>").arg(info));

        // create the view of steps available in the plugin currently used
        GStepViewDefault *view = new GStepViewDefault(&dialog);
        view->init(*getPluginManager());

        // plugin currently used
        view->proxy()->setUseStepsOfPlugins(QList<CT_AbstractStepPlugin*>() << getPluginManager()->getPlugin(problem.getSolutionPluginToUse()));

        // set if must show StepLoadFile, StepCanBeAddedFirst and StepGeneric
        view->proxy()->setTypeVisible(DM_StepsFromPluginsModelConstructor::IT_StepCBAF, true);
        view->proxy()->setTypeVisible(DM_StepsFromPluginsModelConstructor::IT_StepLF, problem.getParentStep() == nullptr);
        view->proxy()->setTypeVisible(DM_StepsFromPluginsModelConstructor::IT_StepG, problem.getParentStep() != nullptr);

        // set the parent step to use
        view->proxy()->setParentStep(problem.getParentStep());

        // don't show step not compatible
        view->proxy()->setShowStepNotCompatible(false);

        // reconstruct the view
        view->reconstruct();

        // search the step
        view->searchStepByNameAndExpandParent(problem.getProblemStepName());

        vLayout->addWidget(label);
        vLayout->addWidget(view);

        hLayout->addLayout(vLayout);
        QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Vertical, &dialog);
        hLayout->addWidget(buttons);

        connect(buttons, SIGNAL(accepted()), &dialog, SLOT(accept()));
        connect(buttons, SIGNAL(rejected()), &dialog, SLOT(reject()));

        if(dialog.exec() == QDialog::Accepted) {
            QModelIndexList l = view->treeView()->selectionModel()->selectedIndexes();

            if(!l.isEmpty()) {
                // if a step was choosed by user
                problem.setSolutionUseStep(view->constructor()->stepFromIndex(view->proxy()->mapToSource(l.first())));

                if(problem.getSolutionUseStep() != nullptr)
                    return;
            }
        }

        // problem of type : step can not be configured with element of script
    } else if((problem.getProblemType() == CDM_ScriptProblem::TOP_StepCanNotBeConfigured)
              || (problem.getProblemType() == CDM_ScriptProblem::TOP_StepPreParamsCanNotBeConfigured)
              || (problem.getProblemType() == CDM_ScriptProblem::TOP_StepInputParamsCanNotBeConfigured)
              || (problem.getProblemType() == CDM_ScriptProblem::TOP_StepPostParamsCanNotBeConfigured)) {

        if(QMessageBox::critical(this, title, tr("<html>%1<br/><br/><b>Voulez vous configurer l'étape manuellement ?</b></html>").arg(info), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            // solution is to configure manually the step
            if(problem.solutionConfigureStep())
                return;
        }
    }


    // problem not resolved or not resolvable

    // solution is just to ask the user if he wants to keep loaded steps or remove it
    int resp = QMessageBox::critical(this, title, tr("<html>%1<br/><br/><b>Que voulez vous faire ?</b></html>").arg(info), tr("Ne pas charger le script"), tr("Charger le script jusqu'à cette erreur"));

    problem.setSolutionKeepSteps(resp == 1);
}

QString GMainWindow::createFileExtensionAvailable() const
{
    CDM_PluginManager *pluginManager = getPluginManager();

    static QString fileExtension;

    if(!fileExtension.isEmpty())
        return fileExtension;

    fileExtension += tr("All Valid Files (");

    // ALL VALID FILES (script and from load file step)
    QSet<QString> uniqueFormats;

    fileExtension += createScriptManagerExtension("", "");

    QHash<QString, QStringList> hashFileFormat;
    CT_StepsMenu *menu = pluginManager->stepsMenu();
    QList<CT_MenuLevel*> levels = menu->levels();
    QListIterator<CT_MenuLevel*> it(levels);

    while(it.hasNext()) {
        getFileExtensionAvailableInStepsOfLevelRecursively(it.next(), hashFileFormat);
    }

    QHashIterator<QString, QStringList> itH(hashFileFormat);

    while(itH.hasNext()) {
        itH.next();

        const QStringList &suffixes = itH.value();
        QListIterator<QString> itS(suffixes);

        while(itS.hasNext()) {
            const QString &f = itS.next();

            if(!uniqueFormats.contains(f) && (f != "*")) {
                uniqueFormats.insert(f);
                fileExtension += " *." + f;
            }
        }
    }

    fileExtension += ");;";

    // ALL files

    fileExtension += tr("All files") + " (*.*)";

    // FILE BY TYPE

    itH.toFront();

    while(itH.hasNext()) {
        itH.next();

        fileExtension += ";;";
        fileExtension += itH.key() + " (";

        const QStringList &suffixes = itH.value();
        QListIterator<QString> itS(suffixes);

        while(itS.hasNext()) {
            fileExtension += " *." + itS.next();
        }

        fileExtension += ")";
    }

    fileExtension.replace("( ", "(");

    return fileExtension;
}

void GMainWindow::getFileExtensionAvailableInStepsOfLevelRecursively(CT_MenuLevel *level, QHash<QString, QStringList> &hash) const
{
    const QList<CT_VirtualAbstractStep*> steps = level->steps();

    for(CT_VirtualAbstractStep* step : steps) {
        CT_AbstractStepLoadFile *lfStep = dynamic_cast<CT_AbstractStepLoadFile*>(step);

        if(lfStep != nullptr) {
            const QList<FileFormat> ffs = lfStep->fileExtensionAccepted();

            for(const FileFormat& ff : ffs) {

                if(!ff.suffixes().isEmpty()) {
                    QStringList sl = hash.value(ff.description(), QStringList());
                    QListIterator<QString> itSs(ff.suffixes());

                    while(itSs.hasNext()) {
                        QString suffixe = itSs.next();

                        if(!sl.contains(suffixe))
                            sl.append(suffixe);
                    }

                    hash.insert(ff.description(), sl);
                }
            }
        }
    }

    const QList<CT_MenuLevel*> levels = level->levels();

    for(CT_MenuLevel* level : levels) {
        getFileExtensionAvailableInStepsOfLevelRecursively(level, hash);
    }
}

QString GMainWindow::createScriptManagerExtension(QString preString, QString postString) const
{
    QString fileExtension;

    QList<QString> listScript = getScriptManager()->getFileExtensionAccepted();
    QListIterator<QString> itScript(listScript);

    if(itScript.hasNext())
    {
        fileExtension = preString + "*" + itScript.next();

        while(itScript.hasNext())
        {
            fileExtension += " *" + itScript.next();
        }

        fileExtension += postString;
    }

    return fileExtension;
}

void GMainWindow::changeLanguage()
{
    int index = ((QAction*)sender())->data().toInt();

    if(GUI_MANAGER->getLanguageManager()->currentLanguage() != index)
    {
        QMessageBox::information(this, tr("Information"), tr("Voud devez redémarrer l'application pour prendre en compte le changement de langue."));

        GUI_MANAGER->getLanguageManager()->useLanguage(index);
    }
}

void GMainWindow::documentToBeClosed(DM_DocumentView *view)
{
    if(_itemDrawableConfigurationView->getDocumentView() == view)
    {
        _itemDrawableConfigurationView->setDocument(nullptr);
    }
}

void GMainWindow::stepAdded(CT_VirtualAbstractStep *step)
{
    Q_UNUSED(step)

    actionSaveScript->setEnabled(getStepManager()->getStepRootList().size() > 0);
}

void GMainWindow::stepToBeRemoved(CT_VirtualAbstractStep *step)
{
    QList<CT_VirtualAbstractStep*> list = getStepManager()->getStepRootList();

    int n = list.size();

    if(list.contains(step))
    {
        --n;
    }

    actionSaveScript->setEnabled(n > 0);
}

void GMainWindow::actionHovered()
{
    QAction *action = (QAction*)sender();

    QPoint p = QCursor::pos();
    QString s = action->toolTip();

    p.setY(p.y() + 10);

    QToolTip::showText(p, s);
}

void GMainWindow::showStepChooser()
{
    m_stepChooserDialog->show();
}

void GMainWindow::hideStepChooser()
{
    m_stepChooserDialog->hide();
}
