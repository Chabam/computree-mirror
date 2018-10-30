#ifndef BOXPERMANENTRENDERER_H
#define BOXPERMANENTRENDERER_H

#include "renderer/instanced/defaultinstancedpermanentrenderer.h"

#include "boxrenderershaders.h"

#include "tools/opengl/boxglrenderer.h"

typedef DefaultInstancedPermanentRenderer< BoxRendererShaders > BoxDefaultInstancedPermanentRendererSuperClass;

/**
 * @brief Use this class to render in opengl box that don't change its position. It is a renderer
 *        optimized for static box
 */
class BoxPermanentRenderer : public BoxDefaultInstancedPermanentRendererSuperClass
{
public:

    BoxPermanentRenderer();

    Scene::ObjectType getTypeOfObjectDrawn() const { return Scene::Box; }

    /**
     * @brief Add a box
     * @param cx : X center position of the box
     * @param cy : Y center position of the box
     * @param cz : Z center position of the box
     * @param width : the width of the box (X)
     * @param height : the height of the box (Y)
     * @param length : the length of the box (Z)
     * @param widthAxisDirection : a vector that describe the direction of the width
     * @param heightAxisDirection : a vector that describe the direction of the height
     * @param info : default element info
     * @param chunk : pointer that will contains the chunk used to store this object
     */
    void addBox(const double& cx, const double& cy, const double &cz,
                const float& width, const float& height, const float& length,
                const Eigen::Vector3f& widthAxisDirection = Eigen::Vector3f(1, 0, 0),
                const Eigen::Vector3f& heightAxisDirection = Eigen::Vector3f(0, 1, 0),
                const ElementInfo &info = 0,
                IChunk** chunk = NULL);

private:
    static BoxGLRenderer            m_basicDrawRenderer;
};

#endif // BOXPERMANENTRENDERER_H
