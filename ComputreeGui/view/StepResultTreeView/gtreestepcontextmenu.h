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


#ifndef GTREESTEPCONTEXTMENU_H
#define GTREESTEPCONTEXTMENU_H

#include <QMenu>

class CT_VirtualAbstractStep;
class CDM_StepManager;

/**
 * @brief Context menu for a step. Do some basic staff like execute it, configure it, etc... and for staff that required
 *        external action this class will send signals (expand, expandAll, locateStepInMenu, etc...)
 */
class GTreeStepContextMenu : public QMenu
{
    Q_OBJECT

public:
    GTreeStepContextMenu(CDM_StepManager &stepManager, QWidget *parent = 0);

    /**
     * @brief Set the step that you want to control
     */
    void setSelectedStep(CT_VirtualAbstractStep *step);

    /**
     * @brief Check if the step can be executed in current mode and show a warning message if not to let the user
     *        choose the final decision.
     * @param executeInDebugMode : true if you want to execute the step in debug mode, false otherwise
     * @return true if you can execute the step, false otherwise
     */
    bool checkIfCanExecuteStepAndShowWarningMessage(bool executeInDebugMode);

private:

    CDM_StepManager&                m_stepManager;
    CT_VirtualAbstractStep          *_selectedStep;

    CT_VirtualAbstractStep* selectedStep();

    /**
     * @brief Call it when you change the step to control
     */
    void reload();

signals:
    /**
     * @brief Emitted when gui must locate the step in the menu of steps
     */
    void locateSelectedStepInMenu(CT_VirtualAbstractStep *selectedStep);

    /**
     * @brief Emitted when gui must expand all steps
     */
    void expandAll();

    /**
     * @brief Emitted when gui must collapse all steps
     */
    void collapseAll();

private slots:

    /**
     * @brief Called when the user trigger the action to execute the step
     */
    void executeStepRequired();

    /**
     * @brief Called when the user trigger the action to modify the step
     */
    void modifyStepRequired();

    /**
     * @brief Called when the user trigger the action to configure input results of the step
     */
    void configureInputResultOfStepRequired();

    /**
     * @brief Called when the user trigger the action to configure the step
     */
    void configureStepRequired();

    /**
     * @brief Called when the user trigger the action to show informations about the step
     */
    void showStepInformations();

    /**
     * @brief Called when the user trigger the action to delete the step
     */
    void deleteStepRequired();

    /**
     * @brief Called when the user trigger the action to locate the step in the menu of steps
     */
    void locateStepInMenu();
};

#endif // TREESTEPCONTEXTMENU_H
