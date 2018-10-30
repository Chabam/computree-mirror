#include "polygonpointspickerbymodel.h"

PolygonPointsPickerByModel::PolygonPointsPickerByModel(const QVector<TrianglePlane> &triangles) : PointsPickerByModel()
{
    m_triangles = triangles;
}

GenericPicker* PolygonPointsPickerByModel::createNewPicker()
{
    return new PolygonPointsPicker(m_triangles);
}
