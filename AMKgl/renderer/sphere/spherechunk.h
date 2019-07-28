#ifndef SPHERECHUNK_H
#define SPHERECHUNK_H

#include "renderer/instanced/chunkedinstanced.h"
#include "renderer/renderercontext.h"

#include "sphererenderershaders.h"

typedef RendererContext<SphereRendererShaders > SphereRendererContext;

typedef ChunkedInstanced< SphereRendererContext > SphereDefaultChunkInstancedSuperClass;

class SpherePermanentRenderer;

class SphereChunk : public SphereDefaultChunkInstancedSuperClass
{
public:
    /**
     * @brief Create a chunk
     * @param uniqueKey : unique key of this chunk
     * @param objectType : type of object draw by this chunk
     * @param glMode : the opengl mode to use to draw vertex
     * @param vertexArray : the array that contains vertex to draw
     * @param offset : offset of shapes that will be added to this chunk
     * @param normalIndices : array that contains indices of points to draw in normal mode, if nullptr all points will be draw in normal mode
     * @param fastIndices : array that contains indices of points to draw in fast mode, if nullptr all points will be draw in fast mode
     * @param funcContext : a context that will be passed to function for draw shapes
     * @param basicDraw, rawDraw, ... : functions that will be called to draw shapes in normal mode (can be nullptr if you want that predefined function was called)
     * @param basicFastDraw, rawFastDraw, ... : functions that will be called to fast draw shapes in fast mode  (can be nullptr if you want that predefined function was called)
     */
    SphereChunk(const uint& uniqueKey,
                Scene::ObjectType objectType,
                GLenum glMode,
                const std::vector<Eigen::Vector3f>& vertexArray,
                const Eigen::Vector3d& offset,
                const CustomVertexAttributeCollection* customVertexAttributesCollection = nullptr,
                const std::vector<Eigen::Vector3f>* fastVertexArray = nullptr,
                const std::vector<GLuint> *normalIndices = nullptr,
                const std::vector<GLuint> *fastIndices = nullptr,
                void* funcContext = nullptr,
                DrawShapeFunc basicDraw = nullptr,
                DrawShapeFunc rawDraw = nullptr,
                DrawShapeFunc VBODraw = nullptr,
                DrawShapeFunc VAODraw = nullptr,
                DrawShapeFunc basicFastDraw = nullptr,
                DrawShapeFunc rawFastDraw = nullptr,
                DrawShapeFunc VBOFastDraw = nullptr,
                DrawShapeFunc VAOFastDraw = nullptr);

private:
    friend class SpherePermanentRenderer;

    SpherePermanentRenderer* m_renderer;

    /**
     * @brief Called in SpherePermanentRenderer to set it
     */
    void setPermanentRenderer(const SpherePermanentRenderer* renderer);

protected:
    /**
     * @brief Called when a shape must be draw in basic mode
     */
    void basicDrawShape(const ShaderParamsType& param,
                        const std::vector<Vertex>& vertexArray,
                        std::vector<GLuint>* indices,
                        const size_t& nVertex,
                        bool fast);

    /**
     * @brief Returns the first theta/phi attribute to use when draw in basic mode per example
     */
    Eigen::Vector2f* getFirstThetaPhiAttribute() const;

    /**
     * @brief Returns true if theta/phi is in the interval of param
     */
    bool checkThetaPhi(const bool& checkThetaTwoSide, const bool& checkPhiTwoSide, const ShaderParamsType& param, const Eigen::Vector2f& thetaPhi) const;
};

#endif // SPHERECHUNK_H
