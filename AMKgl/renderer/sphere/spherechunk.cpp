#include "spherechunk.h"

#include "spherepermanentrenderer.h"

SphereChunk::SphereChunk(const uint& uniqueKey,
                         Scene::ObjectType objectType,
                         GLenum glMode,
                         const std::vector<Eigen::Vector3f> &vertexArray,
                         const Eigen::Vector3d& offset,
                         const CustomVertexAttributeCollection* customVertexAttributesCollection,
                         const std::vector<Eigen::Vector3f> *fastVertexArray,
                         const std::vector<GLuint> *normalIndices,
                         const std::vector<GLuint> *fastIndices,
                         void* funcContext,
                         DrawShapeFunc basicDraw,
                         DrawShapeFunc rawDraw,
                         DrawShapeFunc VBODraw,
                         DrawShapeFunc VAODraw,
                         DrawShapeFunc basicFastDraw,
                         DrawShapeFunc rawFastDraw,
                         DrawShapeFunc VBOFastDraw,
                         DrawShapeFunc VAOFastDraw) : SphereDefaultChunkInstancedSuperClass(uniqueKey,
                                                                                            objectType,
                                                                                            glMode,
                                                                                            vertexArray,
                                                                                            offset,
                                                                                            customVertexAttributesCollection,
                                                                                            fastVertexArray,
                                                                                            normalIndices,
                                                                                            fastIndices,
                                                                                            funcContext,
                                                                                            basicDraw,
                                                                                            rawDraw,
                                                                                            VBODraw,
                                                                                            VAODraw,
                                                                                            basicFastDraw,
                                                                                            rawFastDraw,
                                                                                            VBOFastDraw,
                                                                                            VAOFastDraw)
{
    m_renderer = nullptr;
}

void SphereChunk::setPermanentRenderer(const SpherePermanentRenderer *renderer)
{
    m_renderer = (SpherePermanentRenderer*)renderer;
}

void SphereChunk::basicDrawShape(const ShaderParamsType& param,
                                 const std::vector<Vertex>& vertexArray,
                                 std::vector<GLuint>* indices,
                                 const size_t& nVertex,
                                 bool fast)
{
    Q_UNUSED(fast)

    Eigen::Vector2f* tp = getFirstThetaPhiAttribute();

    if(tp == nullptr)
        return;

    bool checkThetaTwoSide = param.startEndThetaInRadians(0) > param.startEndThetaInRadians(1);
    bool checkPhiTwoSide = param.startEndPhiInRadians(0) > param.startEndPhiInRadians(1);

    if(indices != nullptr) {

        for(size_t j=0; j<nVertex; ++j) {
            const GLuint& index = (*indices)[j];

            if(checkThetaPhi(checkThetaTwoSide, checkPhiTwoSide, param, tp[index])) {
                glVertex3fv(&(vertexArray[index])[0]);
            } else {
                glVertex3f(0, 0, 0);
            }
        }
    } else {
        for(size_t j=0; j<nVertex; ++j) {
            if(checkThetaPhi(checkThetaTwoSide, checkPhiTwoSide, param, tp[j])) {
                glVertex3fv(&(vertexArray[j])[0]);

            } else {
                glVertex3f(0, 0, 0);
            }
        }
    }
}

Eigen::Vector2f* SphereChunk::getFirstThetaPhiAttribute() const
{
    if(m_renderer != nullptr)
        return m_renderer->getFirstThetaPhiAttribute();

    return nullptr;
}

bool SphereChunk::checkThetaPhi(const bool &checkThetaTwoSide, const bool &checkPhiTwoSide,
                                const ChunkedInstanced::ShaderParamsType &param,
                                const Eigen::Vector2f &thetaPhi) const
{
    bool ok;

    if(checkThetaTwoSide) {
        ok = (thetaPhi.x() >= 0 && thetaPhi.x() <= param.startEndThetaInRadians(0))
                || (thetaPhi.x() >= param.startEndThetaInRadians(1));
    } else {
        ok = thetaPhi.x() >= param.startEndThetaInRadians(0)
                && thetaPhi.x() <= param.startEndThetaInRadians(1);
    }

    if(ok) {
        if(checkPhiTwoSide) {
            return (thetaPhi.y() >= 0 && thetaPhi.y() <= param.startEndPhiInRadians(0))
                    || (thetaPhi.y() >= param.startEndPhiInRadians(1));
        } else {
            return thetaPhi.y() >= param.startEndPhiInRadians(0)
                    && thetaPhi.y() <= param.startEndPhiInRadians(1);
        }
    }

    return false;
}

