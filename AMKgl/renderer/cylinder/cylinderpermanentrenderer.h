#ifndef CYLINDERPERMANENTRENDERER_H
#define CYLINDERPERMANENTRENDERER_H

#include "renderer/box/boxpermanentrenderer.h"

#include "tools/opengl/cylinderglrenderer.h"

/**
 * @brief Use this class to render in opengl cylinders that don't change its position. It is a renderer
 *        optimized for static cylinder
 */
class CylinderPermanentRenderer : public BoxDefaultInstancedPermanentRendererSuperClass
{
public:
    CylinderPermanentRenderer();

    Scene::ObjectType getTypeOfObjectDrawn() const { return Scene::Cylinder; }

    /**
     * @brief Add a cylinder
     * @param cx : X center position of the cylinder
     * @param cy : Y center position of the cylinder
     * @param cz : Z center position of the cylinder
     * @param width : the width of the cylinder (X)
     * @param height : the width of the cylinder (Y)
     * @param length : the width of the cylinder (Z)
     * @param widthAxisDirection : a vector that describe the direction of the width
     * @param heightAxisDirection : a vector that describe the direction of the height
     * @param info : default element info
     * @param chunk : pointer that will contains the chunk used to store this object
     */
    void addCylinder(const double& cx, const double& cy, const double &cz,
                     const float& width, const float& height, const float& length,
                     const Eigen::Vector3f& widthAxisDirection = Eigen::Vector3f(1, 0, 0),
                     const Eigen::Vector3f& heightAxisDirection = Eigen::Vector3f(0, 1, 0),
                     const ElementInfo &info = 0,
                     IChunk** chunk = nullptr);

private:
    static CylinderGLRenderer            m_basicDrawRenderer;
    static CylinderGLRenderer            m_fastDrawRenderer;
};

#endif // CYLINDERPERMANENTRENDERER_H
