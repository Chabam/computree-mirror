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

#ifndef PB_TESTBUG_H
#define PB_TESTBUG_H

#include "ct_step/abstract/ct_abstractstepcanbeaddedfirst.h"

#include "ct_itemdrawable/ct_scene.h"
#include "ct_itemdrawable/ct_pointcluster.h"
#include "ct_itemdrawable/ct_loopcounter.h"

#include "ct_view/tools/ct_textfileconfigurationdialog.h"

class PB_TestBug: public CT_AbstractStepCanBeAddedFirst
{
    Q_OBJECT
    using SuperClass = CT_AbstractStepCanBeAddedFirst;

public:

    PB_TestBug();

    QString description() const override;

    QString detailledDescription() const override;

    CT_VirtualAbstractStep* createNewInstance() const final;

protected:

    void declareInputModels(CT_StepInModelStructureManager& manager) final;

    void declareOutputModels(CT_StepOutModelStructureManager& manager) final;

    void compute() final;

private:

    CT_HandleInResultGroup<> _inResult;
    CT_HandleInStdZeroOrMoreGroup _rootGroup;
    CT_HandleInStdGroup<> _inGroup;
    CT_HandleInSingularItem<CT_Scene>   _inScene;

    CT_HandleOutResultGroup                                                             _outResult;
    CT_HandleOutStdGroup                                                                _outGrp;
    CT_HandleOutSingularItem<CT_PointCluster>                                               _outItem;

};

#endif // PB_TESTBUG_H
