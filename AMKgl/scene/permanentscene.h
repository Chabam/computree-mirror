#ifndef PERMANENTSCENE_H
#define PERMANENTSCENE_H

#include "permanentsceneelementtype.h"
#include "renderer/drawmode.h"

#if defined(_WIN32) && defined(_MSC_VER) // Microsoft Visual Studio Compiler
#elif (defined(__linux__) || defined(_WIN32)) && defined(__GNUC__) // GNU Compiler (gcc,g++) for Linux, Unix, and MinGW (Windows)
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#if __GNUC__ > 7
#pragma GCC diagnostic ignored "-Wdeprecated-copy"
#else
#pragma GCC diagnostic ignored "-Wattributes" // See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=89325
#endif
#pragma GCC diagnostic ignored "-Wint-in-bool-context"
#elif defined(__APPLE__) // Clang Compiler (Apple)
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wint-in-bool-context"
#endif
#include "Eigen/Dense"
#if defined(_WIN32) && defined(_MSC_VER)
#elif (defined(__linux__) || defined(_WIN32)) && defined(__GNUC__)
#pragma GCC diagnostic warning "-Wall"
#pragma GCC diagnostic warning "-Wextra"
#if __GNUC__ > 7
#pragma GCC diagnostic warning "-Wdeprecated-copy"
#else
#pragma GCC diagnostic warning "-Wattributes" // See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=89325
#endif
#pragma GCC diagnostic warning "-Wint-in-bool-context"
#elif defined(__APPLE__)
#pragma GCC diagnostic warning "-Wall"
#pragma GCC diagnostic warning "-Wextra"
#pragma GCC diagnostic warning "-Wint-in-bool-context"
#endif

#include "renderer/pointcloud/pointcloudrenderer.h"
#include "renderer/ellipse/ellipsepermanentrenderer.h"
#include "renderer/box/boxpermanentrenderer.h"
#include "renderer/quads/quadspermanentrenderer.h"
#include "renderer/line/linepermanentrenderer.h"
#include "renderer/triangle/trianglepermanentrenderer.h"
#include "renderer/cylinder/cylinderpermanentrenderer.h"
#include "renderer/point/pointpermanentrenderer.h"
#include "renderer/pyramid/pyramidpermanentrenderer.h"
#include "renderer/sphere/spherepermanentrenderer.h"
#include "renderer/mesh/meshfacepermanentrenderer.h"
#include "renderer/mesh/meshedgepermanentrenderer.h"

class IChunk;
class IGraphicsDocument;
class DrawInfo;
class QOpenGLContext;
class IGlobalPointsVisitor;
class IObjectsVisitor;
class ObjectsFlagsPropertyManager;

/**
 * @brief A permanent scene was optimized to draw static elements (points or objects like ellipse, box, etc...)
 */
class PermanentScene : public QObject
{
    Q_OBJECT

public:
    PermanentScene();
    ~PermanentScene();

    /**
     * @brief Set the attributes accessor to use
     */
    void setAttributesAccessor(const IAttributesAccessor* accessor);

    /**
     * @brief Returns the element attributes provider used
     */
    AnyElementAttributesProvider* getAnyElementsAttributesProvider() const;

    /**
     * @brief Call it after you have set the attribute accessor and all necessary elements
     */
    void init();

    /**
     * @brief Call this method when you add this scene to
     *        your graphics view and pass it your opengl context
     */
    void addedTo(const IGraphicsDocument *doc, const QOpenGLContext* newContext);

    /**
     * @brief Call this method when you remove this scene from
     *        your graphics view and pass it your opengl context
     */
    void removedFrom(const IGraphicsDocument* doc, const QOpenGLContext* newContext);

    /**
     * @brief Returns the renderer (if exist) for the type specified
     */
    AbstractPermanentRenderer* getRendererForType(Scene::ObjectType type) const;

    /**
     * @brief Returns the point cloud attributes provider
     */
    PointCloudAttributesProvider* getPointCloudAttributesProvider() const;

    /**
     * @brief Returns the edge cloud attributes provider
     */
    EdgeCloudAttributesProvider* getEdgeCloudAttributesProvider() const;

    /**
     * @brief Returns the face cloud attributes provider
     */
    FaceCloudAttributesProvider* getFaceCloudAttributesProvider() const;

    /**
     * @brief Returns the X cloud attributes provider
     */
    ICloudAttributesProvider* getCloudAttributesProvider(Scene::ObjectType type) const;

    /**
     * @brief Returns the objects flags property manager
     */
    ObjectsFlagsPropertyManager* getObjectsFlagsPropertyManager() const;

    /**
     * @brief Returns true if at least one renderer is not up to date !
     */
    bool mustBeUpdated() const;

public slots:

    /**
     * @brief Call this method to update all elements in all context. It will create all
     *        necessary VBO, IBO, etc... to draw it
     */
    void updateInAllContext();

    /**
     * @brief Call this method to update infos of points in all context. It will update
     *        necessary VBO, etc... to draw it
     */
    void updatePointsInfoInAllContext();

    /**
     * @brief Call this method to update colors of points in all context. It will update
     *        necessary VBO, etc... to draw it
     */
    void updatePointsColorInAllContext();

    /**
     * @brief Call this method to update normals of points in all context. It will update
     *        necessary VBO, etc... to draw it
     */
    void updatePointsNormalInAllContext();

    /**
     * @brief Call this method to update infos of objects in all context. It will update
     *        necessary VBO, etc... to draw it
     */
    void updateObjectsInfoInAllContext();

    /**
     * @brief Call this method to update colors of objects in all context. It will update
     *        necessary VBO, etc... to draw it
     */
    void updateObjectsColorInAllContext();

    /**
     * @brief Call this method to update normals of objects in all context. It will update
     *        necessary VBO, etc... to draw it
     */
    void updateObjectsNormalInAllContext();

    /**
     * @brief Set visible or not (if not the "XXXdraw" method don't draw anything !
     */
    void setVisible(bool visible);

public:

    /**
     * @brief Define the draw mode to use for all type. If you set it to VBO or VAO and the graphics card
     *        cannot do it, it will be changed to RAW after update.
     */
    void setDrawModeToUse(DrawMode mode);

    /**
     * @brief Define the draw mode to use by type. If you set it to VBO or VAO and the graphics card
     *        cannot do it, it will be changed to RAW after update.
     */
    void setDrawModeToUse(Scene::ObjectType type, DrawMode mode);

    /**
     * @brief Set to true if this scene must reduce number of points when method "fastDraw(...)" is called, false otherwise
     */
    void setMustReduceNumberOfPointsInFastDraw(bool enabled);

    /**
     * @brief Set to true if this scene must reduce number of points when method "draw(...)" is called, false otherwise
     */
    void setMustReduceNumberOfPointsInNormalDraw(bool enabled);

    /**
     * @brief Returns true if this scene must reduce number of points when method "fastDraw(...)" is called, false otherwise
     */
    bool mustReduceNumberOfPointsInFastDraw() const;

    /**
     * @brief Returns true if this scene must reduce number of points when method "draw(...)" is called, false otherwise
     */
    bool mustReduceNumberOfPointsInNormalDraw() const;

    /**
     * @brief Call it when you want to destroy all opengl elements (when you close your window)
     */
    void destroyGL(const QOpenGLContext *context);

    /**
     * @brief Returns true if this scene is visible
     */
    bool isVisible() const;

    /**********
     * VISITING
     **********/

    /**
     * @brief Call to visit global points
     * @param visitor : is an object that will receive all points
     */
    void visitPoints(IGlobalPointsVisitor &visitor);

    /**
     * @brief Call to visit all objects
     * @param visitor : is an object that will receive all objects
     */
    void visitObjects(IObjectsVisitor& visitor);

    /**
     * @brief Call to visit objects of specified type
     * @param types : list of types to visit
     * @param visitor : is an object that will receive all objects
     */
    void visitObjectsOfTypes(const QList<Scene::ObjectType>& types, IObjectsVisitor& visitor);

    /**********
     * GENERIC
     **********/

    /**
     * @brief Remove all object of one type
     * @param el : type of object to remove
     */
    void clearObjectsOfType(Scene::ObjectType el);

    /**
     * @brief Remove all object
     */
    void clearAllObjects();

    /**
     * @brief Returns the total number of object that was added to this renderer
     * @param el : type of object to count
     */
    size_t countObject(Scene::ObjectType el) const;

    /**
     * @brief Set the current color used when add objects
     */
    void setCurrentColor(const QColor& color);

    /**
     * @brief Return the current color used when objects is added
     */
    QColor getCurrentColor() const;

    /**********
     * GLOBAL POINTS
     **********/

    /**
     * @brief Add points
     * @param indexes : collection of index of points to add
     * @param dispatchInfos : a pointer to an object that will contains informations on where is dispatched points (nullptr if you don't want this information)
     */
    void addPoints(const CloudIndex *indexes, PermanentItemInformation* dispatchInfos = nullptr);

    /**
     * @brief Add one point
     * @param globalIndex : global index of the point
     * @param dispatchInfos : a pointer to an object that will contains informations on where is dispatched points (nullptr if you don't want this information)
     */
    void addPoint(const size_t& globalIndex, PermanentItemInformation* dispatchInfos = nullptr);

    /**
     * @brief Returns the total number of points that was added to this renderer
     */
    size_t countPoints() const;

    /**
     * @brief Set the maximum number of points to draw when you call the method "fastDraw"
     * @param maxNumber : the maximum number of points
     */
    void setMaxNumberOfPointsToDrawInFastDraw(const size_t &maxNumber);

    /**
     * @brief Returns the maximum number of points to draw when you call the method "fastDraw"
     */
    size_t getMaxNumberOfPointsToDrawInFastDraw() const;

    /**********
     * POINTS
     **********/

    /**
     * @brief Add a point that was not in the global cloud
     * @param p : point
     * @param chunk : pointer of a pointer of a chunk that will contains the chunk used to store this object
     */
    void addLocalPoint(const Eigen::Vector3d& p,
                       IChunk** chunk = nullptr,
                       const Basic::LocalColor &color = Basic::LocalColor(255,255,255,255));

    /**********
     * ELLIPSE
     **********/

    /**
     * @brief Add an ellipse
     * @param cx : X center position of the ellipse
     * @param cy : Y center position of the ellipse
     * @param cz : Z center position of the ellipse
     * @param majorAxisRadius : radius of the major axis
     * @param minorAxisRadius : radius of the minor axis
     * @param majorAxisDirection : a vector that describe the direction of the major axis
     * @param normal : the normal of the ellipse
     * @param chunk : pointer of a pointer of a chunk that will contains the chunk used to store this object
     */
    void addEllipse(const double& cx, const double& cy, const double &cz,
                    const float& majorAxisRadius, const float& minorAxisRadius,
                    const Eigen::Vector3f& majorAxisDirection = Eigen::Vector3f(1, 0, 0),
                    const Eigen::Vector3f& normal = Eigen::Vector3f(0, 1, 0),
                    IChunk** chunk = nullptr);

    /**********
     * BOX
     **********/

    /**
     * @brief Add a box
     * @param cx : X center position of the box
     * @param cy : Y center position of the box
     * @param cz : Z center position of the box
     * @param width : the width of the box (X)
     * @param height : the width of the box (Y)
     * @param length : the width of the box (Z)
     * @param widthAxisDirection : a vector that describe the direction of the width
     * @param heightAxisDirection : a vector that describe the direction of the height
     * @param chunk : pointer of a pointer of a chunk that will contains the chunk used to store this object
     */
    void addBox(const double& cx, const double& cy, const double &cz,
                const float& width, const float& height, const float& length,
                const Eigen::Vector3f& widthAxisDirection = Eigen::Vector3f(1, 0, 0),
                const Eigen::Vector3f& heightAxisDirection = Eigen::Vector3f(0, 1, 0),
                IChunk** chunk = nullptr);

    /**********
     * QUADS
     **********/

    /**
     * @brief Add a quads
     * @param cx : X center position of the quads
     * @param cy : Y center position of the quads
     * @param cz : Z center position of the quads
     * @param width : the width of the quads (X)
     * @param length : the width of the quads (Z)
     * @param widthAxisDirection : a vector that describe the direction of the width
     * @param lengthAxisDirection : a vector that describe the direction of the length
     * @param chunk : pointer of a pointer of a chunk that will contains the chunk used to store this object
     */
    void addQuads(const double& cx, const double& cy, const double &cz,
                  const float& width, const float& length,
                  const Eigen::Vector3f& widthAxisDirection = Eigen::Vector3f(1, 0, 0),
                  const Eigen::Vector3f& lengthAxisDirection = Eigen::Vector3f(0, 0, 1),
                  IChunk** chunk = nullptr);

    /**********
     * LINES
     **********/

    /**
     * @brief Add a line
     * @param p1 : first point
     * @param p2 : second point
     * @param chunk : pointer of a pointer of a chunk that will contains the chunk used to store this object
     */
    void addLine(const Eigen::Vector3d& p1,
                 const Eigen::Vector3d& p2,
                 IChunk** chunk = nullptr,
                 const Basic::LocalColor& c1 = Basic::LocalColor(255,255,255,255),
                 const Basic::LocalColor& c2 = Basic::LocalColor(255,255,255,255));

    /**********
     * TRIANGLES
     **********/

    /**
     * @brief Add a triangle
     * @param p1 : first point
     * @param p2 : second point
     * @param p3 : third point
     * @param chunk : pointer of a pointer of a chunk that will contains the chunk used to store this object
     */
    void addTriangle(const Eigen::Vector3d& p1,
                     const Eigen::Vector3d& p2,
                     const Eigen::Vector3d& p3,
                     IChunk** chunk = nullptr,
                     const Basic::LocalColor &c1 = Basic::LocalColor(255,255,255,255),
                     const Basic::LocalColor &c2 = Basic::LocalColor(255,255,255,255),
                     const Basic::LocalColor &c3 = Basic::LocalColor(255,255,255,255));

    /**********
     * CYLINDER
     **********/

    /**
     * @brief Add a cylinder
     * @param cx : X center position of the cylinder
     * @param cy : Y center position of the cylinder
     * @param cz : Z center position of the cylinder
     * @param width : the width of the cylinder (X)
     * @param height : the height of the cylinder (Y)
     * @param length : the length of the cylinder (Z)
     * @param widthAxisDirection : a vector that describe the direction of the width
     * @param heightAxisDirection : a vector that describe the direction of the height
     * @param chunk : pointer of a pointer of a chunk that will contains the chunk used to store this object
     */
    void addCylinder(const double& cx, const double& cy, const double &cz,
                     const float& width, const float& height, const float& length,
                     const Eigen::Vector3f& widthAxisDirection = Eigen::Vector3f(1, 0, 0),
                     const Eigen::Vector3f& heightAxisDirection = Eigen::Vector3f(0, 1, 0),
                     IChunk** chunk = nullptr);

    /**********
     * Pyramid
     **********/

    /**
     * @brief Add a pyramid
     * @param cx : X center position of the pyramid
     * @param cy : Y center position of the pyramid
     * @param cz : Z center position of the pyramid
     * @param width : the width of the pyramid (X)
     * @param height : the height of the pyramid (Y)
     * @param length : the length of the pyramid (Z)
     * @param widthAxisDirection : a vector that describe the direction of the width
     * @param heightAxisDirection : a vector that describe the direction of the height
     * @param chunk : pointer of a pointer of a chunk that will contains the chunk used to store this object
     */
    void addPyramid(const double& cx, const double& cy, const double &cz,
                     const float& width, const float& height, const float& length,
                     const Eigen::Vector3f& widthAxisDirection = Eigen::Vector3f(1, 0, 0),
                     const Eigen::Vector3f& heightAxisDirection = Eigen::Vector3f(0, 1, 0),
                     IChunk** chunk = nullptr);

    /**********
     * Sphere
     **********/

    /**
     * @brief Add a sphere
     * @param cx : X center position of the box
     * @param cy : Y center position of the box
     * @param cz : Z center position of the box
     * @param radius : the radius of the sphere
     * @param initTheta : the start of theta angle (in radians or degrees => depends on "phiThetaInRadians" value)
     * @param endTheta : the end of theta angle (in radians or degrees => depends on "phiThetaInRadians" value)
     * @param initPhi : the start of phi angle (in radians or degrees => depends on "phiThetaInRadians" value)
     * @param endPhi : the end of phi angle (in radians or degrees => depends on "phiThetaInRadians" value)
     * @param phiThetaInRadians : true if theta and phi is in radians, false if it in degrees
     * @param chunk : pointer of a pointer of a chunk that will contains the chunk used to store this object
     */
    void addSphere(const double& cx, const double& cy, const double &cz,
                   const double& radius,
                   const double &initTheta, const double &endTheta,
                   const double &initPhi, const double &endPhi,
                   bool phiThetaInRadians = true,
                   IChunk** chunk = nullptr);

    /**********
     * Mesh
     **********/

    /**
     * @brief Add a face of a mesh
     * @param fi : face index in the global cloud
     * @param i1 : first point index in the global cloud
     * @param i2 : second point index in the global cloud
     * @param i3 : third point index in the global cloud
     * @param chunk : pointer of a pointer of a chunk that will contains the chunk used to store this object
     */
    void addMeshFace(const size_t& fi,
                     const size_t& i1,
                     const size_t& i2,
                     const size_t& i3,
                     IChunk** chunk = nullptr);

    /**
     * @brief Add a edge of a mesh
     * @param fi : edge index in the global cloud
     * @param i1 : first point index in the global cloud
     * @param i2 : second point index in the global cloud
     * @param chunk : pointer of a pointer of a chunk that will contains the chunk used to store this object
     */
    void addMeshEdge(const size_t& fi,
                     const size_t& i1,
                     const size_t& i2,
                     IChunk** chunk = nullptr);

    /**********
     * DRAW
     **********/

    /**
     * @brief Prepare necessary element before draw
     */
    void preDraw(DrawInfo &info);

    /**
     * @brief Draw all normally
     */
    void draw(DrawInfo &info);

    /**
     * @brief Draw all faster
     */
    void fastDraw(DrawInfo &info);

    /**
     * @brief Draw text or other elements
     */
    void postDraw(DrawInfo &info);

private:
    /**
     * @brief Set if this scene if visible or not
     */
    bool                            m_visible;

    /**
     * @brief Set if this scene must reduce number of points when method fastDraw() is called
     */
    bool                            m_mustReduceNumberOfPointsInFastDraw;

    /**
     * @brief Set if this scene must reduce number of points when method draw() is called
     */
    bool                            m_mustReduceNumberOfPointsInNormalDraw;

    /**
     * @brief A renderer for global points
     */
    PointCloudRenderer              m_globalPointCloudRenderer;
    MeshEdgePermanentRenderer       m_meshEdgeRenderer;
    MeshFacePermanentRenderer       m_meshFaceRenderer;

    /**
     * @brief A renderer for local points
     */
    PointPermanentRenderer          m_localPointCloudRenderer;
    EllipsePermanentRenderer        m_ellipseRenderer;
    BoxPermanentRenderer            m_boxRenderer;
    QuadsPermanentRenderer          m_quadsRenderer;
    LinePermanentRenderer           m_linesRenderer;
    TrianglePermanentRenderer       m_trianglesRenderer;
    CylinderPermanentRenderer       m_cylindersRenderer;
    PyramidPermanentRenderer        m_pyramidRenderer;
    SpherePermanentRenderer         m_sphereRenderer;

    typedef QHash<Scene::ObjectType, AbstractPermanentRenderer*>            ObjectsRendererCollection;
    typedef QHashIterator<Scene::ObjectType, AbstractPermanentRenderer*>    ObjectsRendererCollectionIterator;

    typedef QList<AbstractPermanentRenderer*>           AllRenderersCollection;
    typedef QListIterator<AbstractPermanentRenderer*>   AllRenderersCollectionIterator;

    /**
     * @brief List of all renderers
     */
    AllRenderersCollection                  m_allRenderers;
    ObjectsRendererCollection               m_objectsRenderer;

    /**
     * @brief Current color used when add an object
     */
    QColor                                  m_currentColor;

    /**
     * @brief Flags property manager by object
     */
    ObjectsFlagsPropertyManager*            m_objectFlagsPropertyManager;

signals:
    /**
     * @brief Emitted when changes have occured (add/remove/clear points, change max points, etc...)
     */
    void changesHaveOccured();

    /**
     * @brief Emitted when update was called in a thread different that the main thread
     */
    void updateNecessary();

    /**
     * @brief Emitted when update was called in a thread different that the main thread
     */
    void updatePointsInfoNecessary();

    /**
     * @brief Emitted when update was called in a thread different that the main thread
     */
    void updatePointsColorNecessary();

    /**
     * @brief Emitted when update was called in a thread different that the main thread
     */
    void updatePointsNormalNecessary();

    /**
     * @brief Emitted when update was called in a thread different that the main thread
     */
    void updateObjectsInfoNecessary();

    /**
     * @brief Emitted when update was called in a thread different that the main thread
     */
    void updateObjectsColorNecessary();

    /**
     * @brief Emitted when update was called in a thread different that the main thread
     */
    void updateObjectsNormalNecessary();
};

#endif // PERMANENTSCENE_H
