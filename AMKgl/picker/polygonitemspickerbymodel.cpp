#include "polygonitemspickerbymodel.h"
#include "polygonitemspicker.h"

PolygonItemsPickerByModel::PolygonItemsPickerByModel(const QVector<Plane> &polygonPlanes, const QVector<TrianglePlane> &triangles) : ItemsPickerByModel()
{
    m_polygonPlanes = polygonPlanes;
    m_triangles = triangles;
}

GenericPicker* PolygonItemsPickerByModel::createNewPicker()
{
    return new PolygonItemsPicker(m_polygonPlanes, m_triangles, GenericPickerByModel::getCurrentScene());
}
