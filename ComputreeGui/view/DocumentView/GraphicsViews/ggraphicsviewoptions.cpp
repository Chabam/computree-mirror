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


#include "ggraphicsviewoptions.h"
#include "ui_ggraphicsviewoptions.h"

#include <QTimer>
#include <QMessageBox>

GGraphicsViewOptions::GGraphicsViewOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GGraphicsViewOptions)
{
    ui->setupUi(this);

    _options = new DM_GraphicsViewOptions();

    _colorSelected = new QtColorPicker(this);
    _colorSelected->setStandardColors();

    _colorBackground = new QtColorPicker(this);
    _colorBackground->setStandardColors();

    ui->horizontalLayoutSelectedColor->addWidget(_colorSelected);
    ui->horizontalLayoutBackgroundColor->addWidget(_colorBackground);

    updateUiFromOptions();

    connect(ui->pushButtonSaveDefault, SIGNAL(clicked()), this, SLOT(saveDefault()));

    connect(ui->groupBoxCameraCoordinates, SIGNAL(toggled(bool)), this, SLOT(collapseOrExpandGroupBox(bool)));
    connect(ui->groupBoxColorAndSize, SIGNAL(toggled(bool)), this, SLOT(collapseOrExpandGroupBox(bool)));
    connect(ui->groupBoxDrawing, SIGNAL(toggled(bool)), this, SLOT(collapseOrExpandGroupBox(bool)));
    connect(ui->groupBoxView, SIGNAL(toggled(bool)), this, SLOT(collapseOrExpandGroupBox(bool)));

    QString style = QString("QGroupBox::indicator {"
                                "width: 12px;"
                                "height: 12px;"
                            "}"
                            "QGroupBox::indicator:unchecked {"
                                "image: url(:/Icones/Icones/stylesheet-branch-closed.png);"
                            "}"
                            "QGroupBox::indicator:checked {"
                                "image: url(:/Icones/Icones/stylesheet-branch-open.png);"
                            "}");

    ui->groupBoxCameraCoordinates->setStyleSheet(style);
    ui->groupBoxColorAndSize->setStyleSheet(style);
    ui->groupBoxDrawing->setStyleSheet(style);
    ui->groupBoxView->setStyleSheet(style);

    // disable visibility of this parameters because with OSG it will not be used
    ui->checkBoxUseLight->setVisible(false);
    ui->checkBoxUseTransparency->setVisible(false);
}

GGraphicsViewOptions::~GGraphicsViewOptions()
{
    delete ui;
    delete _options;
}

void GGraphicsViewOptions::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void GGraphicsViewOptions::setOptions(const DM_GraphicsViewOptions &options)
{
    if((*_options) != options) {
        _options->updateFromOtherOptions(options);

        updateUiFromOptions();

        emit optionsChanged();
    }
}

const DM_GraphicsViewOptions& GGraphicsViewOptions::getOptions()
{
    return *_options;
}

void GGraphicsViewOptions::updateUiFromOptions()
{
    _colorSelected->setCurrentColor(_options->getSelectedColor());
    _colorBackground->setCurrentColor(_options->getBackgroundColor());

    ui->doubleSpinBoxPointSize->setValue(_options->getPointSize());
    ui->checkBoxDrawAxis->setChecked(_options->drawAxis());
    ui->checkBoxDrawGrid->setChecked(_options->drawGrid());
    ui->checkBoxUseTransparency->setChecked(_options->useTransparency());
    ui->checkBoxUseLight->setChecked(_options->mustUseLight());
    ui->checkBoxCameraPosition->setChecked(_options->getCameraInformationDisplayed().testFlag(DM_GraphicsViewOptions::CameraPosition));
    ui->checkBoxCameraCenterCoordinates->setChecked(_options->getCameraInformationDisplayed().testFlag(DM_GraphicsViewOptions::CameraSceneCenter));
    ui->checkBoxCameraViewDirection->setChecked(_options->getCameraInformationDisplayed().testFlag(DM_GraphicsViewOptions::CameraViewDirection));
    ui->checkBoxShowFPS->setChecked(_options->getCameraInformationDisplayed().testFlag(DM_GraphicsViewOptions::FpsInformation));
    ui->spinBoxMaxNbrPointsInFastDrawMode->setValue(_options->getMaxNbrPointsInFastDraw());

    // if simplified mode is Normal
    if (_options->mustReduceNbrPointsInFastDraw()
            && !_options->mustReduceNbrPointsInNormalDraw())
        ui->comboBoxFastDrawMode->setCurrentIndex(0);
    // if simplified mode is Always
    else if (_options->mustReduceNbrPointsInFastDraw()
             && _options->mustReduceNbrPointsInNormalDraw())
        ui->comboBoxFastDrawMode->setCurrentIndex(1);
    // if simplified mode is Never
    else
        ui->comboBoxFastDrawMode->setCurrentIndex(2);

    switch(_options->getCameraInformationPosition())
    {
    case DM_GraphicsViewOptions::InfoInUpperLeftCorner : ui->radioButtonUpperLeft->setChecked(true);
        break;

    case DM_GraphicsViewOptions::InfoInUpperRightCorner : ui->radioButtonUpperRight->setChecked(true);
        break;

    case DM_GraphicsViewOptions::InfoInLowerRightCorner : ui->radioButtonLowerRight->setChecked(true);
        break;

    case DM_GraphicsViewOptions::InfoInLowerLeftCorner : ui->radioButtonLowerLeft->setChecked(true);
    }

    switch(_options->getCameraType())
    {
    case CameraInterface::PERSPECTIVE : ui->radioButtonPerspective->setChecked(true);
        break;
    case CameraInterface::ORTHOGRAPHIC : ui->radioButtonOrthographic->setChecked(true);
        break;
    }

    ui->drawModeConfigurator->setDrawModeAccessor(_options);
}

void GGraphicsViewOptions::updateOptionsFromUi()
{
    QColor cc = _colorBackground->currentColor();
    _options->setBackgroudColor(cc);
    cc = _colorSelected->currentColor();
    _options->setSelectedColor(cc);

    _options->setPointSize(ui->doubleSpinBoxPointSize->value());
    _options->setDrawAxis(ui->checkBoxDrawAxis->isChecked());
    _options->setDrawGrid(ui->checkBoxDrawGrid->isChecked());
    _options->setUseTransparency(ui->checkBoxUseTransparency->isChecked());
    _options->setUseLight(ui->checkBoxUseLight->isChecked());

    DM_GraphicsViewOptions::CameraInfoDisplayed info = DM_GraphicsViewOptions::CameraNone;

    if(ui->checkBoxCameraPosition->isChecked())
        info |= DM_GraphicsViewOptions::CameraPosition;

    if(ui->checkBoxCameraCenterCoordinates->isChecked())
        info |= DM_GraphicsViewOptions::CameraSceneCenter;

    if(ui->checkBoxCameraViewDirection->isChecked())
        info |= DM_GraphicsViewOptions::CameraViewDirection;
    if(ui->checkBoxShowFPS->isChecked())
        info |= DM_GraphicsViewOptions::FpsInformation;

    _options->setCameraInformationDisplayed(info);

    DM_GraphicsViewOptions::CameraInfoPosition pos = DM_GraphicsViewOptions::InfoInLowerRightCorner;

    if(ui->buttonGroupCameraInfoPosition->checkedButton() == ui->radioButtonUpperLeft)
        pos = DM_GraphicsViewOptions::InfoInUpperLeftCorner;
    else if(ui->buttonGroupCameraInfoPosition->checkedButton() == ui->radioButtonUpperRight)
        pos = DM_GraphicsViewOptions::InfoInUpperRightCorner;
    else if(ui->buttonGroupCameraInfoPosition->checkedButton() == ui->radioButtonLowerLeft)
        pos = DM_GraphicsViewOptions::InfoInLowerLeftCorner;

    _options->setCameraInformationPosition(pos);

    _options->setCameraType(((ui->buttonGroupViewType->checkedButton() == ui->radioButtonPerspective) ? CameraInterface::PERSPECTIVE : CameraInterface::ORTHOGRAPHIC));

    _options->setMaxNbrPointsInFastDraw(ui->spinBoxMaxNbrPointsInFastDrawMode->value());

    _options->setMustReduceNbrPointsInFastDraw(ui->comboBoxFastDrawMode->currentIndex() != 2);
    _options->setMustReduceNbrPointsInNormalDraw(ui->comboBoxFastDrawMode->currentIndex() == 1);

    ui->drawModeConfigurator->apply();
}

void GGraphicsViewOptions::collapseOrExpandGroupBox(bool val)
{
   QGroupBox *box = (QGroupBox*)sender();

   box->setMaximumHeight(1000);

   if(!val)
       box->setMaximumHeight(25);
   else
       box->adjustSize();

   adjustSize();
   QTimer::singleShot(5, this, SLOT(refreshDialog()));
}

void GGraphicsViewOptions::refreshDialog()
{
    resize(width()+1, height());
}

void GGraphicsViewOptions::saveDefault()
{
    updateOptionsFromUi();

    if(_options->save())
    {
        QMessageBox::information(this, tr("Sauvegarde réussie"), tr("La sauvegarde de la configuration a réussi."));
    }
    else
    {
        QMessageBox::critical(this, tr("Erreur lors de la sauvegarde"), tr("La sauvegarde de la configuration a échoué."));
    }
}

void GGraphicsViewOptions::accept()
{
    updateOptionsFromUi();

    SuperClass::accept();
}
