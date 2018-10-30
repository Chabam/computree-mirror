#include "polygonpointspicker.h"

#include "amkglapp.h"
#include "includepoint.h"
#include "tools/opengl/shapetovolume.h"
#include "interfaces/iglobalpointcloudmanager.h"
#include "includeglobalcloud.h"

#include <QtConcurrent/QtConcurrent>
#include <QElapsedTimer>

#define PACKET_SIZE 100000

PolygonPointsPicker::PolygonPointsPicker(const QVector<TrianglePlane> &triangles) : PointsPicker()
{
    m_triangles = triangles;
    m_hasEnoughTriangle = m_triangles.size() > 0;
    m_pointsAccessor = AMKglEA->getGlobalPointCloudManager();
}

bool PolygonPointsPicker::isValid() const
{
    if(!m_hasEnoughTriangle)
        return false;

    return PointsPicker::isValid();
}

void PolygonPointsPicker::addPointsToCompute(const Eigen::Vector3d &offset,
                                             std::vector<GLuint> &indices,
                                             const size_t& firstPointGlobalIndex)
{
    if(!isValid())
        return;

    FloatPoint* firstPoint = &m_pointsAccessor->getFloatPointAtGlobalIndex(firstPointGlobalIndex);
    ElementInfo* firstSelectInfo = &getSelectionArray()[firstPointGlobalIndex];

    size_t size = indices.size();

    Eigen::Affine3d transform(Eigen::Translation3d(offset[0], offset[1], offset[2]));
    Eigen::Matrix4d matrix = Eigen::Matrix4d::Identity();
    matrix = transform * matrix;

    QVector<TrianglePlane> trianglesTransformed = ShapeToVolume::staticGetTransformedTriangles(m_triangles, matrix);

    for(size_t i=0; i<size; i+=PACKET_SIZE) {
        m_elementsToCompute.append(ToCompute(this,
                                             indices,
                                             firstPoint,
                                             firstSelectInfo,
                                             getSelectionOperation(),
                                             getCheckOperation(),
                                             trianglesTransformed,
                                             i,
                                             qMin(i+PACKET_SIZE, size)));
    }
}

bool PolygonPointsPicker::internalCompute()
{
    QElapsedTimer time;
    time.start();

    AMKglLOG->addInfoMessage(QString("N elements to compute : %1").arg(m_elementsToCompute.size()));

    QtConcurrent::blockingMap(m_elementsToCompute, &staticTestPoints);

    AMKglLOG->addInfoMessage(QString("Picking time elapsed : %1 µs").arg(time.nsecsElapsed()/1000.0));

    return true;
}

void PolygonPointsPicker::staticTestPoints(const PolygonPointsPicker::ToCompute &params)
{
    int nTriangles = params.m_triangles.size();
    int j, k;
    bool ok = false;

    for(size_t i = params.m_begin; i < params.m_end; ++i) {
        const size_t &index = params.m_indices[i];

        const FloatPoint &p = params.m_firstPoint[index];

        if(params.m_checkOperation(p, params.m_firstSelectInfo, index)) {

            for(j=0; !ok && (j != nTriangles); ++j) {

                const TrianglePlane &tr = params.m_triangles[j];

                ok = true;

                for(k=0; ok && k<3; ++k) {
                    if(tr.planes[k].distance(&p(0)) <= 0.0)
                        ok = false;
                }
            }

            if(ok) {
                // point included in the polygon : apply selection operation
                params.m_selectionOperation(p, params.m_firstSelectInfo, index);

                ok = false;
            }
        }
    }
}
