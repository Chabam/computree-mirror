#ifndef SPHEREPERMANENTRENDERER_H
#define SPHEREPERMANENTRENDERER_H

#include "renderer/instanced/instancedpermanentrenderer.h"

#include "spherechunk.h"

#include "tools/opengl/sphereglrenderer.h"

typedef InstancedPermanentRenderer< SphereChunk > SphereDefaultInstancedPermanentRendererSuperClass;

/**
 * @brief Use this class to render in opengl sphere that don't change its position. It is a renderer
 *        optimized for static sphere
 */
class SpherePermanentRenderer : public SphereDefaultInstancedPermanentRendererSuperClass
{
public:
    SpherePermanentRenderer();

    Scene::ObjectType getTypeOfObjectDrawn() const { return Scene::Sphere; }

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
     * @param info : default element info
     * @param chunk : pointer to a Ichunk that will contains the chunk used to store this object
     */
    void addSphere(const double& cx, const double& cy, const double &cz,
                   const double& radius,
                   const double &initTheta, const double &endTheta,
                   const double &initPhi, const double &endPhi,
                   bool phiThetaInRadians = true,
                   const ElementInfo &info = 0,
                   IChunk** chunk = NULL);


    /**
     * @brief Returns the first theta/phi attribute
     */
    Eigen::Vector2f* getFirstThetaPhiAttribute() const;

private:
    static SphereGlRenderer m_basicDrawRenderer;
};

#endif // SPHEREPERMANENTRENDERER_H
