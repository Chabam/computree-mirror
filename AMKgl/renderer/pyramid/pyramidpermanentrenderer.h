#ifndef PYRAMIDPERMANENTRENDERER_H
#define PYRAMIDPERMANENTRENDERER_H

#include "renderer/box/boxpermanentrenderer.h"

#include "tools/opengl/pyramidglrenderer.h"

/**
 * @brief Use this class to render in opengl cylinders that don't change its position. It is a renderer
 *        optimized for static cylinder
 */
class PyramidPermanentRenderer : public BoxDefaultInstancedPermanentRendererSuperClass
{
public:
    PyramidPermanentRenderer();

    Scene::ObjectType getTypeOfObjectDrawn() const { return Scene::Pyramid; }

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
     * @param info : default element info
     * @param chunk : pointer that will contains the chunk used to store this object
     */
    void addPyramid(const double& cx, const double& cy, const double &cz,
                     const float& width, const float& height, const float& length,
                     const Eigen::Vector3f& widthAxisDirection = Eigen::Vector3f(1, 0, 0),
                     const Eigen::Vector3f& heightAxisDirection = Eigen::Vector3f(0, 1, 0),
                     const ElementInfo &info = 0,
                     IChunk** chunk = nullptr);

private:
    static PyramidGLRenderer            m_basicDrawRenderer;
};

#endif // PYRAMIDPERMANENTRENDERER_H
