#ifndef POLYGONPOINTSPICKER_H
#define POLYGONPOINTSPICKER_H

#include "pointspicker.h"
#include "geometry/triangleplane.h"
#include "elementinfo.h"

/**
 * @brief Picker that use a polygone to pick points
 */
class PolygonPointsPicker : public PointsPicker
{
public:
    PolygonPointsPicker(const QVector<TrianglePlane> &triangles);

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
    void addPointsToCompute(const Eigen::Vector3d &offset, std::vector<GLuint> &indices, const size_t& firstPointGlobalIndex);

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
        ToCompute(PolygonPointsPicker* pointsPicker,
                  std::vector<GLuint> &indices,
                  FloatPoint* firstPoint,
                  ElementInfo* firstSelectInfo,
                  selectionOperation sOp,
                  checkOperation cOp,
                  const QVector<TrianglePlane>& triangles,
                  const size_t &begin = 0,
                  const size_t &end = 0) : m_indices(indices)
        {
            m_pointsPicker = pointsPicker;

            m_begin = begin;
            m_end = end;

            if(m_end == 0)
               m_end = m_indices.size();

            if(m_begin > m_end)
               m_begin = m_end;

            m_firstPoint = firstPoint;
            m_firstSelectInfo = firstSelectInfo;

            m_selectionOperation = sOp;
            m_checkOperation = cOp;

            m_triangles = triangles;
        }

        ToCompute& operator=(const ToCompute& other)
        {
            m_indices = other.m_indices;
            m_pointsPicker = other.m_pointsPicker;

            m_begin = other.m_begin;
            m_end = other.m_end;

            m_firstPoint = other.m_firstPoint;
            m_firstSelectInfo = other.m_firstSelectInfo;

            m_selectionOperation = other.m_selectionOperation;
            m_checkOperation = other.m_checkOperation;

            m_triangles = other.m_triangles;

            return *this;
        }

        PolygonPointsPicker*            m_pointsPicker;
        std::vector<GLuint>&            m_indices;
        FloatPoint*                     m_firstPoint;
        ElementInfo*                    m_firstSelectInfo;
        selectionOperation              m_selectionOperation;
        checkOperation                  m_checkOperation;
        size_t                          m_begin;
        size_t                          m_end;
        QVector<TrianglePlane>          m_triangles;
    };

    IGlobalPointCloudManager*   m_pointsAccessor;
    QVector<TrianglePlane>      m_triangles;
    QList<ToCompute>            m_elementsToCompute;
    bool                        m_hasEnoughTriangle;

    /**
     * @brief Called by a thread to compute points
     */
    static void staticTestPoints(const ToCompute &params);
};

#endif // POLYGONPOINTSPICKER_H
