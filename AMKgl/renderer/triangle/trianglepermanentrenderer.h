#ifndef TRIANGLEPERMANENTRENDERER_H
#define TRIANGLEPERMANENTRENDERER_H

#include "renderer/generic/genericpermanentrenderer.h"

#include <vector>
#include <QHash>

/**
 * @brief Use this class to render in opengl triangle that don't change its position. It is a renderer
 *        optimized for static triangle
 */
class TrianglePermanentRenderer : public GenericPermanentRenderer
{
public:
    TrianglePermanentRenderer();

    /**
     * @brief Inherited from IObjectsRendererAttributesAccessor
     */
    int getNumberOfJumpForOneObject() const { return 3; }

    /**
     * @brief Inherited from IObjectsRendererAttributesAccessor
     */
    Scene::ObjectType getTypeOfObjectDrawn() const { return Scene::Triangle; }

    /**
     * @brief Add a triangle
     * @param p1 : first point of the triangle
     * @param p2 : second point of the triangle
     * @param p3 : third point of the triangle
     * @param info : default element info
     * @param chunk : pointer that will contains the chunk used to store this object
     */
    void addTriangle(const Eigen::Vector3d &p1,
                     const Eigen::Vector3d &p2,
                     const Eigen::Vector3d &p3,
                     const ElementInfo &info = 0,
                     IChunk** chunk = nullptr,
                     const Color &c1 = Color(255,255,255,255),
                     const Color &c2 = Color(255,255,255,255),
                     const Color &c3 = Color(255,255,255,255));
};

#endif // TRIANGLEPERMANENTRENDERER_H
