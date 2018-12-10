/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michaël KREBS (ARTS/ENSAM)

 This file is part of PluginShared library 2.0.

 PluginShared is free library: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 PluginShared is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PluginShared.  If not, see <http://www.gnu.org/licenses/lgpl.html>.

*****************************************************************************/

#ifndef INTERFACES_H
#define INTERFACES_H

#include <QtPlugin>
#include <QVariantList>
#include <QColor>
#include <QSharedPointer>
#include <QWidget>
#include <QPainter>
#include <QAbstractItemView>

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
#include <GL/gl.h>
#else
#include <qopengl.h>
#endif

#include "Eigen/Core"

#include "ct_defines.h"
#include "ct_model/interfacesformodels.h"

class QPen;
class QRectF;

class DocumentInterface;
class PainterInterface;
class ActionsManagerInterface;
class GraphicsViewInterface;
class ActionOptionsInterface;
class LogInterface;
class PluginManagerInterface;
class PluginEntryInterface;

class CT_VirtualAbstractStep;
class CT_AbstractStepPlugin;
class CT_AbstractItemDrawable;
class CT_AbstractResult;
class CT_AbstractAction;
class CT_AbstractLogListener;
class CT_OutAbstractItemModel;
class CT_AbstractMeshModel;
class CT_AbstractCloudIndex;
class CT_OutAbstractModel;
class CT_StandardColorCloudRegistered;
class CT_StandardNormalCloudRegistered;
class CT_StepsMenu;

class CameraInterface : public QObject
{
    Q_OBJECT

public:
    enum CameraType {
        PERSPECTIVE,
        ORTHOGRAPHIC
    };

    virtual ~CameraInterface() {}

    virtual double x() const = 0;
    virtual double y() const = 0;
    virtual double z() const = 0;
    virtual double cx() const = 0;
    virtual double cy() const = 0;
    virtual double cz() const = 0;
    virtual double rx() const = 0;
    virtual double ry() const = 0;
    virtual double rz() const = 0;

    virtual CameraInterface::CameraType type() const = 0;

    /**
     * @brief Returns the physical distance between the user's eyes and the screen (in meters).
     */
    virtual float focusDistance() const = 0;

    virtual const GLdouble* orientationMatrix() const = 0;
    virtual const GLdouble* orientationMatrixInversed() const = 0;

    virtual void modelViewMatrix(GLdouble m[16]) const = 0;

    // returns quaternion
    virtual void getOrientation(double &q0, double &q1, double &q2, double &q3) const = 0;

    /**
     * @brief Returns the normalized up vector of the Camera, defined in the world coordinate system.
     *       It corresponds to the Y axis.
     */
    virtual Eigen::Vector3d upVector() const = 0;

    /**
     * @brief Returns the normalized right vector of the Camera, defined in the world coordinate system.
     *       It corresponds to the X axis.
     */
    virtual Eigen::Vector3d rightVector() const = 0;

public slots:
    virtual void setX(double x) = 0;
    virtual void setY(double y) = 0;
    virtual void setZ(double z) = 0;

    virtual void setCX(double cx) = 0;
    virtual void setCY(double cy) = 0;
    virtual void setCZ(double cz) = 0;

    virtual void setRX(double xRot) = 0;
    virtual void setRY(double yRot) = 0;
    virtual void setRZ(double zRot) = 0;

    virtual void setType(CameraInterface::CameraType type) = 0;
    virtual void setPosition(double x, double y, double z, bool redrawView = true) = 0;
    virtual void setSceneCenter(double cx, double cy, double cz, bool redrawView = true) = 0;
    virtual void setViewDirection(double rx, double ry, double rz, bool redrawView = true) = 0;
    virtual void setOrientation(double q0, double q1, double q2, double q3, bool redrawView = true) = 0;

    virtual void showEntireScene() = 0;
    virtual void homePosition() = 0;
    virtual void alignCameraToXAxis() = 0;
    virtual void alignCameraToYAxis() = 0;
    virtual void alignCameraToZAxis() = 0;
    virtual void initCameraCenter() = 0;
    virtual void fixCameraCenterToItemsBarycenter() = 0;
    virtual void fixCameraCenterToSelectedItemsBarycenter() = 0;
    virtual void fitCameraToVisibleItems() = 0;
    virtual void fitToSpecifiedBox(const Eigen::Vector3d &bot, const Eigen::Vector3d &top) = 0;


signals:
    void coordinatesChanged();
};



/**
 * @brief Represent a view that show a tree model
 */
class TreeViewInterface : public InDocumentViewInterface
{
public:
    virtual ~TreeViewInterface() {}

    /**
     * @brief Convert a QModelIndex to a CT_AbstractItemDrawable. Can return NULL if the QModelIndex is not valid.
     */
    virtual CT_AbstractItemDrawable* itemDrawableFromIndex(const QModelIndex &index) const = 0;

    /**
     * @brief Returns the model index of the item at the viewport coordinates point.
     */
    virtual QModelIndex indexAt(const QPoint &point) const = 0;

    /**
     * @brief Returns the viewport widget.
     */
    virtual QWidget* treeViewport() const = 0;

    /**
     * @brief This method controls whether the user can select one or many items
     *        and, in many-item selections, whether the selection must
     *        be a continuous range of items.
     */
    virtual void setSelectionMode(QAbstractItemView::SelectionMode mode) = 0;

    /**
     * @brief Returns the current selection mode
     */
    virtual QAbstractItemView::SelectionMode selectionMode() const = 0;

    /**
     * @brief Returns selected indexes. The list contains no duplicates, and is not sorted.
     */
    virtual QModelIndexList selectedIndexes () const = 0;

    /**
     * @brief Returns the current selection model.
     */
    virtual QItemSelectionModel* selectionModel() const = 0;

    /**
     * @brief Refresh the tree view
     */
    virtual void refreshAll() = 0;

    /**
     * @brief Refresh only children of the list of QModelIndex passed in parameter
     */
    virtual void refreshItems(const QList<QModelIndex> &indexes) = 0;

    /**
     * @brief Search the QModelIndex corresponding to the ItemDrawable passed in parameter
     *        and refresh children for all ItemDrawable in the list
     * @warning slow ! prefer the method "refreshItems" with QModelIndex or "refreshAll"
     */
    virtual void refreshItems(const QList<CT_AbstractItemDrawable*> &items) = 0;
};





class ActionsManagerInterface
{
public:
    virtual ~ActionsManagerInterface() {}

    /**
     * @brief Add a new action. The manager is responsible of the life of the action. You must not delete
     *        your action ! The action is deleted if you remove it or if the manager is destroyed or if the
     *        manual or debug mode is finished.
     *
     *        Before set the current action to a document you must add it in the actions manager by this method.
     *
     * @return return false if the action can't be added because already exist in a plugins.
     */
    virtual bool addAction(CT_AbstractAction *action) = 0;

    /**
     * @brief Remove the action and the associated button from the view. Action is deleted after it has been removed.
     *
     * @warning if you remove a action that is activate in a document it will be finished and deleted.
     *
     * @return return false if it can't be removed because it's a action from a plugin or if she don't exist in the list.
     */
    virtual bool removeAction(CT_AbstractAction *action) = 0;
    virtual bool removeAction(const QString &uniqueName) = 0;

    /**
     * @brief Return true if the action exist in the actions added by a step or available in a plugin
     */
    virtual bool existAction(const CT_AbstractAction *action) const = 0;
    virtual bool existAction(const QString &uniqueName) const = 0;

    /**
     * @brief Return the action that have this unique name. NULL if not exist.
     */
    virtual CT_AbstractAction* action(const QString &uniqueName) const = 0;
};

/**
 * @brief Represent a options widget of a action
 */
class ActionOptionsInterface : public QWidget
{
public:
    virtual ~ActionOptionsInterface() {}

    /**
     * @brief Return the action that created this widget
     */
    virtual CT_AbstractAction* action() const = 0;
};



#define CoreInterface_iid "com.krebs.michael.ONF.PluginSharedV2.CoreInterface"
#define LogInterface_iid "com.krebs.michael.ONF.PluginSharedV2.LogInterface"
#define PluginManagerInterface_iid "com.krebs.michael.ONF.PluginSharedV2.PluginManagerInterface"
#define SettingsWriterInterface_iid "com.krebs.michael.ONF.PluginSharedV2.SettingsWriterInterface"
#define SettingsReaderInterface_iid "com.krebs.michael.ONF.PluginSharedV2.SettingsReaderInterface"

#define GuiManagerInterface_iid "com.krebs.michael.ONF.PluginSharedV2.GuiManagerInterface"
#define GuiContextInterface_iid "com.krebs.michael.ONF.PluginSharedV2.GuiContextInterface"
#define DocumentManagerInterface_iid "com.krebs.michael.ONF.PluginSharedV2.DocumentManagerInterface"
#define DocumentInterface_iid "com.krebs.michael.ONF.PluginSharedV2.DocumentInterface"
#define InDocumentViewInterface_iid "com.krebs.michael.ONF.PluginSharedV2.InDocumentViewInterface"
#define GraphicsViewInterface_iid "com.krebs.michael.ONF.PluginSharedV2.GraphicsViewInterface"
#define GraphicsViewOptionsInterface_iid "com.krebs.michael.ONF.PluginSharedV2.GraphicsViewOptionsInterface"
#define GraphicsViewSignalEmitterInterface_iid "com.krebs.michael.ONF.PluginSharedV2.GraphicsViewSignalEmitterInterface"
#define CameraInterface_iid "com.krebs.michael.ONF.PluginSharedV2.CameraInterface"
#define PainterInterface_iid "com.krebs.michael.ONF.PluginSharedV2.PainterInterface"
#define TreeViewInterface_iid "com.krebs.michael.ONF.PluginSharedV2.TreeViewInterface"
#define ActionsManagerInterface_iid "com.krebs.michael.ONF.PluginSharedV2.ActionsManagerInterface"
#define ActionOptionsInterface_iid "com.krebs.michael.ONF.PluginSharedV2.ActionOptionsInterface"

#define PluginEntryInterface_iid "com.krebs.michael.ONF.PluginSharedV2.PluginEntryInterface"

// CORE
Q_DECLARE_INTERFACE(CoreInterface, CoreInterface_iid)
Q_DECLARE_INTERFACE(LogInterface, LogInterface_iid)
Q_DECLARE_INTERFACE(PluginManagerInterface, PluginManagerInterface_iid)
Q_DECLARE_INTERFACE(SettingsWriterInterface, SettingsWriterInterface_iid)
Q_DECLARE_INTERFACE(SettingsReaderInterface, SettingsReaderInterface_iid)

// GUI
Q_DECLARE_INTERFACE(GuiManagerInterface, GuiManagerInterface_iid)
Q_DECLARE_INTERFACE(GuiContextInterface, GuiContextInterface_iid)
Q_DECLARE_INTERFACE(DocumentManagerInterface, DocumentManagerInterface_iid)
Q_DECLARE_INTERFACE(DocumentInterface, DocumentInterface_iid)
Q_DECLARE_INTERFACE(InDocumentViewInterface, InDocumentViewInterface_iid)
Q_DECLARE_INTERFACE(GraphicsViewInterface, GraphicsViewInterface_iid)
Q_DECLARE_INTERFACE(GraphicsViewOptionsInterface, GraphicsViewOptionsInterface_iid)
Q_DECLARE_INTERFACE(GraphicsViewSignalEmitterInterface, GraphicsViewSignalEmitterInterface_iid)
Q_DECLARE_INTERFACE(CameraInterface, CameraInterface_iid)
Q_DECLARE_INTERFACE(PainterInterface, PainterInterface_iid)
Q_DECLARE_INTERFACE(TreeViewInterface, TreeViewInterface_iid)
Q_DECLARE_INTERFACE(ActionsManagerInterface, ActionsManagerInterface_iid)
Q_DECLARE_INTERFACE(ActionOptionsInterface, ActionOptionsInterface_iid)

// PLUGINS
Q_DECLARE_INTERFACE(PluginEntryInterface, PluginEntryInterface_iid)

#endif // INTERFACES_H
