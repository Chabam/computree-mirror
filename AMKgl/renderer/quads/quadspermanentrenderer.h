#ifndef QUADSPERMANENTRENDERER_H
#define QUADSPERMANENTRENDERER_H

#include "renderer/instanced/defaultinstancedpermanentrenderer.h"

#include "quadsrenderershaders.h"

#include "tools/opengl/quadsglrenderer.h"

typedef DefaultInstancedPermanentRenderer< QuadsRendererShaders > QuadsDefaultInstancedPermanentRendererSuperClass;

/**
 * @brief Use this class to render in opengl quads that don't change its position. It is a renderer
 *        optimized for static quads
 */
class QuadsPermanentRenderer : public QuadsDefaultInstancedPermanentRendererSuperClass
{
public:
    QuadsPermanentRenderer();

    Scene::ObjectType getTypeOfObjectDrawn() const { return Scene::Quads; }

    /**
     * @brief Add a quads
     * @param cx : X center position of the quads
     * @param cy : Y center position of the quads
     * @param cz : Z center position of the quads
     * @param width : the width of the quads (X)
     * @param length : the width of the quads (Z)
     * @param widthAxisDirection : a vector that describe the direction of the width
     * @param lengthAxisDirection : a vector that describe the direction of the length
     * @param info : default element info
     * @param chunk : pointer that will contains the chunk used to store this object
     */
    void addQuads(const double& cx, const double& cy, const double &cz,
                  const float& width, const float& length,
                  const Eigen::Vector3f& widthAxisDirection = Eigen::Vector3f(1, 0, 0),
                  const Eigen::Vector3f& lengthAxisDirection = Eigen::Vector3f(0, 0, 1),
                  const ElementInfo &info = 0,
                  IChunk** chunk = NULL);

private:
    static QuadsGLRenderer            m_basicDrawRenderer;
};

#endif // QUADSPERMANENTRENDERER_H
