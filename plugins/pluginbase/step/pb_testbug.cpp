/****************************************************************************
 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                         All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre PIBOULE (ONF)

 This file is part of PluginONF library.

 PluginONF is free library: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 PluginONF is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PluginONF.  If not, see <http://www.gnu.org/licenses/lgpl.html>.
*****************************************************************************/

#include "pb_testbug.h"

#include "ct_view/ct_asciifilechoicebutton.h"
#include "ct_view/ct_combobox.h"
#include "ct_log/ct_logmanager.h"

#include <QFile>
#include <QTextStream>

PB_TestBug::PB_TestBug() : CT_AbstractStepCanBeAddedFirst()
{
}

QString PB_TestBug::description() const
{
    return tr("Test Bug");
}

QString PB_TestBug::detailledDescription() const
{
    return tr("");
}


CT_VirtualAbstractStep* PB_TestBug::createNewInstance() const
{
    return new PB_TestBug();
}

//////////////////// PROTECTED METHODS //////////////////


void PB_TestBug::declareInputModels(CT_StepInModelStructureManager& manager)
{
    manager.setNotNeedInputResult();
}

void PB_TestBug::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    manager.addResult(_outResult, tr("Resultat"));
    manager.setRootGroup(_outResult, _outGrp, tr("Groupe"));
    manager.addItem(_outGrp, _outItem, tr("Item"));
}

void PB_TestBug::compute()
{
    for (CT_ResultGroup* resOut : _outResult.iterateOutputs())
    {
        CT_StandardItemGroup* grp = nullptr;
        CT_Circle2D* item = nullptr;

        // Groupe vide
        grp = new CT_StandardItemGroup();
        resOut->addRootGroup(_outGrp, grp);

        // item 1
        grp = new CT_StandardItemGroup();
        resOut->addRootGroup(_outGrp, grp);
        item = new CT_Circle2D(new CT_Circle2DData(Eigen::Vector2d(1, 1), 0.5));
        grp->addSingularItem(_outItem, item);

        // item 2
        grp = new CT_StandardItemGroup();
        resOut->addRootGroup(_outGrp, grp);
        item = new CT_Circle2D(new CT_Circle2DData(Eigen::Vector2d(2, 2), 0.5));
        grp->addSingularItem(_outItem, item);
    }
}
