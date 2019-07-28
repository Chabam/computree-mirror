#ifndef POINTPERMANENTRENDERER_H
#define POINTPERMANENTRENDERER_H

#include "renderer/generic/genericpermanentrenderer.h"

#include <vector>
#include <QHash>

/**
 * @brief Use this class to render in opengl point that don't change its position. It is a renderer
 *        optimized for static point
 */
class PointPermanentRenderer : public GenericPermanentRenderer
{
public:
    PointPermanentRenderer();

    /**
     * @brief Inherited from IObjectsRendererAttributesAccessor
     */
    int getNumberOfJumpForOneObject() const { return 1; }

    /**
     * @brief Inherited from IObjectsRendererAttributesAccessor
     */
    Scene::ObjectType getTypeOfObjectDrawn() const { return Scene::Point; }

    /**
     * @brief Add a point
     * @param p : point
     * @param info : default element info
     * @param chunk : pointer that will contains the chunk used to store this object
     */
    void addPoint(const Eigen::Vector3d &p,
                 const ElementInfo &info = 0,
                 IChunk** chunk = nullptr,
                 const Color &color = Color(255,255,255,255));
};

#endif // POINTPERMANENTRENDERER_H
