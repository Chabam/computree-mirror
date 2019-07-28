#include "polygonmeshobjectspicker.h"

#include "renderer/mesh/chunkgenericcloudobjectforoutliers.h"
#include "tools/opengl/shapetovolume.h"
#include "picker/tools/intersectionchecker.h"

#include <QtConcurrent/QtConcurrent>

#define PACKET_SIZE 10000

PolygonMeshObjectsPicker::PolygonMeshObjectsPicker(const QVector<Plane>& polygonPlanes,
                                                   const QVector<TrianglePlane>& triangles,
                                                   const quint8 &nVertexPerObject) : MeshObjectsPicker()
{
    m_polygonPlanes = polygonPlanes;
    m_triangles = triangles;

    m_polygonPlanesF = ShapeToVolume::staticConvertPlaneCollectionTo<float>(polygonPlanes);
    m_trianglesF = ShapeToVolume::staticConvertTrianglePlaneCollectionTo<float>(triangles);

    m_nVertexPerObject = nVertexPerObject;
    m_nPacketSize = PACKET_SIZE;

    if((nVertexPerObject % 2) != 0)
        --m_nPacketSize;

    m_hasEnoughTriangle = m_triangles.size() > 0;
    m_pointsAccessor = AMKglEA->getGlobalPointCloudManager();
}

bool PolygonMeshObjectsPicker::isValid() const
{
    if(!m_hasEnoughTriangle)
        return false;

    return MeshObjectsPicker::isValid();
}

void PolygonMeshObjectsPicker::visitChunk(const IChunk *chunk)
{
    Q_ASSERT(isValid());

    const AbstractChunkGenericCloudObject* objectChunk = dynamic_cast<const AbstractChunkGenericCloudObject*>(chunk);

    if(objectChunk == nullptr)
        return;

    QVector<PlaneF> polygonPlanesTransformed;
    QVector<TrianglePlaneF> trianglesTransformed;

    GLuint* foi = objectChunk->getFirstObjectIndex();
    ElementInfo* foin = objectChunk->createOrGetFirstObjectInfo();

    const size_t nObjects = objectChunk->countObjects();

    const ChunkGenericCloudObjectForOutliers* objectChunkForOutliers = dynamic_cast<const ChunkGenericCloudObjectForOutliers*>(chunk);

    if(objectChunkForOutliers != nullptr) {

        const ChunkGenericCloudObjectForOutliers::VertexesAndOffsetCollection& collection = objectChunkForOutliers->getVertexesWithOffset();

        ChunkGenericCloudObjectForOutliers::VertexesAndOffsetCollection::const_iterator it = collection.begin();
        ChunkGenericCloudObjectForOutliers::VertexesAndOffsetCollection::const_iterator end = collection.end();

        size_t currentObjectIndex = 0;

        while((it != end)
              && (currentObjectIndex < nObjects)) {
            const ChunkGenericCloudObjectForOutliers::VertexesByOffset &v = *it;

            computePlanesForOffset(polygonPlanesTransformed, trianglesTransformed, v.offset);

            ToCompute tc(this,
                         (AMKgl::FloatPoint*)&v.vertexes[0],
                         &foin[foi[currentObjectIndex]],
                         getSelectionOperation(),
                         getCheckOperation(),
                         polygonPlanesTransformed,
                         trianglesTransformed);

            m_elementsToCompute.append(tc);

            ++it;
            ++currentObjectIndex;
        }
    } else {

        GLuint* fvi = objectChunk->getFirstVertexIndex();
        FloatPoint* fv = objectChunk->getFirstVertex();

        const Eigen::Vector3d offset = objectChunk->getOffset();

        computePlanesForOffset(polygonPlanesTransformed, trianglesTransformed, offset);

        for(size_t i=0; i<nObjects; i+=m_nPacketSize) {
            m_elementsToCompute.append(ToCompute(this,
                                                 foi,
                                                 fvi,
                                                 nObjects,
                                                 fv,
                                                 foin,
                                                 getSelectionOperation(),
                                                 getCheckOperation(),
                                                 polygonPlanesTransformed,
                                                 trianglesTransformed,
                                                 i,
                                                 qMin(i+m_nPacketSize, nObjects)));
        }
    }
}

bool PolygonMeshObjectsPicker::internalCompute()
{
    QElapsedTimer time;
    time.start();

    AMKglLOG->addInfoMessage(QString("N elements to compute : %1").arg(m_elementsToCompute.size()));

    QtConcurrent::blockingMap(m_elementsToCompute, &staticTestObjects);

    AMKglLOG->addInfoMessage(QString("Picking time elapsed : %1 µs").arg(time.nsecsElapsed()/1000.0));

    return true;
}

void PolygonMeshObjectsPicker::computePlanesForOffset(QVector<PlaneF> &polygonPlanesTransformed,
                                                      QVector<TrianglePlaneF> &trianglesTransformed,
                                                      const Eigen::Vector3d& offset)
{
    if((offset[0] != 0)
            || (offset[1] != 0)
            || (offset[2] != 0))
    {
        Eigen::Affine3d transform(Eigen::Translation3d(offset[0], offset[1], offset[2]));
        Eigen::Matrix4d matrix = Eigen::Matrix4d::Identity();
        matrix = transform * matrix;

        QVector<Plane> ppt = ShapeToVolume::staticGetTransformedPlanes(m_polygonPlanes, matrix);
        QVector<TrianglePlane> tt = ShapeToVolume::staticGetTransformedTriangles(m_triangles, matrix);

        polygonPlanesTransformed = ShapeToVolume::staticConvertPlaneCollectionTo<float>(ppt);
        trianglesTransformed = ShapeToVolume::staticConvertTrianglePlaneCollectionTo<float>(tt);

    } else {
        polygonPlanesTransformed = m_polygonPlanesF;
        trianglesTransformed = m_trianglesF;
    }
}

void PolygonMeshObjectsPicker::staticTestObjects(const PolygonMeshObjectsPicker::ToCompute& params)
{
    bool ok = false;

    size_t currentObjectVertexIndex = 0;

    if(params.m_thisPicker->m_nVertexPerObject == 2) { // Edges
        for(size_t i = params.m_begin; i < params.m_end; ++i) {
            const GLuint objectIndex = ((params.m_objectIndexes == nullptr) ? GLuint(i) : params.m_objectIndexes[i]);
            ElementInfo& objectInfo = params.m_firstObjectInfo[objectIndex];

            if(params.m_checkOperation(objectInfo)) {

                const FloatPoint& v1 = ((params.m_vertexIndexes == nullptr) ? params.m_firstVertex[currentObjectVertexIndex] : params.m_firstVertex[params.m_vertexIndexes[currentObjectVertexIndex]]);
                ++currentObjectVertexIndex;
                const FloatPoint& v2 = ((params.m_vertexIndexes == nullptr) ? params.m_firstVertex[currentObjectVertexIndex] : params.m_firstVertex[params.m_vertexIndexes[currentObjectVertexIndex]]);
                ++currentObjectVertexIndex;

                ok = staticTestEdge(v1,
                                    v2,
                                    params.m_polygonPlanes,
                                    params.m_triangles);

                if(ok) {
                    // object included in the polygon : apply selection operation
                    params.m_selectionOperation(objectInfo);

                    ok = false;
                }
            } else {
                currentObjectVertexIndex += 2;
            }
        }
    } else if(params.m_thisPicker->m_nVertexPerObject == 3) { // Faces

        for(size_t i = params.m_begin; i < params.m_end; ++i) {
            const GLuint objectIndex = ((params.m_objectIndexes == nullptr) ? GLuint(i) : params.m_objectIndexes[i]);
            ElementInfo& objectInfo = params.m_firstObjectInfo[objectIndex];

            if(params.m_checkOperation(objectInfo)) {

                const FloatPoint& v1 = ((params.m_vertexIndexes == nullptr) ? params.m_firstVertex[currentObjectVertexIndex] : params.m_firstVertex[params.m_vertexIndexes[currentObjectVertexIndex]]);
                ++currentObjectVertexIndex;
                const FloatPoint& v2 = ((params.m_vertexIndexes == nullptr) ? params.m_firstVertex[currentObjectVertexIndex] : params.m_firstVertex[params.m_vertexIndexes[currentObjectVertexIndex]]);
                ++currentObjectVertexIndex;
                const FloatPoint& v3 = ((params.m_vertexIndexes == nullptr) ? params.m_firstVertex[currentObjectVertexIndex] : params.m_firstVertex[params.m_vertexIndexes[currentObjectVertexIndex]]);
                ++currentObjectVertexIndex;

                ok = staticTestFace(v1,
                                    v2,
                                    v3,
                                    params.m_polygonPlanes,
                                    params.m_triangles);

                if(ok) {
                    // object included in the polygon : apply selection operation
                    params.m_selectionOperation(objectInfo);

                    ok = false;
                }
            } else {
                currentObjectVertexIndex += 3;
            }

        }
    }
}

bool PolygonMeshObjectsPicker::staticTestEdge(const FloatPoint &v1,
                                              const FloatPoint &v2,
                                              const QVector<PlaneF> &polygonPlanes,
                                              const QVector<TrianglePlaneF> &triangles)
{
    size_t size = triangles.size();

    // one point is in the polygon ?
    bool ok = IntersectionChecker::staticIsAPointInsideAPolygon(v1, triangles, size)
                || IntersectionChecker::staticIsAPointInsideAPolygon(v2, triangles, size);

    // no ?
    if(!ok) {
        // check if the segment intersect with one of the plane of the polygon
        size = polygonPlanes.size();

        for(size_t j=0; !ok && (j != size); ++j) {

            const PlaneF &plane = polygonPlanes[(int)j];

            ok = IntersectionChecker::staticDoesASegmentIntersectWithAPlane<Eigen::Vector3f>(v1, v2,
                                                                                             plane.m_vertex[0],
                                                                                             plane.m_vertex[1],
                                                                                             plane.m_vertex[2],
                                                                                             plane.m_vertex[3]);
        }
    }

    return ok;
}

bool PolygonMeshObjectsPicker::staticTestFace(const FloatPoint &v1,
                                              const FloatPoint &v2,
                                              const FloatPoint &v3,
                                              const QVector<PlaneF> &polygonPlanes,
                                              const QVector<TrianglePlaneF> &triangles)
{
    size_t size = triangles.size();

    // one point is in the polygon ?
    bool ok = IntersectionChecker::staticIsAPointInsideAPolygon(v1, triangles, size)
                || IntersectionChecker::staticIsAPointInsideAPolygon(v2, triangles, size)
                || IntersectionChecker::staticIsAPointInsideAPolygon(v3, triangles, size);

    // no ? so we check if a line of the triangle intersect with the polygon
    if(!ok) {

        size = polygonPlanes.size();

        // exit loop if one ray of the polygon intersect with the triangle
        for(size_t j=0; !ok && (j != size); ++j) {

            const PlaneF &plane = polygonPlanes[(int)j];

            ok = IntersectionChecker::staticDoesASegmentIntersectWithATriangle<Eigen::Vector3f>(plane.m_vertex[0],
                                                                                                plane.m_vertex[1],
                                                                                                v1,
                                                                                                v2,
                                                                                                v3);

            if(!ok) {
                ok = IntersectionChecker::staticDoesATriangleIntersectWithAPlane<Eigen::Vector3f>(v1, v2, v3,
                                                                                                  plane.m_vertex[0],
                                                                                                  plane.m_vertex[1],
                                                                                                  plane.m_vertex[2],
                                                                                                  plane.m_vertex[3]);
            }
        }
    }

    return ok;
}

PolygonMeshObjectsPicker::ToCompute::ToCompute(PolygonMeshObjectsPicker* picker,
                                               GLuint* objectIndexes,
                                               GLuint* vertexIndexes,
                                               const size_t &nObjects,
                                               FloatPoint* firstVertex,
                                               ElementInfo* firstObjectInfo,
                                               selectionOperation sOp,
                                               checkOperation cOp,
                                               const QVector<PlaneF>& polygonPlanes,
                                               const QVector<TrianglePlaneF>& triangles,
                                               const size_t &begin,
                                               const size_t &end)
{
    m_thisPicker = picker;

    m_objectIndexes = objectIndexes;
    m_vertexIndexes = vertexIndexes;

    m_nObjects = nObjects;

    m_begin = begin;
    m_end = end;

    if(m_end == 0)
       m_end = nObjects;

    if(m_begin > m_end)
       m_begin = m_end;

    m_firstVertex = firstVertex;
    m_firstObjectInfo = firstObjectInfo;

    m_selectionOperation = sOp;
    m_checkOperation = cOp;

    m_polygonPlanes = polygonPlanes;
    m_triangles = triangles;
}

PolygonMeshObjectsPicker::ToCompute::ToCompute(PolygonMeshObjectsPicker *picker,
                                               FloatPoint* firstVertex,
                                               ElementInfo* objectInfo,
                                               MeshObjectsPicker::selectionOperation sOp,
                                               MeshObjectsPicker::checkOperation cOp,
                                               const QVector<PlaneF> &polygonPlanes,
                                               const QVector<TrianglePlaneF> &triangles)
{
    m_thisPicker = picker;

    m_objectIndexes = nullptr;
    m_vertexIndexes = nullptr;

    m_nObjects = 0;

    m_begin = 0;
    m_end = 1;

    m_firstVertex = firstVertex;
    m_firstObjectInfo = objectInfo;

    m_selectionOperation = sOp;
    m_checkOperation = cOp;

    m_polygonPlanes = polygonPlanes;
    m_triangles = triangles;
}
