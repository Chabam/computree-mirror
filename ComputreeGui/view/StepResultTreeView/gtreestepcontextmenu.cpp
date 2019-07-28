/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michaël KREBS (ARTS/ENSAM)

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

#include "gtreestepcontextmenu.h"

#include <QMessageBox>

#include "cdm_stepmanager.h"
#include "dm_guimanager.h"
#include "view/MainView/gaboutstepdialog.h"

GTreeStepContextMenu::GTreeStepContextMenu(CDM_StepManager &stepManager, QWidget *parent) : m_stepManager(stepManager), QMenu(parent)
{
    _selectedStep = nullptr;
}

void GTreeStepContextMenu::setSelectedStep(CT_VirtualAbstractStep *step)
{
    _selectedStep = step;

    reload();
}

/////////////////// PRIVATE ///////////////////

CT_VirtualAbstractStep* GTreeStepContextMenu::selectedStep()
{
    return _selectedStep;
}

/////////////////// SLOT ///////////////////

void GTreeStepContextMenu::reload()
{
    clear();

    QAction *action = new QAction(tr("Exécuter"), this);
    action->setIcon(QIcon(":/Icones/Icones/play.png"));
    action->setEnabled((selectedStep() != nullptr)
                       && ((!selectedStep()->needInputResults())
                           || ((selectedStep()->parentStep() != nullptr)
                               && (selectedStep()->parentStep()->nOutResult() > 0))));
    connect(action, SIGNAL(triggered()), this, SLOT(executeStepRequired()));
    addAction(action);

    action = new QAction(tr("Config. paramètres"), this);
    action->setIcon(QIcon(":/Icones/Icones/preferences-system.png"));
    action->setEnabled(selectedStep() != nullptr);
    connect(action, SIGNAL(triggered()), this, SLOT(configureStepRequired()));
    addAction(action);

    action = new QAction(tr("Config. résultats d'entrée"), this);
    action->setIcon(QIcon(":/Icones/Icones/preferences-system.png"));
    action->setEnabled((selectedStep() != nullptr) && selectedStep()->needInputResults());
    connect(action, SIGNAL(triggered()), this, SLOT(configureInputResultOfStepRequired()));
    addAction(action);

    if((selectedStep() != nullptr) && selectedStep()->isModifiable())
    {
        action = new QAction(tr("Modifier (mode manuel)"), this);
        action->setIcon(QIcon(":/Icones/Icones/hand.png"));
        connect(action, SIGNAL(triggered()), this, SLOT(modifyStepRequired()));
        addAction(action);
    }

    action = new QAction(tr("Supprimer"), this);
    action->setIcon(QIcon(":/Icones/Icones/delete.png"));
    action->setEnabled(selectedStep() != nullptr);
    connect(action, SIGNAL(triggered()), this, SLOT(deleteStepRequired()));
    addAction(action);

    action = new QAction(tr("Documentation de l'étape"), this);
    action->setIcon(QIcon(":/Icones/Icones/info.png"));
    connect(action, SIGNAL(triggered()), this, SLOT(showStepInformations()));
    addAction(action);

    addSeparator();

    action = new QAction(tr("Localiser dans le menu"), this);
    action->setIcon(QIcon(":/Icones/Icones/maps.png"));
    connect(action, SIGNAL(triggered()), this, SLOT(locateStepInMenu()));
    addAction(action);

    addSeparator();

    action = new QAction(tr("Déplier toutes les étapes"), this);
    action->setIcon(QIcon(":/Icones/Icones/expand.png"));
    connect(action, SIGNAL(triggered()), this, SIGNAL(expandAll()));
    addAction(action);

    action = new QAction(tr("Replier toutes les étapes"), this);
    action->setIcon(QIcon(":/Icones/Icones/collapse.png"));
    connect(action, SIGNAL(triggered()), this, SIGNAL(collapseAll()));
    addAction(action);

}

void GTreeStepContextMenu::executeStepRequired()
{
    if(!m_stepManager.isRunning()) {
        if(checkIfCanExecuteStepAndShowWarningMessage(false))
            m_stepManager.executeStep(selectedStep());
    } else
        m_stepManager.executeStep(selectedStep());
}

void GTreeStepContextMenu::modifyStepRequired()
{
    if(selectedStep() != nullptr)
        m_stepManager.executeModifyStep(selectedStep());
}

void GTreeStepContextMenu::configureInputResultOfStepRequired()
{
    if(selectedStep() != nullptr && !m_stepManager.isRunning())
    {
        if(selectedStep()->showInputResultConfigurationDialog())
            selectedStep()->finalizeConfiguration();
    }
}

void GTreeStepContextMenu::configureStepRequired()
{
    if(selectedStep() != nullptr && !m_stepManager.isRunning())
    {
        if(selectedStep()->showPostConfigurationDialog())
            selectedStep()->finalizeConfiguration();
    }
}

void GTreeStepContextMenu::showStepInformations()
{
    if(selectedStep() != nullptr)
        GAboutStepDialog(selectedStep()).exec();
}

void GTreeStepContextMenu::deleteStepRequired()
{
    if(selectedStep() != nullptr)
        GUI_MANAGER->asyncRemoveStep(*selectedStep(), nullptr);
}

void GTreeStepContextMenu::locateStepInMenu()
{
    if(selectedStep() != nullptr)
        emit locateSelectedStepInMenu(selectedStep());
}

bool GTreeStepContextMenu::checkIfCanExecuteStepAndShowWarningMessage(bool executeInDebugMode)
{
    bool continueExecution = true;
    bool oneStepInDebugMode = m_stepManager.checkOneStepIsInDebugModeFromStep(selectedStep());

    if(oneStepInDebugMode && !executeInDebugMode)
    {
        continueExecution = QMessageBox::warning(nullptr, tr("Attention"), tr("Une ou plusieurs étapes sont en mode debug or vous allez lancer"
                                                                           " les traitements en mode normal."), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel) == QMessageBox::Yes;
    }
    else if(!oneStepInDebugMode && executeInDebugMode)
    {
        continueExecution = QMessageBox::warning(nullptr, tr("Attention"), tr("Aucune étape n'est en mode debug or vous allez lancer"
                                                                           " les traitements dans ce mode."), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel) == QMessageBox::Yes;
    }

    return continueExecution;
}
