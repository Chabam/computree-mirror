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

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "view/MainView/gguimanager.h"

#include "amkglapp.h"
#include "tools/amkgl/dm_amkgleasyaccess.h"

#include "ct_global/ct_context.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    #include "qtextcodec.h"
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CDM_Internationalization language;
    language.loadConfiguration();

    #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    #endif

    DM_AMKglEasyAccess amkglEA;
    AMKglApp::uniqueInstance()->setEasyAccess(&amkglEA);

    GGuiManager* gm = new GGuiManager(&language);
    gm->initUi();

    QCommandLineParser parser;
    parser.setApplicationDescription(QString(argv[0]));
    parser.addHelpOption();
    parser.addOptions({
        {{"s", "script"},
         QCoreApplication::translate("main", "Load script file <path>"),
         QCoreApplication::translate("main", "path")},
        {{"r", "run"},
         QCoreApplication::translate("main", "Run script on load")}
    });
    parser.process(a);

    if (parser.isSet("script")) {
        QString scriptPath = parser.value("script");

        CDM_StepManager *stepManager = gm->getStepManager();
        CDM_ScriptManagerAbstract* scriptManager = stepManager->getScriptManager();
        scriptManager->loadScript(scriptPath, *stepManager);

        if (parser.isSet("run"))
            stepManager->executeStep();
    }

    int ret = a.exec();

	delete gm;
	delete AMKglApp::uniqueInstance();

	CT_Context::deleteStaticInstance();

	return ret;
}
