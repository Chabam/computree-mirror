#ifndef POLYGONMESHOBJECTSPICKER_H
#define POLYGONMESHOBJECTSPICKER_H

#include "meshobjectspicker.h"
#include "geometry/triangleplane.h"

/**
 * @brief Picker that use a polygone to pick objects of mesh (face/edge)
 */
class PolygonMeshObjectsPicker : public MeshObjectsPicker
{
public:
    PolygonMeshObjectsPicker(const QVector<Plane>& polygonPlanes,
                             const QVector<TrianglePlane>& triangles,
                             const quint8& nVertexPerObject);

    /**
     * @brief Returns true if all elements was set and if it is at least on triangle plane
     */
    virtual bool isValid() const;

    /**
     * @brief Call this method to add a vector of index of points to test
     * @param offset : the offset to apply to points (coordinate system)
     * @param indices : collection of local indices of points to test (the index 0 is the index "firstPointGlobalIndex" in the global cloud)
     * @param firstPointGlobalIndex : the global index of the first point in the global cloud
     */
    void visitChunk(const IChunk* chunk);

protected:

    /**
     * @brief Call this method to compute it
     */
    bool internalCompute();

private:
    /**
     * @brief Structure that contains elements to compute
     */
    struct ToCompute {
        ToCompute(PolygonMeshObjectsPicker* picker,
                  GLuint* objectIndexes,
                  GLuint* vertexIndexes,
                  const size_t& nObjects,
                  FloatPoint* firstVertex,
                  ElementInfo* firstObjectInfo,
                  selectionOperation sOp,
                  checkOperation cOp,
                  const QVector<PlaneF>& polygonPlanes,
                  const QVector<TrianglePlaneF>& triangles,
                  const size_t &begin = 0,
                  const size_t &end = 0);

        // For outliers !
        ToCompute(PolygonMeshObjectsPicker* picker,
                  FloatPoint* firstVertex,
                  ElementInfo* objectInfo,
                  selectionOperation sOp,
                  checkOperation cOp,
                  const QVector<PlaneF>& polygonPlanes,
                  const QVector<TrianglePlaneF>& triangles);

        PolygonMeshObjectsPicker*       m_thisPicker;
        GLuint*                         m_objectIndexes;
        GLuint*                         m_vertexIndexes;
        size_t                          m_nObjects;
        FloatPoint*                     m_firstVertex;
        ElementInfo*                    m_firstObjectInfo;
        selectionOperation              m_selectionOperation;
        checkOperation                  m_checkOperation;
        size_t                          m_begin;
        size_t                          m_end;
        QVector<PlaneF>                 m_polygonPlanes;
        QVector<TrianglePlaneF>         m_triangles;
    };

    IGlobalPointCloudManager*   m_pointsAccessor;
    QVector<Plane>              m_polygonPlanes;
    QVector<TrianglePlane>      m_triangles;
    QVector<PlaneF>             m_polygonPlanesF;
    QVector<TrianglePlaneF>     m_trianglesF;
    quint8                      m_nVertexPerObject;
    size_t                      m_nPacketSize;
    QList<ToCompute>            m_elementsToCompute;
    bool                        m_hasEnoughTriangle;

    void computePlanesForOffset(QVector<PlaneF>& polygonPlanesTransformed,
                                QVector<TrianglePlaneF>& trianglesTransformed,
                                const Eigen::Vector3d& offset);

    /**
     * @brief Called by a thread to compute objects
     */
    static void staticTestObjects(const ToCompute &params);

    static bool staticTestEdge(const FloatPoint& v1, const FloatPoint& v2, const QVector<PlaneF> &polygonPlanes, const QVector<TrianglePlaneF>& triangles);

    static bool staticTestFace(const FloatPoint& v1, const FloatPoint& v2, const FloatPoint& v3, const QVector<PlaneF> &polygonPlanes, const QVector<TrianglePlaneF> &triangles);
};

#endif // POLYGONMESHOBJECTSPICKER_H
