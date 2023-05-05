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


#ifndef DM_GRAPHICSVIEWOPTIONS_H
#define DM_GRAPHICSVIEWOPTIONS_H

#include "graphicsviewinterface.h"
#include "renderer/drawmodeaccessor.h"

#include <QtGlobal>

#include <QColor>
#include <QDomElement>

class DM_GraphicsViewOptions : public QObject, public GraphicsViewOptionsInterface, public DrawModeAccessor
{
    Q_OBJECT

public:

    enum CameraInfoFlags
    {
        CameraNone = 0x0,
        CameraPosition = 0x1,
        CameraSceneCenter = 0x2,
        CameraViewDirection = 0x4,
        FpsInformation = 0x8
    };

    Q_DECLARE_FLAGS(CameraInfoDisplayed, CameraInfoFlags)

    enum CameraInfoPosition
    {
        InfoInUpperLeftCorner,
        InfoInUpperRightCorner,
        InfoInLowerRightCorner,
        InfoInLowerLeftCorner
    };

    DM_GraphicsViewOptions();
    DM_GraphicsViewOptions(const DM_GraphicsViewOptions& other);
    virtual ~DM_GraphicsViewOptions();

    // TODO : to remove
    int getFastDrawTime() const override { return 0; }

    inline QColor getSelectedColor() const  override { return _selectedColor; }
    inline QColor getBackgroundColor() const  override { return _backgroundColor; }
    inline float getPointSize() const override { return _pointSize; }
    inline bool drawAxis() const override { return _drawAxis; }
    inline bool drawGrid() const { return m_drawGrid; }
    inline bool useColor() const override { return _useColor; }
    inline bool useTransparency() const override { return _useTransparency; }
    inline bool mustUseLight() const { return _useLight; }
    inline DM_GraphicsViewOptions::CameraInfoDisplayed getCameraInformationDisplayed() const { return _cameraInfoDisplayed; }
    inline DM_GraphicsViewOptions::CameraInfoPosition getCameraInformationPosition() const { return _cameraInfoPosition; }
    inline CameraInterface::CameraType getCameraType() const { return _cameraType; }
    inline QColor normalColor() const { return m_normalColor; }
    inline float normalLength() const { return m_normalLength; }
    inline bool mustShowNormals() const { return m_mustShowNormals; }
    DrawMode getDrawModeToUse(Scene::ObjectType type) const override;
    inline bool mustReduceNbrPointsInFastDraw() const { return m_reducePointNbrInFastDraw; }
    inline bool mustReduceNbrPointsInNormalDraw() const { return m_reducePointNbrInNormalDraw; }
    inline int getMaxNbrPointsInFastDraw() const { return m_maxPointInFastDraw; }

    void setSelectedColor(QColor &color);
    void setBackgroudColor(QColor &color) override;
    void setPointSize(float size) override;
    void setDrawAxis(bool draw);
    void setDrawGrid(bool draw);
    void setUseColor(bool enable);
    void setUseTransparency(bool use);
    void setUseLight(bool use);
    void setCameraInformationDisplayed(DM_GraphicsViewOptions::CameraInfoDisplayed info);
    void setCameraInformationPosition(DM_GraphicsViewOptions::CameraInfoPosition pos);
    void setCameraType(CameraInterface::CameraType type);
    void setNormalColor(const QColor &color);
    void setNormalLength(float l);
    void setMustShowNormals(bool enabled);
    void setDrawModeToUse(DrawMode mode) override;
    void setDrawModeToUse(Scene::ObjectType type, DrawMode mode) override;
    void setMustReduceNbrPointsInFastDraw(bool enabled);
    void setMustReduceNbrPointsInNormalDraw(bool enabled);
    void setMaxNbrPointsInFastDraw(int n);
    void updateFromOtherOptions(const DM_GraphicsViewOptions &options);

    DrawModeAccessor& operator =(const DrawModeAccessor& other) override;

    bool operator !=(const DM_GraphicsViewOptions& options) const;

    DM_GraphicsViewOptions& operator =(const DM_GraphicsViewOptions& other);

    bool load();
    bool save();

    bool loadFromXml(const QDomElement &el);
    bool saveToXml(QDomElement &main, QDomDocument& doc) const;

private:

    typedef QHash<Scene::ObjectType, DrawMode> DrawModesCollection;
    typedef QHashIterator<Scene::ObjectType, DrawMode> DrawModesCollectionIterator;
    typedef QMutableHashIterator<Scene::ObjectType, DrawMode> DrawModesCollectionMutableIterator;

    QColor                      _selectedColor;
    QColor                      _backgroundColor;
    float                       _pointSize;
    bool                        _drawAxis;
    bool                        m_drawGrid;
    bool                        _useColor;
    bool                        _useTransparency;
    bool                        _useLight;
    CameraInfoDisplayed         _cameraInfoDisplayed;
    CameraInfoPosition          _cameraInfoPosition;
    CameraInterface::CameraType _cameraType;
    QColor                      m_normalColor;
    float                       m_normalLength;
    bool                        m_mustShowNormals;
    DrawModesCollection         m_drawModes;
    bool                        m_reducePointNbrInFastDraw;
    bool                        m_reducePointNbrInNormalDraw;
    int                         m_maxPointInFastDraw;

signals:

    void optionsChanged();
    void selectionColorChanged(const QColor &c);
    void normalColorChanged(const QColor &c);
    void normalLengthChanged(float f);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(DM_GraphicsViewOptions::CameraInfoDisplayed)

#endif // DM_GRAPHICSVIEWOPTIONS_H
