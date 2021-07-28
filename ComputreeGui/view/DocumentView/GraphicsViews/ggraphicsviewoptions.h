/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
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


#ifndef GGRAPHICSVIEWOPTIONS_H
#define GGRAPHICSVIEWOPTIONS_H

#include <QDialog>
#include "dm_graphicsviewoptions.h"
#include "dm_colorpicker.h"

namespace Ui {
    class GGraphicsViewOptions;
}

class DrawModeConfigurator;

class GGraphicsViewOptions : public QDialog {

    Q_OBJECT
    typedef QDialog SuperClass;

public:
    GGraphicsViewOptions(QWidget *parent = 0);
    ~GGraphicsViewOptions();

    void setOptions(const DM_GraphicsViewOptions &options);
    const DM_GraphicsViewOptions& getOptions();

protected:
    void changeEvent(QEvent *e) override;

private:
    Ui::GGraphicsViewOptions    *ui;
    DM_GraphicsViewOptions      *_options;
    QtColorPicker               *_colorBackground;
    QtColorPicker               *_colorSelected;

    void updateUiFromOptions();
    void updateOptionsFromUi();

private slots:

    void collapseOrExpandGroupBox(bool val);
    void refreshDialog();

    void saveDefault();

public slots:
    void accept() override;

signals:
    void pointSizeChanged(double size);

    void optionsChanged();
};

#endif // GGRAPHICSVIEWOPTIONS_H
