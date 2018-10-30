#include "objectsprocessorforpicker.h"

const std::vector<quint8> ObjectsProcessorForPicker::BOX_SEGMENT_INDICES = ObjectsProcessorForPicker::staticInitBoxSegmentIndices();
const std::vector<quint8> ObjectsProcessorForPicker::BOX_PLANE_INDICES = ObjectsProcessorForPicker::staticInitBoxPlaneIndices();

ObjectsProcessorForPicker::ObjectsProcessorForPicker()
{
    m_interceptor = NULL;
}

void ObjectsProcessorForPicker::setIntersectionInterceptor(const IIntersectionInterceptor *interceptor)
{
    m_interceptor = (IIntersectionInterceptor*)interceptor;
}

IIntersectionInterceptor* ObjectsProcessorForPicker::getIntersectionInterceptor() const
{
    return m_interceptor;
}

void ObjectsProcessorForPicker::staticPrepareMinMaxForBox(const double &cx, const double &cy, const double &cz,
                                              const float &width, const float &height, const float &length,
                                              const Eigen::Vector3f &widthAxisDirection,
                                              const Eigen::Vector3f &lengthAxisDirection,
                                              Eigen::Vector4d &min,
                                              Eigen::Vector4d &max)
{
    const float w2 = width/2.0f;
    const float h2 = height/2.0f;
    const float l2 = length/2.0f;

    min(0) = cx-w2;
    min(1) = cy-h2;
    min(2) = cz-l2;
    min(3) = 1;

    max(0) = cx+w2;
    max(1) = cy+h2;
    max(2) = cz+l2;
    max(3) = 1;

    Eigen::Vector3d wDirection = widthAxisDirection.normalized().cast<double>();
    Eigen::Vector3d lDirection = lengthAxisDirection.normalized().cast<double>();

    if((wDirection != Eigen::Vector3d(1, 0, 0))
            || lDirection != Eigen::Vector3d(0, 0, 1)) {

        Eigen::Vector3d hDirection = wDirection.cross(lDirection);

        wDirection = wDirection * width;
        hDirection = hDirection * height;
        lDirection = lDirection * length;

        Eigen::Matrix4d matrix = Eigen::Matrix4d::Identity();
        matrix.block(0,3,3,1) = Eigen::Vector3d(cx, cy, cz);
        matrix(0,0) = wDirection[0];    // Xx
        matrix(1,0) = wDirection[1];    // Xy
        matrix(2,0) = wDirection[2];    // Xz
        matrix(0,1) = hDirection[0];    // Yx
        matrix(1,1) = hDirection[1];    // Yy
        matrix(2,1) = hDirection[2];    // Yz
        matrix(0,2) = lDirection[0];    // Zx
        matrix(1,2) = lDirection[1];    // Zy
        matrix(2,2) = lDirection[2];    // Zz

        min = matrix * min;
        max = matrix * max;
    }
}


std::vector<quint8> ObjectsProcessorForPicker::staticInitBoxSegmentIndices()
{
    std::vector<quint8> boxSegmentIndices;
    boxSegmentIndices.resize(28);

    int i = 0;

    boxSegmentIndices[i++] = 0;
    boxSegmentIndices[i++] = 1;

    boxSegmentIndices[i++] = 1;
    boxSegmentIndices[i++] = 2;

    boxSegmentIndices[i++] = 2;
    boxSegmentIndices[i++] = 3;

    boxSegmentIndices[i++] = 3;
    boxSegmentIndices[i++] = 0;

    boxSegmentIndices[i++] = 0;
    boxSegmentIndices[i++] = 4;

    boxSegmentIndices[i++] = 4;
    boxSegmentIndices[i++] = 5;

    boxSegmentIndices[i++] = 5;
    boxSegmentIndices[i++] = 6;

    boxSegmentIndices[i++] = 6;
    boxSegmentIndices[i++] = 7;

    boxSegmentIndices[i++] = 7;
    boxSegmentIndices[i++] = 4;

    boxSegmentIndices[i++] = 0;
    boxSegmentIndices[i++] = 4;

    boxSegmentIndices[i++] = 1;
    boxSegmentIndices[i++] = 5;

    boxSegmentIndices[i++] = 2;
    boxSegmentIndices[i++] = 6;

    boxSegmentIndices[i++] = 3;
    boxSegmentIndices[i++] = 7;

    return boxSegmentIndices;
}

std::vector<quint8> ObjectsProcessorForPicker::staticInitBoxPlaneIndices()
{
    std::vector<quint8> boxPlaneIndices;
    boxPlaneIndices.resize(24);

    int i = 0;

    // FRONT
    boxPlaneIndices[i++] = 0;
    boxPlaneIndices[i++] = 1;
    boxPlaneIndices[i++] = 2;
    boxPlaneIndices[i++] = 3;

    // LEFT
    boxPlaneIndices[i++] = 0;
    boxPlaneIndices[i++] = 1;
    boxPlaneIndices[i++] = 5;
    boxPlaneIndices[i++] = 4;

    // BOTTOM
    boxPlaneIndices[i++] = 0;
    boxPlaneIndices[i++] = 4;
    boxPlaneIndices[i++] = 7;
    boxPlaneIndices[i++] = 3;

    //BACK
    boxPlaneIndices[i++] = 4;
    boxPlaneIndices[i++] = 5;
    boxPlaneIndices[i++] = 6;
    boxPlaneIndices[i++] = 7;

    // RIGHT
    boxPlaneIndices[i++] = 2;
    boxPlaneIndices[i++] = 6;
    boxPlaneIndices[i++] = 7;
    boxPlaneIndices[i++] = 3;

    // TOP
    boxPlaneIndices[i++] = 1;
    boxPlaneIndices[i++] = 5;
    boxPlaneIndices[i++] = 6;
    boxPlaneIndices[i++] = 2;

    return boxPlaneIndices;
}

void ObjectsProcessorForPicker::addIntersectionPoint(const Eigen::Vector3d &p, const Eigen::Vector3f& color) const
{
    if(m_interceptor != NULL)
        m_interceptor->addIntersectionPoint(p, color);
}

void ObjectsProcessorForPicker::addIntersectionLine(const Eigen::Vector3d& origin, const Eigen::Vector3d& direction, const Eigen::Vector3f& color) const
{
    if(m_interceptor != NULL)
        m_interceptor->addIntersectionLine(origin, direction, color);
}

void ObjectsProcessorForPicker::addIntersectionSegment(const Eigen::Vector3d &p1, const Eigen::Vector3d &p2, const Eigen::Vector3f& color) const
{
    if(m_interceptor != NULL)
        m_interceptor->addIntersectionSegment(p1, p2, color);
}
