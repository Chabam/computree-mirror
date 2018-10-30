#ifndef POINTCLOUDRENDERER_H
#define POINTCLOUDRENDERER_H

#include "renderer/permanentrenderer.h"

#include "renderer/genericcloud/genericcloudchunkmanagerbycloud.h"
#include "pointcloudchunkmanagerbymemorylimit.h"
#include "interfaces/iglobalpointcloudmanager.h"

#include "definecloudindexes.h"

namespace PointCloudRendererNM {

    typedef PermanentRenderer< PointRendererContext >                                                   PRSuperClass;

    /**
     * @brief For a point cloud manager :
     */
    typedef PointCloudChunkManagerByMemoryLimit                                ByMemoryLimitManager;
    typedef GenericCloudChunkManagerByCloudWithDefaultTraits< IGlobalPointCloudManager, ByMemoryLimitManager, PRSuperClass >   ByVertexCloudManager;
}

class PointCloudRenderer : public PointCloudRendererNM::ByVertexCloudManager
{
public:
    typedef ChunkedPointCloud       PointCloudChunk;

    PointCloudRenderer();
    ~PointCloudRenderer();

    /**
     * @brief Return the type of object draw by this renderer
     */
    Scene::ObjectType getTypeOfObjectDrawn() const;

    /**
     * @brief Add points to this renderer
     * @param indexes : collection of index of points to add
     * @param dispatchInfos : a pointer to an object that will contains informations on where is dispatched points (NULL if you don't want this information)
     */
    void addPoints(const CloudIndex *indexes,
                   PermanentItemInformation* dispatchInfos = NULL);

    /**
     * @brief Add one point
     * @param globalIndex : global index of the point
     * @param dispatchInfos : a pointer to an object that will contains informations on where is dispatched points (NULL if you don't want this information)
     */
    void addPoint(const size_t& globalIndex,
                  PermanentItemInformation* dispatchInfos);

    /**
     * @brief Call to visit points
     * @param visitor : is an object that will receive all points
     */
    void visitPoints(IGlobalPointsVisitor &visitor);

    /**
     * @brief Returns the total number of points in this renderer
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

    /**
     * @brief Returns the number of vertex or element that represent one object in clouds (info, color, etc...).
     *        Per example if this method returns :
     *        - 1 ==> For each object there was 1 color/info/etc... so you can use the object index to
     *                get the color/info/etc... (to simplify 1 object is composed by 1 vertex)
     *
     *        - 3 ==> For each object there was 3 color/info/etc... so you must multiply the object
     *                index by 3 to get the first color/info/etc... and iterate over the next 2 color/info/etc...
     *                (to simplify 1 object is composed by 3 vertex, each vertex has a color/info/etc...)
     */
    int getNumberOfJumpForOneObject() const;

    /**
     * @brief Returns the point cloud attributes provider
     */
    PointCloudAttributesProvider* getPointCloudAttributesProvider() const;

private:
    /**
     * @brief Maximum number of vertex to draw in fast mode
     */
    size_t                                      m_maxNumberOfPointsInFastDrawMode;

    /**
     * @brief The point coud attributes provider
     */
    PointCloudAttributesProvider*               m_attributesCloudProvider;
};


#endif // POINTCLOUDRENDERER_H
