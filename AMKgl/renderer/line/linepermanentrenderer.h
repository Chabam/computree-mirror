#ifndef LINEPERMANENTRENDERER_H
#define LINEPERMANENTRENDERER_H

#include "renderer/generic/genericpermanentrenderer.h"

#include <vector>
#include <QHash>

/**
 * @brief Use this class to render in opengl line that don't change its position. It is a renderer
 *        optimized for static line
 */
class LinePermanentRenderer : public GenericPermanentRenderer
{
public:
    LinePermanentRenderer();

    /**
     * @brief Inherited from IObjectsRendererAttributesAccessor
     */
    int getNumberOfJumpForOneObject() const { return 2; }

    /**
     * @brief Inherited from IObjectsRendererAttributesAccessor
     */
    Scene::ObjectType getTypeOfObjectDrawn() const { return Scene::Line; }

    /**
     * @brief Add a line
     * @param p1 : first point of the line
     * @param p2 : second point of the line
     * @param info : default element info
     * @param chunk : pointer that will contains the chunk used to store this object
     */
    void addLine(const Eigen::Vector3d &p1,
                 const Eigen::Vector3d &p2,
                 const ElementInfo &info = 0,
                 IChunk** chunk = nullptr,
                 const Color& c1 = Color(255,255,255,255),
                 const Color& c2 = Color(255,255,255,255));
};

#endif // LINEPERMANENTRENDERER_H
