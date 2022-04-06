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


#include "dm_graphicsviewoptions.h"
#include "cdm_configfile.h"

#include "dm_domutils.h"
#include "renderer/drawmodeaccessor.h"

#define CHECK_CHANGE(XXX) if(XXX != options.XXX) \
                          { \
                              XXX = options.XXX; \
                              emitChanged = true; \
                          }


#define CHECK_CHANGE_AND_EMIT(XXX, YYY) if(XXX != options.XXX) \
                                        { \
                                            XXX = options.XXX; \
                                            emit YYY(XXX); \
                                            emitChanged = true; \
                                        }

#define CHECK_DIFFERENT(XXX) if(XXX != options.XXX) \
                             { \
                                return true; \
                             }

DM_GraphicsViewOptions::DM_GraphicsViewOptions() : QObject(nullptr)
{
    _backgroundColor = QColor(48, 48, 48);
    _selectedColor = Qt::red;
    _pointSize = 1.0;
    _drawAxis = false;
    m_drawGrid = false;
    _useColor = true;
    _useTransparency = false;
    _useLight = false;
    _cameraInfoDisplayed = DM_GraphicsViewOptions::CameraNone;
    _cameraInfoPosition = DM_GraphicsViewOptions::InfoInLowerRightCorner;
    _cameraType = CameraInterface::ORTHOGRAPHIC;
    m_normalColor = Qt::yellow;
    m_normalLength = 0.05f;
    m_mustShowNormals = false;

    int size = Scene::NumberOfElements;

    for(int i=0; i<size; ++i)
        m_drawModes.insert((Scene::ObjectType)i, DM_VAO);

    m_reducePointNbrInFastDraw = true;
    m_reducePointNbrInNormalDraw = false;
    m_maxPointInFastDraw = 200000;
}

DM_GraphicsViewOptions::DM_GraphicsViewOptions(const DM_GraphicsViewOptions &other) : DM_GraphicsViewOptions()
{
    this->updateFromOtherOptions(other);
}

DM_GraphicsViewOptions::~DM_GraphicsViewOptions()
{
}

DrawMode DM_GraphicsViewOptions::getDrawModeToUse(Scene::ObjectType type) const
{
    return m_drawModes.value(type, DM_VAO);
}

void DM_GraphicsViewOptions::setDrawAxis(bool draw)
{
    _drawAxis = draw;
}

void DM_GraphicsViewOptions::setDrawGrid(bool draw)
{
    m_drawGrid = draw;
}

void DM_GraphicsViewOptions::setUseColor(bool enable)
{
    _useColor = enable;
}

void DM_GraphicsViewOptions::setUseTransparency(bool use)
{
    _useTransparency = use;
}

void DM_GraphicsViewOptions::setUseLight(bool use)
{
    _useLight = use;
}

void DM_GraphicsViewOptions::setSelectedColor(QColor &color)
{
    _selectedColor = color;
}

void DM_GraphicsViewOptions::setBackgroudColor(QColor &color)
{
    _backgroundColor = color;
}

void DM_GraphicsViewOptions::setPointSize(float size)
{
    _pointSize = size;
}

void DM_GraphicsViewOptions::setCameraInformationDisplayed(DM_GraphicsViewOptions::CameraInfoDisplayed info)
{
    _cameraInfoDisplayed = info;
}

void DM_GraphicsViewOptions::setCameraInformationPosition(DM_GraphicsViewOptions::CameraInfoPosition pos)
{
    _cameraInfoPosition = pos;
}

void DM_GraphicsViewOptions::setCameraType(CameraInterface::CameraType type)
{
    _cameraType = type;
}

void DM_GraphicsViewOptions::setNormalColor(const QColor &color)
{
    m_normalColor = color;
}

void DM_GraphicsViewOptions::setNormalLength(float l)
{
    m_normalLength = l;
}

void DM_GraphicsViewOptions::setMustShowNormals(bool enabled)
{
    m_mustShowNormals = enabled;
}

void DM_GraphicsViewOptions::setDrawModeToUse(DrawMode mode)
{
    DrawModesCollectionMutableIterator it(m_drawModes);

    while(it.hasNext()) {
        it.next();
        it.setValue(mode);
    }
}

void DM_GraphicsViewOptions::setDrawModeToUse(Scene::ObjectType type, DrawMode mode)
{
    m_drawModes.insert(type, mode);
}

void DM_GraphicsViewOptions::setMustReduceNbrPointsInFastDraw(bool enabled)
{
    m_reducePointNbrInFastDraw = enabled;
}

void DM_GraphicsViewOptions::setMustReduceNbrPointsInNormalDraw(bool enabled)
{
    m_reducePointNbrInNormalDraw = enabled;
}

void DM_GraphicsViewOptions::setMaxNbrPointsInFastDraw(int n)
{
    m_maxPointInFastDraw = qMax(n, 1);
}

void DM_GraphicsViewOptions::updateFromOtherOptions(const DM_GraphicsViewOptions &options)
{
    bool emitChanged = false;

    CHECK_CHANGE_AND_EMIT(_selectedColor, selectionColorChanged)
    CHECK_CHANGE_AND_EMIT(m_normalColor, normalColorChanged)
    CHECK_CHANGE_AND_EMIT(m_normalLength, normalLengthChanged)
    CHECK_CHANGE(_backgroundColor)
    CHECK_CHANGE(_pointSize)
    CHECK_CHANGE(_drawAxis)
    CHECK_CHANGE(m_drawGrid)
    CHECK_CHANGE(_useColor)
    CHECK_CHANGE(_useTransparency)
    CHECK_CHANGE(_useLight)
    CHECK_CHANGE(_cameraInfoDisplayed)
    CHECK_CHANGE(_cameraInfoPosition)
    CHECK_CHANGE(_cameraType)
    CHECK_CHANGE(m_mustShowNormals)
    CHECK_CHANGE(m_reducePointNbrInFastDraw)
    CHECK_CHANGE(m_reducePointNbrInNormalDraw)
    CHECK_CHANGE(m_maxPointInFastDraw)

    DrawModesCollectionIterator it(options.m_drawModes);

    while(it.hasNext()) {
        it.next();

        if(m_drawModes.value(it.key(), DM_VAO) != it.value()) {
            m_drawModes.insert(it.key(), it.value());

            emitChanged = true;
        }
    }

    if(emitChanged)
        emit optionsChanged();
}

DrawModeAccessor &DM_GraphicsViewOptions::operator =(const DrawModeAccessor &other)
{
    bool emitChanged = false;

    DrawModesCollectionMutableIterator it(m_drawModes);

    while(it.hasNext()) {
        it.next();

        const DrawMode nDm = other.getDrawModeToUse(it.key());

        if(nDm != it.value()) {
            it.setValue(nDm);
            emitChanged = true;
        }
    }

    if(emitChanged)
        emit optionsChanged();

    return *this;
}

bool DM_GraphicsViewOptions::operator !=(const DM_GraphicsViewOptions &options) const
{
    CHECK_DIFFERENT(_selectedColor)
    CHECK_DIFFERENT(m_normalColor)
    CHECK_DIFFERENT(m_normalLength)
    CHECK_DIFFERENT(_backgroundColor)
    CHECK_DIFFERENT(_pointSize)
    CHECK_DIFFERENT(_drawAxis)
    CHECK_DIFFERENT(m_drawGrid)
    CHECK_DIFFERENT(_useColor)
    CHECK_DIFFERENT(_useTransparency)
    CHECK_DIFFERENT(_useLight)
    CHECK_DIFFERENT(_cameraInfoDisplayed)
    CHECK_DIFFERENT(_cameraInfoPosition)
    CHECK_DIFFERENT(_cameraType)
    CHECK_DIFFERENT(m_mustShowNormals)
    CHECK_DIFFERENT(m_reducePointNbrInFastDraw)
    CHECK_DIFFERENT(m_reducePointNbrInNormalDraw)
    CHECK_DIFFERENT(m_maxPointInFastDraw)

    DrawModesCollectionIterator it(options.m_drawModes);

    while(it.hasNext()) {
        it.next();

        if(m_drawModes.value(it.key(), DM_VAO) != it.value()) {
           return true;
        }
    }

    return false;
}

DM_GraphicsViewOptions& DM_GraphicsViewOptions::operator =(const DM_GraphicsViewOptions &other)
{
    this->updateFromOtherOptions(other);

    return *this;
}

bool DM_GraphicsViewOptions::load()
{
    CONFIG_FILE->beginGroup("GraphicsOptions");

    _backgroundColor = CONFIG_FILE->colorValue("backgroundColor", _backgroundColor);
    _selectedColor = CONFIG_FILE->colorValue("selectedColor", _selectedColor);
    _pointSize = CONFIG_FILE->value("pointSize", (double)_pointSize).toDouble();
    _drawAxis = CONFIG_FILE->value("drawAxis", _drawAxis).toBool();
    m_drawGrid = CONFIG_FILE->value("drawGrid", drawGrid()).toBool();
    _useColor = CONFIG_FILE->value("useColor", _useColor).toBool();
    _useTransparency = CONFIG_FILE->value("useTransparency", _useTransparency).toBool();
    _useLight = CONFIG_FILE->value("useLight", _useLight).toBool();
    _cameraInfoDisplayed = (CameraInfoDisplayed)CONFIG_FILE->value("cameraInfoDisplayed", (int)_cameraInfoDisplayed).toInt();
    _cameraInfoPosition = (CameraInfoPosition)CONFIG_FILE->value("cameraInfoPosition", (int)_cameraInfoPosition).toInt();
    _cameraType = (CameraInterface::CameraType)CONFIG_FILE->value("cameraType", (int)_cameraType).toInt();
    setNormalColor(CONFIG_FILE->colorValue("normalColor", m_normalColor));
    setNormalLength(CONFIG_FILE->value("normalLength", m_normalLength).toFloat());
    setMustShowNormals(CONFIG_FILE->value("showNormals", m_mustShowNormals).toBool());
    setMustReduceNbrPointsInFastDraw(CONFIG_FILE->value("reduceNbrPointsInFastDraw", m_reducePointNbrInFastDraw).toBool());
    setMustReduceNbrPointsInNormalDraw(CONFIG_FILE->value("reduceNbrPointsInNormalDraw", m_reducePointNbrInNormalDraw).toBool());
    setMaxNbrPointsInFastDraw(CONFIG_FILE->value("maxNbrPointsInFastDraw", m_maxPointInFastDraw).toInt());

    DrawModesCollectionMutableIterator it(m_drawModes);

    while(it.hasNext()) {
        it.next();

        DrawMode dm = (DrawMode)CONFIG_FILE->value(QString("DrawMode_%1").arg((int)it.key()), (int)DM_VAO).toInt();
        it.setValue(dm);
    }

    CONFIG_FILE->endGroup();

    return true;
}

bool DM_GraphicsViewOptions::save()
{
    CONFIG_FILE->beginGroup("GraphicsOptions");

    CONFIG_FILE->setColorValue("backgroundColor", _backgroundColor);
    CONFIG_FILE->setColorValue("selectedColor", _selectedColor);
    CONFIG_FILE->setValue("pointSize", (double)_pointSize);
    CONFIG_FILE->setValue("drawAxis", _drawAxis);
    CONFIG_FILE->setValue("drawGrid", m_drawGrid);
    CONFIG_FILE->setValue("useColor", _useColor);
    CONFIG_FILE->setValue("useTransparency", _useTransparency);
    CONFIG_FILE->setValue("useLight", _useLight);
    CONFIG_FILE->setValue("cameraInfoDisplayed", (int)_cameraInfoDisplayed);
    CONFIG_FILE->setValue("cameraInfoPosition", (int)_cameraInfoPosition);
    CONFIG_FILE->setValue("cameraType", (int)_cameraType);
    CONFIG_FILE->setColorValue("normalColor", m_normalColor);
    CONFIG_FILE->setValue("showNormals", m_mustShowNormals);
    CONFIG_FILE->setValue("reduceNbrPointsInFastDraw", m_reducePointNbrInFastDraw);
    CONFIG_FILE->setValue("reduceNbrPointsInNormalDraw", m_reducePointNbrInNormalDraw);
    CONFIG_FILE->setValue("maxNbrPointsInFastDraw", m_maxPointInFastDraw);

    DrawModesCollectionIterator it(m_drawModes);

    while(it.hasNext()) {
        it.next();

        CONFIG_FILE->setValue(QString("DrawMode_%1").arg((int)it.key()), (int)it.value());
    }

    CONFIG_FILE->endGroup();

    return true;
}

bool DM_GraphicsViewOptions::loadFromXml(const QDomElement &el)
{
    QDomNodeList l = el.elementsByTagName("backgroundColor");

    if(!l.isEmpty())
        _backgroundColor = DM_DomUtils::QColorFromDom(l.at(0).toElement());

    l = el.elementsByTagName("selectedColor");

    if(!l.isEmpty())
        _selectedColor = DM_DomUtils::QColorFromDom(l.at(0).toElement());

    _pointSize = DM_DomUtils::qrealFromDom(el, "pointSize", _pointSize);
    _drawAxis = DM_DomUtils::boolFromDom(el, "drawAxis", _drawAxis);
    m_drawGrid = DM_DomUtils::boolFromDom(el, "drawGrid", m_drawGrid);
    _useColor = DM_DomUtils::boolFromDom(el, "useColor", _useColor);
    _useTransparency = DM_DomUtils::boolFromDom(el, "useTransparency", _useTransparency);
    _useLight = DM_DomUtils::boolFromDom(el, "useLight", _useLight);
    _cameraInfoDisplayed = (CameraInfoDisplayed)DM_DomUtils::intFromDom(el, "cameraInfoDisplayed", (int)_cameraInfoDisplayed);
    _cameraInfoPosition = (CameraInfoPosition) DM_DomUtils::intFromDom(el, "cameraInfoPosition", (int)_cameraInfoPosition);
    _cameraType = (CameraInterface::CameraType) DM_DomUtils::intFromDom(el, "cameraType", (int)_cameraType);

    l = el.elementsByTagName("normalColor");

    if(!l.isEmpty())
        setNormalColor(DM_DomUtils::QColorFromDom(l.at(0).toElement()));

    setNormalLength(DM_DomUtils::qrealFromDom(el, "normalLength", m_normalLength));
    setMustShowNormals(DM_DomUtils::boolFromDom(el, "showNormals", m_mustShowNormals));
    setMustReduceNbrPointsInFastDraw(DM_DomUtils::boolFromDom(el, "reduceNbrPointsInFastDraw", m_reducePointNbrInFastDraw));
    setMustReduceNbrPointsInNormalDraw(DM_DomUtils::boolFromDom(el, "reduceNbrPointsInNormalDraw", m_reducePointNbrInNormalDraw));
    setMaxNbrPointsInFastDraw(DM_DomUtils::intFromDom(el, "maxNbrPointsInFastDraw", m_maxPointInFastDraw));

    QDomElement drawModes = el.firstChildElement("DrawModes");

    if(!drawModes.isNull()) {
        DrawModesCollectionMutableIterator it(m_drawModes);

        while(it.hasNext()) {
            it.next();

            DrawMode dm = (DrawMode)DM_DomUtils::intFromDom(drawModes, QString("DrawMode_%1").arg((int)it.key()), (int)DM_VAO);
            it.setValue(dm);
        }
    }

    return true;
}

bool DM_GraphicsViewOptions::saveToXml(QDomElement &main, QDomDocument &doc) const
{
    main.appendChild(DM_DomUtils::QColorDomElement(_backgroundColor, "backgroundColor", doc));
    main.appendChild(DM_DomUtils::QColorDomElement(_selectedColor, "selectedColor", doc));
    main.setAttribute("pointSize", _pointSize);
    DM_DomUtils::setBoolAttribute(main, "drawAxis", _drawAxis);
    DM_DomUtils::setBoolAttribute(main, "drawGrid", m_drawGrid);
    DM_DomUtils::setBoolAttribute(main, "useColor", _useColor);
    DM_DomUtils::setBoolAttribute(main, "useTransparency", _useTransparency);
    DM_DomUtils::setBoolAttribute(main, "useLight", _useLight);
    main.setAttribute("cameraInfoDisplayed", (int)_cameraInfoDisplayed);
    main.setAttribute("cameraInfoPosition", (int)_cameraInfoPosition);
    main.setAttribute("cameraType", (int)_cameraType);
    main.appendChild(DM_DomUtils::QColorDomElement(m_normalColor, "normalColor", doc));
    main.setAttribute("normalLength", m_normalLength);
    DM_DomUtils::setBoolAttribute(main, "showNormals", m_mustShowNormals);
    DM_DomUtils::setBoolAttribute(main, "reduceNbrPointsInFastDraw", m_reducePointNbrInFastDraw);
    DM_DomUtils::setBoolAttribute(main, "reduceNbrPointsInNormalDraw", m_reducePointNbrInNormalDraw);
    main.setAttribute("maxNbrPointsInFastDraw", m_maxPointInFastDraw);

    QDomElement drawModes = doc.createElement("DrawModes");

    DrawModesCollectionIterator it(m_drawModes);

    while(it.hasNext()) {
        it.next();

        drawModes.setAttribute(QString("DrawMode_%1").arg((int)it.key()), (int)it.value());
    }

    main.appendChild(drawModes);

    return true;
}
