#ifndef ELLIPSEPERMANENTRENDERER_H
#define ELLIPSEPERMANENTRENDERER_H

#include "renderer/instanced/defaultinstancedpermanentrenderer.h"

#include "ellipserenderershaders.h"

#include "tools/opengl/ellipseglrenderer.h"

typedef DefaultInstancedPermanentRenderer< EllipseRendererShaders > EllipseDefaultInstancedPermanentRendererSuperClass;

/**
 * @brief Use this class to render in opengl ellipses that don't change its position. It is a renderer
 *        optimized for static ellipse
 */
class EllipsePermanentRenderer : public EllipseDefaultInstancedPermanentRendererSuperClass
{
public:
    EllipsePermanentRenderer();

    Scene::ObjectType getTypeOfObjectDrawn() const { return Scene::Ellipse; }

    /**
     * @brief Add an ellipse
     * @param cx : X center position of the ellipse
     * @param cy : Y center position of the ellipse
     * @param cz : Z center position of the ellipse
     * @param majorAxisRadius : radius of the major axis
     * @param minorAxisRadius : radius of the minor axis
     * @param majorAxisDirection : a vector that describe the direction of the major axis
     * @param normal : the normal of the ellipse
     * @param info : default element info
     * @param chunk : pointer that will contains the chunk used to store this object
     */
    void addEllipse(const double& cx, const double& cy, const double &cz,
                    const float& majorAxisRadius, const float& minorAxisRadius,
                    const Eigen::Vector3f& majorAxisDirection = Eigen::Vector3f(1, 0, 0),
                    const Eigen::Vector3f& normal = Eigen::Vector3f(0, 1, 0),
                    const ElementInfo &info = 0,
                    IChunk** chunk = nullptr);

private:
    static EllipseGLRenderer        m_basicDrawRenderer;
    std::vector<GLuint>             m_fastIndicesArray;
};

#endif // ELLIPSEPERMANENTRENDERER_H
