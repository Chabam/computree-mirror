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

#include "interfacessettings.h"
#include "Eigen/Core"

#include "ct_defines.h"

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
class CT_AbstractResult;
class CT_AbstractItemDrawable;
class CT_AbstractAction;
class CT_AbstractLogListener;
class CT_OutAbstractItemModel;
class CT_AbstractMeshModel;
class CT_AbstractCloudIndex;
class CT_OutAbstractModel;
class CT_StandardColorCloudRegistered;
class CT_StandardNormalCloudRegistered;
class CT_StepsMenu;

class CT_Color;
class CT_Normal;

typedef QSharedPointer<CT_AbstractCloudIndexRegistered> CT_SPCIR;
typedef QSharedPointer<CT_AbstractCloudIndexRegistered> CT_SFCIR;
typedef QSharedPointer<CT_AbstractCloudIndexRegistered> CT_SECIR;

// If you change object type in AMKgl Viewer you must change it here too !
enum GraphicsObjectType {
    GOT_Point = 0,              // local point (not from global point cloud)
    GOT_Ellipse,                // ellipse
    GOT_Line,                   // line
    GOT_Box,                    // box or voxel
    GOT_Sphere,                 // sphere
    GOT_Cylinder,               // cylinder
    GOT_Quads,                  // rectangle
    GOT_Triangle,               // triangle
    GOT_Pyramid,                // pyramid
    GOT_PointGlobalCloud,       // point from global point cloud
    GOT_LineGlobalCloud,        // line (edges of mesh) from global point cloud
    GOT_TriangleGlobalCloud     // triangle (faces of mesh) from global point cloud
};

// Flags that can be used to set an object selected or visible or set a specified developper flags in
// action per example.
// !!! Selected and Visible must not be modified !!!
enum GraphicsObjectFlag {
    GOF_Selected = 1,
    GOF_Visible = 2,
    GOF_DevelopperFlag1 = 4,
    GOF_DevelopperFlag2 = 8,
    GOF_DevelopperFlag3 = 16,
    GOF_DevelopperFlag4 = 32,
    GOF_DevelopperFlag5 = 64,
    GOF_Reserved = 128
};

Q_DECLARE_FLAGS(GraphicsObjectFlags, GraphicsObjectFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(GraphicsObjectFlags)

/**
 * @brief Property of a GraphicsObjectFlag (see GraphicsViewInterface for more information)
 */
struct GraphicsObjectFlagProperty
{
    /**
     * @brief Color of objects when the flag is set
     */
    QColor onColor;
};

/*!
 *  \brief Représente un objet qui doit être utilisé par les ItemDrawable
 *         pour dessiner leur élements 3D/2D.
 */
class PainterInterface
{
public:
    virtual ~PainterInterface() {}

    /**
     * @brief Save the current color, pen, etc...
     */
    virtual void save() = 0;

    /**
     * @brief Restore the last color, pen, etc... that was saved
     */
    virtual void restore() = 0;

    /**
     * @brief Set to false if you want to disable all next call to "setPointSize" method will be ignored.
     * @warning Don't forget to call enableSetPointSize(true) at the end of your method where you have called enableSetPointSize(false) !
     */
    virtual void enableSetPointSize(bool enable) = 0;

    /**
     * @brief Change the point size for futur draw
     */
    virtual void setPointSize(float size) = 0;

    /**
     * @brief Restore the default point size
     */
    virtual void restoreDefaultPointSize() = 0;

    /**
     * @brief Modify the pen for futur draw
     */
    virtual void setPen(const QPen &pen) = 0;

    /**
     * @brief Restore the default pen
     */
    virtual void restoreDefaultPen() = 0;

    /**
     * @brief Set to false if you want to disable all next call to "setColor" method will be ignored.
     * @warning Don't forget to call enableSetColor(true) at the end of your method where you have called enableSetColor(false) !
     */
    virtual void enableSetColor(bool enable) = 0;

    /**
     * @brief Change the color for futur draw
     */
    virtual void setColor(int r, int g, int b) = 0;
    virtual void setColor(QColor color) = 0;

    /**
     * @brief Returns the current color used
     */
    virtual QColor getColor() = 0;

    /**
     * @brief Draw a point that was not in the global points cloud (otherwise use drawPoint(globalIndex) method)
     */
    virtual void drawPoint(const double &x, const double &y, const double &z) = 0;

    /**
     * @brief Draw a point that was not in the global points cloud (otherwise use drawPoint(globalIndex) method)
     */
    virtual void drawPoint(double *p) = 0;

    /**
     * @brief Draw a point that was in the global points cloud
     * @param globalIndex: index of the point in the global points cloud
     */
    virtual void drawPoint(const size_t &globalIndex) = 0;

    /**
     * @brief Draw all points that was pointed by indexes in "pci" in the global points cloud
     * @param pci : indexes of points to draw
     */
    virtual void drawPointCloud(const CT_AbstractCloudIndex *pci) = 0;

    /**
     * @brief Draw a 3D cube defined by its bounding box (bottom left and top right corner). (NOT FILLED by default)
     */
    virtual void drawCube(const double &x1, const double &y1, const double &z1, const double &x2, const double &y2, const double &z2) = 0;

    /**
     * @brief Draw a 3D cube defined by its bounding box (bottom left and top right corner).
     * @param faces : Specifies the polygons that mode applies to. Must be GL_FRONT_AND_BACK for front- and back-facing polygons. Show glPolygonMode(...) for more details.
     * @param mode : Specifies how polygons will be rasterized. Accepted values are GL_POINT, GL_LINE, and GL_FILL. Show glPolygonMode(...) for more details.
     */
    virtual void drawCube(const double &x1, const double &y1, const double &z1, const double &x2, const double &y2, const double &z2, GLenum faces, GLenum mode) = 0;

    /**
     * @brief Draw a not filled quad face in 3D
     */
    virtual void drawQuadFace( const double &x1, const double &y1, const double &z1,
                               const double &x2, const double &y2, const double &z2,
                               const double &x3, const double &y3, const double &z3,
                               const double &x4, const double &y4, const double &z4 ) = 0;

    /**
     * @brief Draw a filled quad face in 3D
     */
    virtual void fillQuadFace( const double &x1, const double &y1, const double &z1,
                               const double &x2, const double &y2, const double &z2,
                               const double &x3, const double &y3, const double &z3,
                               const double &x4, const double &y4, const double &z4 ) = 0;

    /**
     * @brief Draw a not filled quad face in 3D with associated colors
     */
    virtual void drawQuadFace( const double &x1, const double &y1, const double &z1, int r1, int g1, int b1,
                               const double &x2, const double &y2, const double &z2, int r2, int g2, int b2,
                               const double &x3, const double &y3, const double &z3, int r3, int g3, int b3,
                               const double &x4, const double &y4, const double &z4, int r4, int g4, int b4 ) = 0;

    /**
     * @brief Draw a filled quad face in 3D with associated colors
     */
    virtual void fillQuadFace( const double &x1, const double &y1, const double &z1, int r1, int g1, int b1,
                               const double &x2, const double &y2, const double &z2, int r2, int g2, int b2,
                               const double &x3, const double &y3, const double &z3, int r3, int g3, int b3,
                               const double &x4, const double &y4, const double &z4, int r4, int g4, int b4 ) = 0;

    /**
     * @brief Draw a rectangle in XY plane at level Z
     */
    virtual void drawRectXY(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &z) = 0;

    /**
     * @brief Draw a filled rectangle in XY plane at level Z
     */
    virtual void fillRectXY(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &z) = 0;


    /**
     * @brief Draw a rectangle in XZ plane at level Y
     */
    virtual void drawRectXZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &y) =0;

    /**
     * @brief Draw a filled rectangle in XZ plane at level Y
     */
    virtual void fillRectXZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &y) = 0;


    /**
     * @brief Draw a rectangle in YZ plane at level X
     */
    virtual void drawRectYZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &x) = 0;

    /**
     * @brief Draw a filled rectangle in YZ plane at level X
     */
    virtual void fillRectYZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &x) = 0;

    /**
     * @brief Draw a line in 3D
     */
    virtual void drawLine(const double &x1, const double &y1, const double &z1, const double &x2, const double &y2, const double &z2) = 0;

    /**
     * @brief Draw a line in 3D from points in the global cloud
     */
    virtual void drawLine(const size_t& globalEdgeIndex,
                          const size_t &p1GlobalIndex,
                          const size_t &p2GlobalIndex) = 0;

    /**
     * @brief Draw a triangle in 3D
     */
    virtual void drawTriangle(const double &x1, const double &y1, const double &z1,
                              const double &x2, const double &y2, const double &z2,
                              const double &x3, const double &y3, const double &z3) = 0;

    /**
     * @brief Draw a triangle in 3D from points in the global cloud
     */
    virtual void drawTriangle(const size_t& globalFaceIndex,
                              const size_t &p1GlobalIndex,
                              const size_t &p2GlobalIndex,
                              const size_t &p3GlobalIndex) = 0;

    /**
     * @brief Draw a 2D circle (XY)
     */
    virtual void drawCircle(const double &x, const double &y, const double &z, const double &radius) = 0;

    /**
     * @brief Draw a 3D circle
     */
    virtual void drawCircle3D(const Eigen::Vector3d &center, const Eigen::Vector3d &direction, const double &radius) = 0;

    /**
     * @brief Draw a 2D ellipse (XY)
     */
    virtual void drawEllipse(const double &x, const double &y, const double &z, const double &radiusA, const double &radiusB) = 0;

    /**
     * @brief Call this method when you want to start to draw a polygon
     */
    virtual void beginPolygon() = 0;

    /**
     * @brief Call this method to add a point to the polygon
     */
    virtual void addPointToPolygon(const double &x, const double &y, const double &z) = 0;

    /**
     * @brief Call this method to finish the draw of the polygon
     */
    virtual void endPolygon() = 0;

    /**
     * @brief Draw a 2D cylinder (height is in Z axis)
     */
    virtual void drawCylinder(const double & x, const double & y, const double & z, const double & radius, const double & height) = 0;

    /**
     * @brief Draw a 3D cylinder
     */
    virtual void drawCylinder3D(const Eigen::Vector3d &center, const Eigen::Vector3d &direction, const double & radius, const double & height) = 0;

    /**
     * @brief Draw a pyramid given its top point and its base
     */
    virtual void drawPyramid(const double &topX, const double &topY, const double &topZ,
                             const double &base1X, const double &base1Y, const double &base1Z,
                             const double &base2X, const double &base2Y, const double &base2Z,
                             const double &base3X, const double &base3Y, const double &base3Z,
                             const double &base4X, const double &base4Y, const double &base4Z) = 0;
    /**
     * @brief Draw a part of a sphere given the angles bounds
     */
    virtual void drawPartOfSphere (const double &centerX, const double &centerY, const double &centerZ,
                                   const double &radius,
                                   const double &initTheta, const double &endTheta,
                                   const double &initPhi, const double &endPhi,
                                   bool radians = true) = 0;

    /**
     * @brief Draw faces of a mesh
     */
    virtual void drawMesh(const CT_AbstractMeshModel *mesh) = 0;

    /**
     * @brief Draw faces of a mesh
     */
    virtual void drawFaces(const CT_AbstractMeshModel *mesh) = 0;

    /**
     * @brief Draw edges of a mesh
     */
    virtual void drawEdges(const CT_AbstractMeshModel *mesh) = 0;

    /**
     * @brief Draw points of a mesh
     */
    virtual void drawPoints(const CT_AbstractMeshModel *mesh) = 0;

};

/*!
 *  \brief Représente les options de la vue
 */
class GraphicsViewOptionsInterface
{
public:
    virtual ~GraphicsViewOptionsInterface() {}

    virtual QColor getSelectedColor() const = 0;
    virtual QColor getBackgroundColor() const = 0;
    virtual float getPointSize() const = 0;
    virtual bool drawAxis() const = 0;
    virtual bool useColor() const = 0;
    virtual bool useTransparency() const = 0;
    virtual int getFastDrawTime() const = 0;

    virtual void setBackgroudColor(QColor &color) { Q_UNUSED(color) }
    virtual void setPointSize(float size) { Q_UNUSED(size) }

};

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
 * @brief Represent a view that a document can contains
 */
class InDocumentViewInterface
{
public:
    virtual ~InDocumentViewInterface() {}

    /**
     * @brief Returns the document that contains this view
     */
    virtual DocumentInterface* document() const = 0;
};

/**
 * @brief With this class you can query the status of an object
 */
class ObjectStatusQueryiorInterface
{
public:
    virtual ~ObjectStatusQueryiorInterface() {}

    /**
     * @brief Returns the number of objects available
     */
    virtual size_t getNumberOfObjects() const = 0;

    /**
     * @brief Returns the number of vertex per object
     */
    virtual size_t getNumberOfVertexPerObject() const = 0;

    /**
     * @brief Returns true if informations (visible, selected) is available, false otherwise
     */
    virtual bool isInfosAvailable() const = 0;

    /**
     * @brief Returns if the specified object is visible or not
     * @warning If method "isInfosAvailable" return false this method will return an
     *          invalid value and generate an assert in debug mode
     */
    virtual bool isVisible(const size_t& objectIndex) const = 0;

    /**
     * @brief Returns if the specified object is selected or not
     * @warning If method "isInfosAvailable" return false this method will return an
     *          invalid value and generate an assert in debug mode
     */
    virtual bool isSelected(const size_t& objectIndex) const = 0;

    /**
     * @brief Returns if the flag of the specified object is on or not
     * @warning If method "isInfosAvailable" return false this method will return an
     *          invalid value and generate an assert in debug mode
     */
    virtual bool isFlagOn(const size_t& objectIndex,
                          const GraphicsObjectFlag& flag) const = 0;

    /**
     * @brief Returns the complete flags value of the specified object
     */
    virtual GraphicsObjectFlags getFlagsValue(const size_t& objectIndex) const = 0;

    /**
     * @brief Returns true if colors is available, false otherwise
     */
    virtual bool isColorsAvailable() const = 0;

    /**
     * @brief Returns the color of the specified vertex of the specified object
     * @warning If method "isColorsAvailable" return false this method will return an
     *          invalid value and generate an assert in debug mode
     */
    virtual const CT_Color& getColor(const size_t& objectIndex,
                                     const size_t& vertexIndex) const = 0;

    /**
     * @brief Returns true if normals is available, false otherwise
     */
    virtual bool isNormalsAvailable() const = 0;

    /**
     * @brief Returns the normal of the specified vertex of the specified object
     * @warning If method "isNormalsAvailable" return false this method will return an
     *          invalid value and generate an assert in debug mode
     */
    virtual const CT_Normal& getNormal(const size_t& objectIndex,
                                       const size_t& vertexIndex) const = 0;
};

/**
 * @brief With this class you can modify an object
 */
class ObjectsModifierInterface : public ObjectStatusQueryiorInterface
{
public:
    virtual ~ObjectsModifierInterface() {}

    /**
     * @brief Apply a transformation matrix to all vertex of all objects. Use it if you want
     *        to translate/rotate/scale objects.
     *
     *        Do nothing for global points/edges/faces !
     *
     * @warning Be carreful when you translate or scale an object because if its
     *          coordinates change from one coordinate system to another the system
     *          won't detect it and the draw of it can be altered !
     */
    virtual void transform(const Eigen::Matrix4f& trMatrix) = 0;

    /**
     * @brief Apply a translation to objects.
     *
     *        Do nothing for global points/edges/faces !
     *
     * @warning Be carreful when you translate an object because if its
     *          coordinates change from one coordinate system to another the system
     *          won't detect it and the draw of the object can be altered !
     */
    virtual void translate(const Eigen::Vector3f& translation) = 0;

    /**
     * @brief Apply a rotation to objects.
     *
     *        Do nothing for global points/edges/faces !
     */
    virtual void rotate(const Eigen::AngleAxis<float>& rotation) = 0;
    virtual void rotate(const Eigen::Quaternion<float>& rotation) = 0;

    /**
     * @brief Scale objects.
     *
     *        Do nothing for global points/edges/faces !
     *
     * @warning Be carreful when you scale the object because if its
     *          coordinates change from one coordinate system to another the system
     *          won't detect it and the draw of the object can be altered !
     */
    virtual void scale(const Eigen::Vector3f& scaling) = 0;

    /**
     * @brief Apply a transformation matrix to all vertex of the specified objects. Use it if you want
     *        to translate/rotate/scale objects.
     *
     *        Do nothing for global points/edges/faces !
     *
     * @warning Be carreful when you translate or scale an object because if its
     *          coordinates change from one coordinate system to another the system
     *          won't detect it and the draw of it can be altered !
     */
    virtual void transform(const size_t& objectIndex,
                           const Eigen::Matrix4f& trMatrix) = 0;

    /**
     * @brief Apply a translation to the specified object.
     *
     *        Do nothing for global points/edges/faces !
     *
     * @warning Be carreful when you translate an object because if its
     *          coordinates change from one coordinate system to another the system
     *          won't detect it and the draw of the object can be altered !
     */
    virtual void translate(const size_t& objectIndex,
                           const Eigen::Vector3f& translation) = 0;

    /**
     * @brief Apply a rotation to the specified object.
     *
     *        Do nothing for global points/edges/faces !
     */
    virtual void rotate(const size_t& objectIndex,
                        const Eigen::AngleAxis<float>& rotation) = 0;
    virtual void rotate(const size_t& objectIndex,
                        const Eigen::Quaternion<float>& rotation) = 0;

    /**
     * @brief Scale the specified object.
     *
     *        Do nothing for global points/edges/faces !
     *
     * @warning Be carreful when you scale the object because if its
     *          coordinates change from one coordinate system to another the system
     *          won't detect it and the draw of the object can be altered !
     */
    virtual void scale(const size_t& objectIndex,
                       const Eigen::Vector3f& scaling) = 0;

    /**
     * @brief Set the new color to all vertex of all objects
     */
    virtual void setColor(const CT_Color& newColor) = 0;

    /**
     * @brief Set the new color to all vertex of the specified object
     */
    virtual void setColor(const size_t& objectIndex,
                          const CT_Color& newColor) = 0;

    /**
     * @brief Set the new color to the specified vertex of the specified object
     */
    virtual void setColor(const size_t& objectIndex,
                          const size_t& vertexIndex,
                          const CT_Color& newColor) = 0;

    /**
     * @brief Set the new normal to all objects
     */
    virtual void setNormal(const CT_Normal& newNormal) = 0;

    /**
     * @brief Set the new normal to the specified object
     */
    virtual void setNormal(const size_t& objectIndex,
                           const CT_Normal& newNormal) = 0;

    /**
     * @brief Set the new normal to the specified vertex of the specified object
     */
    virtual void setNormal(const size_t& objectIndex,
                           const size_t& vertexIndex,
                           const CT_Normal& newNormal) = 0;

    /**
     * @brief Set if all objects is visible or not
     */
    virtual void setVisible(bool visible) = 0;

    /**
     * @brief Set if the specified object is visible or not
     */
    virtual void setVisible(const size_t& objectIndex,
                            bool visible) = 0;

    /**
     * @brief Toggle all objects visibility
     */
    void toggleVisibility() { toggleFlags(GOF_Visible); }

    /**
     * @brief Toggle the specified object visibility
     */
    void toggleVisibility(const size_t& objectIndex) { toggleFlags(objectIndex, GOF_Visible); }

    /**
     * @brief Set if all objects is selected or not
     */
    virtual void setSelected(bool selected) = 0;

    /**
     * @brief Set if the specified object is selected or not
     */
    virtual void setSelected(const size_t& objectIndex,
                             bool selected) = 0;
    /**
     * @brief Toggle all objects selection
     */
    void toggleSelection() { toggleFlags(GOF_Selected); }

    /**
     * @brief Toggle the specified object selection
     */
    void toggleSelection(const size_t& objectIndex) { toggleFlags(objectIndex, GOF_Selected); }

    /**
     * @brief Set one or multiple flags on/off in the same time for all objects.
     */
    virtual void setFlags(const GraphicsObjectFlags& flags,
                          bool on) = 0;

    /**
     * @brief Set one or multiple flags on/off in the same time for the specified object.
     */
    virtual void setFlags(const size_t& objectIndex,
                          const GraphicsObjectFlags& flags,
                          bool on) = 0;
    /**
     * @brief Toggle one or multiple flags on/off in the same time for all objects.
     */
    virtual void toggleFlags(const GraphicsObjectFlags& flags) = 0;

    /**
     * @brief Toggle one or multiple flags on/off in the same time for the specified object.
     */
    virtual void toggleFlags(const size_t& objectIndex,
                             const GraphicsObjectFlags& flags) = 0;
};

/**
 * @brief This class is provided by the gui to follow you
 *        when you visit objects. It will be useful to modify or get
 *        access to information of objects.
 */
class ObjectsVisitorFollowerInterface
{
public:
    virtual ~ObjectsVisitorFollowerInterface() {}

    /**
     * @brief Returns the number of objects available
     */
    virtual size_t getNumberOfObjects() const = 0;

    /**
     * @brief Returns the number of vertex per object
     */
    virtual size_t getNumberOfVertexPerObject() const = 0;

    /**
     * @brief Returns the current object type
     */
    virtual GraphicsObjectType currentObjectType() const = 0;

    /**
     * @brief Returns the current item model used
     */
    virtual CT_OutAbstractItemModel* currentItemModel() const = 0;

    /**
     * @brief Create an interface that allow the developper to query the status of all new objects
     */
    virtual QSharedPointer<ObjectStatusQueryiorInterface> createObjectsStatusQueryior() = 0;

    /**
     * @brief Create an interface that allow the developper to modify all new objects
     */
    virtual QSharedPointer<ObjectsModifierInterface> createObjectsModifier() = 0;
};

/**
 * @brief Inherit from this class in your plugin to visit objects
 *        in a scene
 */
class ObjectsVisitorInterface
{
public:
    virtual ~ObjectsVisitorInterface() {}

    /**
     * @brief Set the follower that allow ou to get some informations and the possibility
     *        to create a objects modifier
     */
    virtual void setFollower(const ObjectsVisitorFollowerInterface* follower) = 0;

    /**
     * @brief You must return true if you want to continue the visit, false to abort it
     */
    virtual bool continueVisit() const = 0;

    /**
     * @brief Called when new objects is available.
     *
     *        If you want to modify colors/normals/visibility/etc.... you must call the
     *        method "createObjectsModifier" that allow you to do this type of operations.
     *
     *        If you want to know the status of elements you must call the method
     *        "createObjectsStatusQueryior" that allow you to do this type of operations.
     */
    virtual void newObjectsAvailable() = 0;
};

/**
 * @brief Represent a manager of signals for the GraphicsViewInterface
 */
class GraphicsViewSignalEmitterInterface : public QObject
{
    Q_OBJECT

public:
    virtual ~GraphicsViewSignalEmitterInterface() {}
    virtual GraphicsViewInterface* graphicsView() const = 0;

signals:
    void drawingStarted();
    void drawingFinished();
};


/**
 * @brief Represent a graphics view in a document
 */
class GraphicsViewInterface : public InDocumentViewInterface
{
public:
    // Selection mode
    enum SelectionMode{
        // don't change the order !!! for a serie the first must always be SELECT_... and the last REMOVE_ONE_...

        NONE = 0,       // do nothing
        SELECT,         // make a new selection, all itemdrawable selected before was removed from list
        ADD,            // add new itemdrawable selected to list
        REMOVE,         // remove new itemdrawable selected from list
        SELECT_ONE,     // add first itemdrawable selected to the list and remove other
        ADD_ONE,        // add first itemdrawable selected to the list
        REMOVE_ONE,     // remove first itemdrawable selected from the list
        // same but for points
        SELECT_POINTS,
        ADD_POINTS,
        REMOVE_POINTS,
        SELECT_ONE_POINT,
        ADD_ONE_POINT,
        REMOVE_ONE_POINT,
        // same but for faces
        SELECT_FACES,
        ADD_FACES,
        REMOVE_FACES,
        SELECT_ONE_FACE,
        ADD_ONE_FACE,
        REMOVE_ONE_FACE,
        // same but for edges
        SELECT_EDGES,
        ADD_EDGES,
        REMOVE_EDGES,
        SELECT_ONE_EDGE,
        ADD_ONE_EDGE,
        REMOVE_ONE_EDGE
    };

    // colors of elements
    enum ColorCloudType {
        CPointCloud,            // colors of points
        CFaceCloud,             // colors of faces
        CEdgeCloud              // colors of edges
    };

    // normals of elements
    enum NormalCloudType {
        NPointCloud,            // normals of points
        NFaceCloud,             // normals of faces
        NEdgeCloud              // normals of edges
    };

    // type of graphics view
    enum GraphicsViewType {
        GV2D,                   // 2D
        GV3D                    // 3D
    };

    virtual ~GraphicsViewInterface() {}

    virtual void validateOptions() {}


    /**
     * @brief Return the signal emitter of the graphics view
     */
    virtual GraphicsViewSignalEmitterInterface* signalEmitter() const = 0;

    /**
     * @brief Return the options of the graphics view
     */
    virtual GraphicsViewOptionsInterface& getOptions() = 0;

    /**
     * @brief Return the color cloud for the type passed in parameter. Can be NULL if it was not created.
     */
    virtual QSharedPointer<CT_StandardColorCloudRegistered> colorCloudOf(ColorCloudType type) const = 0;

    /**
     * @brief Return the normal cloud for the type passed in parameter. Can be NULL if it was not created.
     */
    virtual QSharedPointer<CT_StandardNormalCloudRegistered> normalCloudOf(NormalCloudType type) const = 0;

    /**
     * @brief Change the color of the point. You must call dirtyColorsOfItemDrawablesWithPoints() if you want that change was visible.
     * @param globalIndexOfPoint : the global index of the point ot change his color
     * @param color : the new color
     */
    virtual void setColorOfPoint(const size_t &globalIndexOfPoint, const CT_Color &color) = 0;

    /**
     * @brief Returns the color of the point.
     * @warning if you change the value of the color you must call dirtyColorsOfItemDrawablesWithPoints() to update it.
     * @param globalIndexOfPoint : the global index of the point
     * @return the color of the point
     */
    virtual CT_Color& colorOfPoint(const size_t &globalIndexOfPoint) = 0;

    /**
     * @brief Returns the color of the point.
     * @param globalIndexOfPoint : the global index of the point
     * @return the color of the point
     */
    virtual const CT_Color& constColorOfPoint(const size_t &globalIndexOfPoint) const = 0;

    /**
     * @brief Change the normal of the point. You must call dirtyNormalsOfItemDrawablesWithPoints() if you want that change was visible.
     * @param globalIndexOfPoint : the global index of the point ot change his normal
     * @param normal : the new normal
     */
    virtual void setNormalOfPoint(const size_t &globalIndexOfPoint, const CT_Normal &normal) = 0;

    /**
     * @brief Returns the normal of the point.
     * @warning if you change the value of the normal you must call dirtyNormalsOfItemDrawablesWithPoints() to update it.
     * @param globalIndexOfPoint : the global index of the point
     * @return the normal of the point
     */
    virtual CT_Normal& normalOfPoint(const size_t &globalIndexOfPoint) = 0;

    /**
     * @brief Returns the normal of the point.
     * @param globalIndexOfPoint : the global index of the point
     * @return the normal of the point
     */
    virtual const CT_Normal& constNormalOfPoint(const size_t &globalIndexOfPoint) const = 0;

    /**
     * @brief Update colors of all elements in the graphics view that use global points
     */
    virtual void dirtyColorsOfItemDrawablesWithPoints() = 0;

    /**
     * @brief Update normals of all elements in the graphics view that use global points
     */
    virtual void dirtyNormalsOfItemDrawablesWithPoints() = 0;

    /**
     * @brief Modify the property of the specified flag for all objects
     * @param flag : the flag to change
     * @param property : the new property to use
     */
    virtual void setFlagProperty(const GraphicsObjectFlag& flag,
                                 const GraphicsObjectFlagProperty& property) = 0;

    /**
     * @brief Modify the property of the specified flag for the specified object
     * @param flag : the flag to change
     * @param typeOfObject : the type of the object that will use this property
     * @param property : the new property to use
     */
    virtual void setFlagProperty(const GraphicsObjectFlag& flag,
                                 const GraphicsObjectType& typeOfObject,
                                 const GraphicsObjectFlagProperty& property) = 0;

    /**
     * @brief Returns the flag property currently used for the specified flag and specified object
     */
    virtual GraphicsObjectFlagProperty getFlagProperty(const GraphicsObjectFlag& flag,
                                                       const GraphicsObjectType& typeOfObject) const = 0;

    /**
     * @brief Returns the flag property currently used for the specified flag and all objects
     * @param ok: false if one object type don't use the same property, true if all objects type use the same property.
     * @warning if "ok" is false the property returned can be undetermined
     */
    virtual GraphicsObjectFlagProperty getFlagProperty(const GraphicsObjectFlag& flag,
                                                       bool& ok) const = 0;

    /**
     * @brief Return the camera of the view
     */
    virtual CameraInterface* camera() const = 0;

    /**
     * @brief Return the type of the view
     */
    virtual GraphicsViewType type() const = 0;

    /**
     * @brief Set the selection mode
     */
    virtual void setSelectionMode(SelectionMode mode) = 0;

    /**
     * @brief Get the selection mode
     */
    virtual GraphicsViewInterface::SelectionMode selectionMode() const = 0;

    /**
     * @brief Returns true if the view must select points
     */
    virtual bool mustSelectPoints() const = 0;

    /**
     * @brief Returns true if the view must select edges
     */
    virtual bool mustSelectEdges() const = 0;

    /**
     * @brief Returns true if the view must select faces
     */
    virtual bool mustSelectFaces() const = 0;

    /**
     * @brief Returns true if the view must select items
     */
    virtual bool mustSelectItems() const = 0;

    /**
     * @brief Set the selection width. Default is 3;
     * @deprecated This method will disappear !
     */
    Q_DECL_DEPRECATED virtual void setSelectRegionWidth(int width) = 0;

    /**
     * @brief Set the selection height. Default is 3;
     * @deprecated This method will disappear !
     */
    Q_DECL_DEPRECATED virtual void setSelectRegionHeight(int height) = 0;

    /**
     * @brief Performs a selection in the scene from pixel coordinates with the giver region size.
     * @param point: center of the rectangle
     * @deprecated This method will disappear !
     */
    Q_DECL_DEPRECATED virtual void select(const QPoint& point) = 0;

    /**
     * @brief Performs a selection in the scene with a polygon.
     */
    virtual void select(const QPolygon& polygon) = 0;

    /**
     * @brief Returns all points (indices) selected. Can be NULL if no points where selected.
     */
    virtual CT_SPCIR getSelectedPoints() const = 0;

    /**
     * @brief Returns all faces (indices) selected. Can be NULL if no faces where selected.
     */
    virtual CT_SFCIR getSelectedFaces() const = 0;

    /**
     * @brief Returns all edges (indices) selected. Can be NULL if no edges where selected.
     */
    virtual CT_SECIR getSelectedEdges() const = 0;

    /**
     * @brief Returns all ItemDrawable selected
     */
    virtual QList<CT_AbstractItemDrawable*> getSelectedItems() const = 0;

    /**
     * @brief Add a point to selection. Pass the global index of the point.
     */
    virtual void addPointsIDToSelection(const size_t &id) = 0;

    /**
     * @brief Add multiple point to selection. Pass a vector of global index of points to add.
     */
    virtual void addMultiplePointsIDToSelection(const std::vector<size_t> &idList) = 0;

    /**
     * @brief Add a face to selection. Pass the global index of the face.
     */
    virtual void addFacesIDToSelection(const size_t &id) = 0;

    /**
     * @brief Add multiple face to selection. Pass a vector of global index of faces to add.
     */
    virtual void addMultipleFacesIDToSelection(const std::vector<size_t> &idList) = 0;

    /**
     * @brief Add a edge to selection. Pass the global index of the point.
     */
    virtual void addEdgesIDToSelection(const size_t &id) = 0;

    /**
     * @brief Add multiple edge to selection. Pass a vector of global index of edges to add.
     */
    virtual void addMultipleEdgesIDToSelection(const std::vector<size_t> &idList) = 0;

    /**
     * @brief Remove a point to selection. Pass the global index of the point.
     */
    virtual void removePointsIDFromSelection(const size_t &id) = 0;

    /**
     * @brief Remove multiple point to selection. Pass a vector of global index of points to remove.
     */
    virtual void removeMultiplePointsIDFromSelection(const std::vector<size_t> &idList) = 0;

    /**
     * @brief Remove a face to selection. Pass the global index of the face.
     */
    virtual void removeFacesIDFromSelection(const size_t &id) = 0;

    /**
     * @brief Remove multiple face to selection. Pass a vector of global index of faces to remove.
     */
    virtual void removeMultipleFacesIDFromSelection(const std::vector<size_t> &idList) = 0;

    /**
     * @brief Remove a edge to selection. Pass the global index of the point.
     */
    virtual void removeEdgesIDFromSelection(const size_t &id) = 0;

    /**
     * @brief Remove multiple edge to selection. Pass a vector of global index of edges to remove.
     */
    virtual void removeMultipleEdgesIDFromSelection(const std::vector<size_t> &idList) = 0;

    /**
     * @brief Call this method with true if you want to select all points, false if you want to unselect all points
     */
    virtual void setAllPointsSelected(bool select) = 0;

    /**
     * @brief Call this method with true if you want to select all faces, false if you want to unselect all faces
     */
    virtual void setAllFacesSelected(bool select) = 0;

    /**
     * @brief Call this method with true if you want to select all edges, false if you want to unselect all edges
     */
    virtual void setAllEdgesSelected(bool select) = 0;

    /**
     * @brief Toggle the selection of items
     */
    virtual void toggleItemsSelected() = 0;

    /**
     * @brief Toggle the selection of points
     */
    virtual void togglePointsSelected() = 0;

    /**
     * @brief Toggle the selection of faces
     */
    virtual void toggleFacesSelected() = 0;

    /**
     * @brief Toggle the selection of edges
     */
    virtual void toggleEdgesSelected() = 0;

    /**
     * @brief Returns how many points is drawn
     */
    virtual size_t countPoints() = 0;

    /**
     * @brief Returns how many edges is drawn
     */
    virtual size_t countEdges() = 0;

    /**
     * @brief Returns how many faces is drawn
     */
    virtual size_t countFaces() = 0;

    /**
     * @brief Returns how many items is drawn
     */
    virtual size_t countItems() = 0;

    /**
     * @brief Call to visit all type of objects
     * @param visitor : is an object that will receive all objects
     */
    virtual void visitObjects(ObjectsVisitorInterface& visitor) = 0;

    /**
     * @brief Call to visit objects
     * @brief type : type of object to visit
     * @param visitor : is an object that will receive all objects
     *
     * @example scene->visitObjects(QList<GraphicsObjectType>() << GOT_Quads << GOT_Triangle, myVisitor);
     */
    virtual void visitObjects(const QList<GraphicsObjectType>& types,
                              ObjectsVisitorInterface& visitor) = 0;

    /**
     * @brief Create an interface that allow the developper to modify all objects
     *        of the specified type
     */
    virtual QSharedPointer<ObjectsModifierInterface> createObjectsModifier(GraphicsObjectType type) = 0;

    /**
     * @brief Create an interface that allow the developper to query the status of all objects
     *        of the specified type
     */
    virtual QSharedPointer<ObjectStatusQueryiorInterface> createObjectsStatusQueryior(GraphicsObjectType type) = 0;

    /**
     * @brief Call to visit all type of objects
     * @param visitor : is an object that will receive all objects
     */
    virtual void visitObjectsOfItem(const CT_AbstractItemDrawable* item,
                                    ObjectsVisitorInterface& visitor) = 0;

    /**
     * @brief Call to visit objects
     * @brief type : type of object to visit
     * @param visitor : is an object that will receive all objects
     *
     * @example scene->visitObjects(QList<GraphicsObjectType>() << GOT_Quads << GOT_Triangle, myVisitor);
     */
    virtual void visitObjectsOfItem(const CT_AbstractItemDrawable* item,
                                    const QList<GraphicsObjectType>& types,
                                    ObjectsVisitorInterface& visitor) = 0;

    /**
     * @brief Create an interface that allow the developper to modify all objects
     *        of the specified type for one item
     */
    virtual QSharedPointer<ObjectsModifierInterface> createObjectsModifierForItem(const CT_AbstractItemDrawable* item,
                                                                                  GraphicsObjectType type) = 0;

    /**
     * @brief Call to visit all type of objects of the current action
     * @param uniqueIndex : the unique index of the elements to visit or -1 to visit all objects of all elements.
     * @param visitor : is an object that will receive all objects
     */
    virtual void visitObjectsOfCurrentAction(const int& uniqueIndex,
                                             ObjectsVisitorInterface& visitor) = 0;

    /**
     * @brief Call to visit objects of the current action
     * @param uniqueIndex : the unique index of the elements to visit or -1 to visit all objects of all elements.
     * @brief type : type of object to visit
     * @param visitor : is an object that will receive all objects
     *
     * @example scene->visitObjects(QList<GraphicsObjectType>() << GOT_Quads << GOT_Triangle, myVisitor);
     */
    virtual void visitObjectsOfCurrentAction(const int& uniqueIndex,
                                             const QList<GraphicsObjectType>& types,
                                             ObjectsVisitorInterface& visitor) = 0;

    /**
     * @brief Returns the coordinates of the 3D point located at pixel (x,y) on screen.
     *
     * Calls a glReadPixel to get the pixel depth and applies an unprojectedCoordinatesOf() to
     * the result. found indicates whether a point was found or not (i.e. background pixel, result's
     * depth is zFar() in that case).
     *
     * x and y are expressed in pixel units with an origin in the upper left corner.
     */
    virtual Eigen::Vector3d pointUnderPixel(const QPoint &pixel, bool &found) const = 0;

    /**
     * @brief Gives the coefficients of a 3D half-line passing through the Camera eye and pixel (x,y).
     *
     * The origin of the half line (eye position) is stored in orig, while dir contains the properly oriented and normalized direction of the half line.
     *
     * x and y are expressed in Qt format (origin in the upper left corner).
     *
     * This method is useful for analytical intersection in a selection method.
     */
    virtual void convertClickToLine(const QPoint &pixel, Eigen::Vector3d &orig, Eigen::Vector3d &dir) const = 0;

    /**
     * @brief Returns the screen projected coordinates (pixel) of the variable position.
     */
    virtual void convert3DPositionToPixel(const Eigen::Vector3d &position, QPoint &pixel) const = 0;

    /**
     * @brief Add option of a action to the GraphicsView
     *
     *        If you want to remove the widget from the view you can delete it or
     *        call method "removeActionOptions"
     *
     * @warning You must be in the GUI thread to call this method !
     */
    virtual void addActionOptions(ActionOptionsInterface *options) = 0;

    /**
     * @brief Delete the widget. If you want to hide them call
     *        method "QWidget::hide()".
     */
    virtual void removeActionOptions(ActionOptionsInterface *options) = 0;

    /**
     * @brief Return the width of the graphics
     */
    virtual int width() const = 0;

    /**
     * @brief Return the height of the graphics
     */
    virtual int height() const = 0;
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

/*!
 *  \brief Représente un document.
 *
 *  Un document contient des ItemDrawable. Celui-ci peut être représenté par plusieurs vues (GraphicsViewInterface).
 */
class DocumentInterface : public QObject
{
    Q_OBJECT

public:
    virtual ~DocumentInterface() {}

    /**
      * \brief Title of the document
      */
    virtual QString getTitle() const = 0;

    /**
     * @brief The number of the document (unique)
     */
    virtual int getNumber() const = 0;

    /**
     * @brief Call this method if you plan to add multiple ItemDrawable in one times
     */
    virtual void beginAddMultipleItemDrawable() = 0;

    /**
      * \brief Ajout d'un ItemDrawable au document
      */
    virtual void addItemDrawable(CT_AbstractItemDrawable &item) = 0;

    /**
     * @brief Call this method after you add multiple ItemDrawable and if you have called the method 'beginAddMultipleItemDrawable'
     */
    virtual void endAddMultipleItemDrawable() = 0;

    /**
     * @brief Call this method if you plan to remove multiple ItemDrawable in one times
     */
    virtual void beginRemoveMultipleItemDrawable() = 0;

    /**
      * \brief Supprime l'ItemDrawable du document
      */
    virtual void removeItemDrawable(CT_AbstractItemDrawable &item) = 0;

    /**
     * @brief Call this method after you removed multiple ItemDrawable and if you have called the method 'beginRemoveMultipleItemDrawable'
     */
    virtual void endRemoveMultipleItemDrawable() = 0;

    /**
      * \brief Removes all ItemDrawable in the document that was in the result 'res'. Call the method 'beginRemoveMultipleItemDrawable' and 'endRemoveMultipleItemDrawable' automatically
      */
    virtual void removeAllItemDrawableOfResult(const CT_AbstractResult &res) = 0;

    /**
      * \brief Removes all ItemDrawable in the document that have the model 'model'. Call the method 'beginRemoveMultipleItemDrawable' and 'endRemoveMultipleItemDrawable' automatically
      */
    virtual void removeAllItemDrawableOfModel(const CT_OutAbstractModel &model) = 0;

    /**
      * \brief Removes all selected ItemDrawable in the document. Call the method 'beginRemoveMultipleItemDrawable' and 'endRemoveMultipleItemDrawable' automatically
      */
    virtual void removeAllSelectedItemDrawable() = 0;

    /**
      * \brief Removes all ItemDrawable in the document. Call the method 'beginRemoveMultipleItemDrawable' and 'endRemoveMultipleItemDrawable' automatically
      */
    virtual void removeAllItemDrawable() = 0;

    /**
      * \brief Select/DeSelect of ItemDrawable of the document
      */
    virtual void setSelectAllItemDrawable(bool select) = 0;

    /**
      * \brief Select/DeSelect of ItemDrawable of the document that have the model 'model'
      */
    virtual void setSelectAllItemDrawableOfModel(bool select, const CT_OutAbstractModel &model) = 0;

    /**
      * \brief Returns all ItemDrawable contained in this document
      */
    virtual const QList<CT_AbstractItemDrawable*>& getItemDrawable() const = 0;

    /**
      * \brief Returns all ItemDrawable selected
      */
    virtual QList<CT_AbstractItemDrawable*> getSelectedItemDrawable() const = 0;

    /**
     * @brief Returns true if the itemdrawable is contained in this document
     */
    virtual bool containsItemDrawable(const CT_AbstractItemDrawable *item) const = 0;

    /**
     * @brief Returns true if the itemdrawable or at least one children is contained in this document
     */
    virtual bool containsItemDrawableOrAtLeastOneChildren(const CT_AbstractItemDrawable *item) const = 0;

    /**
     * @brief Returns true if this document use item color.
     */
    virtual bool useItemColor() const = 0;

    /**
     * @brief Set the color of the item passed in parameter. Do nothing if useItemColor() return false.
     */
    virtual void setColor(const CT_AbstractItemDrawable *item, const QColor &color) = 0;

    /**
     * @brief Return the color of the item passed in parameter. Returns an invalid color if useItemColor() return false.
     */
    virtual QColor getColor(const CT_AbstractItemDrawable *item) = 0;

    /**
     * @brief Returns true if this document has the capacity of change the visibility of an itemdrawable
     */
    virtual bool canChangeVisibility() const = 0;

    /**
     * @brief Change the visibility of the item in the document
     */
    virtual void setVisible(const CT_AbstractItemDrawable *item, bool visible) = 0;

    /**
     * @brief Returns the visibility of the item in the document.
     */
    virtual bool isVisible(const CT_AbstractItemDrawable *item) const = 0;

public:
    /**
      * \brief Returns the number of ItemDrawable
      */
    virtual size_t nItemDrawable() const = 0;

    /**
      * \brief Return the ItemDrawable 'i' in the list
      */
    virtual CT_AbstractItemDrawable* getItemDrawable(int i) const = 0;

    /**
      * \brief Recherche des ItemDrawable dans la liste  partir du model pass en paramtre
      * \return une liste vide si aucun ItemDrawable n'est de ce type
      */
    virtual QList<CT_AbstractItemDrawable*> findItemDrawable(const CT_OutAbstractModel &model) const = 0;
    virtual void findItemDrawable(const CT_OutAbstractModel &model, QList<CT_AbstractItemDrawable*> &outList) const = 0;

    /**
      * \brief Recherche le premier ItemDrawable dans la liste a partir du modele
      * \return NULL si aucun ItemDrawable n'est de ce type
      */
    virtual CT_AbstractItemDrawable* findFirstItemDrawable(const CT_OutAbstractModel &model) const = 0;

    /**
     * @brief Return true if the document accept this type of action
     */
    virtual bool acceptAction(const CT_AbstractAction *action) const = 0;

    /**
     * @brief Set the current action to the document. If the document contains graphics, each of
     *        them receives a copy of the action. If the document contains treeview, each of them
     *        receives a copy of the action. etc...
     *
     *        If the action don't exist in the actions manager it will be added automatically.
     *
     * @param action : action to set current
     * @param deleteAction : true if the action must be deleted if she exist in the actions manager (true by default)
     *
     * @return false if the document don't accept this type of action.
     */
    virtual bool setCurrentAction(CT_AbstractAction *action, bool deleteAction = true) = 0;

    /**
     * @brief Return the list of views in the document
     */
    virtual QList<InDocumentViewInterface*> views() const = 0;

    /**
      * \brief Refresh all graphics in the document (if he contains graphicsView)
      */
    virtual void redrawGraphics() = 0;

    virtual void updateItems(const QList<CT_AbstractItemDrawable*> &items) = 0;
};

/**
 * @brief Représente le gestionnaire des documents
 */
class DocumentManagerInterface
{
public:
    virtual ~DocumentManagerInterface() {}

    /**
     * @brief Create a new document with 3D View
     */
    virtual DocumentInterface* new3DDocument(float pointSize = 1.0, bool orthographic = false, QColor *color = NULL) = 0;

    /**
     * @brief Create a new document with 2D View
     */
    virtual DocumentInterface* new2DDocument() = 0;

    /**
     * @brief Create a new document with TreeView
     */
    virtual DocumentInterface* newTreeViewDocument() = 0;

    /**
     * @brief Return the number of documents
     */
    virtual int nDocuments() const = 0;

    /**
     * @brief Return the document at index passed in parameter
     */
    virtual DocumentInterface* documentAt(const int &index) const = 0;

    /**
     * @brief Return the documents collection
     */
    virtual QList<DocumentInterface*> documents() const = 0;

    /**
     * @brief Return the active document
     */
    virtual DocumentInterface* activeDocument() = 0;

    /**
     * @brief Activate the document passed in parameter
     */
    virtual void activateDocument(DocumentInterface *doc) = 0;

    /**
     * @brief Close the document passed in parameter.
     *
     * @return false if it is the last document. One document must always be present.
     */
    virtual bool closeDocument(DocumentInterface *doc) = 0;

    /**
     * @brief Close the document at index passed in parameter.
     *
     * @return false if it is the last document. One document must always be present.
     */
    virtual bool closeDocument(const int &index) = 0;
};

/**
 * @brief Représente le gestionnaire de l'application (la vue)
 */
class GuiManagerInterface
{
public:
    virtual ~GuiManagerInterface() {}

    /**
     * @return le gestionnaire des documents. NULL si il n'y en a pas.
     */
    virtual DocumentManagerInterface* documentManager() const = 0;
};

/**
 * @brief Represent the core of the application
 */
class CoreInterface
{
public:
    virtual ~CoreInterface() {}

    /**
     * @brief get the log
     */
    virtual LogInterface* log() const = 0;

    /**
     * @brief get the plugin manager
     */
    virtual PluginManagerInterface* pluginManager() const = 0;

    /**
     * @brief get the menu to populate with steps
     */
    virtual CT_StepsMenu* stepsMenu() const = 0;
};

/**
 * @brief The log interface
 */
class LogInterface
{
public:

    enum DefaultSeverity
    {
        trace = 0,
        debug,
        info,
        warning,
        error,
        fatal
    };

    enum DefaultType
    {
        core = 0,
        gui,
        plugin,
        step,
        result,
        itemdrawable,
        action,
        reader,
        exporter,
        filter,
        metric,
        unknow
    };

    virtual ~LogInterface() {}

    /**
     * @brief add a message
     *
     * @param severity : the severity (trace, debug, info, etc...)
     * @param type : type of element that add a message to the log (action, step, etc...)
     * @param s : message
     * @param filter : a filter if you want to filter message for your plugin per example
     */
    virtual void addMessage(const int &severity, const int &type, const QString &s, const QString &filter = "") = 0;

    /**
     * @brief add a trace message (hte severity is automatically set to "trace")
     *
     * @param type : type of element that add a message to the log (action, step, etc...)
     * @param s : message
     * @param filter : a filter if you want to filter message for your plugin per example
     */
    virtual void addTraceMessage(const int &type, const QString &s, const QString &filter = "") = 0;

    /**
     * @brief add a debug message (hte severity is automatically set to "debug")
     *
     * @param type : type of element that add a message to the log (action, step, etc...)
     * @param s : message
     * @param filter : a filter if you want to filter message for your plugin per example
     */
    virtual void addDebugMessage(const int &type, const QString &s, const QString &filter = "") = 0;

    /**
     * @brief add a info message (hte severity is automatically set to "info")
     *
     * @param type : type of element that add a message to the log (action, step, etc...)
     * @param s : message
     * @param filter : a filter if you want to filter message for your plugin per example
     */
    virtual void addInfoMessage(const int &type, const QString &s, const QString &filter = "") = 0;

    /**
     * @brief add a warning message (hte severity is automatically set to "warning")
     *
     * @param type : type of element that add a message to the log (action, step, etc...)
     * @param s : message
     * @param filter : a filter if you want to filter message for your plugin per example
     */
    virtual void addWarningMessage(const int &type, const QString &s, const QString &filter = "") = 0;

    /**
     * @brief add a error message (hte severity is automatically set to "error")
     *
     * @param type : type of element that add a message to the log (action, step, etc...)
     * @param s : message
     * @param filter : a filter if you want to filter message for your plugin per example
     */
    virtual void addErrorMessage(const int &type, const QString &s, const QString &filter = "") = 0;

    /**
     * @brief add a fatal message (hte severity is automatically set to "fatal")
     *
     * @param type : type of element that add a message to the log (action, step, etc...)
     * @param s : message
     * @param filter : a filter if you want to filter message for your plugin per example
     */
    virtual void addFatalMessage(const int &type, const QString &s, const QString &filter = "") = 0;

    /**
     * @brief add a message
     *
     * @param severity : the severity (trace, debug, info, etc...)
     * @param type : when you call this method pass "this" for the type and the type is automatically detected
     * @param s : message
     * @param filter : a filter if you want to filter message for your plugin per example
     */
    virtual void addMessage(const int &severity, CT_VirtualAbstractStep *type, const QString &s, const QString &filter = "") = 0;
    virtual void addMessage(const int &severity, CT_AbstractAction *type, const QString &s, const QString &filter = "") = 0;
    virtual void addMessage(const int &severity, PluginEntryInterface *type, const QString &s, const QString &filter = "") = 0;
    virtual void addMessage(const int &severity, CT_AbstractStepPlugin *type, const QString &s, const QString &filter = "") = 0;
    virtual void addMessage(const int &severity, CT_AbstractResult *type, const QString &s, const QString &filter = "") = 0;
    virtual void addMessage(const int &severity, CT_AbstractItemDrawable *type, const QString &s, const QString &filter = "") = 0;

    virtual void addTraceMessage(CT_VirtualAbstractStep *type, const QString &s, const QString &filter = "") = 0;
    virtual void addTraceMessage(CT_AbstractAction *type, const QString &s, const QString &filter = "") = 0;
    virtual void addTraceMessage(PluginEntryInterface *type, const QString &s, const QString &filter = "") = 0;
    virtual void addTraceMessage(CT_AbstractStepPlugin *type, const QString &s, const QString &filter = "") = 0;
    virtual void addTraceMessage(CT_AbstractResult *type, const QString &s, const QString &filter = "") = 0;
    virtual void addTraceMessage(CT_AbstractItemDrawable *type, const QString &s, const QString &filter = "") = 0;

    virtual void addDebugMessage(CT_VirtualAbstractStep *type, const QString &s, const QString &filter = "") = 0;
    virtual void addDebugMessage(CT_AbstractAction *type, const QString &s, const QString &filter = "") = 0;
    virtual void addDebugMessage(PluginEntryInterface *type, const QString &s, const QString &filter = "") = 0;
    virtual void addDebugMessage(CT_AbstractStepPlugin *type, const QString &s, const QString &filter = "") = 0;
    virtual void addDebugMessage(CT_AbstractResult *type, const QString &s, const QString &filter = "") = 0;
    virtual void addDebugMessage(CT_AbstractItemDrawable *type, const QString &s, const QString &filter = "") = 0;

    virtual void addInfoMessage(CT_VirtualAbstractStep *type, const QString &s, const QString &filter = "") = 0;
    virtual void addInfoMessage(CT_AbstractAction *type, const QString &s, const QString &filter = "") = 0;
    virtual void addInfoMessage(PluginEntryInterface *type, const QString &s, const QString &filter = "") = 0;
    virtual void addInfoMessage(CT_AbstractStepPlugin *type, const QString &s, const QString &filter = "") = 0;
    virtual void addInfoMessage(CT_AbstractResult *type, const QString &s, const QString &filter = "") = 0;
    virtual void addInfoMessage(CT_AbstractItemDrawable *type, const QString &s, const QString &filter = "") = 0;

    virtual void addWarningMessage(CT_VirtualAbstractStep *type, const QString &s, const QString &filter = "") = 0;
    virtual void addWarningMessage(CT_AbstractAction *type, const QString &s, const QString &filter = "") = 0;
    virtual void addWarningMessage(PluginEntryInterface *type, const QString &s, const QString &filter = "") = 0;
    virtual void addWarningMessage(CT_AbstractStepPlugin *type, const QString &s, const QString &filter = "") = 0;
    virtual void addWarningMessage(CT_AbstractResult *type, const QString &s, const QString &filter = "") = 0;
    virtual void addWarningMessage(CT_AbstractItemDrawable *type, const QString &s, const QString &filter = "") = 0;

    virtual void addErrorMessage(CT_VirtualAbstractStep *type, const QString &s, const QString &filter = "") = 0;
    virtual void addErrorMessage(CT_AbstractAction *type, const QString &s, const QString &filter = "") = 0;
    virtual void addErrorMessage(PluginEntryInterface *type, const QString &s, const QString &filter = "") = 0;
    virtual void addErrorMessage(CT_AbstractStepPlugin *type, const QString &s, const QString &filter = "") = 0;
    virtual void addErrorMessage(CT_AbstractResult *type, const QString &s, const QString &filter = "") = 0;
    virtual void addErrorMessage(CT_AbstractItemDrawable *type, const QString &s, const QString &filter = "") = 0;

    virtual void addFatalMessage(CT_VirtualAbstractStep *type, const QString &s, const QString &filter = "") = 0;
    virtual void addFatalMessage(CT_AbstractAction *type, const QString &s, const QString &filter = "") = 0;
    virtual void addFatalMessage(PluginEntryInterface *type, const QString &s, const QString &filter = "") = 0;
    virtual void addFatalMessage(CT_AbstractStepPlugin *type, const QString &s, const QString &filter = "") = 0;
    virtual void addFatalMessage(CT_AbstractResult *type, const QString &s, const QString &filter = "") = 0;
    virtual void addFatalMessage(CT_AbstractItemDrawable *type, const QString &s, const QString &filter = "") = 0;

    /**
     * @brief Add log listener that must receive message in prioritary and immediately (Elements like File, std::cout, etc...)
     * @warning Add a log listener to this method can reduce the performance of your application if the message is not compute fast
     */
    virtual void addPrioritaryLogListener(const CT_AbstractLogListener *logl) = 0;

    /**
     * @brief Add a log listener that must receive message in normal mode (delayed) (Elements of ui like QTextEdit, etc...)
     */
    virtual void addNormalLogListener(const CT_AbstractLogListener *logl) = 0;

    /**
     * @brief Remove a log listener (prioritary or normal)
     */
    virtual void removeLogListener(CT_AbstractLogListener *logl) = 0;
};

/**
 * @brief The settings writer is used to save settings of elements in a script. It will use an xml file to write all
 *        parameters that you want to save.
 */
class SettingsWriterInterface
{
public:

    virtual ~SettingsWriterInterface() {}

    /**
     * @brief Set a global description for the caller (the caller is use to set the name of the group)
     *
     *        In the file the result will be like this :
     *
     *        <CALLER_CLASSNAME description="DESCRIPTION">
     *          ...
     *        </CALLER_CLASSNAME>
     *
     * @param caller : always pass "this". The manager use the object to get the classname and set the name of the group.
     * @param description : the description to write in the file for this group
     */
    virtual void setCallerDescription(const QObject* caller, const QString& description) = 0;

    /**
     * @brief Set a unique ID for this caller so you can ensure the compatibility between script when you add/remove a caller
     *
     *        In the file the result will be like this :
     *
     *        <CALLER_CLASSNAME id="UNIQUE_ID">
     *          ...
     *        </CALLER_CLASSNAME>
     * @param caller : always pass "this"
     * @param uniqueID : A positive and unique index if you want to ensure the compatibility between script when you add/remove a caller
     */
    virtual void setCallerUniqueID(const QObject* caller, const int& uniqueID) = 0;

    /**
     * @brief Save a parameter.
     *
     *        In the xml file the result will be like this if it was only one parameter :
     *
     *        <CALLER_CLASSNAME description="DESCRIPTION" id="UNIQUE_ID" PARAMETER_NAME="PARAMETER_VALUE" pDescription="DESC_VALUE" pID="ID_VALUE" />
     *
     *        In the xml file the result will be like this if it was multiple parameters :
     *
     *        <CALLER_CLASSNAME>
     *          <PARAMETER_NAME value="PARAMETER_VALUE" pDescription="DESC_VALUE" pID="ID_VALUE" />
     *          <PARAMETER_NAME value="PARAMETER_VALUE" pDescription="DESC_VALUE" pID="ID_VALUE" />
     *          ...
     *        </CALLER_CLASSNAME>
     *
     *        You can call this method multiple times with the same parameter name. In this situation when the script is readed you must call multiple times
     *        the method "parameter" (until the method return "false").
     *
     * @param caller : always pass "this". The manager use the object to get the classname and set the name of the group.
     * @param parameterName : name of the parameter that you want to save
     * @param parameterValue : value of the parameter to save
     * @param description : a short description to inform the user (that modify a script manually) what this parameter is able to do
     * @param uniqueID : (optionnal)-1 if you don't want to use it. A positive and unique index if you want to ensure the compatibility between script when you add/remove a parameter
     * @return a generated unique and positive ID that represent this parameter and can be used in method "addParameterExtraInfo" per example. 0 or a negative value if it was an error.
     */
    virtual int addParameter(const QObject* caller, const QString& parameterName, const QVariant& parameterValue, const QString& description = "", const int& uniqueID = -1) = 0;

    /**
     * @brief Save a PATH. Use it when you want to save a filepath or a directory path. Same result as the method "addParameter" but
     *        will convert the path to a relative path and add an absolute directory path to another section in the file. So it will simplier for a user to modify path.
     *
     *        In the xml file the result will always be like this if it was one or multiple parameters :
     *
     *        <CALLER_CLASSNAME>
     *          <PARAMETER_NAME path="PARAMETER_VALUE" pDescription="DESC_VALUE" pID="ID_VALUE" />
     *          ...
     *        </CALLER_CLASSNAME>
     *
     *        You can call this method multiple times with the same parameter name. In this situation when the script is readed you must call multiple times
     *        the method "parameterPath" (until the method return "false").
     *
     * @param caller : always pass "this". The manager use the object to get the classname and set the name of the group.
     * @param parameterName : name of the parameter that you want to save
     * @param fileOrDirectoryPath : absolute file or directory path to save
     * @param description : a short description to inform the user (that modify a script manually) what this path will say
     * @param uniqueID : (optionnal)-1 if you don't want to use it. A positive and unique index if you want to ensure the compatibility between script when you add/remove a parameter
     * @return a generated unique and positive ID that represent this parameter and can be used in method "addParameterInfo" per example.
     */
    virtual int addParameterPath(const QObject* caller, const QString& parameterName, const QString& fileOrDirectoryPath, const QString& description = "", const int& uniqueID = -1) = 0;

    /**
     * @brief Save extra informations for a parameter. Only use it to add some extra informations that can help the user that modify
     *        the script manually to have more informations about this parameter.
     *
     *        In the xml file the result is a couple of a key (EXTRA_INFO_NAME) and a value (EXTRA_INFO_VALUE) like this :
     *
     *        <CALLER_CLASSNAME PARAMETER_NAME="PARAMETER_VALUE" DESCRIPTION="DESC_VALUE" UNIQUE_ID="ID_VALUE" EXTRA_INFO_NAME="EXTRA_INFO_VALUE" />
     *
     * @param caller : always pass "this". The manager use the object to get the classname and set the name of the group.
     * @param parameterUniqueID : the unique ID returned by the method "addParameter"
     * @param extraInfoName : a unique name
     * @param extraInfoValue : a value (always a text)
     * @return false if the "extraInfoName" already exist.
     */
    virtual bool addParameterInfo(const QObject* caller, const int& parameterUniqueID, const QString& extraInfoName, const QVariant& extraInfoValue) = 0;

    /**
     * @brief Call it when you want to write settings of childrens of your class
     */
    virtual void beginWriteChildrens(const QObject* caller) = 0;

    /**
     * @brief Call it when you have finished to write settings of childrens in your class
     */
    virtual void endWriteChildrens(const QObject* caller) = 0;

    /**
     * @brief Create a group that will only be an informative part for the user that modify a script manually. Per example if you have a parameter
     *        that can be set with a value among a list of values you can add the list of values in a group.
     *
     *        In the xml file the result will always be like this :
     *
     *        <CALLER_CLASSNAME description="GROUP_DESCRIPTION" vgID="X" >
     *          <VALUES_NAME ... />
     *          ...
     *        </CALLER_CLASSNAME>
     *
     * @param caller : always pass "this". The manager use the object to get the classname and set the name of the group.
     * @param valuesName : the name of all values in the group
     * @param groupDescription : a description to inform the user what this group contains
     * @return a unique ID that you must use in the method "addExtraParameter" and "addParameter" to link with this group
     */
    virtual int createExtraParametersGroup(const QObject* caller, const QString& valuesName, const QString& groupDescription = "") = 0;

    /**
     * @brief Link a parameter and a group of extra parameter
     * @param caller : always pass "this".
     * @param parameterUniqueID : The unique ID returned by the method "addParameter".
     * @param parameterValuesGroupID : The unique ID returned by the method "createParameterValuesGroup".
     * @return false if group was not found or if it was already linked. True otherwise.
     */
    virtual bool linkParameterAndExtraParametersGroup(const QObject* caller, const int& parameterUniqueID, const int& parameterValuesGroupID) = 0;

    /**
     * @brief Add a value in "ExtraParameters" section. This section is only to inform user about the value that
     *        can be set to a parameter among a list of possible values.
     *
     *        In the xml file the result will always be like this :
     *
     *        <CALLER_CLASSNAME description="GROUP_DESCRIPTION" vgID="X" >
     *          <VALUES_NAME value="PARAMETER_VALUE" />
     *          <VALUES_NAME value="PARAMETER_VALUE" />
     *          ...
     *        </CALLER_CLASSNAME>
     *
     * @param caller : always pass "this".
     * @param value : value to save
     * @param parameterValuesGroupID : The unique ID returned by the method "createParameterValuesGroup".
     * @param description : a description of this value.
     * @return false if the group ID doesn't exist.
     */
    virtual bool addExtraParametersValueToGroup(const QObject* caller, const QVariant& value, const int& parameterValuesGroupID, const QString& description = "") = 0;
};

/**
 * @brief The settings reader is used to restore settings of elements from a script. It will use an xml file to read all
 *        parameters that you want to restore.
 */
class SettingsReaderInterface
{
public:

    virtual ~SettingsReaderInterface() {}

    /**
     * @brief Call it if you want to ensure the compatibility between script when you add/remove a parameter
     * @param caller : always pass "this"
     * @param uniqueID : pass the unique ID that you have used with the writer.
     * @return false if the group of the caller was found with this uniqueID, false otherwise.
     */
    virtual bool findCallerWithUniqueIDAndSetToCurrent(const QObject* caller, const int& uniqueID) = 0;

    /**
     * @brief Restore a parameter. If you have multiple parameter with the same name and same unique id you
     *        can call this method multiple times until it will return "false".
     *
     * @param caller : always pass "this". The manager use the object to get the classname to use for the name of the group.
     * @param parameterName : name of the parameter that you want to restore.
     * @param parameterValueRestored : a variable that will receive the readed value if founded or the "defaultParameterValue" otherwise
     * @param defaultParameterValue : a default value to use if the parameter was not found
     * @param uniqueID : (optionnal)-1 if you don't want to use it. A positive and unique index if you want to ensure the compatibility between script when you add/remove a parameter.
     * @return 0 or a negative value if the parameter was not found or it was an error, positive unique ID otherwise.
     */
    virtual int parameter(const QObject* caller, const QString& parameterName, QVariant& parameterValueRestored, const QVariant& defaultParameterValue = QVariant(), const int& uniqueID = -1) = 0;

    /**
     * @brief Count the number of parameter with the same "parameterName" and "uniqueID".
     * @param caller : always pass "this"
     * @param parameterName : name of the parameter that you want to count.
     * @param uniqueID : (optionnal)-1 if you don't want to use it. A positive and unique index if you want to ensure the compatibility between script when you add/remove a parameter.
     * @return the number of parameters founded
     */
    virtual int parameterCount(const QObject *caller, const QString &parameterName, const int& uniqueID = -1) = 0;

    /**
     * @brief Restore a PATH. Use when you want to restore a filepath or a directory path. Same result as the method "parameter" but
     *        will convert the relative path to an absolute path. If you have multiple parameter with the same name and same unique id you
     *        can call this method multiple times until it will return "false".
     *
     * @param caller : always pass "this". The manager use the object to get the classname and set the name of the group.
     * @param parameterName : name of the parameter that you want to restore.
     * @param fileOrDirectoryPathRestored : a variable that will receive the readed path if founded or the "defaultFileOrDirectoryPath" otherwise
     * @param defaultFileOrDirectoryPath : a default path to use if the parameter was not found
     * @param uniqueID : (optionnal)-1 if you don't want to use it. A positive and unique index if you want to ensure the compatibility between script when you add/remove a parameter
     * @return 0 or a negative value if the parameter was not found or it was an error, positive unique ID otherwise.
     */
    virtual int parameterPath(const QObject* caller, const QString& parameterName, QString& fileOrDirectoryPathRestored, const QString& defaultFileOrDirectoryPath = "", const int& uniqueID = -1) = 0;

    /**
     * @brief Count the number of path with the same "parameterName" and "uniqueID". "parameterName" if you want to count all parameters.
     * @param caller : always pass "this"
     * @param parameterName : name of the parameter that you want to count.
     * @param uniqueID : (optionnal)-1 if you don't want to use it. A positive and unique index if you want to ensure the compatibility between script when you add/remove a parameter.
     * @return the number of parameters founded
     */
    virtual int parameterPathCount(const QObject *caller, const QString &parameterName, const int& uniqueID = -1) = 0;

    /**
     * @brief Restore a extra info.
     *
     * @param caller : always pass "this". The manager use the object to get the classname to use for the name of the group.
     * @param parameterUniqueID : The unique ID returned by the method "parameter".
     * @param extraInfoName : name of the extra info that you want to restore
     * @param extraInfoValueRestored : a variable that will receive the readed value if founded or the "defaultValue" otherwise
     * @param defaultValue : a default value to use if the extra info was not found
     * @return false if "extraInfoName" was not found.
     */
    virtual bool parameterInfo(const QObject* caller, const int& parameterUniqueID, const QString& extraInfoName, QVariant& extraInfoValueRestored, const QVariant& defaultValue = QVariant()) = 0;

    /**
     * @brief Call it when you want to read settings of childrens of your class
     */
    virtual void beginReadChildrens(const QObject* caller) = 0;

    /**
     * @brief Call it when you have finished to read settings of childrens in your class
     */
    virtual void endReadChildrens(const QObject* caller) = 0;
};

/**
 * @brief Represent a gui context
 */
class GuiContextInterface
{
public:
    virtual ~GuiContextInterface() {}

    /**
     * @brief Return the document manager
     */
    virtual DocumentManagerInterface* documentManager() const = 0;

    /**
     * @brief Return the action manager
     */
    virtual ActionsManagerInterface* actionsManager() const = 0;
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

/**
 * @brief Represent a format of file for read or export
 */
class FileFormat
{
public:
    FileFormat(const QList<QString> &suffixes, QString description) { _suffixes = suffixes; _description = description; }
    FileFormat(const QString &suffix, QString description) { _suffixes << suffix; _description = description; }

    /**
     * @brief suffixes of file
     */
    const QList<QString>& suffixes() const { return _suffixes; }

    /**
     * @brief description
     */
    QString description() const { return _description; }

private:
    QList<QString> _suffixes;
    QString _description;
};

/**
 * @brief Represent the manager of plugins
 */
class PluginManagerInterface
{
public:
    virtual ~PluginManagerInterface() {}

    /*!
     *  \brief return true if at least one plugin is loaded
     */
    virtual bool isAPluginLoaded() const = 0;

    /*!
     *  \brief return the number of plugins loaded
     */
    virtual int countPluginLoaded() const = 0;

    /*!
     *  \brief return the name of the plugin 'i'
     */
    virtual QString getPluginName(int i = 0) const = 0;

    /*!
     *  \brief return the search path for plugins
     */
    virtual QString getSearchPath() const = 0;

    /*!
     *  \brief return the plugin 'i'
     */
    virtual CT_AbstractStepPlugin* getPlugin(int i) const = 0;

    /*!
     *  \brief return the plugin with name 'pluginName'
     */
    virtual CT_AbstractStepPlugin* getPlugin(QString pluginName) const = 0;
};

/*!
 *  \brief Représente le point d'entrée du Plugin.
 */
class PluginEntryInterface : public QObject
{
    Q_OBJECT

public:

    virtual ~PluginEntryInterface() {}

    /*!
     *  \brief Retourne le numro de version du plugin.
     */
    virtual QString getVersion() const = 0;

    /*!
     *  \brief Retourne le gestionnaire de plugin d'étapes
     */
    virtual CT_AbstractStepPlugin* getPlugin() const = 0;
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
